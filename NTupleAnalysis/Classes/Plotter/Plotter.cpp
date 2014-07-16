/*
    See Plotter.h header for Class Information
*/

#ifndef PLOTTER_CPP
#define PLOTTER_CPP
#include "Plotter.h"

using namespace std;


void Plotter::plotHistograms(bool isMC, bool isReco, bool is2D)
{
    
    plotInteraction(isMC,isReco, is2D);

    plotMuon(isMC,isReco, is2D);
    
    plotProton(isMC,isReco, is2D);
    
    plotPion(isMC,isReco, is2D);
    
    plotPID();
    
}

void Plotter::plotPID()
{
    string rootDir = "Output/RootFiles/Interaction.root";
    string plotDir = "Output/Plots/Interaction/";
    cout<<"Plottting pID for Proton"<<endl;
    inform(rootDir, plotDir);
    
    TFile* f_Root = new TFile(rootDir.c_str());
    
    THStack *hs = new THStack("hs","Proton Score");
    
    TH1D* h_pID_other = (TH1D*)f_Root->Get("pID_other");
    h_pID_other->SetFillColor(kRed);
    h_pID_other->SetMarkerStyle(21);
    h_pID_other->SetMarkerColor(kRed);
    
    TH1D* h_pID_piminus = (TH1D*)f_Root->Get("pID_piminus");
    h_pID_piminus->SetFillColor(kYellow);
    h_pID_piminus->SetMarkerStyle(21);
    h_pID_piminus->SetMarkerColor(kYellow);
    
    TH1D* h_pID_piplus = (TH1D*)f_Root->Get("pID_piplus");
    h_pID_piplus->SetFillColor(kBlue);
    h_pID_piplus->SetMarkerStyle(21);
    h_pID_piplus->SetMarkerColor(kBlue);
    
    TH1D* h_pID_proton = (TH1D*)f_Root->Get("pID_proton");
    h_pID_proton->SetFillColor(kGreen);
    h_pID_proton->SetMarkerStyle(21);
    h_pID_proton->SetMarkerColor(kGreen);
    
    TCanvas* c1 = new TCanvas();

    hs->Add(h_pID_other);
    hs->Add(h_pID_piminus);
    hs->Add(h_pID_piplus);
    hs->Add(h_pID_proton);
    hs->Draw();
    hs->GetXaxis()->SetTitle("Proton Score");
    hs->GetYaxis()->SetTitle(Form("Candidates / %3.2f ",0.05));
    
    c1->Print(Form("%s%s",plotDir.c_str(),"stacked_pID.png"),"png");

}

void Plotter::plotInteraction(bool isMC, bool isReco, bool is2D)
{

    string rootDir = "Output/RootFiles/Interaction.root";
    string plotDir = "Output/Plots/Interaction/";
    inform(rootDir, plotDir);
    
    TFile* f_Root = new TFile(rootDir.c_str());
    
    // Plot Only MC Values
    if( isMC ){
    
        TH1D* h_deltaInvMass_mc = (TH1D*)f_Root->Get("deltaInvMass_mc");
        plot1D_Hist(h_deltaInvMass_mc ,"deltaInvMass_mc.png",plotDir);
        
        TH1D* h_beamEnergy_mc= (TH1D*)f_Root->Get("beamEnergy_mc");
        plot1D_Hist(h_beamEnergy_mc,"beamEnergy_mc.png",plotDir);
        
        TH1D* h_q2_mc= (TH1D*)f_Root->Get("q2_mc");
        plot1D_Hist(h_q2_mc,"q2_mc.png",plotDir);
        
        TH1D* h_int_channel= (TH1D*)f_Root->Get("int_channel");
        plot1D_Hist(h_int_channel,"int_channel.png",plotDir);
        
        TH1D* h_vertex_z_true= (TH1D*)f_Root->Get("vertex_z_true");
        plot1D_Hist(h_vertex_z_true,"vertex_z_true.png",plotDir);
        
        TH2D* h_vertex_x_y_true= (TH2D*)f_Root->Get("vertex_x_y_true");
        plot2D_Hist(h_vertex_x_y_true,"vertex_x_y_true.png",plotDir);
    
    }
    
    // Plot Only Reco Values
    if( isReco ){
        TH1D* h_deltaInvMass_reco = (TH1D*)f_Root->Get("deltaInvMass_reco");
        plot1D_Hist(h_deltaInvMass_reco,"deltaInvMass_reco.png",plotDir);
        
        TH1D* h_vertex_z_reco= (TH1D*)f_Root->Get("vertex_z_reco");
        plot1D_Hist(h_vertex_z_reco,"vertex_z_reco.png",plotDir);
        
        TH2D* h_vertex_x_y_reco= (TH2D*)f_Root->Get("vertex_x_y_reco");
        plot2D_Hist(h_vertex_x_y_reco,"vertex_x_y_reco.png",plotDir);
        
        TH1D* h_pFilter_Status = (TH1D*)f_Root->Get("pFilter_Status");
        plot1D_Hist(h_pFilter_Status,"pFilter_Status.png",plotDir);
        
        TH1D* h_pFilter_RejectedEnergy = (TH1D*)f_Root->Get("pFilter_RejectedEnergy");
        plot1D_Hist(h_pFilter_RejectedEnergy,"pFilter_RejectedEnergy.png",plotDir);
        
//         TH1D* h_beamEnergy_reco= (TH1D*)f_Root->Get("beamEnergy_reco");
//         plot1D_Hist(h_beamEnergy_reco,"beamEnergy_reco.png",plotDir);
//         
//         TH1D* h_q2_reco= (TH1D*)f_Root->Get("q2_reco");
//         plot1D_Hist(h_q2_reco,"q2_reco.png",plotDir);    

    }

    //  Plot 2D Comparison Plots and Error Plots
    if ( is2D ){
        TH2D* h_deltaInvMass_reco_mc = (TH2D*)f_Root->Get("deltaInvMass_reco_mc");
        plot2D_Hist(h_deltaInvMass_reco_mc,"deltaInvMass_reco_mc.png",plotDir);
        
        TH1D* h_deltaInvMass_error = (TH1D*)f_Root->Get("deltaInvMass_error");
        plot1D_Hist(h_deltaInvMass_error,"deltaInvMass_error.png",plotDir);
        
        
//         TH2D* h_beamEnergy_reco_mc= (TH2D*)f_Root->Get("beamEnergy_reco_mc");
//         plot2D_Hist(h_beamEnergy_reco_mc,"beamEnergy_reco_mc.png",plotDir);
// 
//         TH1D* h_beamEnergy_error= (TH1D*)f_Root->Get("beamEnergy_error");
//         plot1D_Hist(h_beamEnergy_error,"beamEnergy_error.png",plotDir);
//         
//         TH2D* h_q2_reco_mc= (TH2D*)f_Root->Get("q2_reco_mc");
//         plot2D_Hist(h_q2_reco_mc,"q2_reco_mc.png",plotDir);
//         
//         TH1D* h_q2_error= (TH1D*)f_Root->Get("q2_error");
//         plot1D_Hist(h_q2_error,"q2_error.png",plotDir);

        TH2D* h_vertex_z_reco_mc= (TH2D*)f_Root->Get("vertex_z_reco_mc");
        plot2D_Hist(h_vertex_z_reco_mc,"vertex_z_reco_mc.png",plotDir);
        
        TH1D* h_vertex_z_error= (TH1D*)f_Root->Get("vertex_z_error");
        plot1D_Hist(h_vertex_z_error,"vertex_z_error.png",plotDir);
    }
    
    
}


void Plotter::plotParticleInfo(  string rootDir, string plotDir, 
                                bool isMC, bool isReco, bool is2D)
{

    inform(rootDir, plotDir);
    
    TFile* f_Root = new TFile(rootDir.c_str());

    // Plot Only MC Values
    if( isMC ){
        TH1D* h_P_mc = (TH1D*)f_Root->Get("P_mc");
        plot1D_Hist(h_P_mc,"P_mc.png",plotDir);
        
        TH1D* h_KE_mc = (TH1D*)f_Root->Get("KE_mc");
        plot1D_Hist(h_KE_mc,"KE_mc.png",plotDir);
        
        TH1D* h_angleBeam_mc = (TH1D*)f_Root->Get("angleBeam_mc");
        plot1D_Hist(h_angleBeam_mc,"angleBeam_mc.png",plotDir);
        
        TH1D* h_angleMuon_mc = (TH1D*)f_Root->Get("angleMuon_mc");
        plot1D_Hist(h_angleMuon_mc,"angleMuon_mc.png",plotDir);
    }
    
    // Plot Only Reco Values
    if( isReco ){
        TH1D* h_partScore = (TH1D*)f_Root->Get("partScore");
        plot1D_Hist(h_partScore,"partScore.png",plotDir);
        
        TH1D* h_P_reco = (TH1D*)f_Root->Get("P_reco");
        plot1D_Hist(h_P_reco,"P_reco.png",plotDir);
        
        TH1D* h_KE_reco = (TH1D*)f_Root->Get("KE_reco");
        plot1D_Hist(h_KE_reco,"KE_reco.png",plotDir);    
        
        TH1D* h_angleBeam_reco = (TH1D*)f_Root->Get("angleBeam_reco");
        plot1D_Hist(h_angleBeam_reco,"angleBeam_reco.png",plotDir);
        
        TH1D* h_angleMuon_reco = (TH1D*)f_Root->Get("angleMuon_reco");
        plot1D_Hist(h_angleMuon_reco,"angleMuon_reco.png",plotDir);
        
    }

    //  Plot 2D Comparison Plots and Error Plots
    if ( is2D ){
        TH2D* h_P_reco_mc = (TH2D*)f_Root->Get("P_reco_mc");
        plot2D_Hist(h_P_reco_mc,"P_reco_mc.png",plotDir);

        TH1D* h_P_error = (TH1D*)f_Root->Get("P_error");
        plot1D_Hist(h_P_error,"P_error.png",plotDir);
        
        TH2D* h_KE_reco_mc = (TH2D*)f_Root->Get("KE_reco_mc");
        plot2D_Hist(h_KE_reco_mc,"KE_reco_mc.png",plotDir);
        
        TH1D* h_KE_error = (TH1D*)f_Root->Get("KE_error");
        plot1D_Hist(h_KE_error,"KE_error.png",plotDir);
        
        TH2D* h_angleBeam_reco_mc = (TH2D*)f_Root->Get("angleBeam_reco_mc");
        plot2D_Hist(h_angleBeam_reco_mc,"angleBeam_reco_mc.png",plotDir);
        
        TH1D* h_angleBeam_error = (TH1D*)f_Root->Get("angleBeam_error");
        plot1D_Hist(h_angleBeam_error,"angleBeam_error.png",plotDir);
        
        TH2D* h_angleMuon_reco_mc = (TH2D*)f_Root->Get("angleMuon_reco_mc");
        plot2D_Hist(h_angleMuon_reco_mc,"angleMuon_reco_mc.png",plotDir);
        
        TH1D* h_angleMuon_error = (TH1D*)f_Root->Get("angleMuon_error");
        plot1D_Hist(h_angleMuon_error,"angleMuon_error.png",plotDir);
    }

}

void Plotter::plotMuon(bool isMC, bool isReco, bool is2D)
{
    string rootDir = "Output/RootFiles/Muon.root";
    string plotDir = "Output/Plots/Muon/";
    
    plotParticleInfo(rootDir, plotDir, isMC, isReco, is2D);
}

void Plotter::plotProton(bool isMC, bool isReco, bool is2D)
{
    string rootDir = "Output/RootFiles/Proton.root";
    string plotDir = "Output/Plots/Proton/";
    
    plotParticleInfo(rootDir, plotDir, isMC, isReco, is2D);
}

void Plotter::plotPion(bool isMC, bool isReco, bool is2D)
{
    string rootDir = "Output/RootFiles/Pion.root";
    string plotDir = "Output/Plots/Pion/";
    
    // Standard Plots
    plotParticleInfo(rootDir, plotDir, isMC, isReco, is2D);
    
    // Unique Plots
    TFile* f_Root = new TFile(rootDir.c_str());
    
    TH1D* h_invMass = (TH1D*)f_Root->Get("invMass");
    plot1D_Hist(h_invMass,"invMass.png",plotDir);
    
    plot_Pi0_P_Stacked();
    plot_Pi0_invMass_Stacked();
    
}

void Plotter::plot_Pi0_P_Stacked()
{
    string rootDir = "Output/RootFiles/Pion.root";
    string plotDir = "Output/Plots/Pion/";
    
    inform(rootDir, plotDir);
    
    THStack *hs = new THStack("hs","Pi0 Momentum");
    
    TFile* f_Root = new TFile(rootDir.c_str());
    
    TH1D* h_P_reco_0Pi0 = (TH1D*)f_Root->Get("P_reco_0Pi0");
    h_P_reco_0Pi0->SetFillColor(kRed);
    h_P_reco_0Pi0->SetMarkerStyle(21);
    h_P_reco_0Pi0->SetMarkerColor(kRed);
    
    TH1D* h_P_reco_1Pi0 = (TH1D*)f_Root->Get("P_reco_1Pi0");
    h_P_reco_1Pi0->SetFillColor(kBlue);
    h_P_reco_1Pi0->SetMarkerStyle(21);
    h_P_reco_1Pi0->SetMarkerColor(kBlue);

    TH1D* h_P_reco_MultPi0 = (TH1D*)f_Root->Get("P_reco_MultPi0");
    h_P_reco_MultPi0->SetFillColor(kYellow);
    h_P_reco_MultPi0->SetMarkerStyle(21);
    h_P_reco_MultPi0->SetMarkerColor(kYellow);
    
    TCanvas* c1 = new TCanvas();

    hs->Add(h_P_reco_0Pi0);
    hs->Add(h_P_reco_1Pi0);
    hs->Add(h_P_reco_MultPi0);
    hs->Draw();
    hs->GetXaxis()->SetTitle("Pi0 Momentum");
    hs->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",100.0));
    
    c1->Print(Form("%s%s",plotDir.c_str(),"stacked_Pi0_Momentum.png"),"png");
    
    // Plot Other Momentum Plots
    TH2D* h_P_reco_mc_1Pi0= (TH2D*)f_Root->Get("P_reco_mc_1Pi0");
    plot2D_Hist(h_P_reco_mc_1Pi0,"P_reco_mc_1Pi0.png",plotDir);
    
    TH1D* h_P_error_1Pi0= (TH1D*)f_Root->Get("P_error_1Pi0");
    plot1D_Hist(h_P_error_1Pi0,"P_error_1Pi0.png",plotDir);
    
    TH2D* h_P_reco_mc_MultPi0= (TH2D*)f_Root->Get("P_reco_mc_MultPi0");
    plot2D_Hist(h_P_reco_mc_MultPi0,"P_reco_mc_MultPi0.png",plotDir);

    TH1D* h_P_error_MultPi0= (TH1D*)f_Root->Get("P_error_MultPi0");
    plot1D_Hist(h_P_error_MultPi0,"P_error_MultPi0.png",plotDir);
}

void Plotter::plot_Pi0_invMass_Stacked()
{
    string rootDir = "Output/RootFiles/Pion.root";
    string plotDir = "Output/Plots/Pion/";
    
    inform(rootDir, plotDir);
    
    THStack *hs = new THStack("hs","Pi0 Invariant Mass");
    
    TFile* f_Root = new TFile(rootDir.c_str());
    
    TH1D* h_invMass_0Pi0 = (TH1D*)f_Root->Get("invMass_0Pi0");
    h_invMass_0Pi0->SetFillColor(kRed);
    h_invMass_0Pi0->SetMarkerStyle(21);
    h_invMass_0Pi0->SetMarkerColor(kRed);
    
    TH1D* h_invMass_1Pi0 = (TH1D*)f_Root->Get("invMass_1Pi0");
    h_invMass_1Pi0->SetFillColor(kBlue);
    h_invMass_1Pi0->SetMarkerStyle(21);
    h_invMass_1Pi0->SetMarkerColor(kBlue);

    TH1D* h_invMass_MultPi0 = (TH1D*)f_Root->Get("invMass_MultPi0");
    h_invMass_MultPi0->SetFillColor(kYellow);
    h_invMass_MultPi0->SetMarkerStyle(21);
    h_invMass_MultPi0->SetMarkerColor(kYellow);
    
    TCanvas* c1 = new TCanvas();
    
    hs->Add(h_invMass_0Pi0);
    hs->Add(h_invMass_1Pi0);
    hs->Add(h_invMass_MultPi0);
    hs->Draw();
    hs->GetXaxis()->SetTitle("Pi0 Invariant Mass");
    hs->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",100.0));
    
    c1->Print(Form("%s%s",plotDir.c_str(),"stacked_Pi0_invMass.png"),"png");

}

void Plotter::inform(string rootDir, string plotDir)
{
    cout<<"------------ Plotting ------------"<<endl;
    cout<<"Input File: "<<rootDir<<endl;
    cout<<"Output Folder: "<<plotDir<<endl;

}

void Plotter::plot1D_Hist(TH1D* hist1D, string fileName, string plotDir)
{
    TCanvas* c1 = new TCanvas();
    hist1D->SetLineColor(kRed);
    hist1D->SetLineWidth(3);
    hist1D->SetFillColor(kRed);
    hist1D->SetFillStyle(3010);
    
    hist1D->Draw();
    gPad->Update();
    
    // Statistics Box
//     TPaveStats *st = (TPaveStats*)hist1D->FindObject("stats");
    
    c1->Print(Form("%s%s",plotDir.c_str(),fileName.c_str()),"png");
    delete c1;
    
}

void Plotter::plot2D_Hist(TH2D* hist2D, string fileName, string plotDir)
{
    // Canvas
    Double_t w = 800; 
    Double_t h = 800;
    TCanvas* c1 = new TCanvas("c","c",w,h);
    c1->SetWindowSize(w,h);
    
    // Pad
    TPad *p = new TPad("p","p",0.05,0.05,0.95,0.95);
    p->Draw();
    
    p->cd();
    hist2D->GetYaxis()->SetTitleOffset(1.8);
    hist2D->Draw("colz");
    gPad->Update();
    
    // Statistics Box
    TPaveStats *st = (TPaveStats*)hist2D->FindObject("stats");
    st->SetOptStat(1000000110);
    st->SetX1NDC(0.1); 
    st->SetX2NDC(0.3); 
    st->SetY1NDC(0.8); 
    st->SetY2NDC(0.9); 
   
    c1->Print(Form("%s%s",plotDir.c_str(),fileName.c_str()),"png");
    
    delete p;
    delete c1;
}

#endif





