/*
   See CutList.h header for Class Information
   */
#ifndef CCProtonPi0_CutList_cpp
#define CCProtonPi0_CutList_cpp

#include "CCProtonPi0_CutList.h"

using namespace PlotUtils;

CCProtonPi0_CutList::CCProtonPi0_CutList(bool isModeReduce, bool isMC) : CCProtonPi0_NTupleAnalysis()
{
    std::cout<<"Initializing CCProtonPi0_CutList"<<std::endl;

    m_isMC = isMC;

    if(isModeReduce){
        // File Locations
        if (isMC) rootDir = Folder_List::rootDir_CutHists_mc;
        else rootDir = Folder_List::rootDir_CutHists_data;       

        std::cout<<"\tRoot File: "<<rootDir<<std::endl;

        // Create Root File 
        f = new TFile(rootDir.c_str(),"RECREATE");
        //f = new TFile(rootDir.c_str(),"CREATE");
        if (!f->IsOpen()){
            std::cout<<"File already exists! Exiting!..."<<std::endl;
            exit(1);
        }

        use_nTrueSignal = true;
        nTrueSignal = 244010.0;

        SetCutNames();
        OpenTextFiles(isMC);

        initHistograms();
    }else{
        std::cout<<"\tNTuple Analysis Mode -- Will not create ROOT & Text Files"<<std::endl;
    }

    std::cout<<"Done!"<<std::endl;
}

/*
 *  Histograms
 *      See Following Page for Histogram Indices
 *          https://cdcvs.fnal.gov/redmine/projects/minerva/wiki/Ozgur's_scratch_page
 * */
void CCProtonPi0_CutList::initHistograms()
{
    MnvH1D* temp = NULL;

    for (int i = 0; i < nHistograms; i++){
        // --------------------------------------------------------------------
        // Common
        // --------------------------------------------------------------------
        temp = new MnvH1D( Form("%s_%d","hCut_nVertices",i),"N(Vertices)",binList.multiplicity.get_nBins(), binList.multiplicity.get_min(), binList.multiplicity.get_max() );
        temp->GetXaxis()->SetTitle("N(Vertex)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        hCut_nVertices.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_nTracks",i),"N(Tracks)",binList.multiplicity.get_nBins(), binList.multiplicity.get_min(), binList.multiplicity.get_max() );
        temp->GetXaxis()->SetTitle("N(Tracks)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        hCut_nTracks.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_nTracks2",i),"N(Tracks_Close) + N(Tracks_Far)",binList.multiplicity.get_nBins(), binList.multiplicity.get_min(), binList.multiplicity.get_max() );
        temp->GetXaxis()->SetTitle("N(Tracks_Close) + N(Tracks_Far) - should be same with nTracks");
        temp->GetYaxis()->SetTitle("Events/Bin");
        hCut_nTracks2.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_nTracks_Close",i),"N(Close Tracks)",binList.multiplicity.get_nBins(), binList.multiplicity.get_min(), binList.multiplicity.get_max() );
        temp->GetXaxis()->SetTitle("N(Tracks_Close)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        hCut_nTracks_Close.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_nTracks_Far",i),"N(Far Tracks)",binList.multiplicity.get_nBins(), binList.multiplicity.get_min(), binList.multiplicity.get_max() );
        temp->GetXaxis()->SetTitle("N(Tracks_Far)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        hCut_nTracks_Far.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_nTracks_Discarded",i),"N(Tracks Discarded)",binList.multiplicity.get_nBins(), binList.multiplicity.get_min(), binList.multiplicity.get_max() );
        temp->GetXaxis()->SetTitle("N(Tracks_Discarded)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        hCut_nTracks_Discarded.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_Michel",i),"Michel Electrons",binList.true_false.get_nBins(), binList.true_false.get_min(), binList.true_false.get_max() );
        temp->GetXaxis()->SetTitle("0 = No Michel, 1 = Michel");
        temp->GetYaxis()->SetTitle("Events/Bin");
        hCut_Michel.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_nProtonCandidates",i),"N(Proton Candidates)",binList.multiplicity.get_nBins(), binList.multiplicity.get_min(), binList.multiplicity.get_max() );
        temp->GetXaxis()->SetTitle("N(Proton Candidates)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        hCut_nProtonCandidates.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_nShowerCandidates",i),"N(Shower Candidates)",binList.multiplicity.get_nBins(), binList.multiplicity.get_min(), binList.multiplicity.get_max() );
        temp->GetXaxis()->SetTitle("N(Shower Candidates)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        hCut_nShowerCandidates.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_pi0invMass",i),"Pi0 InvMass",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
        temp->GetXaxis()->SetTitle("M_{#gamma#gamma} (MeV)");
        temp->GetYaxis()->SetTitle("Events/ (10 MeV)");
        if (m_isMC){
            AddVertErrorBands_MC(temp);
            AddLatErrorBands_MC(temp);
        }
        hCut_pi0invMass.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_W",i),"Hadronic Invariant Mass W", 30, 0.0, 3.0);
        temp->GetXaxis()->SetTitle("W [GeV]");
        temp->GetYaxis()->SetTitle("Events/Bin");
        hCut_W.push_back(temp);

        // --------------------------------------------------------------------
        // 1 Track
        // --------------------------------------------------------------------
        temp = new MnvH1D( Form("%s_%d","hCut_1Track_nShowerCandidates",i),"N(Shower Candidates)",binList.multiplicity.get_nBins(), binList.multiplicity.get_min(), binList.multiplicity.get_max() );
        temp->GetXaxis()->SetTitle("N(Shower Candidates)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        hCut_1Track_nShowerCandidates.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_1Track_eVis_nuclearTarget",i),"Visible Energy in Nuclear Target",binList.eVis_nuclearTarget.get_nBins(), binList.eVis_nuclearTarget.get_min(), binList.eVis_nuclearTarget.get_max() );
        temp->GetXaxis()->SetTitle("Visible Energy in Nuclear Target [MeV]");
        temp->GetYaxis()->SetTitle(Form("Candidates / %3.2f ",binList.eVis_nuclearTarget.get_width()));
        hCut_1Track_eVis_nuclearTarget.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_1Track_eVis_other",i),"Visible Energy in Tracker + ECAL + HCAL",binList.eVis_other.get_nBins(), binList.eVis_other.get_min(), binList.eVis_other.get_max() );
        temp->GetXaxis()->SetTitle("Visible Energy in Tracker + ECAL + HCAL [MeV]");
        temp->GetYaxis()->SetTitle(Form("Candidates / %3.2f ",binList.eVis_other.get_width()));
        hCut_1Track_eVis_other.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_1Track_gamma1ConvDist",i),"Leading Photon Conversion Distance",binList.bin_photonConvLength.get_nBins(), binList.bin_photonConvLength.get_min(), binList.bin_photonConvLength.get_max() );
        temp->GetXaxis()->SetTitle("Leading Photon Conversion Distance");
        temp->GetYaxis()->SetTitle(Form("Candidates / %3.2f [cm]",binList.bin_photonConvLength.get_width()));
        hCut_1Track_gamma1ConvDist.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_1Track_gamma2ConvDist",i),"Second Photon Conversion Distance",binList.bin_photonConvLength.get_nBins(), binList.bin_photonConvLength.get_min(), binList.bin_photonConvLength.get_max() );
        temp->GetXaxis()->SetTitle("Second Photon Conversion Distance");
        temp->GetYaxis()->SetTitle(Form("Candidates / %3.2f [cm]",binList.bin_photonConvLength.get_width()));
        hCut_1Track_gamma2ConvDist.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_1Track_pi0invMass",i),"Reconstructed Pi0 Invariant Mass",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
        temp->GetXaxis()->SetTitle("Reconstructed Pi0 Invariant Mass [MeV]");
        temp->GetYaxis()->SetTitle(Form("Candidates / %3.2f [MeV]",binList.pi0_invMass.get_width()));
        hCut_1Track_pi0invMass.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_1Track_neutrinoE",i),"Reconstructed Beam Energy",binList.beamE.get_nBins(), binList.beamE.get_min(), binList.beamE.get_max() );
        temp->GetXaxis()->SetTitle("Reconstructed Beam Energy [GeV]");
        temp->GetYaxis()->SetTitle(Form("Candidates / %3.2f ",binList.beamE.get_width()));
        hCut_1Track_neutrinoE.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_1Track_W",i),"Hadronic Invariant Mass W", 30, 0.0, 3.0);
        temp->GetXaxis()->SetTitle("W [GeV]");
        temp->GetYaxis()->SetTitle("Events/Bin");
        hCut_1Track_W.push_back(temp);

        // --------------------------------------------------------------------
        // 2 Track
        // --------------------------------------------------------------------
        temp = new MnvH1D( Form("%s_%d","hCut_2Track_nShowerCandidates",i),"N(Shower Candidates)",binList.multiplicity.get_nBins(), binList.multiplicity.get_min(), binList.multiplicity.get_max() );
        temp->GetXaxis()->SetTitle("N(Shower Candidates)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        hCut_2Track_nShowerCandidates.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_2Track_eVis_nuclearTarget",i),"Visible Energy in Nuclear Target",binList.eVis_nuclearTarget.get_nBins(), binList.eVis_nuclearTarget.get_min(), binList.eVis_nuclearTarget.get_max() );
        temp->GetXaxis()->SetTitle("Visible Energy in Nuclear Target [MeV]");
        temp->GetYaxis()->SetTitle(Form("Candidates / %3.2f ",binList.eVis_nuclearTarget.get_width()));
        hCut_2Track_eVis_nuclearTarget.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_2Track_eVis_other",i),"Visible Energy in Tracker + ECAL + HCAL",binList.eVis_other.get_nBins(), binList.eVis_other.get_min(), binList.eVis_other.get_max() );
        temp->GetXaxis()->SetTitle("Visible Energy in Tracker + ECAL + HCAL [MeV]");
        temp->GetYaxis()->SetTitle(Form("Candidates / %3.2f ",binList.eVis_other.get_width()));
        hCut_2Track_eVis_other.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_2Track_gamma1ConvDist",i),"Leading Photon Conversion Distance",binList.bin_photonConvLength.get_nBins(), binList.bin_photonConvLength.get_min(), binList.bin_photonConvLength.get_max() );
        temp->GetXaxis()->SetTitle("Leading Photon Conversion Distance");
        temp->GetYaxis()->SetTitle(Form("Candidates / %3.2f [cm]",binList.bin_photonConvLength.get_width()));
        hCut_2Track_gamma1ConvDist.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_2Track_gamma2ConvDist",i),"Second Photon Conversion Distance",binList.bin_photonConvLength.get_nBins(), binList.bin_photonConvLength.get_min(), binList.bin_photonConvLength.get_max() );
        temp->GetXaxis()->SetTitle("Second Photon Conversion Distance");
        temp->GetYaxis()->SetTitle(Form("Candidates / %3.2f [cm]",binList.bin_photonConvLength.get_width()));
        hCut_2Track_gamma2ConvDist.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_2Track_pi0invMass",i),"Reconstructed Pi0 Invariant Mass",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
        temp->GetXaxis()->SetTitle("Reconstructed Pi0 Invariant Mass [MeV]");
        temp->GetYaxis()->SetTitle(Form("Candidates / %3.2f [MeV]",binList.pi0_invMass.get_width()));
        hCut_2Track_pi0invMass.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_2Track_neutrinoE",i),"Reconstructed Beam Energy",binList.beamE.get_nBins(), binList.beamE.get_min(), binList.beamE.get_max() );
        temp->GetXaxis()->SetTitle("Reconstructed Beam Energy [GeV]");
        temp->GetYaxis()->SetTitle(Form("Candidates / %3.2f ",binList.beamE.get_width()));
        hCut_2Track_neutrinoE.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_2Track_protonScore_LLR",i),"proton_protonScore_LLR",binList.particleScore_LLR.get_nBins(), binList.particleScore_LLR.get_min(), binList.particleScore_LLR.get_max() );
        temp->GetXaxis()->SetTitle("proton_protonScore_LLR");
        temp->GetYaxis()->SetTitle(Form("Candidates / %3.2f ",binList.particleScore_LLR.get_width()));
        hCut_2Track_protonScore_LLR.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_2Track_deltaInvMass",i),"deltaInvMass",binList.deltaInvMass.get_nBins(), binList.deltaInvMass.get_min(), binList.deltaInvMass.get_max() );
        temp->GetXaxis()->SetTitle("hCut_2Track_deltaInvMass");
        temp->GetYaxis()->SetTitle(Form("Candidates / %3.2f ",binList.deltaInvMass.get_width()));
        hCut_2Track_deltaInvMass.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","hCut_2Track_W",i),"Hadronic Invariant Mass W", 30, 0.0, 3.0);
        temp->GetXaxis()->SetTitle("W [GeV]");
        temp->GetYaxis()->SetTitle("Events/Bin");
        hCut_2Track_W.push_back(temp);

        // --------------------------------------------------------------------
        // Side Bands
        // --------------------------------------------------------------------
        temp = new MnvH1D( Form("%s_%d","SideBand_muon_P",i),"Muon Momentum",binList.size_muon_P, binList.a_muon_P);
        temp->GetXaxis()->SetTitle("Muon Momentum [GeV]");
        temp->GetYaxis()->SetTitle("Events/Bin");
        if (m_isMC){
            AddVertErrorBands_MC(temp);
        }
        SideBand_muon_P.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","SideBand_muon_theta",i),"Muon Theta",binList.size_muon_theta, binList.a_muon_theta);
        temp->GetXaxis()->SetTitle("Muon Theta [degree]");
        temp->GetYaxis()->SetTitle("Events/Bin");
        if (m_isMC){
            AddVertErrorBands_MC(temp);
        }
        SideBand_muon_theta.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","SideBand_pi0_P",i),"#pi^{0} Momentum",binList.size_pi0_P, binList.a_pi0_P);
        temp->GetXaxis()->SetTitle("#pi^{0} Momentum [GeV]");
        temp->GetYaxis()->SetTitle("Events/Bin");
        if (m_isMC){
            AddVertErrorBands_MC(temp);
        }
        SideBand_pi0_P.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","SideBand_pi0_KE",i),"#pi^{0} Kinetic Energy",binList.size_pi0_KE, binList.a_pi0_KE);
        temp->GetXaxis()->SetTitle("#pi^{0} Kinetic Energy [GeV]");
        temp->GetYaxis()->SetTitle("Events/Bin");
        if (m_isMC){
            AddVertErrorBands_MC(temp);
        }
        SideBand_pi0_KE.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","SideBand_pi0_theta",i),"#pi^{0} Theta",binList.size_pi0_theta, binList.a_pi0_theta);
        temp->GetXaxis()->SetTitle("#pi^{0} Theta [degree]");
        temp->GetYaxis()->SetTitle("Events/Bin");
        if (m_isMC){
            AddVertErrorBands_MC(temp);
        }
        SideBand_pi0_theta.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","SideBand_neutrino_E",i),"Neutrino Energy",binList.size_Enu, binList.a_Enu);
        temp->GetXaxis()->SetTitle("Neutrino Energy [GeV]");
        temp->GetYaxis()->SetTitle("Events/Bin");
        if (m_isMC){
            AddVertErrorBands_MC(temp);
        }
        SideBand_neutrino_E.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","SideBand_QSq",i),"Q^{2}",binList.size_QSq, binList.a_QSq);
        temp->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
        temp->GetYaxis()->SetTitle("Events/Bin");
        if (m_isMC){
            AddVertErrorBands_MC(temp);
        }
        SideBand_QSq.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","SideBand_W",i),"W",binList.w.get_nBins(), binList.w.get_min(), binList.w.get_max() );
        temp->GetXaxis()->SetTitle("W [GeV]");
        temp->GetYaxis()->SetTitle("Events/Bin");
        if (m_isMC){
            AddVertErrorBands_MC(temp);
        }
        SideBand_W.push_back(temp);

    }

    // ------------------------------------------------------------------------
    // Signal Q2
    // ------------------------------------------------------------------------
    truth_QSq_QE = new TH1D("truth_QSq_QE","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    truth_QSq_QE->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    truth_QSq_QE->GetYaxis()->SetTitle("Events/Bin");

    truth_QSq_RES_1232 = new TH1D("truth_QSq_RES_1232","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    truth_QSq_RES_1232->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    truth_QSq_RES_1232->GetYaxis()->SetTitle("Events/Bin");

    truth_QSq_RES_1535 = new TH1D("truth_QSq_RES_1535","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    truth_QSq_RES_1535->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    truth_QSq_RES_1535->GetYaxis()->SetTitle("Events/Bin");

    truth_QSq_RES_1520 = new TH1D("truth_QSq_RES_1520","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    truth_QSq_RES_1520->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    truth_QSq_RES_1520->GetYaxis()->SetTitle("Events/Bin");

    truth_QSq_RES_Other = new TH1D("truth_QSq_RES_Other","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    truth_QSq_RES_Other->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    truth_QSq_RES_Other->GetYaxis()->SetTitle("Events/Bin");

    truth_QSq_DIS = new TH1D("truth_QSq_DIS","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    truth_QSq_DIS->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    truth_QSq_DIS->GetYaxis()->SetTitle("Events/Bin");

    truth_QSq_2p2h = new TH1D("truth_QSq_2p2h","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    truth_QSq_2p2h->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    truth_QSq_2p2h->GetYaxis()->SetTitle("Events/Bin");

    truth_QSq_Non_RES = new TH1D("truth_QSq_Non_RES","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    truth_QSq_Non_RES->GetXaxis()->SetTitle("Q^{2} [GeV^{2}]");
    truth_QSq_Non_RES->GetYaxis()->SetTitle("Events/Bin");

    // ------------------------------------------------------------------------
    // Signal incomingE
    // ------------------------------------------------------------------------
    truth_Enu_QE = new TH1D("truth_Enu_QE","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    truth_Enu_QE->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    truth_Enu_QE->GetYaxis()->SetTitle("Events/Bin");

    truth_Enu_RES_1232 = new TH1D("truth_Enu_RES_1232","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    truth_Enu_RES_1232->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    truth_Enu_RES_1232->GetYaxis()->SetTitle("Events/Bin");

    truth_Enu_RES_1535 = new TH1D("truth_Enu_RES_1535","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    truth_Enu_RES_1535->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    truth_Enu_RES_1535->GetYaxis()->SetTitle("Events/Bin");

    truth_Enu_RES_1520 = new TH1D("truth_Enu_RES_1520","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    truth_Enu_RES_1520->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    truth_Enu_RES_1520->GetYaxis()->SetTitle("Events/Bin");

    truth_Enu_RES_Other = new TH1D("truth_Enu_RES_Other","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    truth_Enu_RES_Other->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    truth_Enu_RES_Other->GetYaxis()->SetTitle("Events/Bin");

    truth_Enu_DIS = new TH1D("truth_Enu_DIS","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    truth_Enu_DIS->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    truth_Enu_DIS->GetYaxis()->SetTitle("Events/Bin");

    truth_Enu_2p2h = new TH1D("truth_Enu_2p2h","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    truth_Enu_2p2h->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    truth_Enu_2p2h->GetYaxis()->SetTitle("Events/Bin");

    truth_Enu_Non_RES = new TH1D("truth_Enu_Non_RES","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    truth_Enu_Non_RES->GetXaxis()->SetTitle("E_{#nu} [GeV]");
    truth_Enu_Non_RES->GetYaxis()->SetTitle("Events/Bin");

    // ------------------------------------------------------------------------
    // Signal w
    // ------------------------------------------------------------------------
    truth_w_QE = new TH1D("truth_w_QE","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    truth_w_QE->GetXaxis()->SetTitle("W [GeV]");
    truth_w_QE->GetYaxis()->SetTitle("Events/Bin");

    truth_w_RES_1232 = new TH1D("truth_w_RES_1232","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    truth_w_RES_1232->GetXaxis()->SetTitle("W [GeV]");
    truth_w_RES_1232->GetYaxis()->SetTitle("Events/Bin");

    truth_w_RES_1535 = new TH1D("truth_w_RES_1535","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    truth_w_RES_1535->GetXaxis()->SetTitle("W [GeV]");
    truth_w_RES_1535->GetYaxis()->SetTitle("Events/Bin");

    truth_w_RES_1520 = new TH1D("truth_w_RES_1520","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    truth_w_RES_1520->GetXaxis()->SetTitle("W [GeV]");
    truth_w_RES_1520->GetYaxis()->SetTitle("Events/Bin");

    truth_w_RES_Other = new TH1D("truth_w_RES_Other","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    truth_w_RES_Other->GetXaxis()->SetTitle("W [GeV]");
    truth_w_RES_Other->GetYaxis()->SetTitle("Events/Bin");

    truth_w_DIS = new TH1D("truth_w_DIS","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    truth_w_DIS->GetXaxis()->SetTitle("W [GeV]");
    truth_w_DIS->GetYaxis()->SetTitle("Events/Bin");

    truth_w_2p2h = new TH1D("truth_w_2p2h","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    truth_w_2p2h->GetXaxis()->SetTitle("W [GeV]");
    truth_w_2p2h->GetYaxis()->SetTitle("Events/Bin");

    truth_w_Non_RES = new TH1D("truth_w_Non_RES","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    truth_w_Non_RES->GetXaxis()->SetTitle("W [GeV]");
    truth_w_Non_RES->GetYaxis()->SetTitle("Events/Bin");

    // Pi0 Invariant Mass - Used for Correction Fit
    pi0_invMass_1Track = new TH1D("pi0_invMass_1Track","#pi^{0} Invariant Mass 1 Track",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
    pi0_invMass_2Track = new TH1D("pi0_invMass_2Track","#pi^{0} Invariant Mass 2 Track",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );

    // Michel Electron - Truth Match
    michel_piplus_time_diff = new TH1D("michel_piplus_time_diff","Michel Prong Time Difference piplus",50,0.0,5000.0);
    michel_piplus_time_diff->GetXaxis()->SetTitle("Time Difference [ns]");
    michel_piplus_time_diff->GetYaxis()->SetTitle("Events/Bin");

    michel_neutron_time_diff = new TH1D("michel_neutron_time_diff","Michel Prong Time Difference neutron",50,0.0,5000.0);
    michel_neutron_time_diff->GetXaxis()->SetTitle("Time Difference [ns]");
    michel_neutron_time_diff->GetYaxis()->SetTitle("Events/Bin");

    michel_proton_time_diff = new TH1D("michel_proton_time_diff","Michel Prong Time Difference proton",50,0.0,5000.0);
    michel_proton_time_diff->GetXaxis()->SetTitle("Time Difference [ns]");
    michel_proton_time_diff->GetYaxis()->SetTitle("Events/Bin");

    michel_piminus_time_diff = new TH1D("michel_piminus_time_diff","Michel Prong Time Difference piminus",50,0.0,5000.0);
    michel_piminus_time_diff->GetXaxis()->SetTitle("Time Difference [ns]");
    michel_piminus_time_diff->GetYaxis()->SetTitle("Events/Bin");

    michel_other_time_diff = new TH1D("michel_other_time_diff","Michel Prong Time Difference other",50,0.0,5000.0);
    michel_other_time_diff->GetXaxis()->SetTitle("Time Difference [ns]");
    michel_other_time_diff->GetYaxis()->SetTitle("Events/Bin");

    // Energy
    michel_piplus_energy = new TH1D("michel_piplus_energy","Michel Prong Energy piplus",50,0.0,100.0);
    michel_piplus_energy->GetXaxis()->SetTitle("Energy [MeV]");
    michel_piplus_energy->GetYaxis()->SetTitle("Events/Bin");

    michel_neutron_energy = new TH1D("michel_neutron_energy","Michel Prong Energy neutron",50,0.0,100.0);
    michel_neutron_energy->GetXaxis()->SetTitle("Energy [MeV]");
    michel_neutron_energy->GetYaxis()->SetTitle("Events/Bin");

    michel_proton_energy = new TH1D("michel_proton_energy","Michel Prong Energy proton",50,0.0,100.0);
    michel_proton_energy->GetXaxis()->SetTitle("Energy [MeV]");
    michel_proton_energy->GetYaxis()->SetTitle("Events/Bin");

    michel_piminus_energy = new TH1D("michel_piminus_energy","Michel Prong Energy piminus",50,0.0,100.0);
    michel_piminus_energy->GetXaxis()->SetTitle("Energy [MeV]");
    michel_piminus_energy->GetYaxis()->SetTitle("Events/Bin");

    michel_other_energy = new TH1D("michel_other_energy","Michel Prong Energy other",50,0.0,100.0);
    michel_other_energy->GetXaxis()->SetTitle("Energy [MeV]");
    michel_other_energy->GetYaxis()->SetTitle("Events/Bin");

    // Distance 
    michel_piplus_distance = new TH1D("michel_piplus_distance","Michel Prong Distance piplus",50,0.0,1000.0);
    michel_piplus_distance->GetXaxis()->SetTitle("Distance [mm]");
    michel_piplus_distance->GetYaxis()->SetTitle("Events/Bin");

    michel_neutron_distance = new TH1D("michel_neutron_distance","Michel Prong Distance neutron",50,0.0,1000.0);
    michel_neutron_distance->GetXaxis()->SetTitle("Distance [mm]");
    michel_neutron_distance->GetYaxis()->SetTitle("Events/Bin");

    michel_proton_distance = new TH1D("michel_proton_distance","Michel Prong Distance proton",50,0.0,1000.0);
    michel_proton_distance->GetXaxis()->SetTitle("Distance [mm]");
    michel_proton_distance->GetYaxis()->SetTitle("Events/Bin");

    michel_piminus_distance = new TH1D("michel_piminus_distance","Michel Prong Distance piminus",50,0.0,1000.0);
    michel_piminus_distance->GetXaxis()->SetTitle("Distance [mm]");
    michel_piminus_distance->GetYaxis()->SetTitle("Events/Bin");

    michel_other_distance = new TH1D("michel_other_distance","Michel Prong Distance other",50,0.0,1000.0);
    michel_other_distance->GetXaxis()->SetTitle("Distance [mm]");
    michel_other_distance->GetYaxis()->SetTitle("Events/Bin");

    // Longitudinal Distance
    michel_piplus_distance_z = new TH1D("michel_piplus_distance_z","Michel Prong Longitudinal Distance piplus",50,0.0,125.0);
    michel_piplus_distance_z->GetXaxis()->SetTitle("Longitudinal Distance [mm]");
    michel_piplus_distance_z->GetYaxis()->SetTitle("Events/Bin");

    michel_neutron_distance_z = new TH1D("michel_neutron_distance_z","Michel Prong Longitudinal Distance neutron",50,0.0,125.0);
    michel_neutron_distance_z->GetXaxis()->SetTitle("Longitudinal Distance [mm]");
    michel_neutron_distance_z->GetYaxis()->SetTitle("Events/Bin");

    michel_proton_distance_z = new TH1D("michel_proton_distance_z","Michel Prong Longitudinal Distance proton",50,0.0,125.0);
    michel_proton_distance_z->GetXaxis()->SetTitle("Longitudinal Distance [mm]");
    michel_proton_distance_z->GetYaxis()->SetTitle("Events/Bin");

    michel_piminus_distance_z = new TH1D("michel_piminus_distance_z","Michel Prong Longitudinal Distance piminus",50,0.0,125.0);
    michel_piminus_distance_z->GetXaxis()->SetTitle("Longitudinal Distance [mm]");
    michel_piminus_distance_z->GetYaxis()->SetTitle("Events/Bin");

    michel_other_distance_z = new TH1D("michel_other_distance_z","Michel Prong Longitudinal Distance other",50,0.0,125.0);
    michel_other_distance_z->GetXaxis()->SetTitle("Longitudinal Distance [mm]");
    michel_other_distance_z->GetYaxis()->SetTitle("Events/Bin");



    // Pi0 Invariant Mass - Truth Match
    signal_invMass_pizero = new TH1D("signal_invMass_pizero","#pi^{0} Invariant Mass - Most PDG: pizero",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
    signal_invMass_pizero->GetXaxis()->SetTitle("#pi^{0} Invariant Mass [MeV]");
    signal_invMass_pizero->GetYaxis()->SetTitle("Events/Bin");

    signal_invMass_piplus = new TH1D("signal_invMass_piplus","#pi^{0} Invariant Mass - Most PDG: piplus",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
    signal_invMass_piplus->GetXaxis()->SetTitle("#pi^{0} Invariant Mass [MeV]");
    signal_invMass_piplus->GetYaxis()->SetTitle("Events/Bin");

    signal_invMass_proton = new TH1D("signal_invMass_proton","#pi^{0} Invariant Mass - Most PDG: proton",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
    signal_invMass_proton->GetXaxis()->SetTitle("#pi^{0} Invariant Mass [MeV]");
    signal_invMass_proton->GetYaxis()->SetTitle("Events/Bin");

    signal_invMass_neutron = new TH1D("signal_invMass_neutron","#pi^{0} Invariant Mass - Most PDG: neutron",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
    signal_invMass_neutron->GetXaxis()->SetTitle("#pi^{0} Invariant Mass [MeV]");
    signal_invMass_neutron->GetYaxis()->SetTitle("Events/Bin");

    signal_invMass_other = new TH1D("signal_invMass_other","#pi^{0} Invariant Mass - Most PDG: other",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
    signal_invMass_other->GetXaxis()->SetTitle("#pi^{0} Invariant Mass [MeV]");
    signal_invMass_other->GetYaxis()->SetTitle("Events/Bin");

    background_invMass_pizero = new TH1D("background_invMass_pizero","#pi^{0} Invariant Mass - Most PDG: pizero",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
    background_invMass_pizero->GetXaxis()->SetTitle("#pi^{0} Invariant Mass [MeV]");
    background_invMass_pizero->GetYaxis()->SetTitle("Events/Bin");

    background_invMass_piplus = new TH1D("background_invMass_piplus","#pi^{0} Invariant Mass - Most PDG: piplus",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
    background_invMass_piplus->GetXaxis()->SetTitle("#pi^{0} Invariant Mass [MeV]");
    background_invMass_piplus->GetYaxis()->SetTitle("Events/Bin");

    background_invMass_proton = new TH1D("background_invMass_proton","#pi^{0} Invariant Mass - Most PDG: proton",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
    background_invMass_proton->GetXaxis()->SetTitle("#pi^{0} Invariant Mass [MeV]");
    background_invMass_proton->GetYaxis()->SetTitle("Events/Bin");

    background_invMass_neutron = new TH1D("background_invMass_neutron","#pi^{0} Invariant Mass - Most PDG: neutron",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
    background_invMass_neutron->GetXaxis()->SetTitle("#pi^{0} Invariant Mass [MeV]");
    background_invMass_neutron->GetYaxis()->SetTitle("Events/Bin");

    background_invMass_other = new TH1D("background_invMass_other","#pi^{0} Invariant Mass - Most PDG: other",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
    background_invMass_other->GetXaxis()->SetTitle("#pi^{0} Invariant Mass [MeV]");
    background_invMass_other->GetYaxis()->SetTitle("Events/Bin");

    // Pi0 Invariant Mass - Background Subtraction
    invMass_all = new MnvH1D("invMass_all","Data #pi^{0} Invariant Mass",binList.pi0_invMass_signal.get_nBins(), binList.pi0_invMass_signal.get_min(), binList.pi0_invMass_signal.get_max() );
    invMass_all->GetXaxis()->SetTitle("#pi^{0} Invariant Mass [MeV]");
    invMass_all->GetYaxis()->SetTitle("Events/Bin");

    invMass_mc_reco_all = new MnvH1D("invMass_mc_reco_all","MC Reconstructed #pi^{0} Invariant Mass",binList.pi0_invMass_signal.get_nBins(), binList.pi0_invMass_signal.get_min(), binList.pi0_invMass_signal.get_max() );
    invMass_mc_reco_all->GetXaxis()->SetTitle("#pi^{0} Invariant Mass [MeV]");
    invMass_mc_reco_all->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(invMass_mc_reco_all);
    AddLatErrorBands_MC(invMass_mc_reco_all);

    invMass_mc_reco_signal = new MnvH1D("invMass_mc_reco_signal","Signal #pi^{0} Invariant Mass",binList.pi0_invMass_signal.get_nBins(), binList.pi0_invMass_signal.get_min(), binList.pi0_invMass_signal.get_max() );
    invMass_mc_reco_signal->GetXaxis()->SetTitle("#pi^{0} Invariant Mass [MeV]");
    invMass_mc_reco_signal->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(invMass_mc_reco_signal);
    AddLatErrorBands_MC(invMass_mc_reco_signal);

    invMass_mc_reco_bckg = new MnvH1D("invMass_mc_reco_bckg","Background #pi^{0} Invariant Mass",binList.pi0_invMass_signal.get_nBins(), binList.pi0_invMass_signal.get_min(), binList.pi0_invMass_signal.get_max() );
    invMass_mc_reco_bckg->GetXaxis()->SetTitle("#pi^{0} Invariant Mass [MeV]");
    invMass_mc_reco_bckg->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(invMass_mc_reco_bckg);
    AddLatErrorBands_MC(invMass_mc_reco_bckg);

    int nBins = 20;
    double min_photon_E = 0.0;
    double max_photon_E = 1.0;
    double min_cos_angle = 0.5;
    double max_cos_angle = 1.0;
    signal_gamma_E_cos_openingAngle = new TH2D( "signal_gamma_E_cos_openingAngle","Signal E_{#gamma}+E_{#gamma} vs. cos(#theta_{#gamma#gamma})",nBins, min_photon_E, max_photon_E, nBins, min_cos_angle, max_cos_angle);
    signal_gamma_E_cos_openingAngle->GetXaxis()->SetTitle("E_{#gamma_{1}}+E_{#gamma_{2}} [GeV]");
    signal_gamma_E_cos_openingAngle->GetYaxis()->SetTitle("cos(#theta_{#gamma#gamma})");

    bckg_gamma_E_cos_openingAngle = new TH2D( "bckg_gamma_E_cos_openingAngle","Background E_{#gamma}+E_{#gamma} vs. cos(#theta_{#gamma#gamma})",nBins, min_photon_E, max_photon_E, nBins, min_cos_angle, max_cos_angle);
    bckg_gamma_E_cos_openingAngle->GetXaxis()->SetTitle("E_{#gamma_{1}}+E_{#gamma_{2}} [GeV]");
    bckg_gamma_E_cos_openingAngle->GetYaxis()->SetTitle("cos(#theta_{#gamma#gamma})");

    bckg_signal_diff_E_cos_openingAngle = new TH2D( "bckg_signal_diff_E_cos_openingAngle","Background - Signal E_{#gamma}+E_{#gamma} vs. cos(#theta_{#gamma#gamma})",nBins, min_photon_E, max_photon_E, nBins, min_cos_angle, max_cos_angle);
    bckg_signal_diff_E_cos_openingAngle->GetXaxis()->SetTitle("E_{#gamma_{1}}+E_{#gamma_{2}} [GeV]");
    bckg_signal_diff_E_cos_openingAngle->GetYaxis()->SetTitle("cos(#theta_{#gamma#gamma})");

    double min_convLength = 0.0 ;
    double max_convLength = 100.0;
    signal_E_cosTheta_convLength = new TH3D( "signal_E_cosTheta_convLength","Signal E_{#gamma}+E_{#gamma} vs. cos(#theta_{#gamma#gamma}) vs. d_{#gamma}+d_{#gamma}",nBins, min_photon_E, max_photon_E, nBins, min_cos_angle, max_cos_angle, nBins, min_convLength, max_convLength );
    signal_E_cosTheta_convLength->GetXaxis()->SetTitle("E_{#gamma_{1}}+E_{#gamma_{2}} [GeV]");
    signal_E_cosTheta_convLength->GetYaxis()->SetTitle("cos(#theta_{#gamma#gamma})");
    signal_E_cosTheta_convLength->GetZaxis()->SetTitle("Conversion Distance [cm]");

    bckg_E_cosTheta_convLength = new TH3D( "bckg_E_cosTheta_convLength","Background E_{#gamma}+E_{#gamma} vs. cos(#theta_{#gamma#gamma}) vs. d_{#gamma}+d_{#gamma}",nBins, min_photon_E, max_photon_E, nBins, min_cos_angle, max_cos_angle, nBins, min_convLength, max_convLength );
    bckg_E_cosTheta_convLength->GetXaxis()->SetTitle("E_{#gamma_{1}}+E_{#gamma_{2}} [GeV]");
    bckg_E_cosTheta_convLength->GetYaxis()->SetTitle("cos(#theta_{#gamma#gamma})");
    bckg_E_cosTheta_convLength->GetZaxis()->SetTitle("Conversion Distance [cm]");

    bckg_signal_diff_E_cosTheta_convLength = new TH3D( "bckg_signal_diff_E_cosTheta_convLength","Background - Signal E_{#gamma}+E_{#gamma} vs. cos(#theta_{#gamma#gamma}) vs. d_{#gamma}+d_{#gamma}",nBins, min_photon_E, max_photon_E, nBins, min_cos_angle, max_cos_angle, nBins, min_convLength, max_convLength );
    bckg_signal_diff_E_cosTheta_convLength->GetXaxis()->SetTitle("E_{#gamma_{1}}+E_{#gamma_{2}} [GeV]");
    bckg_signal_diff_E_cosTheta_convLength->GetYaxis()->SetTitle("cos(#theta_{#gamma#gamma})");
    bckg_signal_diff_E_cosTheta_convLength->GetZaxis()->SetTitle("Conversion Distance [cm]");
}

void CCProtonPi0_CutList::SetCutNames()
{
    nCut_All.set_Name("All");
    nCut_Vertex_None.set_Name("Vertex_None");
    nCut_Vertex_Not_Reconstructable.set_Name("Vertex_Not_Reconstructable"); 
    nCut_Vertex_Not_Fiducial.set_Name("Vertex_Not_Fiducial");
    nCut_Muon_None.set_Name("Muon_None");              
    nCut_Muon_Angle.set_Name("Muon_Angle");              
    nCut_Muon_Charge.set_Name("Muon_Charge");
    nCut_Vertex_Michel_Exist.set_Name("Vertex_Michel_Exist"); 
    nCut_EndPoint_Michel_Exist.set_Name("EndPoint_Michel_Exist");
    nCut_secEndPoint_Michel_Exist.set_Name("secEndPoint_Michel_Exist");
    nCut_Particle_None.set_Name("Particle_None");
    nCut_Proton_None.set_Name("Proton_None");            
    nCut_Proton_Bad.set_Name("Proton_Bad");            
    nCut_ProtonScore.set_Name("Proton_Score");
    nCut_PreFilter_Pi0.set_Name("PreFilter_Pi0");
    nCut_ConeBlobs.set_Name("ConeBlobs");
    nCut_BlobDirectionBad.set_Name("BlobDirectionBad");
    nCut_Pi0_Bad.set_Name("Pi0_Bad");
    nCut_Shower_Michel_Exist.set_Name("Shower_Michel_Exist");
    nCut_Photon1DistanceLow.set_Name("Photon1DistanceLow");
    nCut_Photon2DistanceLow.set_Name("Photon2DistanceLow");
    nCut_LowE_SmallAngle.set_Name("LowE_SmallAngle");
    nCut_beamEnergy.set_Name("beamEnergy");
    nCut_W.set_Name("W");
    nCut_Pi0_invMass.set_Name("Pi0_invMass");

    // 1Track
    nCut_1Track_All.set_Name("All_1Track");
    nCut_1Track_PreFilter_Pi0.set_Name("PreFilter_Pi0");
    nCut_1Track_ConeBlobs.set_Name("ConeBlobs");
    nCut_1Track_BlobDirectionBad.set_Name("BlobDirectionBad");
    nCut_1Track_Pi0_Bad.set_Name("Pi0_Bad");
    nCut_1Track_Photon1DistanceLow.set_Name("Photon1DistanceLow");
    nCut_1Track_Photon2DistanceLow.set_Name("Photon2DistanceLow");
    nCut_1Track_Pi0_invMass.set_Name("Pi0_invMass");
    nCut_1Track_beamEnergy.set_Name("beamEnergy");
    nCut_1Track_W.set_Name("W");

    // 2 Track
    nCut_2Track_All.set_Name("All_2Track");
    nCut_2Track_ProtonScore.set_Name("Proton_Score");
    nCut_2Track_PreFilter_Pi0.set_Name("PreFilter_Pi0");
    nCut_2Track_ConeBlobs.set_Name("ConeBlobs");
    nCut_2Track_BlobDirectionBad.set_Name("BlobDirectionBad");
    nCut_2Track_Pi0_Bad.set_Name("Pi0_Bad");
    nCut_2Track_Photon1DistanceLow.set_Name("Photon1DistanceLow");
    nCut_2Track_Photon2DistanceLow.set_Name("Photon2DistanceLow");
    nCut_2Track_Pi0_invMass.set_Name("Pi0_invMass");
    nCut_2Track_beamEnergy.set_Name("beamEnergy");
    nCut_2Track_W.set_Name("W");
}

void CCProtonPi0_CutList::OpenTextFiles(bool isMC)
{
    std::string tag = ""; 

    std::string type;
    if (isMC) type = "CutTable_MC_";
    else type = "CutTable_Data_";

    // File Names
    std::string f_all = Folder_List::output + Folder_List::textOut + type + "All_" + tag + version +".txt";
    std::string f_1Track = Folder_List::output + Folder_List::textOut + type + "1Track_" + tag + version + ".txt";
    std::string f_2Track = Folder_List::output + Folder_List::textOut + type + "2Track_" + tag + version + ".txt";

    OpenTextFile(f_all,cutText_All);
    OpenTextFile(f_1Track,cutText_1Track);
    OpenTextFile(f_2Track,cutText_2Track);
}

void CCProtonPi0_CutList::writeCutTableHeader(ofstream &file)
{
    file<<std::left;
    file.width(35); file<<"Cut"<<" "; 
    file.width(12); file<<"Events/Bin"<<" ";    
    file.width(12); file<<"N(Signal)"<<" ";      
    file.width(12); file<<"Eff(All)"<<" ";      
    file.width(12); file<<"Eff(MINOS)"<<" ";      
    file.width(12); file<<"Purity"<<" ";
    file<<std::endl;
}

double CCProtonPi0_CutList::getCutEfficiency(CCProtonPi0_Cut& currentCut, CCProtonPi0_Cut& effBase) const
{
    double nSignal_current = currentCut.nSignal.getCount();
    double nSignal_effBase = effBase.nSignal.getCount(); 
    double eff = (nSignal_current / nSignal_effBase) * 100.0;
    return eff;   
}

double CCProtonPi0_CutList::getCutEfficiency(CCProtonPi0_Cut& currentCut, double effBase) const
{
    double nSignal_current = currentCut.nSignal.getCount();
    double nSignal_effBase = effBase; 
    double eff = (nSignal_current / nSignal_effBase) * 100.0;
    return eff;   
}

double CCProtonPi0_CutList::getCutPurity(CCProtonPi0_Cut& currentCut) const
{
    double nSignal_current = currentCut.nSignal.getCount();
    double nEvents_current = currentCut.nEvent.getCount();
    double purity  = (nSignal_current / nEvents_current) * 100.0;
    return purity;   
}

void CCProtonPi0_CutList::formCutVectors()
{   
    nCutVector_All.push_back(nCut_All);
    nCutVector_All.push_back(nCut_Vertex_None);
    nCutVector_All.push_back(nCut_Vertex_Not_Reconstructable); 
    nCutVector_All.push_back(nCut_Vertex_Not_Fiducial);
    nCutVector_All.push_back(nCut_Muon_None);              
    nCutVector_All.push_back(nCut_Muon_Charge);
    nCutVector_All.push_back(nCut_Muon_Angle);
    nCutVector_All.push_back(nCut_Vertex_Michel_Exist); 
    nCutVector_All.push_back(nCut_EndPoint_Michel_Exist);
    nCutVector_All.push_back(nCut_secEndPoint_Michel_Exist);
    nCutVector_All.push_back(nCut_Particle_None);
    nCutVector_All.push_back(nCut_Proton_None);
    nCutVector_All.push_back(nCut_Proton_Bad);
    nCutVector_All.push_back(nCut_ProtonScore);
    nCutVector_All.push_back(nCut_PreFilter_Pi0);
    nCutVector_All.push_back(nCut_ConeBlobs);
    nCutVector_All.push_back(nCut_BlobDirectionBad);
    nCutVector_All.push_back(nCut_Pi0_Bad);
    nCutVector_All.push_back(nCut_Shower_Michel_Exist);
    nCutVector_All.push_back(nCut_Photon1DistanceLow);
    nCutVector_All.push_back(nCut_Photon2DistanceLow);
    nCutVector_All.push_back(nCut_LowE_SmallAngle);
    nCutVector_All.push_back(nCut_beamEnergy);
    nCutVector_All.push_back(nCut_W);
    nCutVector_All.push_back(nCut_Pi0_invMass);

    // 1 Track
    nCutVector_1Track.push_back(nCut_1Track_All);
    nCutVector_1Track.push_back(nCut_1Track_PreFilter_Pi0);
    nCutVector_1Track.push_back(nCut_1Track_ConeBlobs);
    nCutVector_1Track.push_back(nCut_1Track_BlobDirectionBad);
    nCutVector_1Track.push_back(nCut_1Track_Pi0_Bad);
    nCutVector_1Track.push_back(nCut_1Track_Photon1DistanceLow);
    nCutVector_1Track.push_back(nCut_1Track_Photon2DistanceLow);
    nCutVector_1Track.push_back(nCut_1Track_beamEnergy);
    nCutVector_1Track.push_back(nCut_1Track_W);
    nCutVector_1Track.push_back(nCut_1Track_Pi0_invMass);

    // 2 Track
    nCutVector_2Track.push_back(nCut_2Track_All);
    nCutVector_2Track.push_back(nCut_2Track_ProtonScore);
    nCutVector_2Track.push_back(nCut_2Track_PreFilter_Pi0);
    nCutVector_2Track.push_back(nCut_2Track_ConeBlobs);
    nCutVector_2Track.push_back(nCut_2Track_BlobDirectionBad);
    nCutVector_2Track.push_back(nCut_2Track_Pi0_Bad);
    nCutVector_2Track.push_back(nCut_2Track_Photon1DistanceLow);
    nCutVector_2Track.push_back(nCut_2Track_Photon2DistanceLow);
    nCutVector_2Track.push_back(nCut_2Track_beamEnergy);
    nCutVector_2Track.push_back(nCut_2Track_W);
    nCutVector_2Track.push_back(nCut_2Track_Pi0_invMass);
}

void CCProtonPi0_CutList::writeCutTable()
{
    formCutVectors();

    writeAllCuts(); 
    write1TrackCuts();
    write2TrackCuts();
}

void CCProtonPi0_CutList::writeAllCuts()
{
    writeCutTableHeader(cutText_All);
    writeCutTableRows(cutText_All, nCutVector_All, true);
}

void CCProtonPi0_CutList::write1TrackCuts()
{
    writeCutTableHeader(cutText_1Track);
    writeCutTableRows(cutText_1Track, nCutVector_1Track, false);
}

void CCProtonPi0_CutList::write2TrackCuts()
{
    writeCutTableHeader(cutText_2Track);
    writeCutTableRows(cutText_2Track, nCutVector_2Track, false);
}

void CCProtonPi0_CutList::writeCutTableRows(ofstream &file, std::vector<CCProtonPi0_Cut> &nCutVector, bool isAll)
{
    // First Element on the nCutVector is the All Events
    CCProtonPi0_Cut eff_base_all = nCutVector[0];
    CCProtonPi0_Cut eff_base_MINOS;
    if (isAll){
        eff_base_MINOS = nCutVector_All[4];
    }else{
        eff_base_MINOS = nCutVector[0];
    }


    for( unsigned int i = 0; i < nCutVector.size(); i++){
        writeSingleRow(file, nCutVector[i], eff_base_all, eff_base_MINOS);    
    }
}

void CCProtonPi0_CutList::writeSingleRow(ofstream &file, CCProtonPi0_Cut& currentCut, CCProtonPi0_Cut &eff_base_all, CCProtonPi0_Cut &eff_base_MINOS)
{
    double eff_AllSignal;
    double eff_MINOS;
    double purity;    

    if (use_nTrueSignal){
        eff_AllSignal = getCutEfficiency(currentCut, nTrueSignal);
    }else{
        eff_AllSignal = getCutEfficiency(currentCut, eff_base_all);
    }
    eff_MINOS = getCutEfficiency(currentCut, eff_base_MINOS);
    purity = getCutPurity(currentCut);

    file.width(35); file<<currentCut.get_Name()<<" ";
    file.width(12); file<<currentCut.nEvent.getCount()<<" ";

    // Total Signal
    file.width(12); file<<currentCut.nSignal.getCount()<<" ";

    // Efficiency
    if ( eff_AllSignal <= 100){
        file.width(12); file<<eff_AllSignal<<" ";
    }else{
        file.width(12); file<<"N/A"<<" ";    
    }

    // Efficiency
    if ( eff_MINOS <= 100){
        file.width(12); file<<eff_MINOS<<" ";
    }else{
        file.width(12); file<<"N/A"<<" ";    
    }

    // Purity
    file.width(12); file<<purity<<" ";

    file<<std::endl;
}

CCProtonPi0_CutList::~CCProtonPi0_CutList()
{
    cutText_All.close();
    cutText_1Track.close();
    cutText_2Track.close();
}

void CCProtonPi0_CutList::writeHistograms()
{
    std::cout<<">> Writing "<<rootDir<<std::endl;
    f->cd();
    for (int i = 0; i < nHistograms; i++){
        // Common
        hCut_nVertices[i]->Write();
        hCut_nTracks[i]->Write();
        hCut_nTracks2[i]->Write();
        hCut_nTracks_Close[i]->Write();
        hCut_nTracks_Far[i]->Write();
        hCut_nTracks_Discarded[i]->Write();
        hCut_Michel[i]->Write();
        hCut_nProtonCandidates[i]->Write();
        hCut_nShowerCandidates[i]->Write();
        hCut_pi0invMass[i]->Write();
        hCut_W[i]->Write();

        // 1 Track
        hCut_1Track_nShowerCandidates[i]->Write();
        hCut_1Track_eVis_nuclearTarget[i]->Write();
        hCut_1Track_eVis_other[i]->Write();
        hCut_1Track_pi0invMass[i]->Write();
        hCut_1Track_gamma1ConvDist[i]->Write();
        hCut_1Track_gamma2ConvDist[i]->Write();
        hCut_1Track_neutrinoE[i]->Write();
        hCut_1Track_W[i]->Write();

        // 2 Track 
        hCut_2Track_nShowerCandidates[i]->Write();
        hCut_2Track_eVis_nuclearTarget[i]->Write();
        hCut_2Track_eVis_other[i]->Write();
        hCut_2Track_pi0invMass[i]->Write();
        hCut_2Track_gamma1ConvDist[i]->Write();
        hCut_2Track_gamma2ConvDist[i]->Write();
        hCut_2Track_neutrinoE[i]->Write();
        hCut_2Track_protonScore_LLR[i]->Write();
        hCut_2Track_deltaInvMass[i]->Write();
        hCut_2Track_W[i]->Write();

        // Side Bands
        SideBand_muon_P[i]->Write();
        SideBand_muon_theta[i]->Write();
        SideBand_pi0_P[i]->Write();
        SideBand_pi0_KE[i]->Write();
        SideBand_pi0_theta[i]->Write();
        SideBand_neutrino_E[i]->Write();
        SideBand_QSq[i]->Write();
        SideBand_W[i]->Write();
    }

    pi0_invMass_1Track->Write();
    pi0_invMass_2Track->Write();

    michel_piplus_time_diff->Write();
    michel_neutron_time_diff->Write();
    michel_proton_time_diff->Write();
    michel_piminus_time_diff->Write();
    michel_other_time_diff->Write();

    michel_piplus_energy->Write();
    michel_neutron_energy->Write();
    michel_proton_energy->Write();
    michel_piminus_energy->Write();
    michel_other_energy->Write();

    michel_piplus_distance->Write();
    michel_neutron_distance->Write();
    michel_proton_distance->Write();
    michel_piminus_distance->Write();
    michel_other_distance->Write();

    michel_piplus_distance_z->Write();
    michel_neutron_distance_z->Write();
    michel_proton_distance_z->Write();
    michel_piminus_distance_z->Write();
    michel_other_distance_z->Write();

    signal_invMass_pizero->Write();
    signal_invMass_piplus->Write();
    signal_invMass_proton->Write();
    signal_invMass_neutron->Write();
    signal_invMass_other->Write();

    background_invMass_pizero->Write();
    background_invMass_piplus->Write();
    background_invMass_proton->Write();
    background_invMass_neutron->Write();
    background_invMass_other->Write();

    invMass_all->Write();
    invMass_mc_reco_all->Write();
    invMass_mc_reco_signal->Write();
    invMass_mc_reco_bckg->Write();

    bckg_signal_diff_E_cos_openingAngle->Add(signal_gamma_E_cos_openingAngle, -1);
    bckg_signal_diff_E_cos_openingAngle->Add(bckg_gamma_E_cos_openingAngle, +1);
    signal_gamma_E_cos_openingAngle->Write();
    bckg_gamma_E_cos_openingAngle->Write();
    bckg_signal_diff_E_cos_openingAngle->Write();

    bckg_signal_diff_E_cosTheta_convLength->Add(signal_E_cosTheta_convLength, -1);
    bckg_signal_diff_E_cosTheta_convLength->Add(bckg_E_cosTheta_convLength, +1);
    signal_E_cosTheta_convLength->Write();
    bckg_E_cosTheta_convLength->Write();
    bckg_signal_diff_E_cosTheta_convLength->Write();

    // Signal Q2
    truth_QSq_QE->Write();
    truth_QSq_RES_1232->Write();
    truth_QSq_RES_1535->Write();
    truth_QSq_RES_1520->Write();
    truth_QSq_RES_Other->Write();

    truth_QSq_DIS->Write();
    truth_QSq_2p2h->Write();
    truth_QSq_Non_RES->Write();

    // Signal incomingE
    truth_Enu_QE->Write();
    truth_Enu_RES_1232->Write();
    truth_Enu_RES_1535->Write();
    truth_Enu_RES_1520->Write();
    truth_Enu_RES_Other->Write();

    truth_Enu_DIS->Write();
    truth_Enu_2p2h->Write();
    truth_Enu_Non_RES->Write();

    // Signal w
    truth_w_QE->Write();
    truth_w_RES_1232->Write();
    truth_w_RES_1535->Write();
    truth_w_RES_1520->Write();
    truth_w_RES_Other->Write();

    truth_w_DIS->Write();
    truth_w_2p2h->Write();
    truth_w_Non_RES->Write();

    f->Close();
}

#endif

