/*
    See CCProtonPi0_Interaction.h header for Class Information
*/
#ifndef CCProtonPi0_Interaction_cpp
#define CCProtonPi0_Interaction_cpp

#include "CCProtonPi0_Interaction.h"

using namespace PlotUtils;

CCProtonPi0_Interaction::CCProtonPi0_Interaction(bool isModeReduce, bool isMC) 
{
    std::cout<<"Initializing CCProtonPi0_Interaction"<<std::endl;
   
    m_isMC = isMC;

    if(isModeReduce){
        std::cout<<"\tNTuple Reduce Mode -- Will not create ROOT Files"<<std::endl;
    }else{
        if (isMC) rootDir = Folder_List::rootDir_Interaction_mc;
        else rootDir = Folder_List::rootDir_Interaction_data;
        
        std::cout<<"\tRoot File: "<<rootDir<<std::endl;
 
        // Create Root File 
        f = new TFile(rootDir.c_str(),"RECREATE");
        
        initHistograms();
    }
    
    std::cout<<"Done!"<<std::endl;
}


void CCProtonPi0_Interaction::initHistograms()
{
    MnvH1D* temp = NULL;
    MnvH2D* temp2D = NULL;

    for (int i = 0; i < nHistograms; i++){
        temp = new MnvH1D( Form("%s_%d","CV_weight_Flux",i),"Central Value Weight -- Flux", 80,0.0,2.0);
        temp->GetXaxis()->SetTitle("Central Value Weight -- Flux");
        temp->GetYaxis()->SetTitle("Events/Bin");
        CV_weight_Flux.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","CV_weight_2p2h",i),"Central Value Weight -- 2p2h", 80,0.0,2.0);
        temp->GetXaxis()->SetTitle("Central Value Weight -- 2p2h");
        temp->GetYaxis()->SetTitle("Events/Bin");
        CV_weight_2p2h.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","CV_weight_Delta",i),"Central Value Weight -- Delta", 80,0.0,2.0);
        temp->GetXaxis()->SetTitle("Central Value Weight -- Delta");
        temp->GetYaxis()->SetTitle("Events/Bin");
        CV_weight_Delta.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","CV_weight_CCRES",i),"Central Value Weight -- CCRES", 80,0.0,2.0);
        temp->GetXaxis()->SetTitle("Central Value Weight -- CCRES");
        temp->GetYaxis()->SetTitle("Events/Bin");
        CV_weight_CCRES.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","CV_weight_NonRes1pi",i),"Central Value Weight -- NonRes1pi", 80,0.0,2.0);
        temp->GetXaxis()->SetTitle("Central Value Weight -- NonRes1pi");
        temp->GetYaxis()->SetTitle("Events/Bin");
        CV_weight_NonRes1pi.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","CV_weight",i),"Central Value Weight", 80,0.0,2.0);
        temp->GetXaxis()->SetTitle("Central Value Weight");
        temp->GetYaxis()->SetTitle("Events/Bin");
        CV_weight.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","err_2p2h",i),"2p2h 1#sigma Weights", 80,0.0,2.0);
        temp->GetXaxis()->SetTitle("2p2h 1#sigma Weights");
        temp->GetYaxis()->SetTitle("Events/Bin");
        err_2p2h.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","genie_wgt_VecFFCCQEshape",i),"VecFFCCQEshape GENIE 1#sigma Weights", 80,0.0,2.0);
        temp->GetXaxis()->SetTitle("VecFFCCQEshape GENIE 1#sigma Weights");
        temp->GetYaxis()->SetTitle("Events/Bin");
        genie_wgt_VecFFCCQEshape.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","genie_wgt_NormDISCC",i),"NormDISCC GENIE 1#sigma Weights", 80,0.0,2.0);
        temp->GetXaxis()->SetTitle("NormDISCC GENIE 1#sigma Weights");
        temp->GetYaxis()->SetTitle("Events/Bin");
        genie_wgt_NormDISCC.push_back(temp);


        temp = new MnvH1D( Form("%s_%d","genie_wgt_Theta_Delta2Npi",i),"Theta_Delta2Npi GENIE 1#sigma Weights", 80,0.0,2.0);
        temp->GetXaxis()->SetTitle("Theta_Delta2Npi GENIE 1#sigma Weights");
        temp->GetYaxis()->SetTitle("Events/Bin");
        genie_wgt_Theta_Delta2Npi.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","updated_wgt_Theta_Delta2Npi",i),"Theta_Delta2Npi Updated 1#sigma Weights", 80,0.0,2.0);
        temp->GetXaxis()->SetTitle("Theta_Delta2Npi Updated 1#sigma Weights");
        temp->GetYaxis()->SetTitle("Events/Bin");
        updated_wgt_Theta_Delta2Npi.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","genie_wgt_MaRES",i),"MaRES GENIE 1#sigma Weights", 80,0.0,2.0);
        temp->GetXaxis()->SetTitle("MaRES GENIE 1#sigma Weights");
        temp->GetYaxis()->SetTitle("Events/Bin");
        genie_wgt_MaRES.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","updated_wgt_MaRES",i),"MaRES Updated 1#sigma Weights", 80,0.0,2.0);
        temp->GetXaxis()->SetTitle("MaRES Updated 1#sigma Weights");
        temp->GetYaxis()->SetTitle("Events/Bin");
        updated_wgt_MaRES.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","genie_wgt_MvRES",i),"MvRES GENIE 1#sigma Weights", 80,0.0,2.0);
        temp->GetXaxis()->SetTitle("MvRES GENIE 1#sigma Weights");
        temp->GetYaxis()->SetTitle("Events/Bin");
        genie_wgt_MvRES.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","updated_wgt_MvRES",i),"MvRES Updated 1#sigma Weights", 80,0.0,2.0);
        temp->GetXaxis()->SetTitle("MvRES Updated 1#sigma Weights");
        temp->GetYaxis()->SetTitle("Events/Bin");
        updated_wgt_MvRES.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","genie_wgt_Rvn1pi",i),"Rvn1pi GENIE 1#sigma Weights", 80,0.0,2.0);
        temp->GetXaxis()->SetTitle("Rvn1pi GENIE 1#sigma Weights");
        temp->GetYaxis()->SetTitle("Events/Bin");
        genie_wgt_Rvn1pi.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","updated_wgt_Rvn1pi",i),"Rvn1pi Updated 1#sigma Weights", 80,0.0,2.0);
        temp->GetXaxis()->SetTitle("Rvn1pi Updated 1#sigma Weights");
        temp->GetYaxis()->SetTitle("Events/Bin");
        updated_wgt_Rvn1pi.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","Enu",i),"Reconstructed Beam Energy - All Events", binList.size_Enu, binList.a_Enu);
        temp->GetXaxis()->SetTitle("E_{#nu} (GeV)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        Enu.push_back(temp);
 
        temp = new MnvH1D( Form("%s_%d","Enu_1Track",i),"Reconstructed Beam Energy - 1 Track", binList.size_Enu, binList.a_Enu);
        temp->GetXaxis()->SetTitle("E_{#nu} (GeV)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        Enu_1Track.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","Enu_2Track",i),"Reconstructed Beam Energy - 2 Track", binList.size_Enu, binList.a_Enu);
        temp->GetXaxis()->SetTitle("E_{#nu} (GeV)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        Enu_2Track.push_back(temp);
      
        temp = new MnvH1D( Form("%s_%d","QSq",i),"Reconstructed Q^{2}", binList.size_QSq, binList.a_QSq);
        temp->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
        temp->GetYaxis()->SetTitle("Events/Bin");
        QSq.push_back(temp);    

        temp = new MnvH1D( Form("%s_%d","QSq_1Track",i),"Reconstructed Q^{2} 1Track", binList.size_QSq, binList.a_QSq);
        temp->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
        temp->GetYaxis()->SetTitle("Events/Bin");
        QSq_1Track.push_back(temp);    

        temp = new MnvH1D( Form("%s_%d","QSq_2Track",i),"Reconstructed Q^{2} 2Track", binList.size_QSq, binList.a_QSq);
        temp->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
        temp->GetYaxis()->SetTitle("Events/Bin");
        QSq_2Track.push_back(temp);    

        temp = new MnvH1D( Form("%s_%d","WSq",i),"Reconstructed W^{2}",binList.wSq.get_nBins(), binList.wSq.get_min(), binList.wSq.get_max() );
        temp->GetXaxis()->SetTitle("W^{2} (GeV^{2})");
        temp->GetYaxis()->SetTitle("Events/Bin");
        WSq.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","WSq_1Track",i),"Reconstructed W^{2} 1Track",binList.wSq.get_nBins(), binList.wSq.get_min(), binList.wSq.get_max() );
        temp->GetXaxis()->SetTitle("W^{2} (GeV^{2})");
        temp->GetYaxis()->SetTitle("Events/Bin");
        WSq_1Track.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","WSq_2Track",i),"Reconstructed W^{2} 2Track",binList.wSq.get_nBins(), binList.wSq.get_min(), binList.wSq.get_max() );
        temp->GetXaxis()->SetTitle("W^{2} (GeV^{2})");
        temp->GetYaxis()->SetTitle("Events/Bin");
        WSq_2Track.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","W",i),"Reconstructed W",binList.w.get_nBins(), binList.w.get_min(), binList.w.get_max() );
        temp->GetXaxis()->SetTitle("W_{exp} (GeV)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        W.push_back(temp);
 
        temp = new MnvH1D( Form("%s_%d","W_1Track",i),"Reconstructed W 1Track",binList.w.get_nBins(), binList.w.get_min(), binList.w.get_max() );
        temp->GetXaxis()->SetTitle("W_{exp} (GeV)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        W_1Track.push_back(temp);
 
        temp = new MnvH1D( Form("%s_%d","W_2Track",i),"Reconstructed W 2Track",binList.w.get_nBins(), binList.w.get_min(), binList.w.get_max() );
        temp->GetXaxis()->SetTitle("W_{exp} (GeV)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        W_2Track.push_back(temp);
        
        temp = new MnvH1D( Form("%s_%d","vertex_z",i),"Reconstructed Interaction Vertex",binList.vertex_z.get_nBins(), binList.vertex_z.get_min(), binList.vertex_z.get_max() );
        //temp->GetXaxis()->SetTitle("z = 4293 Target, #bf{z = 5810 Interaction Region}, z = 8614 ECAL, z = 9088 HCAL");
        temp->GetXaxis()->SetTitle("z = 5810 Interaction Region, z = 8614 ECAL");
        temp->GetYaxis()->SetTitle(Form("Events / %3.2f ",binList.vertex_z.get_width()));
        vertex_z.push_back(temp);

        // Extra Energy
        temp = new MnvH1D( Form("%s_%d","extra_leftover_energy_1Track",i),"Extra Leftover Visible Energy (r = 300mm)",binList.extra_energy.get_nBins(), binList.extra_energy.get_min(), binList.extra_energy.get_max() );
        temp->GetXaxis()->SetTitle("Extra Leftover Energy (MeV)");
        temp->GetYaxis()->SetTitle("Events/(25 MeV)");
        extra_leftover_energy_1Track.push_back(temp);
       
        temp = new MnvH1D( Form("%s_%d","extra_muon_energy_1Track",i),"Extra Muon Energy 1 Track",binList.extra_energy.get_nBins(), binList.extra_energy.get_min(), binList.extra_energy.get_max() );
        temp->GetXaxis()->SetTitle("Extra Muon Energy (MeV)");
        temp->GetYaxis()->SetTitle("Events/(25 MeV)");
        extra_muon_energy_1Track.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","extra_rejected_energy_1Track",i),"Extra Rejected Visible Energy 1 Track",binList.extra_energy.get_nBins(), binList.extra_energy.get_min(), binList.extra_energy.get_max() );
        temp->GetXaxis()->SetTitle("Extra Rejected Visible Energy (MeV)");
        temp->GetYaxis()->SetTitle("Events/(25 MeV)");
        extra_rejected_energy_1Track.push_back(temp);
    
        temp = new MnvH1D( Form("%s_%d","extra_total_energy_1Track",i),"Total Extra Energy 1 Track",binList.extra_energy.get_nBins(), binList.extra_energy.get_min(), binList.extra_energy.get_max() );
        temp->GetXaxis()->SetTitle("Extra Energy (MeV)");
        temp->GetYaxis()->SetTitle("Events/(25 MeV)");
        extra_total_energy_1Track.push_back(temp);
     
        temp = new MnvH1D( Form("%s_%d","extra_leftover_energy_2Track",i),"Extra Leftover Visible Energy (r = 300mm)",binList.extra_energy.get_nBins(), binList.extra_energy.get_min(), binList.extra_energy.get_max() );
        temp->GetXaxis()->SetTitle("Extra Leftover Energy (MeV)");
        temp->GetYaxis()->SetTitle("Events/(25 MeV)");
        extra_leftover_energy_2Track.push_back(temp);
       
        temp = new MnvH1D( Form("%s_%d","extra_muon_energy_2Track",i),"Extra Muon Energy 2 Track",binList.extra_energy.get_nBins(), binList.extra_energy.get_min(), binList.extra_energy.get_max() );
        temp->GetXaxis()->SetTitle("Extra Muon Energy (MeV)");
        temp->GetYaxis()->SetTitle("Events/(25 MeV)");
        extra_muon_energy_2Track.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","extra_rejected_energy_2Track",i),"Extra Rejected Visible Energy 2 Track",binList.extra_energy.get_nBins(), binList.extra_energy.get_min(), binList.extra_energy.get_max() );
        temp->GetXaxis()->SetTitle("Extra Rejected Visible Energy (MeV)");
        temp->GetYaxis()->SetTitle("Events/(25 MeV)");
        extra_rejected_energy_2Track.push_back(temp);
    
        temp = new MnvH1D( Form("%s_%d","extra_total_energy_2Track",i),"Total Extra Energy 2 Track",binList.extra_energy.get_nBins(), binList.extra_energy.get_min(), binList.extra_energy.get_max() );
        temp->GetXaxis()->SetTitle("Extra Energy (MeV)");
        temp->GetYaxis()->SetTitle("Events/(25 MeV)");
        extra_total_energy_2Track.push_back(temp);

        temp = new MnvH1D(Form("%s_%d","W_p_pi0",i),"Reconstructed p#pi^{0} Invariant Mass",30,0.5,2.0);
        temp->GetXaxis()->SetTitle("Reconstructed p#pi^{0} Inv. Mass (GeV)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        W_p_pi0.push_back(temp);
    
        temp = new MnvH1D(Form("%s_%d","W_All",i),"Reconstructed W All Events",30,0.5,2.0);
        temp->GetXaxis()->SetTitle("Reconstructed W_{exp} (GeV)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        W_All.push_back(temp);
     
        temp = new MnvH1D(Form("%s_%d","W_1",i),"Reconstructed W 1 Track Events",30,0.5,2.0);
        temp->GetXaxis()->SetTitle("Reconstructed W_{exp} (GeV)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        W_1.push_back(temp);
 
        temp = new MnvH1D(Form("%s_%d","W_2",i),"Reconstructed W 2 Track Events",30,0.5,2.0);
        temp->GetXaxis()->SetTitle("Reconstructed W_{exp} (GeV)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        W_2.push_back(temp);

        temp = new MnvH1D(Form("%s_%d","QSq_CV",i),"Q^{2} for All Events",  binList.size_QSq, binList.a_QSq);
        temp->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
        temp->GetYaxis()->SetTitle("Events/Bin");
        QSq_CV.push_back(temp);
 
        temp = new MnvH1D( Form("%s_%d","QSq_MaRES",i),"Reconstructed Q^{2}", binList.size_QSq, binList.a_QSq);
        temp->GetXaxis()->SetTitle("Reconstructed Q^{2} (GeV^{2})");
        temp->GetYaxis()->SetTitle(Form("Events / %3.2f ",binList.Q2.get_width()));
        if (m_isMC){
            AddVertErrorBands_MC(temp);
        }
        QSq_MaRES.push_back(temp);    

        temp = new MnvH1D(Form("%s_%d","pi0_invMass_All",i),"#pi^{0} Invariant Mass",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
        temp->GetXaxis()->SetTitle("#pi^{0} Invariant Mass (MeV)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        pi0_invMass_All.push_back(temp);

        temp = new MnvH1D(Form("%s_%d","pi0_invMass_1Track",i),"#pi^{0} Invariant Mass",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
        temp->GetXaxis()->SetTitle("#pi^{0} Invariant Mass (MeV)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        pi0_invMass_1Track.push_back(temp);

        temp = new MnvH1D(Form("%s_%d","pi0_invMass_2Track",i),"#pi^{0} Invariant Mass",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
        temp->GetXaxis()->SetTitle("#pi^{0} Invariant Mass (MeV)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        pi0_invMass_2Track.push_back(temp);

        temp = new MnvH1D(Form("%s_%d","pi0_invMass_DeltaRES",i),"#pi^{0} Invariant Mass",binList.pi0_invMass.get_nBins(), binList.pi0_invMass.get_min(), binList.pi0_invMass.get_max() );
        temp->GetXaxis()->SetTitle("#pi^{0} Invariant Mass (MeV)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        pi0_invMass_DeltaRES.push_back(temp);

        // 2p2h Study
        temp = new MnvH1D( Form("%s_%d","vertex_energy_1Track",i),"Vertex Blob Energy (r = 90mm)",binList.vertex_energy.get_nBins(), binList.vertex_energy.get_min(), binList.vertex_energy.get_max() );
        temp->GetXaxis()->SetTitle("Vertex Energy (MeV)");
        temp->GetYaxis()->SetTitle("Events/(25 MeV)");
        vertex_energy_1Track.push_back(temp);
  
        temp = new MnvH1D( Form("%s_%d","vertex_energy_2Track",i),"Vertex Blob Energy (r = 90mm)",binList.vertex_energy.get_nBins(), binList.vertex_energy.get_min(), binList.vertex_energy.get_max() );
        temp->GetXaxis()->SetTitle("Vertex Energy (MeV)");
        temp->GetYaxis()->SetTitle("Events/(25 MeV)");
        vertex_energy_2Track.push_back(temp);
 
        temp = new MnvH1D( Form("%s_%d","vertex_energy_All",i),"Vertex Blob Energy (r = 90mm)",binList.vertex_energy.get_nBins(), binList.vertex_energy.get_min(), binList.vertex_energy.get_max() );
        temp->GetXaxis()->SetTitle("Vertex Energy (MeV)");
        temp->GetYaxis()->SetTitle("Events/(25 MeV)");
        vertex_energy_All.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","vertex_evis_1Track",i),"Vertex Blob Visible Energy (r = 90mm)",binList.vertex_evis.get_nBins(), binList.vertex_evis.get_min(), binList.vertex_evis.get_max() );
        temp->GetXaxis()->SetTitle("Vertex Visible Energy (MeV)");
        temp->GetYaxis()->SetTitle("Events/(25 MeV)");
        vertex_evis_1Track.push_back(temp);
  
        temp = new MnvH1D( Form("%s_%d","vertex_evis_2Track",i),"Vertex Blob Visible Energy (r = 90mm)",binList.vertex_evis.get_nBins(), binList.vertex_evis.get_min(), binList.vertex_evis.get_max() );
        temp->GetXaxis()->SetTitle("Vertex Visible Energy (MeV)");
        temp->GetYaxis()->SetTitle("Events/(25 MeV)");
        vertex_evis_2Track.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","vertex_evis_All",i),"Vertex Blob Visible Energy (r = 90mm)",binList.vertex_evis.get_nBins(), binList.vertex_evis.get_min(), binList.vertex_evis.get_max() );
        temp->GetXaxis()->SetTitle("Vertex Visible Energy (MeV)");
        temp->GetYaxis()->SetTitle("Events/(25 MeV)");
        vertex_evis_All.push_back(temp);
 
        temp2D = new MnvH2D( Form("%s_%d","W_QSq",i),"Q^2 vs W", 15, 0.0, 2.0, 15, 0.0, 1.5);
        temp2D->GetXaxis()->SetTitle("W_{exp} (GeV)");
        temp2D->GetYaxis()->SetTitle("Q^{2} (GeV^{2})");
        W_QSq.push_back(temp2D);
    }
   
    resID = new TH1D( "resID","Resonance ID",10,0.0,10.0);
    resID->GetXaxis()->SetTitle("RES ID");
    resID->GetYaxis()->SetTitle("Events/Bin");

    resID_theta = new TH1D( "resID_theta","Resonance ID",10,0.0,10.0);
    resID_theta->GetXaxis()->SetTitle("RES ID");
    resID_theta->GetYaxis()->SetTitle("Events/Bin");

    Enu_flux_wgt = new TH2D( "Enu_flux_wgt","Data All E_{#nu}", binList.size_Enu, binList.a_Enu, 20, 0.0, 2.0);
    Enu_flux_wgt->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    Enu_flux_wgt->GetYaxis()->SetTitle("wgt(Flux)");

    Enu_cvweight = new TH2D( "Enu_cvweight","Data All E_{#nu}", binList.size_Enu, binList.a_Enu, 20, 0.0, 2.0);
    Enu_cvweight->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    Enu_cvweight->GetYaxis()->SetTitle("cvweight");

    // Cross Section Variables
    QSq_all = new MnvH1D( "QSq_all","Data All Q^{2}", binList.size_QSq, binList.a_QSq);
    QSq_all->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    QSq_all->GetYaxis()->SetTitle("Events/Bin");

    QSq_mc_truth_signal = new MnvH1D( "QSq_mc_truth_signal","MC Truth Signal Q^{2}", binList.size_QSq, binList.a_QSq);
    QSq_mc_truth_signal->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    QSq_mc_truth_signal->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(QSq_mc_truth_signal);
    AddLatErrorBands_MC(QSq_mc_truth_signal);

    QSq_mc_reco_all = new MnvH1D( "QSq_mc_reco_all","MC All Reconstructed Q^{2}", binList.size_QSq, binList.a_QSq);
    QSq_mc_reco_all->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    QSq_mc_reco_all->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(QSq_mc_reco_all);
    AddLatErrorBands_MC(QSq_mc_reco_all);

    QSq_mc_reco_signal = new MnvH1D( "QSq_mc_reco_signal","MC Reconstructed Signal Q^{2}", binList.size_QSq, binList.a_QSq);
    QSq_mc_reco_signal->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    QSq_mc_reco_signal->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(QSq_mc_reco_signal);
    AddLatErrorBands_MC(QSq_mc_reco_signal);

    QSq_mc_reco_bckg = new MnvH1D( "QSq_mc_reco_bckg","MC Reconstructed Background Q^{2}", binList.size_QSq, binList.a_QSq);
    QSq_mc_reco_bckg->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    QSq_mc_reco_bckg->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(QSq_mc_reco_bckg);
    AddLatErrorBands_MC(QSq_mc_reco_bckg);

    QSq_response = new MnvH2D( "QSq_response","Signal Q^{2}", binList.size_QSq, binList.a_QSq, binList.size_QSq, binList.a_QSq);
    QSq_response->GetXaxis()->SetTitle("Reconstructed Q^{2} (GeV^{2})");
    QSq_response->GetYaxis()->SetTitle("True Q^{2} (GeV^{2})");
    AddVertErrorBands_MC(QSq_response);
    AddLatErrorBands_MC(QSq_response);

    Enu_all = new MnvH1D( "Enu_all","Data All Neutrino Energy", binList.size_Enu, binList.a_Enu);
    Enu_all->GetXaxis()->SetTitle("Neutrino Energy (GeV)");
    Enu_all->GetYaxis()->SetTitle("Events/Bin");

    Enu_mc_truth_signal = new MnvH1D( "Enu_mc_truth_signal","MC Truth Signal Neutrino Energy", binList.size_Enu, binList.a_Enu);
    Enu_mc_truth_signal->GetXaxis()->SetTitle("Neutrino Energy (GeV)");
    Enu_mc_truth_signal->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(Enu_mc_truth_signal);
    AddLatErrorBands_MC(Enu_mc_truth_signal);

    Enu_mc_reco_all = new MnvH1D( "Enu_mc_reco_all","MC All Reconstructed Neutrino Energy", binList.size_Enu, binList.a_Enu);
    Enu_mc_reco_all->GetXaxis()->SetTitle("Neutrino Energy (GeV)");
    Enu_mc_reco_all->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(Enu_mc_reco_all);
    AddLatErrorBands_MC(Enu_mc_reco_all);

    Enu_mc_reco_signal = new MnvH1D( "Enu_mc_reco_signal","MC Reconstructed Signal Neutrino Energy", binList.size_Enu, binList.a_Enu);
    Enu_mc_reco_signal->GetXaxis()->SetTitle("Neutrino Energy (GeV)");
    Enu_mc_reco_signal->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(Enu_mc_reco_signal);
    AddLatErrorBands_MC(Enu_mc_reco_signal);

    Enu_mc_reco_bckg = new MnvH1D( "Enu_mc_reco_bckg","MC Reconstructed Background Neutrino Energy", binList.size_Enu, binList.a_Enu);
    Enu_mc_reco_bckg->GetXaxis()->SetTitle("Neutrino Energy (GeV)");
    Enu_mc_reco_bckg->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(Enu_mc_reco_bckg);
    AddLatErrorBands_MC(Enu_mc_reco_bckg);

    Enu_response = new MnvH2D( "Enu_response","Signal Neutrino Energy", binList.size_Enu, binList.a_Enu, binList.size_Enu, binList.a_Enu);
    Enu_response->GetXaxis()->SetTitle("Reconstructed Neutrino Energy (GeV)");
    Enu_response->GetYaxis()->SetTitle("True Neutrino Energy (GeV)");
    AddVertErrorBands_MC(Enu_response);
    AddLatErrorBands_MC(Enu_response);

    W_all = new MnvH1D( "W_all","Data All W", binList.size_W, binList.a_W);
    W_all->GetXaxis()->SetTitle("W_{exp} (GeV)");
    W_all->GetYaxis()->SetTitle("Events/Bin");

    W_mc_truth_signal = new MnvH1D( "W_mc_truth_signal","MC Truth Signal W", binList.size_W, binList.a_W);
    W_mc_truth_signal->GetXaxis()->SetTitle("W_{exp} (GeV)");
    W_mc_truth_signal->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(W_mc_truth_signal);
    AddLatErrorBands_MC(W_mc_truth_signal);

    W_mc_reco_all = new MnvH1D( "W_mc_reco_all","MC All Reconstructed W", binList.size_W, binList.a_W);
    W_mc_reco_all->GetXaxis()->SetTitle("W_{exp} (GeV)");
    W_mc_reco_all->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(W_mc_reco_all);
    AddLatErrorBands_MC(W_mc_reco_all);

    W_mc_reco_signal = new MnvH1D( "W_mc_reco_signal","MC Reconstructed Signal W", binList.size_W, binList.a_W);
    W_mc_reco_signal->GetXaxis()->SetTitle("W_{exp} (GeV)");
    W_mc_reco_signal->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(W_mc_reco_signal);
    AddLatErrorBands_MC(W_mc_reco_signal);

    W_mc_reco_bckg = new MnvH1D( "W_mc_reco_bckg","MC Reconstructed Background W", binList.size_W, binList.a_W);
    W_mc_reco_bckg->GetXaxis()->SetTitle("W_{exp} (GeV)");
    W_mc_reco_bckg->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(W_mc_reco_bckg);
    AddLatErrorBands_MC(W_mc_reco_bckg);

    W_response = new MnvH2D( "W_response","Signal W", binList.size_W, binList.a_W, binList.size_W, binList.a_W);
    W_response->GetXaxis()->SetTitle("Reconstructed W_{exp} (GeV)");
    W_response->GetYaxis()->SetTitle("True W_{exp} (GeV)");
    AddVertErrorBands_MC(W_response);
    AddLatErrorBands_MC(W_response);

    deltaInvMass_all = new MnvH1D( "deltaInvMass_all","Data All", binList.size_deltaInvMass, binList.a_deltaInvMass);
    deltaInvMass_all->GetXaxis()->SetTitle("p#pi^{0} Invariant Mass (GeV)");
    deltaInvMass_all->GetYaxis()->SetTitle("Events/Bin");

    deltaInvMass_mc_truth_signal = new MnvH1D( "deltaInvMass_mc_truth_signal","MC Truth Signal", binList.size_deltaInvMass, binList.a_deltaInvMass);
    deltaInvMass_mc_truth_signal->GetXaxis()->SetTitle("p#pi^{0} Invariant Mass (GeV)");
    deltaInvMass_mc_truth_signal->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(deltaInvMass_mc_truth_signal);
    AddLatErrorBands_MC(deltaInvMass_mc_truth_signal);

    deltaInvMass_mc_reco_all = new MnvH1D( "deltaInvMass_mc_reco_all","MC All Reconstructed", binList.size_deltaInvMass, binList.a_deltaInvMass);
    deltaInvMass_mc_reco_all->GetXaxis()->SetTitle("p#pi^{0} Invariant Mass (GeV)");
    deltaInvMass_mc_reco_all->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(deltaInvMass_mc_reco_all);
    AddLatErrorBands_MC(deltaInvMass_mc_reco_all);

    deltaInvMass_mc_reco_signal = new MnvH1D( "deltaInvMass_mc_reco_signal","MC Reconstructed Signal", binList.size_deltaInvMass, binList.a_deltaInvMass);
    deltaInvMass_mc_reco_signal->GetXaxis()->SetTitle("p#pi^{0} Invariant Mass (GeV)");
    deltaInvMass_mc_reco_signal->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(deltaInvMass_mc_reco_signal);
    AddLatErrorBands_MC(deltaInvMass_mc_reco_signal);

    deltaInvMass_mc_reco_bckg = new MnvH1D( "deltaInvMass_mc_reco_bckg","MC Reconstructed Background ", binList.size_deltaInvMass, binList.a_deltaInvMass);
    deltaInvMass_mc_reco_bckg->GetXaxis()->SetTitle("p#pi^{0} Invariant Mass (GeV)");
    deltaInvMass_mc_reco_bckg->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(deltaInvMass_mc_reco_bckg);
    AddLatErrorBands_MC(deltaInvMass_mc_reco_bckg);

    deltaInvMass_response = new MnvH2D( "deltaInvMass_response","Signal", binList.size_deltaInvMass, binList.a_deltaInvMass, binList.size_deltaInvMass, binList.a_deltaInvMass);
    deltaInvMass_response->GetXaxis()->SetTitle("Reco p#pi^{0} Invariant Mass (GeV)");
    deltaInvMass_response->GetYaxis()->SetTitle("True p#pi^{0} Invariant Mass (GeV)");
    AddVertErrorBands_MC(deltaInvMass_response);
    AddLatErrorBands_MC(deltaInvMass_response);

    Delta_pi_theta_all = new MnvH1D( "Delta_pi_theta_all","Data All", binList.Delta_pi_theta.get_nBins(), binList.Delta_pi_theta.get_min(), binList.Delta_pi_theta.get_max());
    Delta_pi_theta_all->GetXaxis()->SetTitle("cos(#theta)");
    Delta_pi_theta_all->GetYaxis()->SetTitle("Events/Bin");

    Delta_pi_theta_mc_truth_signal = new MnvH1D( "Delta_pi_theta_mc_truth_signal","MC Truth Signal",binList.Delta_pi_theta.get_nBins(), binList.Delta_pi_theta.get_min(), binList.Delta_pi_theta.get_max());
    Delta_pi_theta_mc_truth_signal->GetXaxis()->SetTitle("cos(#theta)");
    Delta_pi_theta_mc_truth_signal->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(Delta_pi_theta_mc_truth_signal);
    AddLatErrorBands_MC(Delta_pi_theta_mc_truth_signal);

    Delta_pi_theta_mc_reco_all = new MnvH1D( "Delta_pi_theta_mc_reco_all","MC All Reconstructed",binList.Delta_pi_theta.get_nBins(), binList.Delta_pi_theta.get_min(), binList.Delta_pi_theta.get_max());
    Delta_pi_theta_mc_reco_all->GetXaxis()->SetTitle("cos(#theta)");
    Delta_pi_theta_mc_reco_all->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(Delta_pi_theta_mc_reco_all);
    AddLatErrorBands_MC(Delta_pi_theta_mc_reco_all);

    Delta_pi_theta_mc_reco_signal = new MnvH1D( "Delta_pi_theta_mc_reco_signal","MC Reconstructed Signal",binList.Delta_pi_theta.get_nBins(), binList.Delta_pi_theta.get_min(), binList.Delta_pi_theta.get_max());
    Delta_pi_theta_mc_reco_signal->GetXaxis()->SetTitle("cos(#theta)");
    Delta_pi_theta_mc_reco_signal->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(Delta_pi_theta_mc_reco_signal);
    AddLatErrorBands_MC(Delta_pi_theta_mc_reco_signal);

    Delta_pi_theta_mc_reco_bckg = new MnvH1D( "Delta_pi_theta_mc_reco_bckg","MC Reconstructed Background ",binList.Delta_pi_theta.get_nBins(), binList.Delta_pi_theta.get_min(), binList.Delta_pi_theta.get_max());
    Delta_pi_theta_mc_reco_bckg->GetXaxis()->SetTitle("cos(#theta)");
    Delta_pi_theta_mc_reco_bckg->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(Delta_pi_theta_mc_reco_bckg);
    AddLatErrorBands_MC(Delta_pi_theta_mc_reco_bckg);

    Delta_pi_theta_response = new MnvH2D( "Delta_pi_theta_response","Signal",binList.Delta_pi_theta.get_nBins(), binList.Delta_pi_theta.get_min(), binList.Delta_pi_theta.get_max(), binList.Delta_pi_theta.get_nBins(), binList.Delta_pi_theta.get_min(), binList.Delta_pi_theta.get_max());
    Delta_pi_theta_response->GetXaxis()->SetTitle("Reco cos(#theta)");
    Delta_pi_theta_response->GetYaxis()->SetTitle("Reco cos(#theta)");
    AddVertErrorBands_MC(Delta_pi_theta_response);
    AddLatErrorBands_MC(Delta_pi_theta_response);

    Delta_pi_phi_all = new MnvH1D( "Delta_pi_phi_all","Data All",binList.Delta_pi_phi.get_nBins(), binList.Delta_pi_phi.get_min(), binList.Delta_pi_phi.get_max());
    Delta_pi_phi_all->GetXaxis()->SetTitle("#phi");
    Delta_pi_phi_all->GetYaxis()->SetTitle("Events/Bin");

    Delta_pi_phi_mc_truth_signal = new MnvH1D( "Delta_pi_phi_mc_truth_signal","MC Truth Signal",binList.Delta_pi_phi.get_nBins(), binList.Delta_pi_phi.get_min(), binList.Delta_pi_phi.get_max());
    Delta_pi_phi_mc_truth_signal->GetXaxis()->SetTitle("#phi");
    Delta_pi_phi_mc_truth_signal->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(Delta_pi_phi_mc_truth_signal);
    AddLatErrorBands_MC(Delta_pi_phi_mc_truth_signal);

    Delta_pi_phi_mc_reco_all = new MnvH1D( "Delta_pi_phi_mc_reco_all","MC All Reconstructed",binList.Delta_pi_phi.get_nBins(), binList.Delta_pi_phi.get_min(), binList.Delta_pi_phi.get_max()); 
    Delta_pi_phi_mc_reco_all->GetXaxis()->SetTitle("#phi");
    Delta_pi_phi_mc_reco_all->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(Delta_pi_phi_mc_reco_all);
    AddLatErrorBands_MC(Delta_pi_phi_mc_reco_all);

    Delta_pi_phi_mc_reco_signal = new MnvH1D( "Delta_pi_phi_mc_reco_signal","MC Reconstructed Signal",binList.Delta_pi_phi.get_nBins(), binList.Delta_pi_phi.get_min(), binList.Delta_pi_phi.get_max());
    Delta_pi_phi_mc_reco_signal->GetXaxis()->SetTitle("#phi");
    Delta_pi_phi_mc_reco_signal->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(Delta_pi_phi_mc_reco_signal);
    AddLatErrorBands_MC(Delta_pi_phi_mc_reco_signal);

    Delta_pi_phi_mc_reco_bckg = new MnvH1D( "Delta_pi_phi_mc_reco_bckg","MC Reconstructed Background ",binList.Delta_pi_phi.get_nBins(), binList.Delta_pi_phi.get_min(), binList.Delta_pi_phi.get_max());
    Delta_pi_phi_mc_reco_bckg->GetXaxis()->SetTitle("#phi");
    Delta_pi_phi_mc_reco_bckg->GetYaxis()->SetTitle("Events/Bin");
    AddVertErrorBands_MC(Delta_pi_phi_mc_reco_bckg);
    AddLatErrorBands_MC(Delta_pi_phi_mc_reco_bckg);

    Delta_pi_phi_response = new MnvH2D( "Delta_pi_phi_response","Signal",binList.Delta_pi_phi.get_nBins(), binList.Delta_pi_phi.get_min(), binList.Delta_pi_phi.get_max(),binList.Delta_pi_phi.get_nBins(), binList.Delta_pi_phi.get_min(), binList.Delta_pi_phi.get_max());
    Delta_pi_phi_response->GetXaxis()->SetTitle("Reco #phi");
    Delta_pi_phi_response->GetYaxis()->SetTitle("Reco #phi");
    AddVertErrorBands_MC(Delta_pi_phi_response);
    AddLatErrorBands_MC(Delta_pi_phi_response);

    muon_theta_muon_KE_all = new MnvH2D( "muon_theta_muon_KE_all","Data All", 14, 0.92, 1.0, 14, 1.6, 3.0);
    muon_theta_muon_KE_all->GetXaxis()->SetTitle("muon cos(#theta)");
    muon_theta_muon_KE_all->GetYaxis()->SetTitle("muon kinetic energy (GeV)");

    muon_theta_muon_KE_mc_truth_signal = new MnvH2D( "muon_theta_muon_KE_mc_truth_signal","MC Truth Signal", 14, 0.92, 1.0, 14, 1.6, 3.0);
    muon_theta_muon_KE_mc_truth_signal->GetXaxis()->SetTitle("muon cos(#theta)");
    muon_theta_muon_KE_mc_truth_signal->GetYaxis()->SetTitle("muon kinetic energy (GeV)");
    AddLeadingErrorBands_MC(muon_theta_muon_KE_mc_truth_signal);

    muon_theta_muon_KE_mc_reco_all = new MnvH2D( "muon_theta_muon_KE_mc_reco_all","MC All Reconstructed", 14, 0.92, 1.0, 14, 1.6, 3.0);
    muon_theta_muon_KE_mc_reco_all->GetXaxis()->SetTitle("muon cos(#theta)");
    muon_theta_muon_KE_mc_reco_all->GetYaxis()->SetTitle("muon kinetic energy (GeV)");
    AddLeadingErrorBands_MC(muon_theta_muon_KE_mc_reco_all);

    muon_theta_muon_KE_mc_reco_signal = new MnvH2D( "muon_theta_muon_KE_mc_reco_signal","MC Reconstructed Signal", 14, 0.92, 1.0, 14, 1.6, 3.0);
    muon_theta_muon_KE_mc_reco_signal->GetXaxis()->SetTitle("muon cos(#theta)");
    muon_theta_muon_KE_mc_reco_signal->GetYaxis()->SetTitle("muon kinetic energy (GeV)");
    AddLeadingErrorBands_MC(muon_theta_muon_KE_mc_reco_signal);

    muon_theta_muon_KE_mc_reco_bckg = new MnvH2D( "muon_theta_muon_KE_mc_reco_bckg","MC Reconstructed Background ", 14, 0.92, 1.0, 14, 1.6, 3.0);
    muon_theta_muon_KE_mc_reco_bckg->GetXaxis()->SetTitle("muon cos(#theta)");
    muon_theta_muon_KE_mc_reco_bckg->GetYaxis()->SetTitle("muon kinetic energy (GeV)");
    AddLeadingErrorBands_MC(muon_theta_muon_KE_mc_reco_bckg);

    q3_q0_all = new MnvH2D( "q3_q0_all","Data All", 15, 0.0, 1.5, 15, 0.0, 1.5);
    q3_q0_all->GetXaxis()->SetTitle("three-momentum transfer (GeV)");
    q3_q0_all->GetYaxis()->SetTitle("energy transfer (GeV)");

    q3_q0_mc_truth_signal = new MnvH2D( "q3_q0_mc_truth_signal","MC Truth Signal", 15, 0.0, 1.5, 15, 0.0, 1.5);
    q3_q0_mc_truth_signal->GetXaxis()->SetTitle("three-momentum transfer (GeV)");
    q3_q0_mc_truth_signal->GetYaxis()->SetTitle("energy transfer (GeV)");
    AddLeadingErrorBands_MC(q3_q0_mc_truth_signal);

    q3_q0_mc_reco_all = new MnvH2D( "q3_q0_mc_reco_all","MC All Reconstructed", 15, 0.0, 1.5, 15, 0.0, 1.5);
    q3_q0_mc_reco_all->GetXaxis()->SetTitle("three-momentum transfer (GeV)");
    q3_q0_mc_reco_all->GetYaxis()->SetTitle("energy transfer (GeV)");
    AddLeadingErrorBands_MC(q3_q0_mc_reco_all);

    q3_q0_mc_reco_signal = new MnvH2D( "q3_q0_mc_reco_signal","MC Reconstructed Signal", 15, 0.0, 1.5, 15, 0.0, 1.5);
    q3_q0_mc_reco_signal->GetXaxis()->SetTitle("three-momentum transfer (GeV)");
    q3_q0_mc_reco_signal->GetYaxis()->SetTitle("energy transfer (GeV)");
    AddLeadingErrorBands_MC(q3_q0_mc_reco_signal);

    q3_q0_mc_reco_bckg = new MnvH2D( "q3_q0_mc_reco_bckg","MC Reconstructed Background ", 15, 0.0, 1.5, 15, 0.0, 1.5);
    q3_q0_mc_reco_bckg->GetXaxis()->SetTitle("three-momentum transfer (GeV)");
    q3_q0_mc_reco_bckg->GetYaxis()->SetTitle("energy transfer (GeV)");
    AddLeadingErrorBands_MC(q3_q0_mc_reco_bckg);

    // QSq Error, Diff
    WSq_QSq_Diff = new TH2D( "WSq_QSq_Diff","Q^{2} Difference vs W^{2}",binList.wSq.get_nBins(), binList.wSq.get_min(), binList.wSq.get_max(),binList.QSq_Diff.get_nBins(), binList.QSq_Diff.get_min(), binList.QSq_Diff.get_max() );
    WSq_QSq_Diff->GetXaxis()->SetTitle("W^{2} (GeV^{2})");
    WSq_QSq_Diff->GetYaxis()->SetTitle("Q^{2}_{Reco} - Q^{2}_{True} (GeV^{2})");

    QSq_All_response = new MnvH2D( "QSq_All_response","Signal Q^{2} All", binList.size_QSq, binList.a_QSq, binList.size_QSq, binList.a_QSq);
    QSq_All_response->GetXaxis()->SetTitle("Reconstructed Q^{2} (GeV^{2})");
    QSq_All_response->GetYaxis()->SetTitle("True Q^{2} (GeV^{2})");

    QSq_1Track_response = new MnvH2D( "QSq_1Track_response","Signal Q^{2} 1Track", binList.size_QSq, binList.a_QSq, binList.size_QSq, binList.a_QSq);
    QSq_1Track_response->GetXaxis()->SetTitle("Reconstructed Q^{2} (GeV^{2})");
    QSq_1Track_response->GetYaxis()->SetTitle("True Q^{2} (GeV^{2})");

    QSq_2Track_response = new MnvH2D( "QSq_2Track_response","Signal Q^{2} 1Track", binList.size_QSq, binList.a_QSq, binList.size_QSq, binList.a_QSq);
    QSq_2Track_response->GetXaxis()->SetTitle("Reconstructed Q^{2} (GeV^{2})");
    QSq_2Track_response->GetYaxis()->SetTitle("True Q^{2} (GeV^{2})");

    QSq_Error = new MnvH1D( "QSq_Error","Q^{2} Error",binList.error.get_nBins(), binList.error.get_min(), binList.error.get_max() );
    QSq_Error->GetXaxis()->SetTitle("(Q^{2}_{Reco} - Q^{2}_{True})/Q^{2}_{True}");
    QSq_Error->GetYaxis()->SetTitle("Events/Bin");

    QSq_1Track_Error = new MnvH1D( "QSq_1Track_Error","Q^{2} Error - 1Track",binList.error.get_nBins(), binList.error.get_min(), binList.error.get_max() );
    QSq_1Track_Error->GetXaxis()->SetTitle("(Q^{2}_{Reco} - Q^{2}_{True})/Q^{2}_{True}");
    QSq_1Track_Error->GetYaxis()->SetTitle("Events/Bin");

    QSq_2Track_Error = new MnvH1D( "QSq_2Track_Error","Q^{2} Error - 2Track",binList.error.get_nBins(), binList.error.get_min(), binList.error.get_max() );
    QSq_2Track_Error->GetXaxis()->SetTitle("(Q^{2}_{Reco} - Q^{2}_{True})/Q^{2}_{True}");
    QSq_2Track_Error->GetYaxis()->SetTitle("Events/Bin");

    QSq_Diff = new MnvH1D( "QSq_Diff","Q^{2} Difference ",binList.QSq_Diff.get_nBins(), binList.QSq_Diff.get_min(), binList.QSq_Diff.get_max() );
    QSq_Diff->GetXaxis()->SetTitle("Q^{2}_{Reco} - Q^{2}_{True}");
    QSq_Diff->GetYaxis()->SetTitle("Events/Bin");

    QSq_1Track_Diff = new MnvH1D( "QSq_1Track_Diff","Q^{2} Difference - 1Track",binList.QSq_Diff.get_nBins(), binList.QSq_Diff.get_min(), binList.QSq_Diff.get_max() );
    QSq_1Track_Diff->GetXaxis()->SetTitle("Q^{2}_{Reco} - Q^{2}_{True}");
    QSq_1Track_Diff->GetYaxis()->SetTitle("Events/Bin");

    QSq_2Track_Diff = new MnvH1D( "QSq_2Track_Diff","Q^{2} Difference - 2Track",binList.QSq_Diff.get_nBins(), binList.QSq_Diff.get_min(), binList.QSq_Diff.get_max() );
    QSq_2Track_Diff->GetXaxis()->SetTitle("Q^{2}_{Reco} - Q^{2}_{True}");
    QSq_2Track_Diff->GetYaxis()->SetTitle("Events/Bin");

    // Short Proton True Information
    nProtons = new MnvH1D( "nProtons","Number of Tracked Protons",5,0.0,5.0);
    nProtons->GetXaxis()->SetTitle("N(Protons)");
    nProtons->GetYaxis()->SetTitle("Events/Bin");

    proton_true_P_1Track = new TH1D( "proton_true_P_1Track","Short Proton True Momentum",binList.short_proton_P.get_nBins(), binList.short_proton_P.get_min(), binList.short_proton_P.get_max() );
    proton_true_P_1Track->GetXaxis()->SetTitle("Short Proton P_{True} (MeV)");
    proton_true_P_1Track->GetYaxis()->SetTitle(Form("Candidates / %3.2f ",binList.short_proton_P.get_width()));

    proton_true_KE_1Track = new TH1D( "proton_true_KE_1Track","Short Proton True Momentum",binList.short_proton_KE.get_nBins(), binList.short_proton_KE.get_min(), binList.short_proton_KE.get_max() );
    proton_true_KE_1Track->GetXaxis()->SetTitle("Short Proton P_{True} (MeV)");
    proton_true_KE_1Track->GetYaxis()->SetTitle(Form("Candidates / %3.2f ",binList.short_proton_KE.get_width()));

    proton_true_theta_1Track = new TH1D( "proton_true_theta_1Track","Short Proton True #theta_{p}",binList.angle.get_nBins(), binList.angle.get_min(), binList.angle.get_max() );
    proton_true_theta_1Track->GetXaxis()->SetTitle("True #theta_{p} [Degree]");
    proton_true_theta_1Track->GetYaxis()->SetTitle(Form("Protons / %3.1f [Degree]",binList.angle.get_width()));


    DeltaTransverse_data = new MnvH1D( "DeltaTransverse_data","Delta Transverse Momentum (Double Transverse)",21,-500,500);
    DeltaTransverse_data->GetXaxis()->SetTitle("Delta Transverse Momentum (MeV)");
    DeltaTransverse_data->GetYaxis()->SetTitle("Events/Bin");

    DeltaTransverse_mc = new MnvH1D( "DeltaTransverse_mc","Delta Transverse Momentum (Double Transverse)",21,-500,500);
    DeltaTransverse_mc->GetXaxis()->SetTitle("Delta Transverse Momentum (MeV)");
    DeltaTransverse_mc->GetYaxis()->SetTitle("Events/Bin");

    DeltaTransverse_mc_res = new MnvH2D( "DeltaTransverse_mc_res","Double Transverse Momentum Residual vs Truth",21,-500,500,21,-50,50);
    DeltaTransverse_mc_res->GetXaxis()->SetTitle("Truth Double Transverse Momentum (MeV)");
    DeltaTransverse_mc_res->GetYaxis()->SetTitle("Double Transverse Momentum Residual (MeV)");

    // Ejected Nucleons
    n_ejected_nucleons_1Track = new TH1D( "n_ejected_nucleons_1Track","N(Nucleons) out of Nucleus",binList.multiplicity.get_nBins(), binList.multiplicity.get_min(), binList.multiplicity.get_max() );
    n_ejected_nucleons_1Track->GetXaxis()->SetTitle("N(Nucleons) out of Nucleus");
    n_ejected_nucleons_1Track->GetYaxis()->SetTitle("Events/Bin");

    n_ejected_nucleons_2Track = new TH1D( "n_ejected_nucleons_2Track","N(Nucleons) out of Nucleus",binList.multiplicity.get_nBins(), binList.multiplicity.get_min(), binList.multiplicity.get_max() );
    n_ejected_nucleons_2Track->GetXaxis()->SetTitle("N(Nucleons) out of Nucleus");
    n_ejected_nucleons_2Track->GetYaxis()->SetTitle("Events/Bin");

    // ------------------------------------------------------------------------
    // W: Truth, Error, Difference
    // ------------------------------------------------------------------------
    W_Error = new TH1D("W_Error","W Error",binList.error.get_nBins(), binList.error.get_min(), binList.error.get_max() );
    W_Error->GetXaxis()->SetTitle("(W_{Reco}-W_{True})/W_{True}");
    W_Error->GetYaxis()->SetTitle(Form("Events / %3.2f ",binList.error.get_width()));
 
    W_Diff = new TH1D("W_Diff","W Difference",binList.W_Diff.get_nBins(), binList.W_Diff.get_min(), binList.W_Diff.get_max() );
    W_Diff->GetXaxis()->SetTitle("W_{Reco}-W_{True} (GeV)");
    W_Diff->GetYaxis()->SetTitle("Events/Bin");

    // ------------------------------------------------------------------------
    // Neutrino Energy: Truth, Error, Difference
    // ------------------------------------------------------------------------
    Enu_All_response = new MnvH2D( "Enu_All_response","Signal E_{#nu} All", binList.size_Enu, binList.a_Enu, binList.size_Enu, binList.a_Enu);
    Enu_All_response->GetXaxis()->SetTitle("Reconstructed E_{#nu} (GeV)");
    Enu_All_response->GetYaxis()->SetTitle("True E_{#nu} (GeV)");

    Enu_1Track_response = new MnvH2D( "Enu_1Track_response","Signal E_{#nu} 1Track", binList.size_Enu, binList.a_Enu, binList.size_Enu, binList.a_Enu);
    Enu_1Track_response->GetXaxis()->SetTitle("Reconstructed E_{#nu} (GeV)");
    Enu_1Track_response->GetYaxis()->SetTitle("True E_{#nu} (GeV)");

    Enu_2Track_response = new MnvH2D( "Enu_2Track_response","Signal E_{#nu} 2Track", binList.size_Enu, binList.a_Enu, binList.size_Enu, binList.a_Enu);
    Enu_2Track_response->GetXaxis()->SetTitle("Reconstructed E_{#nu} (GeV)");
    Enu_2Track_response->GetYaxis()->SetTitle("True E_{#nu} (GeV)");

    Enu_Error = new TH1D("Enu_Error","Neutrino Energy Error",binList.error.get_nBins(), binList.error.get_min(), binList.error.get_max() );
    Enu_Error->GetXaxis()->SetTitle("(E_{#nu}^{Reco}-E_{#nu}^{True})/E_{#nu}^{True}");
    Enu_Error->GetYaxis()->SetTitle(Form("Events / %3.2f ",binList.error.get_width()));
  
    Enu_1Track_Error = new TH1D("Enu_1Track_Error","Neutrino Energy Error - 1 Track",binList.error.get_nBins(), binList.error.get_min(), binList.error.get_max() );
    Enu_1Track_Error->GetXaxis()->SetTitle("(E_{#nu}^{Reco}-E_{#nu}^{True})/E_{#nu}^{True}");
    Enu_1Track_Error->GetYaxis()->SetTitle(Form("Events / %3.2f ",binList.error.get_width()));

    Enu_2Track_Error = new TH1D("Enu_2Track_Error","Neutrino Energy Error - 2 Track",binList.error.get_nBins(), binList.error.get_min(), binList.error.get_max() );
    Enu_2Track_Error->GetXaxis()->SetTitle("(E_{#nu}^{Reco}-E_{#nu}^{True})/E_{#nu}^{True}");
    Enu_2Track_Error->GetYaxis()->SetTitle(Form("Events / %3.2f ",binList.error.get_width()));

    Enu_Diff = new TH1D("Enu_Diff","Neutrino Energy Difference",binList.beamE_Diff.get_nBins(), binList.beamE_Diff.get_min(), binList.beamE_Diff.get_max() );
    Enu_Diff->GetXaxis()->SetTitle("E_{#nu}^{Reco}-E_{#nu}^{True} (GeV)");
    Enu_Diff->GetYaxis()->SetTitle("Events/Bin");

    Enu_1Track_Diff = new TH1D("Enu_1Track_Diff","Neutrino Energy Difference - 1 Track",binList.beamE_Diff.get_nBins(), binList.beamE_Diff.get_min(), binList.beamE_Diff.get_max() );
    Enu_1Track_Diff->GetXaxis()->SetTitle("E_{#nu}^{Reco}-E_{#nu}^{True} (GeV)");
    Enu_1Track_Diff->GetYaxis()->SetTitle("Events/Bin");

    Enu_2Track_Diff = new TH1D("Enu_2Track_Diff","Neutrino Energy Difference - 2 Track",binList.beamE_Diff.get_nBins(), binList.beamE_Diff.get_min(), binList.beamE_Diff.get_max() );
    Enu_2Track_Diff->GetXaxis()->SetTitle("E_{#nu}^{Reco}-E_{#nu}^{True} (GeV)");
    Enu_2Track_Diff->GetYaxis()->SetTitle("Events/Bin");

    // Extra Energy
    h_extra_muon_energy = new TH1D("h_extra_muon_energy","Extra Muon Energy",20,0.0,1000);
    h_extra_muon_energy->GetXaxis()->SetTitle("Extra Muon Energy (MeV)");
    h_extra_muon_energy->GetYaxis()->SetTitle("Events/Bin");

    h_extra_leftover_energy = new TH1D("h_extra_leftover_energy","Extra Leftover Energy",20,0.0,1000);
    h_extra_leftover_energy->GetXaxis()->SetTitle("Extra Leftover Energy (MeV)");
    h_extra_leftover_energy->GetYaxis()->SetTitle("Events/Bin");
 
    h_extra_rejected_energy = new TH1D("h_extra_rejected_energy","Extra Rejected Energy",20,0.0,1000);
    h_extra_rejected_energy->GetXaxis()->SetTitle("Extra Rejected Energy (MeV)");
    h_extra_rejected_energy->GetYaxis()->SetTitle("Events/Bin");

    // ------------------------------------------------------------------------
    // Signal Q2
    // ------------------------------------------------------------------------
    mc_Q2_QE = new TH1D("mc_Q2_QE","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    mc_Q2_QE->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    mc_Q2_QE->GetYaxis()->SetTitle("Events/Bin");

    mc_Q2_RES_1232 = new TH1D("mc_Q2_RES_1232","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    mc_Q2_RES_1232->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    mc_Q2_RES_1232->GetYaxis()->SetTitle("Events/Bin");

    mc_Q2_RES_1535 = new TH1D("mc_Q2_RES_1535","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    mc_Q2_RES_1535->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    mc_Q2_RES_1535->GetYaxis()->SetTitle("Events/Bin");

    mc_Q2_RES_1520 = new TH1D("mc_Q2_RES_1520","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    mc_Q2_RES_1520->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    mc_Q2_RES_1520->GetYaxis()->SetTitle("Events/Bin");

    mc_Q2_RES_Other = new TH1D("mc_Q2_RES_Other","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    mc_Q2_RES_Other->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    mc_Q2_RES_Other->GetYaxis()->SetTitle("Events/Bin");

    mc_Q2_DIS = new TH1D("mc_Q2_DIS","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    mc_Q2_DIS->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    mc_Q2_DIS->GetYaxis()->SetTitle("Events/Bin");

    mc_Q2_2p2h = new TH1D("mc_Q2_2p2h","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    mc_Q2_2p2h->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    mc_Q2_2p2h->GetYaxis()->SetTitle("Events/Bin");

    mc_Q2_Non_RES = new TH1D("mc_Q2_Non_RES","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    mc_Q2_Non_RES->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    mc_Q2_Non_RES->GetYaxis()->SetTitle("Events/Bin");

    // ------------------------------------------------------------------------
    // Signal Truth Q2
    // ------------------------------------------------------------------------
    truth_QSq_QE = new TH1D("truth_QSq_QE","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    truth_QSq_QE->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    truth_QSq_QE->GetYaxis()->SetTitle("Events/Bin");

    truth_QSq_RES_1232 = new TH1D("truth_QSq_RES_1232","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    truth_QSq_RES_1232->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    truth_QSq_RES_1232->GetYaxis()->SetTitle("Events/Bin");

    truth_QSq_RES_1535 = new TH1D("truth_QSq_RES_1535","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    truth_QSq_RES_1535->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    truth_QSq_RES_1535->GetYaxis()->SetTitle("Events/Bin");

    truth_QSq_RES_1520 = new TH1D("truth_QSq_RES_1520","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    truth_QSq_RES_1520->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    truth_QSq_RES_1520->GetYaxis()->SetTitle("Events/Bin");

    truth_QSq_RES_Other = new TH1D("truth_QSq_RES_Other","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    truth_QSq_RES_Other->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    truth_QSq_RES_Other->GetYaxis()->SetTitle("Events/Bin");

    truth_QSq_DIS = new TH1D("truth_QSq_DIS","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    truth_QSq_DIS->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    truth_QSq_DIS->GetYaxis()->SetTitle("Events/Bin");

    truth_QSq_2p2h = new TH1D("truth_QSq_2p2h","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    truth_QSq_2p2h->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    truth_QSq_2p2h->GetYaxis()->SetTitle("Events/Bin");

    truth_QSq_Non_RES = new TH1D("truth_QSq_Non_RES","Q^{2} for Signal Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    truth_QSq_Non_RES->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    truth_QSq_Non_RES->GetYaxis()->SetTitle("Events/Bin");

    // ------------------------------------------------------------------------
    // Background Truth Q2
    // ------------------------------------------------------------------------
    reco_bckg_QSq_QE = new TH1D("reco_bckg_QSq_QE","Q^{2} for Background Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    reco_bckg_QSq_QE->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    reco_bckg_QSq_QE->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_QSq_RES_1232 = new TH1D("reco_bckg_QSq_RES_1232","Q^{2} for Background Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    reco_bckg_QSq_RES_1232->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    reco_bckg_QSq_RES_1232->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_QSq_RES_1535 = new TH1D("reco_bckg_QSq_RES_1535","Q^{2} for Background Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    reco_bckg_QSq_RES_1535->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    reco_bckg_QSq_RES_1535->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_QSq_RES_1520 = new TH1D("reco_bckg_QSq_RES_1520","Q^{2} for Background Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    reco_bckg_QSq_RES_1520->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    reco_bckg_QSq_RES_1520->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_QSq_RES_Other = new TH1D("reco_bckg_QSq_RES_Other","Q^{2} for Background Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    reco_bckg_QSq_RES_Other->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    reco_bckg_QSq_RES_Other->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_QSq_DIS = new TH1D("reco_bckg_QSq_DIS","Q^{2} for Background Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    reco_bckg_QSq_DIS->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    reco_bckg_QSq_DIS->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_QSq_2p2h = new TH1D("reco_bckg_QSq_2p2h","Q^{2} for Background Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    reco_bckg_QSq_2p2h->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    reco_bckg_QSq_2p2h->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_QSq_Non_RES = new TH1D("reco_bckg_QSq_Non_RES","Q^{2} for Background Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    reco_bckg_QSq_Non_RES->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    reco_bckg_QSq_Non_RES->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_QSq_Coh = new TH1D("reco_bckg_QSq_Coh","Q^{2} for Background Events",binList.mc_QSq.get_nBins(), binList.mc_QSq.get_min(), binList.mc_QSq.get_max());
    reco_bckg_QSq_Coh->GetXaxis()->SetTitle("Q^{2} (GeV^{2})");
    reco_bckg_QSq_Coh->GetYaxis()->SetTitle("Events/Bin");

    // ------------------------------------------------------------------------
    // Signal incomingE
    // ------------------------------------------------------------------------
    mc_incomingE_QE = new TH1D("mc_incomingE_QE","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    mc_incomingE_QE->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    mc_incomingE_QE->GetYaxis()->SetTitle("Events/Bin");

    mc_incomingE_RES_1232 = new TH1D("mc_incomingE_RES_1232","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    mc_incomingE_RES_1232->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    mc_incomingE_RES_1232->GetYaxis()->SetTitle("Events/Bin");

    mc_incomingE_RES_1535 = new TH1D("mc_incomingE_RES_1535","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    mc_incomingE_RES_1535->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    mc_incomingE_RES_1535->GetYaxis()->SetTitle("Events/Bin");

    mc_incomingE_RES_1520 = new TH1D("mc_incomingE_RES_1520","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    mc_incomingE_RES_1520->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    mc_incomingE_RES_1520->GetYaxis()->SetTitle("Events/Bin");

    mc_incomingE_RES_Other = new TH1D("mc_incomingE_RES_Other","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    mc_incomingE_RES_Other->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    mc_incomingE_RES_Other->GetYaxis()->SetTitle("Events/Bin");

    mc_incomingE_DIS = new TH1D("mc_incomingE_DIS","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    mc_incomingE_DIS->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    mc_incomingE_DIS->GetYaxis()->SetTitle("Events/Bin");

    mc_incomingE_2p2h = new TH1D("mc_incomingE_2p2h","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    mc_incomingE_2p2h->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    mc_incomingE_2p2h->GetYaxis()->SetTitle("Events/Bin");

    mc_incomingE_Non_RES = new TH1D("mc_incomingE_Non_RES","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    mc_incomingE_Non_RES->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    mc_incomingE_Non_RES->GetYaxis()->SetTitle("Events/Bin");

    // ------------------------------------------------------------------------
    // Signal Truth incomingE
    // ------------------------------------------------------------------------
    truth_Enu_QE = new TH1D("truth_Enu_QE","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    truth_Enu_QE->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    truth_Enu_QE->GetYaxis()->SetTitle("Events/Bin");

    truth_Enu_RES_1232 = new TH1D("truth_Enu_RES_1232","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    truth_Enu_RES_1232->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    truth_Enu_RES_1232->GetYaxis()->SetTitle("Events/Bin");

    truth_Enu_RES_1535 = new TH1D("truth_Enu_RES_1535","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    truth_Enu_RES_1535->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    truth_Enu_RES_1535->GetYaxis()->SetTitle("Events/Bin");

    truth_Enu_RES_1520 = new TH1D("truth_Enu_RES_1520","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    truth_Enu_RES_1520->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    truth_Enu_RES_1520->GetYaxis()->SetTitle("Events/Bin");

    truth_Enu_RES_Other = new TH1D("truth_Enu_RES_Other","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    truth_Enu_RES_Other->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    truth_Enu_RES_Other->GetYaxis()->SetTitle("Events/Bin");

    truth_Enu_DIS = new TH1D("truth_Enu_DIS","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    truth_Enu_DIS->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    truth_Enu_DIS->GetYaxis()->SetTitle("Events/Bin");

    truth_Enu_2p2h = new TH1D("truth_Enu_2p2h","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    truth_Enu_2p2h->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    truth_Enu_2p2h->GetYaxis()->SetTitle("Events/Bin");

    truth_Enu_Non_RES = new TH1D("truth_Enu_Non_RES","E_{#nu} for Signal Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    truth_Enu_Non_RES->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    truth_Enu_Non_RES->GetYaxis()->SetTitle("Events/Bin");

    // ------------------------------------------------------------------------
    // Background Truth incomingE
    // ------------------------------------------------------------------------
    reco_bckg_Enu_QE = new TH1D("reco_bckg_Enu_QE","E_{#nu} for Background Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    reco_bckg_Enu_QE->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    reco_bckg_Enu_QE->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_Enu_RES_1232 = new TH1D("reco_bckg_Enu_RES_1232","E_{#nu} for Background Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    reco_bckg_Enu_RES_1232->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    reco_bckg_Enu_RES_1232->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_Enu_RES_1535 = new TH1D("reco_bckg_Enu_RES_1535","E_{#nu} for Background Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    reco_bckg_Enu_RES_1535->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    reco_bckg_Enu_RES_1535->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_Enu_RES_1520 = new TH1D("reco_bckg_Enu_RES_1520","E_{#nu} for Background Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    reco_bckg_Enu_RES_1520->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    reco_bckg_Enu_RES_1520->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_Enu_RES_Other = new TH1D("reco_bckg_Enu_RES_Other","E_{#nu} for Background Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    reco_bckg_Enu_RES_Other->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    reco_bckg_Enu_RES_Other->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_Enu_DIS = new TH1D("reco_bckg_Enu_DIS","E_{#nu} for Background Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    reco_bckg_Enu_DIS->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    reco_bckg_Enu_DIS->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_Enu_2p2h = new TH1D("reco_bckg_Enu_2p2h","E_{#nu} for Background Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    reco_bckg_Enu_2p2h->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    reco_bckg_Enu_2p2h->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_Enu_Non_RES = new TH1D("reco_bckg_Enu_Non_RES","E_{#nu} for Background Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    reco_bckg_Enu_Non_RES->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    reco_bckg_Enu_Non_RES->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_Enu_Coh = new TH1D("reco_bckg_Enu_Coh","E_{#nu} for Background Events",binList.mc_incomingE.get_nBins(), binList.mc_incomingE.get_min(), binList.mc_incomingE.get_max());
    reco_bckg_Enu_Coh->GetXaxis()->SetTitle("E_{#nu} (GeV)");
    reco_bckg_Enu_Coh->GetYaxis()->SetTitle("Events/Bin");

    // ------------------------------------------------------------------------
    // Signal w
    // ------------------------------------------------------------------------
    mc_w_QE = new TH1D("mc_w_QE","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    mc_w_QE->GetXaxis()->SetTitle("W_{exp} (GeV)");
    mc_w_QE->GetYaxis()->SetTitle("Events/Bin");
 
    mc_w_RES_1232 = new TH1D("mc_w_RES_1232","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    mc_w_RES_1232->GetXaxis()->SetTitle("W_{exp} (GeV)");
    mc_w_RES_1232->GetYaxis()->SetTitle("Events/Bin");

    mc_w_RES_1535 = new TH1D("mc_w_RES_1535","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    mc_w_RES_1535->GetXaxis()->SetTitle("W_{exp} (GeV)");
    mc_w_RES_1535->GetYaxis()->SetTitle("Events/Bin");

    mc_w_RES_1520 = new TH1D("mc_w_RES_1520","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    mc_w_RES_1520->GetXaxis()->SetTitle("W_{exp} (GeV)");
    mc_w_RES_1520->GetYaxis()->SetTitle("Events/Bin");

    mc_w_RES_Other = new TH1D("mc_w_RES_Other","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    mc_w_RES_Other->GetXaxis()->SetTitle("W_{exp} (GeV)");
    mc_w_RES_Other->GetYaxis()->SetTitle("Events/Bin");

    mc_w_DIS = new TH1D("mc_w_DIS","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    mc_w_DIS->GetXaxis()->SetTitle("W_{exp} (GeV)");
    mc_w_DIS->GetYaxis()->SetTitle("Events/Bin");

    mc_w_2p2h = new TH1D("mc_w_2p2h","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    mc_w_2p2h->GetXaxis()->SetTitle("W_{exp} (GeV)");
    mc_w_2p2h->GetYaxis()->SetTitle("Events/Bin");

    mc_w_Non_RES = new TH1D("mc_w_Non_RES","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    mc_w_Non_RES->GetXaxis()->SetTitle("W_{exp} (GeV)");
    mc_w_Non_RES->GetYaxis()->SetTitle("Events/Bin");

    // ------------------------------------------------------------------------
    // Signal Truth w
    // ------------------------------------------------------------------------
    truth_w_QE = new TH1D("truth_w_QE","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    truth_w_QE->GetXaxis()->SetTitle("W_{exp} (GeV)");
    truth_w_QE->GetYaxis()->SetTitle("Events/Bin");
 
    truth_w_RES_1232 = new TH1D("truth_w_RES_1232","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    truth_w_RES_1232->GetXaxis()->SetTitle("W_{exp} (GeV)");
    truth_w_RES_1232->GetYaxis()->SetTitle("Events/Bin");

    truth_w_RES_1535 = new TH1D("truth_w_RES_1535","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    truth_w_RES_1535->GetXaxis()->SetTitle("W_{exp} (GeV)");
    truth_w_RES_1535->GetYaxis()->SetTitle("Events/Bin");

    truth_w_RES_1520 = new TH1D("truth_w_RES_1520","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    truth_w_RES_1520->GetXaxis()->SetTitle("W_{exp} (GeV)");
    truth_w_RES_1520->GetYaxis()->SetTitle("Events/Bin");

    truth_w_RES_Other = new TH1D("truth_w_RES_Other","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    truth_w_RES_Other->GetXaxis()->SetTitle("W_{exp} (GeV)");
    truth_w_RES_Other->GetYaxis()->SetTitle("Events/Bin");

    truth_w_DIS = new TH1D("truth_w_DIS","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    truth_w_DIS->GetXaxis()->SetTitle("W_{exp} (GeV)");
    truth_w_DIS->GetYaxis()->SetTitle("Events/Bin");

    truth_w_2p2h = new TH1D("truth_w_2p2h","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    truth_w_2p2h->GetXaxis()->SetTitle("W_{exp} (GeV)");
    truth_w_2p2h->GetYaxis()->SetTitle("Events/Bin");

    truth_w_Non_RES = new TH1D("truth_w_Non_RES","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    truth_w_Non_RES->GetXaxis()->SetTitle("W_{exp} (GeV)");
    truth_w_Non_RES->GetYaxis()->SetTitle("Events/Bin");

    // ------------------------------------------------------------------------
    // Background Truth w
    // ------------------------------------------------------------------------
    reco_bckg_w_QE = new TH1D("reco_bckg_w_QE","W for Background Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    reco_bckg_w_QE->GetXaxis()->SetTitle("W_{exp} (GeV)");
    reco_bckg_w_QE->GetYaxis()->SetTitle("Events/Bin");
 
    reco_bckg_w_RES_1232 = new TH1D("reco_bckg_w_RES_1232","W for Background Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    reco_bckg_w_RES_1232->GetXaxis()->SetTitle("W_{exp} (GeV)");
    reco_bckg_w_RES_1232->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_w_RES_1535 = new TH1D("reco_bckg_w_RES_1535","W for Background Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    reco_bckg_w_RES_1535->GetXaxis()->SetTitle("W_{exp} (GeV)");
    reco_bckg_w_RES_1535->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_w_RES_1520 = new TH1D("reco_bckg_w_RES_1520","W for Background Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    reco_bckg_w_RES_1520->GetXaxis()->SetTitle("W_{exp} (GeV)");
    reco_bckg_w_RES_1520->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_w_RES_Other = new TH1D("reco_bckg_w_RES_Other","W for Background Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    reco_bckg_w_RES_Other->GetXaxis()->SetTitle("W_{exp} (GeV)");
    reco_bckg_w_RES_Other->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_w_DIS = new TH1D("reco_bckg_w_DIS","W for Background Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    reco_bckg_w_DIS->GetXaxis()->SetTitle("W_{exp} (GeV)");
    reco_bckg_w_DIS->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_w_2p2h = new TH1D("reco_bckg_w_2p2h","W for Background Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    reco_bckg_w_2p2h->GetXaxis()->SetTitle("W_{exp} (GeV)");
    reco_bckg_w_2p2h->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_w_Non_RES = new TH1D("reco_bckg_w_Non_RES","W for Background Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    reco_bckg_w_Non_RES->GetXaxis()->SetTitle("W_{exp} (GeV)");
    reco_bckg_w_Non_RES->GetYaxis()->SetTitle("Events/Bin");

    reco_bckg_w_Coh = new TH1D("reco_bckg_w_Coh","W for Background Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    reco_bckg_w_Coh->GetXaxis()->SetTitle("W_{exp} (GeV)");
    reco_bckg_w_Coh->GetYaxis()->SetTitle("Events/Bin");

    // ------------------------------------------------------------------------
    // Signal w
    // ------------------------------------------------------------------------
    reco_w_QE = new TH1D("reco_w_QE","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    reco_w_QE->GetXaxis()->SetTitle("W_{exp} (GeV)");
    reco_w_QE->GetYaxis()->SetTitle("Events/Bin");
 
    reco_w_RES_1232 = new TH1D("reco_w_RES_1232","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    reco_w_RES_1232->GetXaxis()->SetTitle("W_{exp} (GeV)");
    reco_w_RES_1232->GetYaxis()->SetTitle("Events/Bin");

    reco_w_RES_1535 = new TH1D("reco_w_RES_1535","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    reco_w_RES_1535->GetXaxis()->SetTitle("W_{exp} (GeV)");
    reco_w_RES_1535->GetYaxis()->SetTitle("Events/Bin");

    reco_w_RES_1520 = new TH1D("reco_w_RES_1520","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    reco_w_RES_1520->GetXaxis()->SetTitle("W_{exp} (GeV)");
    reco_w_RES_1520->GetYaxis()->SetTitle("Events/Bin");

    reco_w_RES_Other = new TH1D("reco_w_RES_Other","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    reco_w_RES_Other->GetXaxis()->SetTitle("W_{exp} (GeV)");
    reco_w_RES_Other->GetYaxis()->SetTitle("Events/Bin");

    reco_w_DIS = new TH1D("reco_w_DIS","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    reco_w_DIS->GetXaxis()->SetTitle("W_{exp} (GeV)");
    reco_w_DIS->GetYaxis()->SetTitle("Events/Bin");

    reco_w_2p2h = new TH1D("reco_w_2p2h","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    reco_w_2p2h->GetXaxis()->SetTitle("W_{exp} (GeV)");
    reco_w_2p2h->GetYaxis()->SetTitle("Events/Bin");

    reco_w_Non_RES = new TH1D("reco_w_Non_RES","W for Signal Events",binList.mc_w.get_nBins(), binList.mc_w.get_min(), binList.mc_w.get_max());
    reco_w_Non_RES->GetXaxis()->SetTitle("W_{exp} (GeV)");
    reco_w_Non_RES->GetYaxis()->SetTitle("Events/Bin");

    // Random Number Generator Histograms 
    normal_rand_numbers = new TH1D( "normal_rand_numbers","Normal Random Numbers",50,-3.0,3.0);
    normal_rand_numbers->GetXaxis()->SetTitle("Normal Random Numbers");
    normal_rand_numbers->GetYaxis()->SetTitle("N(Numbers)");

    em_shift_rand_numbers = new TH1D( "em_shift_rand_numbers","EM Energy Scale Shift Random Numbers",50,-0.1,0.1);
    em_shift_rand_numbers->GetXaxis()->SetTitle("EM Energy Scale Shift");
    em_shift_rand_numbers->GetYaxis()->SetTitle("N(Numbers)");

    muonP_shift_rand_numbers = new TH1D( "muonP_shift_rand_numbers","Muon Momentum Shift Random Numbers",50,-0.1,0.1);
    muonP_shift_rand_numbers->GetXaxis()->SetTitle("Muon Momentum Shift");
    muonP_shift_rand_numbers->GetYaxis()->SetTitle("N(Numbers)");

    muon_theta_shift_rand_numbers = new TH1D( "muon_theta_shift_rand_numbers","Muon Theta Shift Random Numbers",50,-0.02,0.02);
    muon_theta_shift_rand_numbers->GetXaxis()->SetTitle("Muon Theta Shift");
    muon_theta_shift_rand_numbers->GetYaxis()->SetTitle("N(Numbers)");

    Birks_shift_rand_numbers = new TH1D( "Birks_shift_rand_numbers","Proton Energy Birks Shift Random Numbers",50,-0.02,0.02);
    Birks_shift_rand_numbers->GetXaxis()->SetTitle("Proton Energy Birks Shift");
    Birks_shift_rand_numbers->GetYaxis()->SetTitle("N(Numbers)");

    Err_NeutronResponse = new TH1D( "Err_NeutronResponse","Neutron Response Error",50,-1,1);
    Err_NeutronResponse->GetXaxis()->SetTitle("Error used as (wgt = 1 #pm error)");
    Err_NeutronResponse->GetYaxis()->SetTitle("Events/Bin");

    Err_PionResponse = new TH1D( "Err_PionResponse","Pion Response Error",50,-1,1);
    Err_PionResponse->GetXaxis()->SetTitle("Error used as (wgt = 1 #pm error)");
    Err_PionResponse->GetYaxis()->SetTitle("Events/Bin");

    Err_MuonTracking = new TH1D( "Err_MuonTracking","Muon Tracking Error",50,-1,1);
    Err_MuonTracking->GetXaxis()->SetTitle("Error used as (wgt = 1 #pm error)");
    Err_MuonTracking->GetYaxis()->SetTitle("Events/Bin");


    for (int i = 0; i < 151; ++i){
        temp = new MnvH1D( Form("%s_%d","W_Shift",i),"",13,0.5,1.8);
        temp->GetXaxis()->SetTitle("W_{exp} (GeV)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        W_Shift.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","W_Shift_Bckg",i),"",13,0.5,1.8);
        temp->GetXaxis()->SetTitle("W_{exp} (GeV)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        W_Shift_Bckg.push_back(temp);

        temp = new MnvH1D( Form("%s_%d","W_Shift_Signal",i),"",13,0.5,1.8);
        temp->GetXaxis()->SetTitle("W_{exp} (GeV)");
        temp->GetYaxis()->SetTitle("Events/Bin");
        W_Shift_Signal.push_back(temp);
    }
}

void CCProtonPi0_Interaction::writeHistograms()
{
    std::cout<<">> Writing "<<rootDir<<std::endl;
   
    f->cd();

    for (int i = 0; i < nHistograms; i++){
        CV_weight[i]->Write();
        CV_weight_Flux[i]->Write();
        CV_weight_2p2h[i]->Write();
        CV_weight_Delta[i]->Write();
        CV_weight_CCRES[i]->Write();
        CV_weight_NonRes1pi[i]->Write();
        err_2p2h[i]->Write();
        genie_wgt_VecFFCCQEshape[i]->Write();
        genie_wgt_NormDISCC[i]->Write();
        genie_wgt_Theta_Delta2Npi[i]->Write();
        updated_wgt_Theta_Delta2Npi[i]->Write();
        genie_wgt_MaRES[i]->Write();
        updated_wgt_MaRES[i]->Write();
        genie_wgt_MvRES[i]->Write();
        updated_wgt_MvRES[i]->Write();
        genie_wgt_Rvn1pi[i]->Write();
        updated_wgt_Rvn1pi[i]->Write();

        // Event Kinematics
        Enu[i]->Write();
        Enu_1Track[i]->Write();
        Enu_2Track[i]->Write();
        QSq[i]->Write();
        QSq_1Track[i]->Write();
        QSq_2Track[i]->Write();
        WSq[i]->Write();
        WSq_1Track[i]->Write();
        WSq_2Track[i]->Write();
        W[i]->Write();
        W_1Track[i]->Write();
        W_2Track[i]->Write();
       
        // 2p2h Study 
        vertex_energy_All[i]->Write();
        vertex_energy_1Track[i]->Write();
        vertex_energy_2Track[i]->Write();
        vertex_evis_All[i]->Write();
        vertex_evis_1Track[i]->Write();
        vertex_evis_2Track[i]->Write();
        W_QSq[i]->Write();

        // Extra Energy
        extra_leftover_energy_1Track[i]->Write();
        extra_muon_energy_1Track[i]->Write();
        extra_rejected_energy_1Track[i]->Write();
        extra_total_energy_1Track[i]->Write();

        extra_leftover_energy_2Track[i]->Write();
        extra_muon_energy_2Track[i]->Write();
        extra_rejected_energy_2Track[i]->Write();
        extra_total_energy_2Track[i]->Write();

        // Invariant Mass
        pi0_invMass_All[i]->Write();
        pi0_invMass_1Track[i]->Write();
        pi0_invMass_2Track[i]->Write();
        pi0_invMass_DeltaRES[i]->Write();

        W_p_pi0[i]->Write();
        W_All[i]->Write();
        W_1[i]->Write();
        W_2[i]->Write();
        QSq_CV[i]->Write();
        QSq_MaRES[i]->Write();

        vertex_z[i]->Write();
    }

    resID->Write();
    resID_theta->Write();

    QSq_all->Write();
    QSq_mc_truth_signal->Write();
    QSq_mc_reco_all->Write();
    QSq_mc_reco_signal->Write();
    QSq_mc_reco_bckg->Write();
    QSq_response->Write();
 
    Enu_all->Write();
    Enu_mc_truth_signal->Write();
    Enu_mc_reco_all->Write();
    Enu_mc_reco_signal->Write();
    Enu_mc_reco_bckg->Write();
    Enu_response->Write();
 
    W_all->Write();
    W_mc_truth_signal->Write();
    W_mc_reco_all->Write();
    W_mc_reco_signal->Write();
    W_mc_reco_bckg->Write();
    W_response->Write();
 
    deltaInvMass_all->Write();
    deltaInvMass_mc_truth_signal->Write();
    deltaInvMass_mc_reco_all->Write();
    deltaInvMass_mc_reco_signal->Write();
    deltaInvMass_mc_reco_bckg->Write();
    deltaInvMass_response->Write();
    
    Delta_pi_theta_all->Write();
    Delta_pi_theta_mc_truth_signal->Write();
    Delta_pi_theta_mc_reco_all->Write();
    Delta_pi_theta_mc_reco_signal->Write();
    Delta_pi_theta_mc_reco_bckg->Write();
    Delta_pi_theta_response->Write();
    
    Delta_pi_phi_all->Write();
    Delta_pi_phi_mc_truth_signal->Write();
    Delta_pi_phi_mc_reco_all->Write();
    Delta_pi_phi_mc_reco_signal->Write();
    Delta_pi_phi_mc_reco_bckg->Write();
    Delta_pi_phi_response->Write();
     
    muon_theta_muon_KE_all->Write();
    muon_theta_muon_KE_mc_truth_signal->Write();
    muon_theta_muon_KE_mc_reco_all->Write();
    muon_theta_muon_KE_mc_reco_signal->Write();
    muon_theta_muon_KE_mc_reco_bckg->Write();
   
    q3_q0_all->Write();
    q3_q0_mc_truth_signal->Write();
    q3_q0_mc_reco_all->Write();
    q3_q0_mc_reco_signal->Write();
    q3_q0_mc_reco_bckg->Write();

    // Short Proton
    nProtons->Write();
    proton_true_P_1Track->Write();
    proton_true_KE_1Track->Write();
    proton_true_theta_1Track->Write();
    
    // Ejected Nucleons
    n_ejected_nucleons_1Track->Write();
    n_ejected_nucleons_2Track->Write();

    //Extra Energy
    h_extra_muon_energy->Write();
    h_extra_rejected_energy->Write();
    
    // QSq Truth, Error, Difference
    WSq_QSq_Diff->Write();
    QSq_All_response->Write();
    QSq_1Track_response->Write();
    QSq_2Track_response->Write();
    
    QSq_Error->Write();
    QSq_1Track_Error->Write();
    QSq_2Track_Error->Write();

    QSq_Diff->Write();
    QSq_1Track_Diff->Write();
    QSq_2Track_Diff->Write();
   
    // W Error, Difference
    W_Diff->Write();
    W_Error->Write();

    // Neutrino Energy: Error, Difference
    Enu_All_response->Write();
    Enu_1Track_response->Write();
    Enu_2Track_response->Write();
    
    Enu_Error->Write();
    Enu_1Track_Error->Write();
    Enu_2Track_Error->Write();

    Enu_Diff->Write();
    Enu_1Track_Diff->Write();
    Enu_2Track_Diff->Write();

    // Signal Q2
    mc_Q2_QE->Write();
    mc_Q2_RES_1232->Write();
    mc_Q2_RES_1535->Write();
    mc_Q2_RES_1520->Write();
    mc_Q2_RES_Other->Write();
   
    mc_Q2_DIS->Write();
    mc_Q2_2p2h->Write();
    mc_Q2_Non_RES->Write();
 
    truth_QSq_QE->Write();
    truth_QSq_RES_1232->Write();
    truth_QSq_RES_1535->Write();
    truth_QSq_RES_1520->Write();
    truth_QSq_RES_Other->Write();
   
    truth_QSq_DIS->Write();
    truth_QSq_2p2h->Write();
    truth_QSq_Non_RES->Write();
 
    reco_bckg_QSq_QE->Write();
    reco_bckg_QSq_RES_1232->Write();
    reco_bckg_QSq_RES_1535->Write();
    reco_bckg_QSq_RES_1520->Write();
    reco_bckg_QSq_RES_Other->Write();
   
    reco_bckg_QSq_DIS->Write();
    reco_bckg_QSq_2p2h->Write();
    reco_bckg_QSq_Non_RES->Write();
    reco_bckg_QSq_Coh->Write();

    // Signal incomingE
    mc_incomingE_QE->Write();
    mc_incomingE_RES_1232->Write();
    mc_incomingE_RES_1535->Write();
    mc_incomingE_RES_1520->Write();
    mc_incomingE_RES_Other->Write();
   
    mc_incomingE_DIS->Write();
    mc_incomingE_2p2h->Write();
    mc_incomingE_Non_RES->Write();
 
    truth_Enu_QE->Write();
    truth_Enu_RES_1232->Write();
    truth_Enu_RES_1535->Write();
    truth_Enu_RES_1520->Write();
    truth_Enu_RES_Other->Write();
   
    truth_Enu_DIS->Write();
    truth_Enu_2p2h->Write();
    truth_Enu_Non_RES->Write();
 
    reco_bckg_Enu_QE->Write();
    reco_bckg_Enu_RES_1232->Write();
    reco_bckg_Enu_RES_1535->Write();
    reco_bckg_Enu_RES_1520->Write();
    reco_bckg_Enu_RES_Other->Write();
   
    reco_bckg_Enu_DIS->Write();
    reco_bckg_Enu_2p2h->Write();
    reco_bckg_Enu_Non_RES->Write();
    reco_bckg_Enu_Coh->Write();

    // Signal w
    mc_w_QE->Write();
    mc_w_RES_1232->Write();
    mc_w_RES_1535->Write();
    mc_w_RES_1520->Write();
    mc_w_RES_Other->Write();
   
    mc_w_DIS->Write();
    mc_w_2p2h->Write();
    mc_w_Non_RES->Write();

    truth_w_QE->Write();
    truth_w_RES_1232->Write();
    truth_w_RES_1535->Write();
    truth_w_RES_1520->Write();
    truth_w_RES_Other->Write();
   
    truth_w_DIS->Write();
    truth_w_2p2h->Write();
    truth_w_Non_RES->Write();

    reco_bckg_w_QE->Write();
    reco_bckg_w_RES_1232->Write();
    reco_bckg_w_RES_1535->Write();
    reco_bckg_w_RES_1520->Write();
    reco_bckg_w_RES_Other->Write();
   
    reco_bckg_w_DIS->Write();
    reco_bckg_w_2p2h->Write();
    reco_bckg_w_Non_RES->Write();
    reco_bckg_w_Coh->Write();

    // Signal reco w
    reco_w_QE->Write();
    reco_w_RES_1232->Write();
    reco_w_RES_1535->Write();
    reco_w_RES_1520->Write();
    reco_w_RES_Other->Write();
   
    reco_w_DIS->Write();
    reco_w_2p2h->Write();
    reco_w_Non_RES->Write();

    DeltaTransverse_data->Write();
    DeltaTransverse_mc->Write();
    DeltaTransverse_mc_res->Write();

    normal_rand_numbers->Write();
    em_shift_rand_numbers->Write();
    muonP_shift_rand_numbers->Write();
    muon_theta_shift_rand_numbers->Write();
    Birks_shift_rand_numbers->Write();

    Err_NeutronResponse->Write();
    Err_PionResponse->Write();
    Err_MuonTracking->Write();

    // Flux Study
    Enu_flux_wgt->Write();
    Enu_cvweight->Write();

    for (int i = 0; i < 151; ++i){
        W_Shift[i]->Write();
        W_Shift_Bckg[i]->Write();
        W_Shift_Signal[i]->Write();
    }

    f->Close();
}



#endif
