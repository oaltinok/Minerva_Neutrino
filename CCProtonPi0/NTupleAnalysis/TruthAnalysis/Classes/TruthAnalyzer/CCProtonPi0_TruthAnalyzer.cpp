#ifndef CCProtonPi0_TruthAnalyzer_cpp
#define CCProtonPi0_TruthAnalyzer_cpp

#include "CCProtonPi0_TruthAnalyzer.h"

using namespace PlotUtils;

void CCProtonPi0_TruthAnalyzer::Loop(std::string playlist)
{
    // Control Flow
    bool applyMaxEvents = false;
    double nMaxEvents = 1000000;

    //------------------------------------------------------------------------
    // Create chain
    //------------------------------------------------------------------------
    TChain* fChain = new TChain("Truth");
    Init(playlist, fChain);

    if (!fChain) return;
    if (fChain == 0) return;
    
    // Disable Branches for Performance
    fChain->SetBranchStatus("*", false);
    fChain->SetBranchStatus("truth_is*", true);
    fChain->SetBranchStatus("truth_pi0_*", true);
    fChain->SetBranchStatus("truth_muon_*", true);
    fChain->SetBranchStatus("*genie_wgt_*", true);
    fChain->SetBranchStatus("mc_run", true);
    fChain->SetBranchStatus("mc_Q2", true);
    fChain->SetBranchStatus("mc_w", true);
    fChain->SetBranchStatus("mc_intType", true);
    fChain->SetBranchStatus("mc_resID", true);
    fChain->SetBranchStatus("mc_incomingE", true);
    fChain->SetBranchStatus("mc_nFSPart", true);
    fChain->SetBranchStatus("mc_FSPartPDG", true);
    fChain->SetBranchStatus("mc_er_nPart", true);
    fChain->SetBranchStatus("mc_er_ID", true);
    fChain->SetBranchStatus("mc_er_status", true);
    fChain->SetBranchStatus("mc_er_mother", true);

    //------------------------------------------------------------------------
    // Loop over Chain
    //------------------------------------------------------------------------
    std::cout<<"Looping over all entries"<<std::endl;

    Long64_t nentries = fChain->GetEntriesFast();
    Long64_t nbytes = 0, nb = 0;

    for (Long64_t jentry=0; jentry<nentries;jentry++) {

        nb = fChain->GetEntry(jentry);   nbytes += nb;    
        Long64_t ientry = fChain->GetEntry(jentry);

        if (ientry == 0) {
            std::cout<<"\tGetEntry failure "<<jentry<<std::endl;
            break;
        }

        // Progress Message on Terminal
        if (jentry % 1000000 == 0 ) std::cout<<"Entry = "<<jentry<<std::endl;

        if (applyMaxEvents && jentry == nMaxEvents){
            std::cout<<"\tReached Event Limit!"<<std::endl;
            break;
        }

        nAll.count++;
        if (truth_isFidVol) nFidVol.count++;
        else{
            nNoFidVol.count++;
            continue;
        }

        UpdateSignalDef();
        
        // Count Signal and Background inside Fiducial Volume
        if (truth_isSignal) nFidVol_Signal.count++;
        else nFidVol_Bckg.count++;

        // Count Signal Type & Fill Histograms
        if (truth_isSignal){
            FillSignalHistograms();
        }
    }

    // Add Other Error Bands and Fill With CV
    AddOtherErrorBands_FillWithCV();

    writeTextFile();
    writeHistograms();
}

void CCProtonPi0_TruthAnalyzer::writeTextFile() 
{
    // Formatting for Text Output
    textFile<<std::fixed;
    textFile<<std::setprecision(1);

    // All Events
    WriteCounter(nAll, nAll);
    WriteCounter(nFidVol, nAll);
    WriteCounter(nNoFidVol, nAll);
    textFile<<std::endl;

    // Events inside Fiducial Volume
    WriteCounter(nFidVol_Signal, nFidVol);
    WriteCounter(nFidVol_Bckg, nFidVol);
    textFile<<std::endl;
 
    // Signal Types
    WriteCounter(nQE, nFidVol_Signal);
    textFile<<std::endl;

    WriteCounter(nRES_1232, nFidVol_Signal);
    WriteCounter(nRES_1535, nFidVol_Signal);
    WriteCounter(nRES_1520, nFidVol_Signal);
    WriteCounter(nRES_Other, nFidVol_Signal);
    textFile<<std::endl;
   
    WriteCounter(nDIS_1_pi, nFidVol_Signal);
    WriteCounter(nDIS_2_pi, nFidVol_Signal);
    WriteCounter(nDIS_Multi_pi, nFidVol_Signal);
    WriteCounter(nDIS_Other, nFidVol_Signal);
    textFile<<std::endl;
   
    textFile.close();
}

void CCProtonPi0_TruthAnalyzer::WriteCounter(counter Counter, counter PercentBase)
{
    textFile<<Counter.name<<"\t"<<Counter.count<<"\t"<<GetPercent(PercentBase,Counter)<<std::endl;
}

double CCProtonPi0_TruthAnalyzer::GetPercent(counter nAll, counter nOther)
{
    double percent = (nOther.count/nAll.count) * 100;
    return percent;
}

CCProtonPi0_TruthAnalyzer::CCProtonPi0_TruthAnalyzer() : CCProtonPi0_NTupleAnalysis()

{
    std::cout<<"Initializing TruthAnalyzer!"<<std::endl;

    // Required for MINERvA Framework Classes
    ROOT::Cintex::Cintex::Enable();

    // Open ROOT File
    rootDir = Folder_List::rootDir_Truth_mc;

    std::cout<<"\tRoot File: "<<rootDir<<std::endl;
    f = new TFile(rootDir.c_str(),"RECREATE");
 
    initHistograms();
    
    openTextFiles();

    resetCounters();

    std::cout<<"Finished"<<std::endl;
}

void CCProtonPi0_TruthAnalyzer::resetCounters() 
{
    nAll.name = "nAll";
    nFidVol.name = "nFidVol";
    nNoFidVol.name = "nNoFidVol";
    
    nFidVol_Signal.name = "nFidVol_Signal";
    nFidVol_Bckg.name = "nFidVol_Bckg";

    // Signal Type
    nQE.name = "nQuasi_Elastic";
    
    nRES_1232.name = "nSignal_RES_Delta";
    nRES_1535.name = "nSignal_RES_1535";
    nRES_1520.name = "nSignal_RES_1520";
    nRES_Other.name = "nSignal_RES_Other";
    
    nDIS_1_pi.name = "nSignal_DIS_1pi";
    nDIS_2_pi.name = "nSignal_DIS_2pi";
    nDIS_Multi_pi.name = "nSignal_DIS_Multi_pi";
    nDIS_Other.name = "nSignal_DIS_Other";
}


void CCProtonPi0_TruthAnalyzer::openTextFiles() 
{
    // Open TextFiles
    file_name = "TruthInfo.dat";
    textFile.open(file_name.c_str());
    if (!textFile.is_open()){
        std::cerr<<"Cannot open output text file: "<<file_name<<std::endl;
        exit(1);
    }else{
        std::cout<<"\t"<<file_name<<std::endl;
    }
}

void CCProtonPi0_TruthAnalyzer::initHistograms()
{
    // ------------------------------------------------------------------------
    // Muon Variables
    // ------------------------------------------------------------------------
    muon_P_mc_truth_all_signal = new MnvH1D( "muon_P_mc_truth_all_signal","Muon Momentum for Signal Events",binList.size_muon_P, binList.a_muon_P);
    muon_P_mc_truth_all_signal->GetXaxis()->SetTitle("Momentum [GeV]");
    muon_P_mc_truth_all_signal->GetYaxis()->SetTitle("N(Events)");
    AddVertErrorBand_Flux(muon_P_mc_truth_all_signal);
    AddVertErrorBand_Genie(muon_P_mc_truth_all_signal);

    muon_theta_mc_truth_all_signal = new MnvH1D( "muon_theta_mc_truth_all_signal","Pi0 Muon Theta for Signal Events",binList.size_muon_theta, binList.a_muon_theta);
    muon_theta_mc_truth_all_signal->GetXaxis()->SetTitle("Theta");
    muon_theta_mc_truth_all_signal->GetYaxis()->SetTitle("N(Events)");
    AddVertErrorBand_Flux(muon_theta_mc_truth_all_signal);
    AddVertErrorBand_Genie(muon_theta_mc_truth_all_signal);

    // ------------------------------------------------------------------------
    // Pi0 Variables
    // ------------------------------------------------------------------------
    pi0_P_mc_truth_all_signal = new MnvH1D( "pi0_P_mc_truth_all_signal","Pi0 Momentum for Signal Events",binList.size_pi0_P, binList.a_pi0_P);
    pi0_P_mc_truth_all_signal->GetXaxis()->SetTitle("Momentum [GeV]");
    pi0_P_mc_truth_all_signal->GetYaxis()->SetTitle("N(Events)");
    AddVertErrorBand_Flux(pi0_P_mc_truth_all_signal);
    AddVertErrorBand_Genie(pi0_P_mc_truth_all_signal);

    pi0_KE_mc_truth_all_signal = new MnvH1D( "pi0_KE_mc_truth_all_signal","Pi0 Kinetic Energy for Signal Events",binList.size_pi0_KE, binList.a_pi0_KE);
    pi0_KE_mc_truth_all_signal->GetXaxis()->SetTitle("Kinetic Energy [GeV]");
    pi0_KE_mc_truth_all_signal->GetYaxis()->SetTitle("N(Events)");
    AddVertErrorBand_Flux(pi0_KE_mc_truth_all_signal);
    AddVertErrorBand_Genie(pi0_KE_mc_truth_all_signal);

    pi0_theta_mc_truth_all_signal = new MnvH1D( "pi0_theta_mc_truth_all_signal","Theta for Signal Events",binList.size_pi0_theta, binList.a_pi0_theta);
    pi0_theta_mc_truth_all_signal->GetXaxis()->SetTitle("Theta");
    pi0_theta_mc_truth_all_signal->GetYaxis()->SetTitle("N(Events)");
    AddVertErrorBand_Flux(pi0_theta_mc_truth_all_signal);
    AddVertErrorBand_Genie(pi0_theta_mc_truth_all_signal);

    // ------------------------------------------------------------------------
    // Neutrino Energy & Q2
    // ------------------------------------------------------------------------
    neutrino_E_mc_truth_all_signal = new MnvH1D( "neutrino_E_mc_truth_all_signal","Neutrino Energy for Signal Events",binList.beamE.get_nBins(), binList.beamE.get_min(), binList.beamE.get_max());
    neutrino_E_mc_truth_all_signal->GetXaxis()->SetTitle("Neutrino Energy [GeV]");
    neutrino_E_mc_truth_all_signal->GetYaxis()->SetTitle("N(Events)");
    AddVertErrorBand_Flux(neutrino_E_mc_truth_all_signal);
    AddVertErrorBand_Genie(neutrino_E_mc_truth_all_signal);

    QSq_mc_truth_all_signal = new MnvH1D( "QSq_mc_truth_all_signal","Q^{2} for Signal Events",binList.QSq.get_nBins(), binList.QSq.get_min(), binList.QSq.get_max());
    QSq_mc_truth_all_signal->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    QSq_mc_truth_all_signal->GetYaxis()->SetTitle("N(Events)");
    AddVertErrorBand_Flux(QSq_mc_truth_all_signal);
    AddVertErrorBand_Genie(QSq_mc_truth_all_signal);

    // ------------------------------------------------------------------------
    // Signal Q2
    // ------------------------------------------------------------------------
    mc_Q2_QE = new TH1D("mc_Q2_QE","Q^{2} for Signal Events",binList.QSq.get_nBins(), binList.QSq.get_min(), binList.QSq.get_max());
    mc_Q2_QE->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    mc_Q2_QE->GetYaxis()->SetTitle("N(Events)");

    mc_Q2_RES_1232 = new TH1D("mc_Q2_RES_1232","Q^{2} for Signal Events",binList.QSq.get_nBins(), binList.QSq.get_min(), binList.QSq.get_max());
    mc_Q2_RES_1232->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    mc_Q2_RES_1232->GetYaxis()->SetTitle("N(Events)");

    mc_Q2_RES_1535 = new TH1D("mc_Q2_RES_1535","Q^{2} for Signal Events",binList.QSq.get_nBins(), binList.QSq.get_min(), binList.QSq.get_max());
    mc_Q2_RES_1535->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    mc_Q2_RES_1535->GetYaxis()->SetTitle("N(Events)");

    mc_Q2_RES_1520 = new TH1D("mc_Q2_RES_1520","Q^{2} for Signal Events",binList.QSq.get_nBins(), binList.QSq.get_min(), binList.QSq.get_max());
    mc_Q2_RES_1520->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    mc_Q2_RES_1520->GetYaxis()->SetTitle("N(Events)");

    mc_Q2_RES_Other = new TH1D("mc_Q2_RES_Other","Q^{2} for Signal Events",binList.QSq.get_nBins(), binList.QSq.get_min(), binList.QSq.get_max());
    mc_Q2_RES_Other->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    mc_Q2_RES_Other->GetYaxis()->SetTitle("N(Events)");

    mc_Q2_DIS_1_pi = new TH1D("mc_Q2_DIS_1_pi","Q^{2} for Signal Events",binList.QSq.get_nBins(), binList.QSq.get_min(), binList.QSq.get_max());
    mc_Q2_DIS_1_pi->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    mc_Q2_DIS_1_pi->GetYaxis()->SetTitle("N(Events)");

    mc_Q2_DIS_2_pi = new TH1D("mc_Q2_DIS_2_pi","Q^{2} for Signal Events",binList.QSq.get_nBins(), binList.QSq.get_min(), binList.QSq.get_max());
    mc_Q2_DIS_2_pi->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    mc_Q2_DIS_2_pi->GetYaxis()->SetTitle("N(Events)");

    mc_Q2_DIS_Multi_pi = new TH1D("mc_Q2_DIS_Multi_pi","Q^{2} for Signal Events",binList.QSq.get_nBins(), binList.QSq.get_min(), binList.QSq.get_max());
    mc_Q2_DIS_Multi_pi->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    mc_Q2_DIS_Multi_pi->GetYaxis()->SetTitle("N(Events)");

    mc_Q2_DIS_Other = new TH1D("mc_Q2_DIS_Other","Q^{2} for Signal Events",binList.QSq.get_nBins(), binList.QSq.get_min(), binList.QSq.get_max());
    mc_Q2_DIS_Other->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    mc_Q2_DIS_Other->GetYaxis()->SetTitle("N(Events)");

    // ------------------------------------------------------------------------
    // Signal incomingE
    // ------------------------------------------------------------------------
    mc_incomingE_QE = new TH1D("mc_incomingE_QE","E_{#nu} for Signal Events",binList.beamE.get_nBins(), binList.beamE.get_min(), binList.beamE.get_max());
    mc_incomingE_QE->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    mc_incomingE_QE->GetYaxis()->SetTitle("N(Events)");

    mc_incomingE_RES_1232 = new TH1D("mc_incomingE_RES_1232","E_{#nu} for Signal Events",binList.beamE.get_nBins(), binList.beamE.get_min(), binList.beamE.get_max());
    mc_incomingE_RES_1232->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    mc_incomingE_RES_1232->GetYaxis()->SetTitle("N(Events)");

    mc_incomingE_RES_1535 = new TH1D("mc_incomingE_RES_1535","E_{#nu} for Signal Events",binList.beamE.get_nBins(), binList.beamE.get_min(), binList.beamE.get_max());
    mc_incomingE_RES_1535->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    mc_incomingE_RES_1535->GetYaxis()->SetTitle("N(Events)");

    mc_incomingE_RES_1520 = new TH1D("mc_incomingE_RES_1520","E_{#nu} for Signal Events",binList.beamE.get_nBins(), binList.beamE.get_min(), binList.beamE.get_max());
    mc_incomingE_RES_1520->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    mc_incomingE_RES_1520->GetYaxis()->SetTitle("N(Events)");

    mc_incomingE_RES_Other = new TH1D("mc_incomingE_RES_Other","E_{#nu} for Signal Events",binList.beamE.get_nBins(), binList.beamE.get_min(), binList.beamE.get_max());
    mc_incomingE_RES_Other->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    mc_incomingE_RES_Other->GetYaxis()->SetTitle("N(Events)");

    mc_incomingE_DIS_1_pi = new TH1D("mc_incomingE_DIS_1_pi","E_{#nu} for Signal Events",binList.beamE.get_nBins(), binList.beamE.get_min(), binList.beamE.get_max());
    mc_incomingE_DIS_1_pi->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    mc_incomingE_DIS_1_pi->GetYaxis()->SetTitle("N(Events)");

    mc_incomingE_DIS_2_pi = new TH1D("mc_incomingE_DIS_2_pi","E_{#nu} for Signal Events",binList.beamE.get_nBins(), binList.beamE.get_min(), binList.beamE.get_max());
    mc_incomingE_DIS_2_pi->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    mc_incomingE_DIS_2_pi->GetYaxis()->SetTitle("N(Events)");

    mc_incomingE_DIS_Multi_pi = new TH1D("mc_incomingE_DIS_Multi_pi","E_{#nu} for Signal Events",binList.beamE.get_nBins(), binList.beamE.get_min(), binList.beamE.get_max());
    mc_incomingE_DIS_Multi_pi->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    mc_incomingE_DIS_Multi_pi->GetYaxis()->SetTitle("N(Events)");

    mc_incomingE_DIS_Other = new TH1D("mc_incomingE_DIS_Other","E_{#nu} for Signal Events",binList.beamE.get_nBins(), binList.beamE.get_min(), binList.beamE.get_max());
    mc_incomingE_DIS_Other->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    mc_incomingE_DIS_Other->GetYaxis()->SetTitle("N(Events)");

    // ------------------------------------------------------------------------
    // Signal w
    // ------------------------------------------------------------------------
    mc_w_QE = new TH1D("mc_w_QE","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    mc_w_QE->GetXaxis()->SetTitle("W [GeV]");
    mc_w_QE->GetYaxis()->SetTitle("N(Events)");
 
    mc_w_RES_1232 = new TH1D("mc_w_RES_1232","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    mc_w_RES_1232->GetXaxis()->SetTitle("W [GeV]");
    mc_w_RES_1232->GetYaxis()->SetTitle("N(Events)");

    mc_w_RES_1535 = new TH1D("mc_w_RES_1535","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    mc_w_RES_1535->GetXaxis()->SetTitle("W [GeV]");
    mc_w_RES_1535->GetYaxis()->SetTitle("N(Events)");

    mc_w_RES_1520 = new TH1D("mc_w_RES_1520","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    mc_w_RES_1520->GetXaxis()->SetTitle("W [GeV]");
    mc_w_RES_1520->GetYaxis()->SetTitle("N(Events)");

    mc_w_RES_Other = new TH1D("mc_w_RES_Other","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    mc_w_RES_Other->GetXaxis()->SetTitle("W [GeV]");
    mc_w_RES_Other->GetYaxis()->SetTitle("N(Events)");

    mc_w_DIS_1_pi = new TH1D("mc_w_DIS_1_pi","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    mc_w_DIS_1_pi->GetXaxis()->SetTitle("W [GeV]");
    mc_w_DIS_1_pi->GetYaxis()->SetTitle("N(Events)");

    mc_w_DIS_2_pi = new TH1D("mc_w_DIS_2_pi","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    mc_w_DIS_2_pi->GetXaxis()->SetTitle("W [GeV]");
    mc_w_DIS_2_pi->GetYaxis()->SetTitle("N(Events)");

    mc_w_DIS_Multi_pi = new TH1D("mc_w_DIS_Multi_pi","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    mc_w_DIS_Multi_pi->GetXaxis()->SetTitle("W [GeV]");
    mc_w_DIS_Multi_pi->GetYaxis()->SetTitle("N(Events)");

    mc_w_DIS_Other = new TH1D("mc_w_DIS_Other","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    mc_w_DIS_Other->GetXaxis()->SetTitle("W [GeV]");
    mc_w_DIS_Other->GetYaxis()->SetTitle("N(Events)");

}

void CCProtonPi0_TruthAnalyzer::FillHistogram(MnvH1D* hist, double var)
{
    hist->Fill(var, cvweight);
    FillVertErrorBand_Flux(hist, var);
    FillVertErrorBand_Genie(hist, var);
}

void CCProtonPi0_TruthAnalyzer::FillHistogram(TH1D* hist, double var)
{
    hist->Fill(var, cvweight);
}

void CCProtonPi0_TruthAnalyzer::FillVertErrorBand_Flux(MnvH1D* h, double var)
{
    std::vector<double> flux_errors = GetFluxError();
    h->FillVertErrorBand("Flux",  var, &flux_errors[0],  cvweight, 1.0);
}

void CCProtonPi0_TruthAnalyzer::FillVertErrorBand_Genie(MnvH1D* h, double var)
{
    h->FillVertErrorBand("GENIE_AGKYxF1pi"         ,var, truth_genie_wgt_AGKYxF1pi[2]        , truth_genie_wgt_AGKYxF1pi[4]        , cvweight);
    h->FillVertErrorBand("GENIE_AhtBY"             ,var, truth_genie_wgt_AhtBY[2]            , truth_genie_wgt_AhtBY[4]            , cvweight);
    h->FillVertErrorBand("GENIE_BhtBY"             ,var, truth_genie_wgt_BhtBY[2]            , truth_genie_wgt_BhtBY[4]            , cvweight);
    h->FillVertErrorBand("GENIE_CCQEPauliSupViaKF" ,var, truth_genie_wgt_CCQEPauliSupViaKF[2], truth_genie_wgt_CCQEPauliSupViaKF[4], cvweight);
    h->FillVertErrorBand("GENIE_CV1uBY"            ,var, truth_genie_wgt_CV1uBY[2]           , truth_genie_wgt_CV1uBY[4]           , cvweight);
    h->FillVertErrorBand("GENIE_CV2uBY"            ,var, truth_genie_wgt_CV2uBY[2]           , truth_genie_wgt_CV2uBY[4]           , cvweight);
    h->FillVertErrorBand("GENIE_EtaNCEL"           ,var, truth_genie_wgt_EtaNCEL[2]          , truth_genie_wgt_EtaNCEL[4]          , cvweight);
    h->FillVertErrorBand("GENIE_FrAbs_N"           ,var, truth_genie_wgt_FrAbs_N[2]          , truth_genie_wgt_FrAbs_N[4]          , cvweight);
    h->FillVertErrorBand("GENIE_FrAbs_pi"          ,var, truth_genie_wgt_FrAbs_pi[2]         , truth_genie_wgt_FrAbs_pi[4]         , cvweight);
    h->FillVertErrorBand("GENIE_FrCEx_N"           ,var, truth_genie_wgt_FrCEx_N[2]          , truth_genie_wgt_FrCEx_N[4]          , cvweight);
    h->FillVertErrorBand("GENIE_FrCEx_pi"          ,var, truth_genie_wgt_FrCEx_pi[2]         , truth_genie_wgt_FrCEx_pi[4]         , cvweight);
    h->FillVertErrorBand("GENIE_FrElas_N"          ,var, truth_genie_wgt_FrElas_N[2]         , truth_genie_wgt_FrElas_N[4]         , cvweight);
    h->FillVertErrorBand("GENIE_FrElas_pi"         ,var, truth_genie_wgt_FrElas_pi[2]        , truth_genie_wgt_FrElas_pi[4]        , cvweight);
    h->FillVertErrorBand("GENIE_FrInel_N"          ,var, truth_genie_wgt_FrInel_N[2]         , truth_genie_wgt_FrInel_N[4]         , cvweight);
    h->FillVertErrorBand("GENIE_FrInel_pi"         ,var, truth_genie_wgt_FrInel_pi[2]        , truth_genie_wgt_FrInel_pi[4]        , cvweight);
    h->FillVertErrorBand("GENIE_FrPiProd_N"        ,var, truth_genie_wgt_FrPiProd_N[2]       , truth_genie_wgt_FrPiProd_N[4]       , cvweight);
    h->FillVertErrorBand("GENIE_FrPiProd_pi"       ,var, truth_genie_wgt_FrPiProd_pi[2]      , truth_genie_wgt_FrPiProd_pi[4]      , cvweight);
    h->FillVertErrorBand("GENIE_MFP_N"             ,var, truth_genie_wgt_MFP_N[2]            , truth_genie_wgt_MFP_N[4]            , cvweight);
    h->FillVertErrorBand("GENIE_MFP_pi"            ,var, truth_genie_wgt_MFP_pi[2]           , truth_genie_wgt_MFP_pi[4]           , cvweight);
    h->FillVertErrorBand("GENIE_MaCCQE"            ,var, truth_genie_wgt_MaCCQE[2]           , truth_genie_wgt_MaCCQE[4]           , cvweight);
    h->FillVertErrorBand("GENIE_MaCCQEshape"       ,var, truth_genie_wgt_MaCCQEshape[2]      , truth_genie_wgt_MaCCQEshape[4]      , cvweight);
    h->FillVertErrorBand("GENIE_MaNCEL"            ,var, truth_genie_wgt_MaNCEL[2]           , truth_genie_wgt_MaNCEL[4]           , cvweight);
    h->FillVertErrorBand("GENIE_MaRES"             ,var, truth_genie_wgt_MaRES[2]            , truth_genie_wgt_MaRES[4]            , cvweight);
    h->FillVertErrorBand("GENIE_MvRES"             ,var, truth_genie_wgt_MvRES[2]            , truth_genie_wgt_MvRES[4]            , cvweight);
    h->FillVertErrorBand("GENIE_NormCCQE"          ,var, truth_genie_wgt_NormCCQE[2]         , truth_genie_wgt_NormCCQE[4]         , cvweight);
    h->FillVertErrorBand("GENIE_NormCCRES"         ,var, truth_genie_wgt_NormCCRES[2]        , truth_genie_wgt_NormCCRES[4]        , cvweight);
    h->FillVertErrorBand("GENIE_NormDISCC"         ,var, truth_genie_wgt_NormDISCC[2]        , truth_genie_wgt_NormDISCC[4]        , cvweight);
    h->FillVertErrorBand("GENIE_NormNCRES"         ,var, truth_genie_wgt_NormNCRES[2]        , truth_genie_wgt_NormNCRES[4]        , cvweight);
    h->FillVertErrorBand("GENIE_RDecBR1gamma"      ,var, truth_genie_wgt_RDecBR1gamma[2]     , truth_genie_wgt_RDecBR1gamma[4]     , cvweight);
    h->FillVertErrorBand("GENIE_Rvn1pi"            ,var, truth_genie_wgt_Rvn1pi[2]           , truth_genie_wgt_Rvn1pi[4]           , cvweight);
    h->FillVertErrorBand("GENIE_Rvn2pi"            ,var, truth_genie_wgt_Rvn2pi[2]           , truth_genie_wgt_Rvn2pi[4]           , cvweight);
    h->FillVertErrorBand("GENIE_Rvp1pi"            ,var, truth_genie_wgt_Rvp1pi[2]           , truth_genie_wgt_Rvp1pi[4]           , cvweight);
    h->FillVertErrorBand("GENIE_Rvp2pi"            ,var, truth_genie_wgt_Rvp2pi[2]           , truth_genie_wgt_Rvp2pi[4]           , cvweight);
    h->FillVertErrorBand("GENIE_Theta_Delta2Npi"   ,var, truth_genie_wgt_Theta_Delta2Npi[2]  , truth_genie_wgt_Theta_Delta2Npi[4]  , cvweight);
    h->FillVertErrorBand("GENIE_VecFFCCQEshape"    ,var, truth_genie_wgt_VecFFCCQEshape[2]   , truth_genie_wgt_VecFFCCQEshape[4]   , cvweight);
}

void CCProtonPi0_TruthAnalyzer::Calc_WeightFromSystematics()
{
    UpdateFluxReweighter(mc_run); 
        
    // Replace cvweight with Flux Weight
    cvweight = GetFluxWeight();
}

double CCProtonPi0_TruthAnalyzer::GetFluxWeight()
{
    double Enu = mc_incomingE * MeV_to_GeV; // true neutrino energy (GeV)
    int nuPDG = mc_incoming; //neutrino PDG code

    double flux_weight = frw->GetFluxCVWeight(Enu, nuPDG);

    return flux_weight;
}

std::vector<double> CCProtonPi0_TruthAnalyzer::GetFluxError()
{
    double Enu = mc_incomingE * MeV_to_GeV; // true neutrino energy (GeV)
    int nuPDG = mc_incoming; //neutrino PDG code

    std::vector<double> flux_error = frw->GetFluxErrorWeights(Enu, nuPDG);

    return flux_error;
}

void CCProtonPi0_TruthAnalyzer::AddOtherErrorBands_FillWithCV()
{
    AddErrorBands_FillWithCV(muon_P_mc_truth_all_signal);
    AddErrorBands_FillWithCV(muon_theta_mc_truth_all_signal);
    AddErrorBands_FillWithCV(pi0_P_mc_truth_all_signal);
    AddErrorBands_FillWithCV(pi0_KE_mc_truth_all_signal);
    AddErrorBands_FillWithCV(pi0_theta_mc_truth_all_signal);
    AddErrorBands_FillWithCV(neutrino_E_mc_truth_all_signal);
    AddErrorBands_FillWithCV(QSq_mc_truth_all_signal);
}

void CCProtonPi0_TruthAnalyzer::AddErrorBands_FillWithCV(MnvH1D* hist)
{
    AddVertErrorBandAndFillWithCV_MuonTracking(hist);
}

void CCProtonPi0_TruthAnalyzer::FillSignalHistograms()
{
    Calc_WeightFromSystematics();

    // Cross Section Variables
    FillHistogram(muon_P_mc_truth_all_signal, truth_muon_P * MeV_to_GeV);
    FillHistogram(muon_theta_mc_truth_all_signal, truth_muon_theta * rad_to_deg);
    FillHistogram(pi0_P_mc_truth_all_signal, truth_pi0_P * MeV_to_GeV);
    FillHistogram(pi0_KE_mc_truth_all_signal, truth_pi0_KE * MeV_to_GeV);
    FillHistogram(pi0_theta_mc_truth_all_signal, truth_pi0_theta * rad_to_deg);
    FillHistogram(neutrino_E_mc_truth_all_signal, mc_incomingE * MeV_to_GeV);
    FillHistogram(QSq_mc_truth_all_signal, mc_Q2 * MeVSq_to_GeVSq);
    
    // Signal Characteristics
    if (mc_intType == 1){
        nQE.count++;
        FillHistogram(mc_Q2_QE, mc_Q2 * MeVSq_to_GeVSq);
        FillHistogram(mc_incomingE_QE, mc_incomingE * MeV_to_GeV);
        FillHistogram(mc_w_QE, mc_w * MeV_to_GeV);
    }else if (mc_intType == 2){
        if (mc_resID == 0){ 
            nRES_1232.count++; 
            FillHistogram(mc_Q2_RES_1232, mc_Q2 * MeVSq_to_GeVSq);
            FillHistogram(mc_incomingE_RES_1232, mc_incomingE * MeV_to_GeV);
            FillHistogram(mc_w_RES_1232, mc_w * MeV_to_GeV);
        }else if (mc_resID == 1){
            nRES_1535.count++; 
            FillHistogram(mc_Q2_RES_1535, mc_Q2 * MeVSq_to_GeVSq);
            FillHistogram(mc_incomingE_RES_1535, mc_incomingE * MeV_to_GeV);
            FillHistogram(mc_w_RES_1535, mc_w * MeV_to_GeV);
        }else if (mc_resID == 2){
            nRES_1520.count++; 
            FillHistogram(mc_Q2_RES_1520, mc_Q2 * MeVSq_to_GeVSq);
            FillHistogram(mc_incomingE_RES_1520, mc_incomingE * MeV_to_GeV);
            FillHistogram(mc_w_RES_1520, mc_w * MeV_to_GeV);
        }else{
            nRES_Other.count++; 
            FillHistogram(mc_Q2_RES_Other, mc_Q2 * MeVSq_to_GeVSq);
            FillHistogram(mc_incomingE_RES_Other, mc_incomingE * MeV_to_GeV);
            FillHistogram(mc_w_RES_Other, mc_w * MeV_to_GeV);
        }
    }else if (mc_intType == 3){
        int nFS_pions = Get_nFS_pions();
        if (nFS_pions == 1){
            nDIS_1_pi.count++;
            FillHistogram(mc_Q2_DIS_1_pi, mc_Q2 * MeVSq_to_GeVSq);
            FillHistogram(mc_incomingE_DIS_1_pi, mc_incomingE * MeV_to_GeV);
            FillHistogram(mc_w_DIS_1_pi, mc_w * MeV_to_GeV);
        }else if (nFS_pions == 2){
            nDIS_2_pi.count++;
            FillHistogram(mc_Q2_DIS_2_pi, mc_Q2 * MeVSq_to_GeVSq);
            FillHistogram(mc_incomingE_DIS_2_pi, mc_incomingE * MeV_to_GeV);
            FillHistogram(mc_w_DIS_2_pi, mc_w * MeV_to_GeV);
        }else if (nFS_pions > 2){
            nDIS_Multi_pi.count++;
            FillHistogram(mc_Q2_DIS_Multi_pi, mc_Q2 * MeVSq_to_GeVSq);
            FillHistogram(mc_incomingE_DIS_Multi_pi, mc_incomingE * MeV_to_GeV);
            FillHistogram(mc_w_DIS_Multi_pi, mc_w * MeV_to_GeV);
        }else{
            nDIS_Other.count++;
            FillHistogram(mc_Q2_DIS_Other, mc_Q2 * MeVSq_to_GeVSq);
            FillHistogram(mc_incomingE_DIS_Other, mc_incomingE * MeV_to_GeV);
            FillHistogram(mc_w_DIS_Other, mc_w * MeV_to_GeV);
        }
    }else{
        std::cout<<"WARNING! Signal Event with different interaction Type!"<<std::endl;
    }
}

void CCProtonPi0_TruthAnalyzer::UpdateSignalDef()
{
    // Signal Definition with Neutrino Energy
    if (truth_isSignal){
        bool isEnu_inRange = mc_incomingE >= min_Enu && mc_incomingE <= max_Enu; 
        truth_isSignal = isEnu_inRange;
        // If event no longer a signal due to Enu Range -- it is background
        if (!truth_isSignal){
            truth_isBckg_Compact_WithPi0 = true;
            truth_isBckg_SinglePi0 = true;
            truth_isBckg_Other = true;
        }
    }
}

int CCProtonPi0_TruthAnalyzer::Get_nFS_pions()
{
    int nFS_pions = 0;

    for (int i = 0; i < mc_er_nPart; ++i){
        if (std::abs(mc_er_ID[i]) == 211 || mc_er_ID[i] == 111){
            if (isMother_DIS_Fragment(i)) nFS_pions++;
        }
    }

    return nFS_pions;
}

bool CCProtonPi0_TruthAnalyzer::isMother_DIS_Fragment(int ind)
{
    int mother_ind = mc_er_mother[ind];

    if (mc_er_status[mother_ind] == 12) return true;
    else return false;
}

void CCProtonPi0_TruthAnalyzer::PrintEventRecord()
{
    std::cout<<"-------------"<<std::endl;
    for (int i = 0; i < mc_er_nPart; ++i){
        std::cout<<mc_er_ID[i]<<" "<<mc_er_status[i]<<" "<<mc_er_mother[i]<<std::endl;
    }
}

void CCProtonPi0_TruthAnalyzer::writeHistograms()
{
    f->cd();

    // Cross Section Variables
    muon_P_mc_truth_all_signal->Write();
    muon_theta_mc_truth_all_signal->Write();
    pi0_P_mc_truth_all_signal->Write();
    pi0_KE_mc_truth_all_signal->Write();
    pi0_theta_mc_truth_all_signal->Write();
    neutrino_E_mc_truth_all_signal->Write();
    QSq_mc_truth_all_signal->Write();

    // Signal Q2
    mc_Q2_QE->Write();
    mc_Q2_RES_1232->Write();
    mc_Q2_RES_1535->Write();
    mc_Q2_RES_1520->Write();
    mc_Q2_RES_Other->Write();
   
    mc_Q2_DIS_1_pi->Write();
    mc_Q2_DIS_2_pi->Write();
    mc_Q2_DIS_Multi_pi->Write();
    mc_Q2_DIS_Other->Write();
 
    // Signal incomingE
    mc_incomingE_QE->Write();
    mc_incomingE_RES_1232->Write();
    mc_incomingE_RES_1535->Write();
    mc_incomingE_RES_1520->Write();
    mc_incomingE_RES_Other->Write();
   
    mc_incomingE_DIS_1_pi->Write();
    mc_incomingE_DIS_2_pi->Write();
    mc_incomingE_DIS_Multi_pi->Write();
    mc_incomingE_DIS_Other->Write();
 
    // Signal w
    mc_w_QE->Write();
    mc_w_RES_1232->Write();
    mc_w_RES_1535->Write();
    mc_w_RES_1520->Write();
    mc_w_RES_Other->Write();
   
    mc_w_DIS_1_pi->Write();
    mc_w_DIS_2_pi->Write();
    mc_w_DIS_Multi_pi->Write();
    mc_w_DIS_Other->Write();
  
    
    f->Close();
}

#endif


