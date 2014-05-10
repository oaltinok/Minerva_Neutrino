/*
    See CCDeltaPlus.h header for Class Information
*/

#ifndef CCDeltaPlus_cpp
#define CCDeltaPlus_cpp

#include "CCDeltaPlus.h"

using namespace std;

void CCDeltaPlus::specifyRunTime()
{
    isDataAnalysis = true;
    isMC = true;
    applyProtonScore = true;
    minProtonScore = 0.3;
    is_pID_Studies = false;
}

void CCDeltaPlus::run(string playlist)
{
    //------------------------------------------------------------------------
    // Create chain
    //------------------------------------------------------------------------

    TChain* fChain = new TChain("CCDeltaPlusAna");
    Init(playlist, fChain);

    if (!fChain) return;
    if (fChain == 0) return;
    
    //------------------------------------------------------------------------
    // Initialize the Analysis Variables and Histograms
    //------------------------------------------------------------------------
    specifyRunTime();
    initVariables();
    initHistograms();
    
    //------------------------------------------------------------------------
    // Open files for writing
    //------------------------------------------------------------------------
    openFiles();
    
    //------------------------------------------------------------------------
    // Branch Selection for Performance
    //------------------------------------------------------------------------
    fChain->SetBranchStatus("*",0);  // disable all branches

    fChain->SetBranchStatus("ev_run",1);  // activate
    
    fChain->SetBranchStatus("truth_isSignal",1);  // activate
    fChain->SetBranchStatus("Cut_EndPoint_Michel_Exist",1);  // activate
    fChain->SetBranchStatus("Cut_Muon_None",1);  // activate
    fChain->SetBranchStatus("Cut_Muon_Score_Low",1);  // activate
    fChain->SetBranchStatus("Cut_Proton_None",1);  // activate
    fChain->SetBranchStatus("Cut_Vertex_Michel_Exist",1);  // activate
    fChain->SetBranchStatus("Cut_Vertex_None",1);  // activate
    fChain->SetBranchStatus("Cut_Vertex_Not_Analyzable",1);  // activate
    fChain->SetBranchStatus("Cut_Vertex_Not_Fiducial",1);  // activate
    fChain->SetBranchStatus("Cut_Vertex_Null",1);  // activate
    fChain->SetBranchStatus("Cut_secEndPoint_Michel_Exist",1);  // activate
    
    fChain->SetBranchStatus("CCDeltaPlusAna_muon_px",1);  // activate
    fChain->SetBranchStatus("CCDeltaPlusAna_muon_py",1);  // activate
    fChain->SetBranchStatus("CCDeltaPlusAna_muon_pz",1);  // activate
    fChain->SetBranchStatus("CCDeltaPlusAna_muon_E",1);  // activate
    fChain->SetBranchStatus("CCDeltaPlusAna_muon_muScore",1);  // activate
    fChain->SetBranchStatus("CCDeltaPlusAna_trajMuonProngPx",1);
    fChain->SetBranchStatus("CCDeltaPlusAna_trajMuonProngPy",1);  // activate
    fChain->SetBranchStatus("CCDeltaPlusAna_trajMuonProngPz",1);  // activate
    
    fChain->SetBranchStatus("CCDeltaPlusAna_trajProtonProngPx",1);  // activate
    fChain->SetBranchStatus("CCDeltaPlusAna_trajProtonProngPy",1);  // activate
    fChain->SetBranchStatus("CCDeltaPlusAna_trajProtonProngPz",1);  // activate
    fChain->SetBranchStatus("CCDeltaPlusAna_proton_px",1);  // activate
    fChain->SetBranchStatus("CCDeltaPlusAna_proton_py",1);  // activate
    fChain->SetBranchStatus("CCDeltaPlusAna_proton_pz",1);  // activate
    fChain->SetBranchStatus("CCDeltaPlusAna_proton_E",1);  // activate
    fChain->SetBranchStatus("CCDeltaPlusAna_proton_score",1);  // activate
    fChain->SetBranchStatus("CCDeltaPlusAna_trajProtonProngPDG",1);  // activate
    
    fChain->SetBranchStatus("mc_Q2",1);  // activate
    fChain->SetBranchStatus("mc_incomingE",1);  // activate
    fChain->SetBranchStatus("mc_vtx",1);  // activate
    fChain->SetBranchStatus("mc_intType",1);  // activate
    fChain->SetBranchStatus("mc_nFSPart",1);  // activate
    fChain->SetBranchStatus("CCDeltaPlusAna_vtx",1);  // activate
    
    fChain->SetBranchStatus("truth_muon_charge",1);  // activate
    fChain->SetBranchStatus("truth_reco_muonCharge",1);  // activate
    
    
    
    // Cut Statistics
    double nAll = 0;

    double nCut_Vertex_None = 0;
    double nCut_Vertex_Null = 0;
    double nCut_Vertex_Not_Analyzable = 0; 
    double nCut_Vertex_Not_Fiducial = 0;    
    double nCut_Vertex_Michel_Exist = 0;           
    double nCut_Muon_None = 0;              
    double nCut_Muon_Score_Low = 0;
    double nCut_EndPoint_Michel_Exist = 0;
    double nCut_secEndPoint_Michel_Exist = 0;
    double nCut_Proton_None = 0;            
    double nCut_Proton_Score = 0;
    double nCut_Reco_Muon_NoProblem = 0;
    
    double nSignal = 0;
    double nSignal_Vertex_None = 0;
    double nSignal_Vertex_Null = 0;
    double nSignal_Vertex_Not_Analyzable = 0; 
    double nSignal_Vertex_Not_Fiducial = 0;    
    double nSignal_Vertex_Michel_Exist = 0;           
    double nSignal_Muon_None = 0;              
    double nSignal_Muon_Score_Low = 0;
    double nSignal_EndPoint_Michel_Exist = 0;
    double nSignal_secEndPoint_Michel_Exist = 0;
    double nSignal_Proton_None = 0;            
    double nSignal_Proton_Score = 0;
    double nSignal_Reco_Muon_NoProblem = 0;
    
    double nAntiMuon = 0;
    
    int indRecoProton;

    //------------------------------------------------------------------------
    // Loop over Chain
    //------------------------------------------------------------------------
    Long64_t nbytes = 0, nb = 0;
    
    cout<<"Looping over all entries"<<endl;
    
    Long64_t nentries = fChain->GetEntriesFast();

    for (Long64_t jentry=0; jentry<nentries;jentry++) {

        nb = fChain->GetEntry(jentry);   nbytes += nb;    
        Long64_t ientry = fChain->GetEntry(jentry);
        if (ientry == 0) {
            cout<<"\tGetEntry failure "<<jentry<<endl;
            break;
        }
    
        // Progress Message on Terminal
        if (jentry%25000 == 0){
            cout<<"\tEntry "<<jentry<<endl;
        }
        
        //----------------------------------------------------------------------
        // Plausibility Cuts
        //----------------------------------------------------------------------
        if (mc_nFSPart > max_nFSPart) continue;

        //----------------------------------------------------------------------
        // Count Events after each Reconstruction Cut
        //----------------------------------------------------------------------
        
        // Count All Events before Cuts
        nAll++;
        if (truth_isSignal) nSignal++;
        
        if( Cut_Vertex_None == 1) continue;
        nCut_Vertex_None++;
        if(truth_isSignal) nSignal_Vertex_None ++;
        
        if( Cut_Vertex_Null == 1) continue;
        nCut_Vertex_Null++;
        if(truth_isSignal) nSignal_Vertex_Null++;
        
        if( Cut_Vertex_Not_Analyzable == 1) continue;
        nCut_Vertex_Not_Analyzable++;
        if(truth_isSignal) nSignal_Vertex_Not_Analyzable++;
        
        if( Cut_Vertex_Not_Fiducial == 1) continue;
        nCut_Vertex_Not_Fiducial++;
        if(truth_isSignal) nSignal_Vertex_Not_Fiducial++;
        
        if( Cut_Muon_None == 1) continue;
        nCut_Muon_None++;
        if(truth_isSignal) nSignal_Muon_None++;
        
        if( Cut_Muon_Score_Low == 1) continue;
        nCut_Muon_Score_Low++;
        if(truth_isSignal) nSignal_Muon_Score_Low++;
        
           
        if( Cut_Vertex_Michel_Exist == 1) continue;
        nCut_Vertex_Michel_Exist++;
        if(truth_isSignal) nSignal_Vertex_Michel_Exist++;

        if( Cut_EndPoint_Michel_Exist == 1) continue;
        nCut_EndPoint_Michel_Exist++;
        if(truth_isSignal) nSignal_EndPoint_Michel_Exist++;
 
        if( Cut_secEndPoint_Michel_Exist == 1) continue;
        nCut_secEndPoint_Michel_Exist++;
        if(truth_isSignal) nSignal_secEndPoint_Michel_Exist++;
        
        if( Cut_Proton_None == 1) continue;
        nCut_Proton_None++;
        if(truth_isSignal) nSignal_Proton_None++;
        
        // Find Best Proton in Reco
        indRecoProton = findBestProton();
        
        if ( applyProtonScore && (CCDeltaPlusAna_proton_score[indRecoProton] < minProtonScore) ) continue;
        nCut_Proton_Score++;
        if(truth_isSignal) nSignal_Proton_Score++;
        
        if(truth_muon_charge == 1 && truth_isSignal) nAntiMuon++;
        
        
        //------------------------------------------------------------------
        // Sanity Checks
        //------------------------------------------------------------------
        if(CCDeltaPlusAna_muon_pz == 0) continue;
        nCut_Reco_Muon_NoProblem++;
        if(truth_isSignal) nSignal_Reco_Muon_NoProblem++;
      
        //------------------------------------------------------------------
        // pID Studies
        //------------------------------------------------------------------
        if( is_pID_Studies){
            for(int i = 0; i < 10; i++){
                if(CCDeltaPlusAna_proton_score[i] == -1) break;
                if(CCDeltaPlusAna_trajProtonProngPDG[i] == 2212){
                    pID_proton->Fill(CCDeltaPlusAna_proton_score[i]);
                }else if(CCDeltaPlusAna_trajProtonProngPDG[i] == 211){
                    pID_piplus->Fill(CCDeltaPlusAna_proton_score[i]);
                }else if(CCDeltaPlusAna_trajProtonProngPDG[i] == -211){
                    pID_piminus->Fill(CCDeltaPlusAna_proton_score[i]);
                }else{
                    pID_other->Fill(CCDeltaPlusAna_proton_score[i]);
                }
            }
        }
        
        
        if ( isDataAnalysis){
            //------------------------------------------------------------------
            // Fill Particles
            //------------------------------------------------------------------
            if( isMC ){
                fillMuonTrue();
                fillProtonTrue();
//                 fillPionTrue();
            }
            
            // Fill Reconstructed Information
            fillMuonReco();
            fillProtonReco(indRecoProton);
//             fillPionReco();
            
            muon.set_errors();
            proton.set_errors();
//             pion.set_errors();

            //------------------------------------------------------------------
            // Fill Histograms
            //------------------------------------------------------------------
            fillHistograms();            
        }

    } // end for-loop
    
    if(is_pID_Studies) get_pID_Stats();
    
    cout<<nAntiMuon<<endl;
    
    cout<<"Done!"<<endl;
    
    cout<<">> Writing "<<cutFile<<endl;
    cutText<<"nAll                          "<<nAll<<endl;
    cutText<<"Cut_Vertex_None               "<<nCut_Vertex_None<<endl;
    cutText<<"Cut_Vertex_Null               "<<nCut_Vertex_Null<<endl;
    cutText<<"Cut_Vertex_Not_Analyzable     "<<nCut_Vertex_Not_Analyzable<<endl;
    cutText<<"Cut_Vertex_Not_Fiducial       "<<nCut_Vertex_Not_Fiducial<<endl;
    cutText<<"Cut_Muon_None                 "<<nCut_Muon_None<<endl;
    cutText<<"Cut_Muon_Score_Low            "<<nCut_Muon_Score_Low<<endl; 
    cutText<<"Cut_Vertex_Michel_Exist       "<<nCut_Vertex_Michel_Exist<<endl;
    cutText<<"Cut_EndPoint_Michel_Exist     "<<nCut_EndPoint_Michel_Exist<<endl;
    cutText<<"Cut_secEndPoint_Michel_Exist  "<<nCut_secEndPoint_Michel_Exist<<endl;
    cutText<<"Cut_Proton_None               "<<nCut_Proton_None<<endl;
    cutText<<"Cut_Proton_Score              "<<nCut_Proton_Score<<endl;
    cutText<<"Cut_Reco_Muon_NoProblem       "<<nCut_Reco_Muon_NoProblem<<endl;
    cutText<<endl;
    cutText<<"nSignal                          "<<nSignal<<endl;
    cutText<<"Signal_Vertex_None               "<<nSignal_Vertex_None<<endl;
    cutText<<"Signal_Vertex_Null               "<<nSignal_Vertex_Null<<endl;
    cutText<<"Signal_Vertex_Not_Analyzable     "<<nSignal_Vertex_Not_Analyzable<<endl;
    cutText<<"Signal_Vertex_Not_Fiducial       "<<nSignal_Vertex_Not_Fiducial<<endl;
    cutText<<"Signal_Muon_None                 "<<nSignal_Muon_None<<endl;
    cutText<<"Signal_Muon_Score_Low            "<<nSignal_Muon_Score_Low<<endl; 
    cutText<<"Signal_Vertex_Michel_Exist       "<<nSignal_Vertex_Michel_Exist<<endl;
    cutText<<"Signal_EndPoint_Michel_Exist     "<<nSignal_EndPoint_Michel_Exist<<endl;
    cutText<<"Signal_secEndPoint_Michel_Exist  "<<nSignal_secEndPoint_Michel_Exist<<endl;
    cutText<<"Signal_Proton_None               "<<nSignal_Proton_None<<endl;
    cutText<<"Signal_Proton_Score              "<<nSignal_Proton_Score<<endl;
    cutText<<"Signal_Reco_Muon_NoProblem       "<<nSignal_Reco_Muon_NoProblem<<endl;
    
    
    // Write the Root Files
    write_RootFile();           //CCDeltaPlus
    muon.write_RootFile();
    proton.write_RootFile();
    pion.write_RootFile();
    
    
    closeFiles();
    
    
}

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
//
//     Specific Functions
//
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

void CCDeltaPlus::get_pID_Stats()
{
    cout<<"Calculating pID Statististics"<<endl;
    double nProton = 0;
    double nTotalProton = 0;
    double nCapturedEvents = 0;
    double purity;
    double efficiency;
    //Get Total Proton
    for(int i = 39; i >= 0; i--){
        nTotalProton = nTotalProton + pID_proton->GetBinContent(i);
    }
    cout<<"nTotalProton = "<<nTotalProton<<endl;
    
    for(int i = 39; i >= 0; i--){
        nProton = nProton + pID_proton->GetBinContent(i);
        nCapturedEvents =   nCapturedEvents +
                            pID_proton->GetBinContent(i) +
                            pID_piplus->GetBinContent(i) +
                            pID_piminus->GetBinContent(i) +
                            pID_other->GetBinContent(i);
                            
        purity = nProton / nCapturedEvents;
        efficiency = nProton / nTotalProton;
        cout<<"pID = "<<0.025*i<<" Purity = "<<purity<<" Efficiency "<<efficiency<<endl;
    }
}

void CCDeltaPlus::fillCCDeltaPlus()
{
    beamEnergy_mc->Fill(mc_incomingE);
//     beamEnergy_reco->Fill(Erec);
//     beamEnergy_error->Fill( Data_Functions::getError(mc_incomingE,Erec) );
//     beamEnergy_reco_mc->Fill(Erec,mc_incomingE);
    
    q2_mc->Fill(mc_Q2 / mevSq_to_gevSq);
//     q2_reco->Fill(Q2/ mevSq_to_gevSq);
//     q2_error->Fill( Data_Functions::getError(mc_Q2,Q2) );
//     q2_reco_mc->Fill(Q2/mevSq_to_gevSq,mc_Q2 /mevSq_to_gevSq);

    vertex_z_true->Fill(mc_vtx[2]);
    vertex_z_reco->Fill(CCDeltaPlusAna_vtx[2]);
    vertex_z_error->Fill( Data_Functions::getError(mc_vtx[2],CCDeltaPlusAna_vtx[2]) );
    vertex_z_reco_mc->Fill(CCDeltaPlusAna_vtx[2],mc_vtx[2]);
    
    vertex_x_y_true->Fill(mc_vtx[0],mc_vtx[1]);
    vertex_x_y_reco->Fill(CCDeltaPlusAna_vtx[0],CCDeltaPlusAna_vtx[1]);
    
    int_channel->Fill(mc_intType);

    n_FSParticles->Fill(mc_nFSPart);
//     n_gammas->Fill();

}

void CCDeltaPlus::initVariables()
{
    cout<<"Initializing CCDeltaPlus Class"<<endl;
    
    channelTag = "Test";
    
    // File Locations
    rootDir =   "Output/RootFiles/CCDeltaPlus.root";
    plotDir =   "Output/Plots/CCDeltaPlus/";
    
    
    cout<<"\tRoot File: "<<rootDir<<endl;
    cout<<"\tPlot Output Folder: "<<plotDir<<endl;
    
    // Create Root File 
    f = new TFile(rootDir.c_str(),"RECREATE");
    
    // -------------------------------------------------------------------------
    //     Initialization
    //--------------------------------------------------------------------------
    // Default Beam Configuration
    beam_p3.SetXYZ(1.0,1.0,1.0);
    beam_p3.SetPhi(-1.554);
    beam_p3.SetTheta(0.059);
    
    max_nFSPart = 15;
    maxBeamEnergy = 20000; //MeV
    
    cout<<"Done!"<<endl;
}


void CCDeltaPlus::fillHistograms()
{
    fillCCDeltaPlus();
    muon.fill_Histograms();
    pion.fill_Histograms();
    proton.fill_Histograms();
}

void CCDeltaPlus::write_RootFile()
{
    cout<<">> Writing "<<rootDir<<endl;
    f->Write();
}

void CCDeltaPlus::closeFiles()
{
    readme.close();
}

void CCDeltaPlus::openFiles()
{
    // Open Readme File
    readmeFile = "Output/TextFiles/readme.txt";
    readme.open( readmeFile.c_str() );
    
    if( !readme.is_open() ){
        cerr<<"Cannot open output text file: "<<readmeFile<<endl;
        exit(1);
    }
    
    // Open Cut File
    cutFile = "Output/TextFiles/CutTable";
    cutFile = cutFile + "_" + channelTag + ".txt";
    cutText.open( cutFile.c_str() );
    
    if( !cutText.is_open() ){
        cerr<<"Cannot open output text file: "<<cutFile<<endl;
        exit(1);
    }

    writeReadme();
}

void CCDeltaPlus::writeReadme()
{
    readme<<"Test"<<endl;
}

/*
--------------------------------------------------------------------------------
 Beam Energy Cu: isBeamEnergyLow(double maxEnergy)
    Incoming Beam Energy must be lower than a maximum Energy
--------------------------------------------------------------------------------
*/
bool CCDeltaPlus::isBeamEnergyLow(double maxEnergy)
{
    if(mc_incomingE > maxEnergy){
        return false;
    }else{
        return true;
    }

}

/*
--------------------------------------------------------------------------------
 findParticle:
    Returns the array indice of the Final State Particle for given PDG
--------------------------------------------------------------------------------
*/
int CCDeltaPlus::findTrueParticle(int targetPDG)
{
    int current_ind = -1;
    double current_P = 0;
    
    TVector3 p3;
    
    for(int i = 0; i < mc_nFSPart && i < max_nFSPart; i++ ){
    
        // Momentum of Particle (No Particle at rest)
        // Update Particle Momentum until you find the fastest particle
        if( mc_FSPartPDG[i] == targetPDG){
            p3.SetXYZ(mc_FSPartPx[i],mc_FSPartPy[i],mc_FSPartPz[i]);
            if (current_P < p3.Mag()){
                current_P = p3.Mag();
                current_ind = i;
            }
        }
    }
    
    if (current_P > 0){
        return current_ind;
    }else{
        return -1;
    }
}


/*
--------------------------------------------------------------------------------
 countParticles:
    Returns the number of particles in the Final State
        Input 
            int targetPDG
            bool applyPCut - Variable for selecting particles with momentum 
                                (no particle at rest)
--------------------------------------------------------------------------------
*/
int CCDeltaPlus::countParticles(int targetPDG, bool applyPCut)
{
    int count = 0;
    TVector3 p3;
    
    for(int i = 0; i < mc_nFSPart && i < max_nFSPart; i++ ){
        if( mc_FSPartPDG[i] == targetPDG){
            if(applyPCut){
                p3.SetXYZ(mc_FSPartPx[i],mc_FSPartPy[i],mc_FSPartPz[i]);
                if(p3.Mag() > 0){
                    count++;
                }
            }
            else{
                count++;
            }
        }
    }
    
    return count;

}

void CCDeltaPlus::fillProtonTrue()
{
    int ind = proton.ind;
    
    // Fill 4-Momentum
    proton.set_p4(  CCDeltaPlusAna_trajProtonProngPx[ind],
                    CCDeltaPlusAna_trajProtonProngPy[ind],
                    CCDeltaPlusAna_trajProtonProngPz[ind],
                    -1.0, 
                    true);
       
    // set Angle wrt Beam
    proton.set_angleBeam(beam_p3, true);
    
    // set Angle wrt Muon
    proton.set_angleMuon(muon, true);
    
}

void CCDeltaPlus::fillProtonReco(int ind)
{
    // Set Particle Score
    proton.particleScore = CCDeltaPlusAna_proton_score[ind];
    
    // Fill 4-Momentum
    proton.set_p4(  CCDeltaPlusAna_proton_px[ind],
                    CCDeltaPlusAna_proton_py[ind],
                    CCDeltaPlusAna_proton_pz[ind],
                    CCDeltaPlusAna_proton_E[ind],
                    false);
                    
    // set Angle wrt Beam
    proton.set_angleBeam(beam_p3, false);
    
    // set Angle wrt Muon
    proton.set_angleMuon(muon, false);
}

int CCDeltaPlus::findBestProton()
{
    double tempScore = CCDeltaPlusAna_proton_score[0];
    int tempInd = 0;
    
    for( int i = 1; i < 10; i++){
        if ( CCDeltaPlusAna_proton_score[i] == -1 ) break;
        if( CCDeltaPlusAna_proton_score[i] > tempScore){
            tempScore = CCDeltaPlusAna_proton_score[i];
            tempInd = i;
        }
    }
    
    return tempInd;

}

bool CCDeltaPlus::isProtonShort(int ind)
{
    const double protonMass = 938; 
    const double minProtonKE = 120;
    double protonKE;
    double protonE;
    
    protonE = mc_FSPartE[ind];
    protonKE =  protonE - protonMass;
    if( protonKE < minProtonKE){
        return true;
    }else{
        return false;
    }
    
}


void CCDeltaPlus::fillPionReco()
{
    // Fill 4-Momentum
//     pion.set_p4(    pimom[0],
//                     pimom[1],
//                     pimom[2],
//                     pienergy,
//                     false);
    
    // set Angle wrt Beam
    pion.set_angleBeam(beam_p3, false);
    
    // set Angle wrt Muon
    pion.set_angleMuon(muon, false);
}

void CCDeltaPlus::fillPionTrue()
{
    
}


bool CCDeltaPlus::isSinglePion()
{
    int nPlus;
    int nMinus;
    int nZero;
    
    nPlus = countParticles(PDG_List::pi_plus,false);
    nMinus = countParticles(PDG_List::pi_minus,false);
    nZero = countParticles(PDG_List::pi_zero,true);
    
    if(nMinus == 0 && nZero == 1 && nPlus == 0){
        return true;
    }else{
        return false;
    }

}

bool CCDeltaPlus::isNoMeson()
{
    int nPlus;
    int nMinus;
    int nZero;
    int nKaons;
    int nGamma;
    int nPions;

    
    nPlus = countParticles(PDG_List::pi_plus,false);
    nMinus = countParticles(PDG_List::pi_minus,false);
    nZero = countParticles(PDG_List::pi_zero,false);
    nKaons = countParticles(PDG_List::kaon_zero_L,false);
    nKaons = nKaons +  countParticles(PDG_List::kaon_zero_S,false);
    nKaons = nKaons +  countParticles(PDG_List::kaon_zero,false);
    nKaons = nKaons +  countParticles(PDG_List::kaon_plus,false);
    nKaons = nKaons +  countParticles(PDG_List::kaon_minus,false);
    nGamma = countParticles(PDG_List::gamma,false);
    
    nPions = nMinus + nZero + nPlus;
    
    if(nPions == 0 && nKaons == 0 && nGamma == 0 ){
        return true;
    }else{
        return false;
    }

}

void CCDeltaPlus::fillMuonTrue()
{

    // Fill 4-Momentum
    muon.set_p4(    CCDeltaPlusAna_trajMuonProngPx,
                    CCDeltaPlusAna_trajMuonProngPy,
                    CCDeltaPlusAna_trajMuonProngPz,
                    -1.0, 
                    true);
       
    // set Angle wrt Beam
    muon.set_angleBeam(beam_p3, true);
    
    // set Angle wrt Muon
    muon.set_angleMuon(muon, true);
    
}

void CCDeltaPlus::fillMuonReco()
{
    // Set Particle Score
    muon.particleScore = CCDeltaPlusAna_muon_muScore;
    
    // Fill 4-Momentum
    muon.set_p4(    CCDeltaPlusAna_muon_px,
                    CCDeltaPlusAna_muon_py,
                    CCDeltaPlusAna_muon_pz,
                    CCDeltaPlusAna_muon_E,
                    false);
    
    // set Angle wrt Beam
    muon.set_angleBeam(beam_p3, false);
    
    // set Angle wrt Muon
    muon.set_angleMuon(muon, false);
}

void CCDeltaPlus::Init(string playlist, TChain* fChain)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   ifstream input_pl(playlist.c_str());
   string filename;
    cout<<"Initializing Playlist"<<endl;
    
    if( !input_pl.is_open() ){
        cerr<<"Cannot open Playlist File!"<<endl;
        exit(1);
    }else{
        cout<<"\tPlaylist: "<<playlist.c_str()<<endl;
    }
    
    
   while (input_pl) {
     input_pl>>filename;
     
     if (!input_pl) break;
    
     if (filename[0] != '/') break;
    
     fChain->Add( filename.c_str() );
//      cout<<" Added "<<filename.c_str()<<endl;
   }

   // Set branch addresses and branch pointers
   fChain->SetMakeClass(1);
   fChain->SetBranchAddress("eventID", &eventID, &b_eventID);
   fChain->SetBranchAddress("physEvtNum", &physEvtNum, &b_physEvtNum);
   fChain->SetBranchAddress("n_hyps", &n_hyps, &b_n_hyps);
   fChain->SetBranchAddress("processType", &processType, &b_processType);
   fChain->SetBranchAddress("primaryPart", &primaryPart, &b_primaryPart);
   fChain->SetBranchAddress("n_slices", &n_slices, &b_n_slices);
   fChain->SetBranchAddress("slice_numbers", slice_numbers, &b_slice_numbers);
   fChain->SetBranchAddress("shared_slice", &shared_slice, &b_shared_slice);
   fChain->SetBranchAddress("vtx", vtx, &b_vtx);
   fChain->SetBranchAddress("vtxErr", vtxErr, &b_vtxErr);
   fChain->SetBranchAddress("E", E, &b_E);
   fChain->SetBranchAddress("found_truth", &found_truth, &b_found_truth);
   fChain->SetBranchAddress("phys_front_activity", &phys_front_activity, &b_phys_front_activity);
   fChain->SetBranchAddress("phys_energy_in_road_upstream_is_rockmuon_consistent", &phys_energy_in_road_upstream_is_rockmuon_consistent, &b_phys_energy_in_road_upstream_is_rockmuon_consistent);
   fChain->SetBranchAddress("rock_muons_removed", &rock_muons_removed, &b_rock_muons_removed);
   fChain->SetBranchAddress("minos_track_match", &minos_track_match, &b_minos_track_match);
   fChain->SetBranchAddress("minos_stub_match", &minos_stub_match, &b_minos_stub_match);
   fChain->SetBranchAddress("unknown_helicity", &unknown_helicity, &b_unknown_helicity);
   fChain->SetBranchAddress("minos_track_inside_partial_plane", &minos_track_inside_partial_plane, &b_minos_track_inside_partial_plane);
   fChain->SetBranchAddress("prim_vtx_has_misassigned_track_direction", &prim_vtx_has_misassigned_track_direction, &b_prim_vtx_has_misassigned_track_direction);
   fChain->SetBranchAddress("prim_vtx_has_broken_track", &prim_vtx_has_broken_track, &b_prim_vtx_has_broken_track);
   fChain->SetBranchAddress("isMinosMatchTrack", &isMinosMatchTrack, &b_isMinosMatchTrack);
   fChain->SetBranchAddress("isMinosMatchStub", &isMinosMatchStub, &b_isMinosMatchStub);
   fChain->SetBranchAddress("well_fit_vertex", &well_fit_vertex, &b_well_fit_vertex);
   fChain->SetBranchAddress("isBrokenTrack", &isBrokenTrack, &b_isBrokenTrack);
   fChain->SetBranchAddress("Cut_EndPoint_Michel_Exist", &Cut_EndPoint_Michel_Exist, &b_Cut_EndPoint_Michel_Exist);
   fChain->SetBranchAddress("Cut_Muon_None", &Cut_Muon_None, &b_Cut_Muon_None);
   fChain->SetBranchAddress("Cut_Muon_Score_Low", &Cut_Muon_Score_Low, &b_Cut_Muon_Score_Low);
   fChain->SetBranchAddress("Cut_Proton_None", &Cut_Proton_None, &b_Cut_Proton_None);
   fChain->SetBranchAddress("Cut_Vertex_Michel_Exist", &Cut_Vertex_Michel_Exist, &b_Cut_Vertex_Michel_Exist);
   fChain->SetBranchAddress("Cut_Vertex_None", &Cut_Vertex_None, &b_Cut_Vertex_None);
   fChain->SetBranchAddress("Cut_Vertex_Not_Analyzable", &Cut_Vertex_Not_Analyzable, &b_Cut_Vertex_Not_Analyzable);
   fChain->SetBranchAddress("Cut_Vertex_Not_Fiducial", &Cut_Vertex_Not_Fiducial, &b_Cut_Vertex_Not_Fiducial);
   fChain->SetBranchAddress("Cut_Vertex_Null", &Cut_Vertex_Null, &b_Cut_Vertex_Null);
   fChain->SetBranchAddress("Cut_secEndPoint_Michel_Exist", &Cut_secEndPoint_Michel_Exist, &b_Cut_secEndPoint_Michel_Exist);
   fChain->SetBranchAddress("broken_track_most_us_plane", &broken_track_most_us_plane, &b_broken_track_most_us_plane);
   fChain->SetBranchAddress("n_anchored_long_trk_prongs", &n_anchored_long_trk_prongs, &b_n_anchored_long_trk_prongs);
   fChain->SetBranchAddress("n_anchored_short_trk_prongs", &n_anchored_short_trk_prongs, &b_n_anchored_short_trk_prongs);
   fChain->SetBranchAddress("n_dsp_blob_prongs", &n_dsp_blob_prongs, &b_n_dsp_blob_prongs);
   fChain->SetBranchAddress("n_iso_blob_prongs", &n_iso_blob_prongs, &b_n_iso_blob_prongs);
   fChain->SetBranchAddress("n_iso_trk_prongs", &n_iso_trk_prongs, &b_n_iso_trk_prongs);
   fChain->SetBranchAddress("n_long_tracks", &n_long_tracks, &b_n_long_tracks);
   fChain->SetBranchAddress("n_short_tracks", &n_short_tracks, &b_n_short_tracks);
   fChain->SetBranchAddress("n_startpoint_vertices", &n_startpoint_vertices, &b_n_startpoint_vertices);
   fChain->SetBranchAddress("n_us_muon_clusters", &n_us_muon_clusters, &b_n_us_muon_clusters);
   fChain->SetBranchAddress("n_vtx_michel_views", &n_vtx_michel_views, &b_n_vtx_michel_views);
   fChain->SetBranchAddress("n_vtx_prongs", &n_vtx_prongs, &b_n_vtx_prongs);
   fChain->SetBranchAddress("phys_energy_in_road_downstream_nplanes", &phys_energy_in_road_downstream_nplanes, &b_phys_energy_in_road_downstream_nplanes);
   fChain->SetBranchAddress("phys_energy_in_road_upstream_nplanes", &phys_energy_in_road_upstream_nplanes, &b_phys_energy_in_road_upstream_nplanes);
   fChain->SetBranchAddress("phys_n_dead_discr_pair", &phys_n_dead_discr_pair, &b_phys_n_dead_discr_pair);
   fChain->SetBranchAddress("phys_n_dead_discr_pair_in_prim_track_region", &phys_n_dead_discr_pair_in_prim_track_region, &b_phys_n_dead_discr_pair_in_prim_track_region);
   fChain->SetBranchAddress("phys_n_dead_discr_pair_two_mod_downstream_prim_track", &phys_n_dead_discr_pair_two_mod_downstream_prim_track, &b_phys_n_dead_discr_pair_two_mod_downstream_prim_track);
   fChain->SetBranchAddress("phys_n_dead_discr_pair_two_mod_upstream_prim_vtx", &phys_n_dead_discr_pair_two_mod_upstream_prim_vtx, &b_phys_n_dead_discr_pair_two_mod_upstream_prim_vtx);
   fChain->SetBranchAddress("phys_n_dead_discr_pair_upstream_prim_track_proj", &phys_n_dead_discr_pair_upstream_prim_track_proj, &b_phys_n_dead_discr_pair_upstream_prim_track_proj);
   fChain->SetBranchAddress("phys_vertex_is_fiducial", &phys_vertex_is_fiducial, &b_phys_vertex_is_fiducial);
   fChain->SetBranchAddress("dispersedExtraE", &dispersedExtraE, &b_dispersedExtraE);
   fChain->SetBranchAddress("energy_from_mc", &energy_from_mc, &b_energy_from_mc);
   fChain->SetBranchAddress("energy_from_mc_fraction", &energy_from_mc_fraction, &b_energy_from_mc_fraction);
   fChain->SetBranchAddress("energy_from_mc_fraction_of_highest", &energy_from_mc_fraction_of_highest, &b_energy_from_mc_fraction_of_highest);
   fChain->SetBranchAddress("hadronVisibleE", &hadronVisibleE, &b_hadronVisibleE);
   fChain->SetBranchAddress("muonVisibleE", &muonVisibleE, &b_muonVisibleE);
   fChain->SetBranchAddress("muon_phi", &muon_phi, &b_muon_phi);
   fChain->SetBranchAddress("muon_theta", &muon_theta, &b_muon_theta);
   fChain->SetBranchAddress("muon_thetaX", &muon_thetaX, &b_muon_thetaX);
   fChain->SetBranchAddress("muon_thetaY", &muon_thetaY, &b_muon_thetaY);
   fChain->SetBranchAddress("phys_energy_dispersed", &phys_energy_dispersed, &b_phys_energy_dispersed);
   fChain->SetBranchAddress("phys_energy_in_road_downstream", &phys_energy_in_road_downstream, &b_phys_energy_in_road_downstream);
   fChain->SetBranchAddress("phys_energy_in_road_upstream", &phys_energy_in_road_upstream, &b_phys_energy_in_road_upstream);
   fChain->SetBranchAddress("phys_energy_unattached", &phys_energy_unattached, &b_phys_energy_unattached);
   fChain->SetBranchAddress("prim_vtx_smallest_opening_angle", &prim_vtx_smallest_opening_angle, &b_prim_vtx_smallest_opening_angle);
   fChain->SetBranchAddress("time", &time, &b_time);
   fChain->SetBranchAddress("totalIDVisibleE", &totalIDVisibleE, &b_totalIDVisibleE);
   fChain->SetBranchAddress("totalODVisibleE", &totalODVisibleE, &b_totalODVisibleE);
   fChain->SetBranchAddress("totalVisibleE", &totalVisibleE, &b_totalVisibleE);
   fChain->SetBranchAddress("unattachedExtraE", &unattachedExtraE, &b_unattachedExtraE);
   fChain->SetBranchAddress("vtxBlobExtraE", &vtxBlobExtraE, &b_vtxBlobExtraE);
   fChain->SetBranchAddress("vtx_michel_distance", &vtx_michel_distance, &b_vtx_michel_distance);
   fChain->SetBranchAddress("well_fit_vertex_angle", &well_fit_vertex_angle, &b_well_fit_vertex_angle);
   fChain->SetBranchAddress("truth_has_physics_event", &truth_has_physics_event, &b_truth_has_physics_event);
   fChain->SetBranchAddress("truth_reco_hasGoodObjects", &truth_reco_hasGoodObjects, &b_truth_reco_hasGoodObjects);
   fChain->SetBranchAddress("truth_reco_isGoodVertex", &truth_reco_isGoodVertex, &b_truth_reco_isGoodVertex);
   fChain->SetBranchAddress("truth_reco_isWellFitVertex", &truth_reco_isWellFitVertex, &b_truth_reco_isWellFitVertex);
   fChain->SetBranchAddress("truth_reco_isFidVol", &truth_reco_isFidVol, &b_truth_reco_isFidVol);
   fChain->SetBranchAddress("truth_reco_isFidVol_smeared", &truth_reco_isFidVol_smeared, &b_truth_reco_isFidVol_smeared);
   fChain->SetBranchAddress("truth_reco_isMinosMatch", &truth_reco_isMinosMatch, &b_truth_reco_isMinosMatch);
   fChain->SetBranchAddress("truth_reco_isBrokenTrack", &truth_reco_isBrokenTrack, &b_truth_reco_isBrokenTrack);
   fChain->SetBranchAddress("truth_isSignal", &truth_isSignal, &b_truth_isSignal);
   fChain->SetBranchAddress("truth_isFidVol", &truth_isFidVol, &b_truth_isFidVol);
   fChain->SetBranchAddress("truth_isPlausible", &truth_isPlausible, &b_truth_isPlausible);
   fChain->SetBranchAddress("truth_N_deltaplus", &truth_N_deltaplus, &b_truth_N_deltaplus);
   fChain->SetBranchAddress("truth_N_gamma", &truth_N_gamma, &b_truth_N_gamma);
   fChain->SetBranchAddress("truth_N_muminus", &truth_N_muminus, &b_truth_N_muminus);
   fChain->SetBranchAddress("truth_N_muplus", &truth_N_muplus, &b_truth_N_muplus);
   fChain->SetBranchAddress("truth_N_neutron", &truth_N_neutron, &b_truth_N_neutron);
   fChain->SetBranchAddress("truth_N_other", &truth_N_other, &b_truth_N_other);
   fChain->SetBranchAddress("truth_N_pi0", &truth_N_pi0, &b_truth_N_pi0);
   fChain->SetBranchAddress("truth_N_piminus", &truth_N_piminus, &b_truth_N_piminus);
   fChain->SetBranchAddress("truth_N_piplus", &truth_N_piplus, &b_truth_N_piplus);
   fChain->SetBranchAddress("truth_N_proton", &truth_N_proton, &b_truth_N_proton);
   fChain->SetBranchAddress("truth_muon_charge", &truth_muon_charge, &b_truth_muon_charge);
   fChain->SetBranchAddress("truth_reco_muonCharge", &truth_reco_muonCharge, &b_truth_reco_muonCharge);
   fChain->SetBranchAddress("truth_target_material", &truth_target_material, &b_truth_target_material);
   fChain->SetBranchAddress("truth_vertex_module", &truth_vertex_module, &b_truth_vertex_module);
   fChain->SetBranchAddress("truth_vertex_plane", &truth_vertex_plane, &b_truth_vertex_plane);
   fChain->SetBranchAddress("truth_muon_E", &truth_muon_E, &b_truth_muon_E);
   fChain->SetBranchAddress("truth_muon_px", &truth_muon_px, &b_truth_muon_px);
   fChain->SetBranchAddress("truth_muon_py", &truth_muon_py, &b_truth_muon_py);
   fChain->SetBranchAddress("truth_muon_pz", &truth_muon_pz, &b_truth_muon_pz);
   fChain->SetBranchAddress("truth_muon_theta_wrtbeam", &truth_muon_theta_wrtbeam, &b_truth_muon_theta_wrtbeam);
   fChain->SetBranchAddress("truth_pi0_trackID", truth_pi0_trackID, &b_truth_pi0_trackID);
   fChain->SetBranchAddress("truth_proton_trackID", truth_proton_trackID, &b_truth_proton_trackID);
   fChain->SetBranchAddress("genie_wgt_n_shifts", &genie_wgt_n_shifts, &b_genie_wgt_n_shifts);
   fChain->SetBranchAddress("truth_genie_wgt_AGKYxF1pi", truth_genie_wgt_AGKYxF1pi, &b_truth_genie_wgt_AGKYxF1pi);
   fChain->SetBranchAddress("truth_genie_wgt_AhtBY", truth_genie_wgt_AhtBY, &b_truth_genie_wgt_AhtBY);
   fChain->SetBranchAddress("truth_genie_wgt_BhtBY", truth_genie_wgt_BhtBY, &b_truth_genie_wgt_BhtBY);
   fChain->SetBranchAddress("truth_genie_wgt_CCQEPauliSupViaFK", truth_genie_wgt_CCQEPauliSupViaFK, &b_truth_genie_wgt_CCQEPauliSupViaFK);
   fChain->SetBranchAddress("truth_genie_wgt_CV1uBY", truth_genie_wgt_CV1uBY, &b_truth_genie_wgt_CV1uBY);
   fChain->SetBranchAddress("truth_genie_wgt_CV2uBY", truth_genie_wgt_CV2uBY, &b_truth_genie_wgt_CV2uBY);
   fChain->SetBranchAddress("truth_genie_wgt_EtaNCEL", truth_genie_wgt_EtaNCEL, &b_truth_genie_wgt_EtaNCEL);
   fChain->SetBranchAddress("truth_genie_wgt_FrAbs_N", truth_genie_wgt_FrAbs_N, &b_truth_genie_wgt_FrAbs_N);
   fChain->SetBranchAddress("truth_genie_wgt_FrAbs_pi", truth_genie_wgt_FrAbs_pi, &b_truth_genie_wgt_FrAbs_pi);
   fChain->SetBranchAddress("truth_genie_wgt_FrCEx_N", truth_genie_wgt_FrCEx_N, &b_truth_genie_wgt_FrCEx_N);
   fChain->SetBranchAddress("truth_genie_wgt_FrCEx_pi", truth_genie_wgt_FrCEx_pi, &b_truth_genie_wgt_FrCEx_pi);
   fChain->SetBranchAddress("truth_genie_wgt_FrElas_N", truth_genie_wgt_FrElas_N, &b_truth_genie_wgt_FrElas_N);
   fChain->SetBranchAddress("truth_genie_wgt_FrElas_pi", truth_genie_wgt_FrElas_pi, &b_truth_genie_wgt_FrElas_pi);
   fChain->SetBranchAddress("truth_genie_wgt_FrInel_N", truth_genie_wgt_FrInel_N, &b_truth_genie_wgt_FrInel_N);
   fChain->SetBranchAddress("truth_genie_wgt_FrInel_pi", truth_genie_wgt_FrInel_pi, &b_truth_genie_wgt_FrInel_pi);
   fChain->SetBranchAddress("truth_genie_wgt_FrPiProd_N", truth_genie_wgt_FrPiProd_N, &b_truth_genie_wgt_FrPiProd_N);
   fChain->SetBranchAddress("truth_genie_wgt_FrPiProd_pi", truth_genie_wgt_FrPiProd_pi, &b_truth_genie_wgt_FrPiProd_pi);
   fChain->SetBranchAddress("truth_genie_wgt_MFP_N", truth_genie_wgt_MFP_N, &b_truth_genie_wgt_MFP_N);
   fChain->SetBranchAddress("truth_genie_wgt_MFP_pi", truth_genie_wgt_MFP_pi, &b_truth_genie_wgt_MFP_pi);
   fChain->SetBranchAddress("truth_genie_wgt_MaCCQE", truth_genie_wgt_MaCCQE, &b_truth_genie_wgt_MaCCQE);
   fChain->SetBranchAddress("truth_genie_wgt_MaCCQEshape", truth_genie_wgt_MaCCQEshape, &b_truth_genie_wgt_MaCCQEshape);
   fChain->SetBranchAddress("truth_genie_wgt_MaNCEL", truth_genie_wgt_MaNCEL, &b_truth_genie_wgt_MaNCEL);
   fChain->SetBranchAddress("truth_genie_wgt_MaRES", truth_genie_wgt_MaRES, &b_truth_genie_wgt_MaRES);
   fChain->SetBranchAddress("truth_genie_wgt_MvRES", truth_genie_wgt_MvRES, &b_truth_genie_wgt_MvRES);
   fChain->SetBranchAddress("truth_genie_wgt_NormCCQE", truth_genie_wgt_NormCCQE, &b_truth_genie_wgt_NormCCQE);
   fChain->SetBranchAddress("truth_genie_wgt_NormCCRES", truth_genie_wgt_NormCCRES, &b_truth_genie_wgt_NormCCRES);
   fChain->SetBranchAddress("truth_genie_wgt_NormDISCC", truth_genie_wgt_NormDISCC, &b_truth_genie_wgt_NormDISCC);
   fChain->SetBranchAddress("truth_genie_wgt_NormNCRES", truth_genie_wgt_NormNCRES, &b_truth_genie_wgt_NormNCRES);
   fChain->SetBranchAddress("truth_genie_wgt_RDecBR1gamma", truth_genie_wgt_RDecBR1gamma, &b_truth_genie_wgt_RDecBR1gamma);
   fChain->SetBranchAddress("truth_genie_wgt_Rvn1pi", truth_genie_wgt_Rvn1pi, &b_truth_genie_wgt_Rvn1pi);
   fChain->SetBranchAddress("truth_genie_wgt_Rvn2pi", truth_genie_wgt_Rvn2pi, &b_truth_genie_wgt_Rvn2pi);
   fChain->SetBranchAddress("truth_genie_wgt_Rvp1pi", truth_genie_wgt_Rvp1pi, &b_truth_genie_wgt_Rvp1pi);
   fChain->SetBranchAddress("truth_genie_wgt_Rvp2pi", truth_genie_wgt_Rvp2pi, &b_truth_genie_wgt_Rvp2pi);
   fChain->SetBranchAddress("truth_genie_wgt_Theta_Delta2Npi", truth_genie_wgt_Theta_Delta2Npi, &b_truth_genie_wgt_Theta_Delta2Npi);
   fChain->SetBranchAddress("truth_genie_wgt_VecFFCCQEshape", truth_genie_wgt_VecFFCCQEshape, &b_truth_genie_wgt_VecFFCCQEshape);
   fChain->SetBranchAddress("truth_genie_wgt_shifts", truth_genie_wgt_shifts, &b_truth_genie_wgt_shifts);
   fChain->SetBranchAddress("truth_pi0_E", truth_pi0_E, &b_truth_pi0_E);
   fChain->SetBranchAddress("truth_pi0_px", truth_pi0_px, &b_truth_pi0_px);
   fChain->SetBranchAddress("truth_pi0_py", truth_pi0_py, &b_truth_pi0_py);
   fChain->SetBranchAddress("truth_pi0_pz", truth_pi0_pz, &b_truth_pi0_pz);
   fChain->SetBranchAddress("truth_pi0_theta_wrtbeam", truth_pi0_theta_wrtbeam, &b_truth_pi0_theta_wrtbeam);
   fChain->SetBranchAddress("truth_proton_E", truth_proton_E, &b_truth_proton_E);
   fChain->SetBranchAddress("truth_proton_px", truth_proton_px, &b_truth_proton_px);
   fChain->SetBranchAddress("truth_proton_py", truth_proton_py, &b_truth_proton_py);
   fChain->SetBranchAddress("truth_proton_pz", truth_proton_pz, &b_truth_proton_pz);
   fChain->SetBranchAddress("truth_proton_theta_wrtbeam", truth_proton_theta_wrtbeam, &b_truth_proton_theta_wrtbeam);
   fChain->SetBranchAddress("CCDeltaPlusAna_nuFlavor", &CCDeltaPlusAna_nuFlavor, &b_CCDeltaPlusAna_nuFlavor);
   fChain->SetBranchAddress("CCDeltaPlusAna_nuHelicity", &CCDeltaPlusAna_nuHelicity, &b_CCDeltaPlusAna_nuHelicity);
   fChain->SetBranchAddress("CCDeltaPlusAna_intCurrent", &CCDeltaPlusAna_intCurrent, &b_CCDeltaPlusAna_intCurrent);
   fChain->SetBranchAddress("CCDeltaPlusAna_intType", &CCDeltaPlusAna_intType, &b_CCDeltaPlusAna_intType);
   fChain->SetBranchAddress("CCDeltaPlusAna_E", &CCDeltaPlusAna_E, &b_CCDeltaPlusAna_E);
   fChain->SetBranchAddress("CCDeltaPlusAna_Q2", &CCDeltaPlusAna_Q2, &b_CCDeltaPlusAna_Q2);
   fChain->SetBranchAddress("CCDeltaPlusAna_x", &CCDeltaPlusAna_x, &b_CCDeltaPlusAna_x);
   fChain->SetBranchAddress("CCDeltaPlusAna_y", &CCDeltaPlusAna_y, &b_CCDeltaPlusAna_y);
   fChain->SetBranchAddress("CCDeltaPlusAna_W", &CCDeltaPlusAna_W, &b_CCDeltaPlusAna_W);
   fChain->SetBranchAddress("CCDeltaPlusAna_score", &CCDeltaPlusAna_score, &b_CCDeltaPlusAna_score);
   fChain->SetBranchAddress("CCDeltaPlusAna_leptonE", CCDeltaPlusAna_leptonE, &b_CCDeltaPlusAna_leptonE);
   fChain->SetBranchAddress("CCDeltaPlusAna_vtx", CCDeltaPlusAna_vtx, &b_CCDeltaPlusAna_vtx);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_is_contained", &CCDeltaPlusAna_minos_trk_is_contained, &b_CCDeltaPlusAna_minos_trk_is_contained);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_is_ok", &CCDeltaPlusAna_minos_trk_is_ok, &b_CCDeltaPlusAna_minos_trk_is_ok);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_used_range", &CCDeltaPlusAna_minos_used_range, &b_CCDeltaPlusAna_minos_used_range);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_used_curvature", &CCDeltaPlusAna_minos_used_curvature, &b_CCDeltaPlusAna_minos_used_curvature);
   fChain->SetBranchAddress("CCDeltaPlusAna_isMuonInsideOD", &CCDeltaPlusAna_isMuonInsideOD, &b_CCDeltaPlusAna_isMuonInsideOD);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_end_plane", &CCDeltaPlusAna_minos_trk_end_plane, &b_CCDeltaPlusAna_minos_trk_end_plane);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_quality", &CCDeltaPlusAna_minos_trk_quality, &b_CCDeltaPlusAna_minos_trk_quality);
   fChain->SetBranchAddress("CCDeltaPlusAna_muon_N_minosTracks", &CCDeltaPlusAna_muon_N_minosTracks, &b_CCDeltaPlusAna_muon_N_minosTracks);
   fChain->SetBranchAddress("CCDeltaPlusAna_muon_minervaTrack_types", &CCDeltaPlusAna_muon_minervaTrack_types, &b_CCDeltaPlusAna_muon_minervaTrack_types);
   fChain->SetBranchAddress("CCDeltaPlusAna_muon_minosTrackQuality", &CCDeltaPlusAna_muon_minosTrackQuality, &b_CCDeltaPlusAna_muon_minosTrackQuality);
   fChain->SetBranchAddress("CCDeltaPlusAna_muon_roadUpstreamPlanes", &CCDeltaPlusAna_muon_roadUpstreamPlanes, &b_CCDeltaPlusAna_muon_roadUpstreamPlanes);
   fChain->SetBranchAddress("CCDeltaPlusAna_ntrajMuonProng", &CCDeltaPlusAna_ntrajMuonProng, &b_CCDeltaPlusAna_ntrajMuonProng);
   fChain->SetBranchAddress("CCDeltaPlusAna_r_minos_trk_vtx_plane", &CCDeltaPlusAna_r_minos_trk_vtx_plane, &b_CCDeltaPlusAna_r_minos_trk_vtx_plane);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_numFSMuons", &CCDeltaPlusAna_t_minos_trk_numFSMuons, &b_CCDeltaPlusAna_t_minos_trk_numFSMuons);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLeptonPDG", &CCDeltaPlusAna_t_minos_trk_primFSLeptonPDG, &b_CCDeltaPlusAna_t_minos_trk_primFSLeptonPDG);
   fChain->SetBranchAddress("CCDeltaPlusAna_trajMuonProngPDG", &CCDeltaPlusAna_trajMuonProngPDG, &b_CCDeltaPlusAna_trajMuonProngPDG);
   fChain->SetBranchAddress("CCDeltaPlusAna_trajMuonProngPrimary", &CCDeltaPlusAna_trajMuonProngPrimary, &b_CCDeltaPlusAna_trajMuonProngPrimary);
   fChain->SetBranchAddress("CCDeltaPlusAna_vtx_module", &CCDeltaPlusAna_vtx_module, &b_CCDeltaPlusAna_vtx_module);
   fChain->SetBranchAddress("CCDeltaPlusAna_vtx_plane", &CCDeltaPlusAna_vtx_plane, &b_CCDeltaPlusAna_vtx_plane);
   fChain->SetBranchAddress("CCDeltaPlusAna_endMuonTrajMomentum", &CCDeltaPlusAna_endMuonTrajMomentum, &b_CCDeltaPlusAna_endMuonTrajMomentum);
   fChain->SetBranchAddress("CCDeltaPlusAna_endMuonTrajXPosition", &CCDeltaPlusAna_endMuonTrajXPosition, &b_CCDeltaPlusAna_endMuonTrajXPosition);
   fChain->SetBranchAddress("CCDeltaPlusAna_endMuonTrajYPosition", &CCDeltaPlusAna_endMuonTrajYPosition, &b_CCDeltaPlusAna_endMuonTrajYPosition);
   fChain->SetBranchAddress("CCDeltaPlusAna_endMuonTrajZPosition", &CCDeltaPlusAna_endMuonTrajZPosition, &b_CCDeltaPlusAna_endMuonTrajZPosition);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_bave", &CCDeltaPlusAna_minos_trk_bave, &b_CCDeltaPlusAna_minos_trk_bave);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_chi2", &CCDeltaPlusAna_minos_trk_chi2, &b_CCDeltaPlusAna_minos_trk_chi2);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_end_u", &CCDeltaPlusAna_minos_trk_end_u, &b_CCDeltaPlusAna_minos_trk_end_u);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_end_v", &CCDeltaPlusAna_minos_trk_end_v, &b_CCDeltaPlusAna_minos_trk_end_v);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_end_x", &CCDeltaPlusAna_minos_trk_end_x, &b_CCDeltaPlusAna_minos_trk_end_x);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_end_y", &CCDeltaPlusAna_minos_trk_end_y, &b_CCDeltaPlusAna_minos_trk_end_y);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_end_z", &CCDeltaPlusAna_minos_trk_end_z, &b_CCDeltaPlusAna_minos_trk_end_z);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_eqp", &CCDeltaPlusAna_minos_trk_eqp, &b_CCDeltaPlusAna_minos_trk_eqp);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_eqp_qp", &CCDeltaPlusAna_minos_trk_eqp_qp, &b_CCDeltaPlusAna_minos_trk_eqp_qp);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_fit_pass", &CCDeltaPlusAna_minos_trk_fit_pass, &b_CCDeltaPlusAna_minos_trk_fit_pass);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_ndf", &CCDeltaPlusAna_minos_trk_ndf, &b_CCDeltaPlusAna_minos_trk_ndf);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_p", &CCDeltaPlusAna_minos_trk_p, &b_CCDeltaPlusAna_minos_trk_p);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_p_curvature", &CCDeltaPlusAna_minos_trk_p_curvature, &b_CCDeltaPlusAna_minos_trk_p_curvature);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_p_range", &CCDeltaPlusAna_minos_trk_p_range, &b_CCDeltaPlusAna_minos_trk_p_range);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_qp", &CCDeltaPlusAna_minos_trk_qp, &b_CCDeltaPlusAna_minos_trk_qp);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_vtx_x", &CCDeltaPlusAna_minos_trk_vtx_x, &b_CCDeltaPlusAna_minos_trk_vtx_x);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_vtx_y", &CCDeltaPlusAna_minos_trk_vtx_y, &b_CCDeltaPlusAna_minos_trk_vtx_y);
   fChain->SetBranchAddress("CCDeltaPlusAna_minos_trk_vtx_z", &CCDeltaPlusAna_minos_trk_vtx_z, &b_CCDeltaPlusAna_minos_trk_vtx_z);
   fChain->SetBranchAddress("CCDeltaPlusAna_muon_E", &CCDeltaPlusAna_muon_E, &b_CCDeltaPlusAna_muon_E);
   fChain->SetBranchAddress("CCDeltaPlusAna_muon_E_shift", &CCDeltaPlusAna_muon_E_shift, &b_CCDeltaPlusAna_muon_E_shift);
   fChain->SetBranchAddress("CCDeltaPlusAna_muon_muScore", &CCDeltaPlusAna_muon_muScore, &b_CCDeltaPlusAna_muon_muScore);
   fChain->SetBranchAddress("CCDeltaPlusAna_muon_p", &CCDeltaPlusAna_muon_p, &b_CCDeltaPlusAna_muon_p);
   fChain->SetBranchAddress("CCDeltaPlusAna_muon_px", &CCDeltaPlusAna_muon_px, &b_CCDeltaPlusAna_muon_px);
   fChain->SetBranchAddress("CCDeltaPlusAna_muon_py", &CCDeltaPlusAna_muon_py, &b_CCDeltaPlusAna_muon_py);
   fChain->SetBranchAddress("CCDeltaPlusAna_muon_pz", &CCDeltaPlusAna_muon_pz, &b_CCDeltaPlusAna_muon_pz);
   fChain->SetBranchAddress("CCDeltaPlusAna_muon_qp", &CCDeltaPlusAna_muon_qp, &b_CCDeltaPlusAna_muon_qp);
   fChain->SetBranchAddress("CCDeltaPlusAna_muon_qpqpe", &CCDeltaPlusAna_muon_qpqpe, &b_CCDeltaPlusAna_muon_qpqpe);
   fChain->SetBranchAddress("CCDeltaPlusAna_muon_roadUpstreamEnergy", &CCDeltaPlusAna_muon_roadUpstreamEnergy, &b_CCDeltaPlusAna_muon_roadUpstreamEnergy);
   fChain->SetBranchAddress("CCDeltaPlusAna_muon_theta", &CCDeltaPlusAna_muon_theta, &b_CCDeltaPlusAna_muon_theta);
   fChain->SetBranchAddress("CCDeltaPlusAna_muon_theta_biasDown", &CCDeltaPlusAna_muon_theta_biasDown, &b_CCDeltaPlusAna_muon_theta_biasDown);
   fChain->SetBranchAddress("CCDeltaPlusAna_muon_theta_biasUp", &CCDeltaPlusAna_muon_theta_biasUp, &b_CCDeltaPlusAna_muon_theta_biasUp);
   fChain->SetBranchAddress("CCDeltaPlusAna_r_minos_trk_bdL", &CCDeltaPlusAna_r_minos_trk_bdL, &b_CCDeltaPlusAna_r_minos_trk_bdL);
   fChain->SetBranchAddress("CCDeltaPlusAna_r_minos_trk_end_dcosx", &CCDeltaPlusAna_r_minos_trk_end_dcosx, &b_CCDeltaPlusAna_r_minos_trk_end_dcosx);
   fChain->SetBranchAddress("CCDeltaPlusAna_r_minos_trk_end_dcosy", &CCDeltaPlusAna_r_minos_trk_end_dcosy, &b_CCDeltaPlusAna_r_minos_trk_end_dcosy);
   fChain->SetBranchAddress("CCDeltaPlusAna_r_minos_trk_end_dcosz", &CCDeltaPlusAna_r_minos_trk_end_dcosz, &b_CCDeltaPlusAna_r_minos_trk_end_dcosz);
   fChain->SetBranchAddress("CCDeltaPlusAna_r_minos_trk_vtx_dcosx", &CCDeltaPlusAna_r_minos_trk_vtx_dcosx, &b_CCDeltaPlusAna_r_minos_trk_vtx_dcosx);
   fChain->SetBranchAddress("CCDeltaPlusAna_r_minos_trk_vtx_dcosy", &CCDeltaPlusAna_r_minos_trk_vtx_dcosy, &b_CCDeltaPlusAna_r_minos_trk_vtx_dcosy);
   fChain->SetBranchAddress("CCDeltaPlusAna_r_minos_trk_vtx_dcosz", &CCDeltaPlusAna_r_minos_trk_vtx_dcosz, &b_CCDeltaPlusAna_r_minos_trk_vtx_dcosz);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjPx", &CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjPx, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjPx);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjPy", &CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjPy, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjPy);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjPz", &CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjPz, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjPz);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjX", &CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjX, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjX);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjY", &CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjY, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjY);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjZ", &CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjZ, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMinosInitProjZ);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalPx", &CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalPx, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalPx);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalPy", &CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalPy, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalPy);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalPz", &CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalPz, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalPz);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalX", &CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalX, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalX);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalY", &CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalY, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalY);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalZ", &CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalZ, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMnvFinalZ);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitPx", &CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitPx, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitPx);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitPy", &CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitPy, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitPy);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitPz", &CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitPz, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitPz);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitX", &CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitX, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitX);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitY", &CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitY, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitY);
   fChain->SetBranchAddress("CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitZ", &CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitZ, &b_CCDeltaPlusAna_t_minos_trk_primFSLepMnvInitZ);
   fChain->SetBranchAddress("CCDeltaPlusAna_trajMuonPhi", &CCDeltaPlusAna_trajMuonPhi, &b_CCDeltaPlusAna_trajMuonPhi);
   fChain->SetBranchAddress("CCDeltaPlusAna_trajMuonProngMomentum", &CCDeltaPlusAna_trajMuonProngMomentum, &b_CCDeltaPlusAna_trajMuonProngMomentum);
   fChain->SetBranchAddress("CCDeltaPlusAna_trajMuonProngPx", &CCDeltaPlusAna_trajMuonProngPx, &b_CCDeltaPlusAna_trajMuonProngPx);
   fChain->SetBranchAddress("CCDeltaPlusAna_trajMuonProngPy", &CCDeltaPlusAna_trajMuonProngPy, &b_CCDeltaPlusAna_trajMuonProngPy);
   fChain->SetBranchAddress("CCDeltaPlusAna_trajMuonProngPz", &CCDeltaPlusAna_trajMuonProngPz, &b_CCDeltaPlusAna_trajMuonProngPz);
   fChain->SetBranchAddress("CCDeltaPlusAna_trajMuonTheta", &CCDeltaPlusAna_trajMuonTheta, &b_CCDeltaPlusAna_trajMuonTheta);
   fChain->SetBranchAddress("CCDeltaPlusAna_vtx_x", &CCDeltaPlusAna_vtx_x, &b_CCDeltaPlusAna_vtx_x);
   fChain->SetBranchAddress("CCDeltaPlusAna_vtx_y", &CCDeltaPlusAna_vtx_y, &b_CCDeltaPlusAna_vtx_y);
   fChain->SetBranchAddress("CCDeltaPlusAna_vtx_z", &CCDeltaPlusAna_vtx_z, &b_CCDeltaPlusAna_vtx_z);
   fChain->SetBranchAddress("CCDeltaPlusAna_isProtonInsideOD", CCDeltaPlusAna_isProtonInsideOD, &b_CCDeltaPlusAna_isProtonInsideOD);
   fChain->SetBranchAddress("CCDeltaPlusAna_ntrajProtonProng", CCDeltaPlusAna_ntrajProtonProng, &b_CCDeltaPlusAna_ntrajProtonProng);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_kinked", CCDeltaPlusAna_proton_kinked, &b_CCDeltaPlusAna_proton_kinked);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_odMatch", CCDeltaPlusAna_proton_odMatch, &b_CCDeltaPlusAna_proton_odMatch);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_trk_pat_history", CCDeltaPlusAna_proton_trk_pat_history, &b_CCDeltaPlusAna_proton_trk_pat_history);
   fChain->SetBranchAddress("CCDeltaPlusAna_trajProtonProngPDG", CCDeltaPlusAna_trajProtonProngPDG, &b_CCDeltaPlusAna_trajProtonProngPDG);
   fChain->SetBranchAddress("CCDeltaPlusAna_trajProtonProngPrimary", CCDeltaPlusAna_trajProtonProngPrimary, &b_CCDeltaPlusAna_trajProtonProngPrimary);
   fChain->SetBranchAddress("CCDeltaPlusAna_endProtonTrajMomentum", CCDeltaPlusAna_endProtonTrajMomentum, &b_CCDeltaPlusAna_endProtonTrajMomentum);
   fChain->SetBranchAddress("CCDeltaPlusAna_endProtonTrajXPosition", CCDeltaPlusAna_endProtonTrajXPosition, &b_CCDeltaPlusAna_endProtonTrajXPosition);
   fChain->SetBranchAddress("CCDeltaPlusAna_endProtonTrajYPosition", CCDeltaPlusAna_endProtonTrajYPosition, &b_CCDeltaPlusAna_endProtonTrajYPosition);
   fChain->SetBranchAddress("CCDeltaPlusAna_endProtonTrajZPosition", CCDeltaPlusAna_endProtonTrajZPosition, &b_CCDeltaPlusAna_endProtonTrajZPosition);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_E", CCDeltaPlusAna_proton_E, &b_CCDeltaPlusAna_proton_E);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_chi2_ndf", CCDeltaPlusAna_proton_chi2_ndf, &b_CCDeltaPlusAna_proton_chi2_ndf);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_ekin", CCDeltaPlusAna_proton_ekin, &b_CCDeltaPlusAna_proton_ekin);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_endPointX", CCDeltaPlusAna_proton_endPointX, &b_CCDeltaPlusAna_proton_endPointX);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_endPointY", CCDeltaPlusAna_proton_endPointY, &b_CCDeltaPlusAna_proton_endPointY);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_endPointZ", CCDeltaPlusAna_proton_endPointZ, &b_CCDeltaPlusAna_proton_endPointZ);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_p", CCDeltaPlusAna_proton_p, &b_CCDeltaPlusAna_proton_p);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_p_calCorrection", CCDeltaPlusAna_proton_p_calCorrection, &b_CCDeltaPlusAna_proton_p_calCorrection);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_p_dEdXTool", CCDeltaPlusAna_proton_p_dEdXTool, &b_CCDeltaPlusAna_proton_p_dEdXTool);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_p_visEnergy", CCDeltaPlusAna_proton_p_visEnergy, &b_CCDeltaPlusAna_proton_p_visEnergy);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_phi", CCDeltaPlusAna_proton_phi, &b_CCDeltaPlusAna_proton_phi);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_px", CCDeltaPlusAna_proton_px, &b_CCDeltaPlusAna_proton_px);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_py", CCDeltaPlusAna_proton_py, &b_CCDeltaPlusAna_proton_py);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_pz", CCDeltaPlusAna_proton_pz, &b_CCDeltaPlusAna_proton_pz);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_score", CCDeltaPlusAna_proton_score, &b_CCDeltaPlusAna_proton_score);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_score1", CCDeltaPlusAna_proton_score1, &b_CCDeltaPlusAna_proton_score1);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_score2", CCDeltaPlusAna_proton_score2, &b_CCDeltaPlusAna_proton_score2);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_startPointX", CCDeltaPlusAna_proton_startPointX, &b_CCDeltaPlusAna_proton_startPointX);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_startPointY", CCDeltaPlusAna_proton_startPointY, &b_CCDeltaPlusAna_proton_startPointY);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_startPointZ", CCDeltaPlusAna_proton_startPointZ, &b_CCDeltaPlusAna_proton_startPointZ);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_theta", CCDeltaPlusAna_proton_theta, &b_CCDeltaPlusAna_proton_theta);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_thetaX", CCDeltaPlusAna_proton_thetaX, &b_CCDeltaPlusAna_proton_thetaX);
   fChain->SetBranchAddress("CCDeltaPlusAna_proton_thetaY", CCDeltaPlusAna_proton_thetaY, &b_CCDeltaPlusAna_proton_thetaY);
   fChain->SetBranchAddress("CCDeltaPlusAna_trajProtonPhi", CCDeltaPlusAna_trajProtonPhi, &b_CCDeltaPlusAna_trajProtonPhi);
   fChain->SetBranchAddress("CCDeltaPlusAna_trajProtonProngMomentum", CCDeltaPlusAna_trajProtonProngMomentum, &b_CCDeltaPlusAna_trajProtonProngMomentum);
   fChain->SetBranchAddress("CCDeltaPlusAna_trajProtonProngPx", CCDeltaPlusAna_trajProtonProngPx, &b_CCDeltaPlusAna_trajProtonProngPx);
   fChain->SetBranchAddress("CCDeltaPlusAna_trajProtonProngPy", CCDeltaPlusAna_trajProtonProngPy, &b_CCDeltaPlusAna_trajProtonProngPy);
   fChain->SetBranchAddress("CCDeltaPlusAna_trajProtonProngPz", CCDeltaPlusAna_trajProtonProngPz, &b_CCDeltaPlusAna_trajProtonProngPz);
   fChain->SetBranchAddress("CCDeltaPlusAna_trajProtonTheta", CCDeltaPlusAna_trajProtonTheta, &b_CCDeltaPlusAna_trajProtonTheta);
   fChain->SetBranchAddress("ev_run", &ev_run, &b_ev_run);
   fChain->SetBranchAddress("ev_subrun", &ev_subrun, &b_ev_subrun);
   fChain->SetBranchAddress("ev_detector", &ev_detector, &b_ev_detector);
   fChain->SetBranchAddress("ev_triggerType", &ev_triggerType, &b_ev_triggerType);
   fChain->SetBranchAddress("ev_gate", &ev_gate, &b_ev_gate);
   fChain->SetBranchAddress("ev_global_gate", &ev_global_gate, &b_ev_global_gate);
   fChain->SetBranchAddress("ev_gps_time_sec", &ev_gps_time_sec, &b_ev_gps_time_sec);
   fChain->SetBranchAddress("ev_gps_time_usec", &ev_gps_time_usec, &b_ev_gps_time_usec);
   fChain->SetBranchAddress("mc_run", &mc_run, &b_mc_run);
   fChain->SetBranchAddress("mc_subrun", &mc_subrun, &b_mc_subrun);
   fChain->SetBranchAddress("mc_nInteractions", &mc_nInteractions, &b_mc_nInteractions);
   fChain->SetBranchAddress("mc_MIState", &mc_MIState, &b_mc_MIState);
   fChain->SetBranchAddress("mc_pot", &mc_pot, &b_mc_pot);
   fChain->SetBranchAddress("mc_beamConfig", &mc_beamConfig, &b_mc_beamConfig);
   fChain->SetBranchAddress("mc_processType", &mc_processType, &b_mc_processType);
   fChain->SetBranchAddress("mc_nthEvtInSpill", &mc_nthEvtInSpill, &b_mc_nthEvtInSpill);
   fChain->SetBranchAddress("mc_nthEvtInFile", &mc_nthEvtInFile, &b_mc_nthEvtInFile);
   fChain->SetBranchAddress("mc_intType", &mc_intType, &b_mc_intType);
   fChain->SetBranchAddress("mc_current", &mc_current, &b_mc_current);
   fChain->SetBranchAddress("mc_charm", &mc_charm, &b_mc_charm);
   fChain->SetBranchAddress("mc_weight", &mc_weight, &b_mc_weight);
   fChain->SetBranchAddress("mc_XSec", &mc_XSec, &b_mc_XSec);
   fChain->SetBranchAddress("mc_diffXSec", &mc_diffXSec, &b_mc_diffXSec);
   fChain->SetBranchAddress("mc_incoming", &mc_incoming, &b_mc_incoming);
   fChain->SetBranchAddress("mc_fluxDriverProb", &mc_fluxDriverProb, &b_mc_fluxDriverProb);
   fChain->SetBranchAddress("mc_targetNucleus", &mc_targetNucleus, &b_mc_targetNucleus);
   fChain->SetBranchAddress("mc_targetZ", &mc_targetZ, &b_mc_targetZ);
   fChain->SetBranchAddress("mc_targetA", &mc_targetA, &b_mc_targetA);
   fChain->SetBranchAddress("mc_targetNucleon", &mc_targetNucleon, &b_mc_targetNucleon);
   fChain->SetBranchAddress("mc_struckQuark", &mc_struckQuark, &b_mc_struckQuark);
   fChain->SetBranchAddress("mc_seaQuark", &mc_seaQuark, &b_mc_seaQuark);
   fChain->SetBranchAddress("mc_resID", &mc_resID, &b_mc_resID);
   fChain->SetBranchAddress("mc_primaryLepton", &mc_primaryLepton, &b_mc_primaryLepton);
   fChain->SetBranchAddress("mc_incomingE", &mc_incomingE, &b_mc_incomingE);
   fChain->SetBranchAddress("mc_Bjorkenx", &mc_Bjorkenx, &b_mc_Bjorkenx);
   fChain->SetBranchAddress("mc_Bjorkeny", &mc_Bjorkeny, &b_mc_Bjorkeny);
   fChain->SetBranchAddress("mc_Q2", &mc_Q2, &b_mc_Q2);
   fChain->SetBranchAddress("mc_nuT", &mc_nuT, &b_mc_nuT);
   fChain->SetBranchAddress("mc_w", &mc_w, &b_mc_w);
   fChain->SetBranchAddress("mc_vtx", mc_vtx, &b_mc_vtx);
   fChain->SetBranchAddress("mc_incomingPartVec", mc_incomingPartVec, &b_mc_incomingPartVec);
   fChain->SetBranchAddress("mc_initNucVec", mc_initNucVec, &b_mc_initNucVec);
   fChain->SetBranchAddress("mc_primFSLepton", mc_primFSLepton, &b_mc_primFSLepton);
   fChain->SetBranchAddress("mc_nFSPart", &mc_nFSPart, &b_mc_nFSPart);
   fChain->SetBranchAddress("mc_FSPartPx", mc_FSPartPx, &b_mc_FSPartPx);
   fChain->SetBranchAddress("mc_FSPartPy", mc_FSPartPy, &b_mc_FSPartPy);
   fChain->SetBranchAddress("mc_FSPartPz", mc_FSPartPz, &b_mc_FSPartPz);
   fChain->SetBranchAddress("mc_FSPartE", mc_FSPartE, &b_mc_FSPartE);
   fChain->SetBranchAddress("mc_FSPartPDG", mc_FSPartPDG, &b_mc_FSPartPDG);
   fChain->SetBranchAddress("mc_er_nPart", &mc_er_nPart, &b_mc_er_nPart);
   fChain->SetBranchAddress("mc_er_ID", mc_er_ID, &b_mc_er_ID);
   fChain->SetBranchAddress("mc_er_status", mc_er_status, &b_mc_er_status);
   fChain->SetBranchAddress("mc_er_posInNucX", mc_er_posInNucX, &b_mc_er_posInNucX);
   fChain->SetBranchAddress("mc_er_posInNucY", mc_er_posInNucY, &b_mc_er_posInNucY);
   fChain->SetBranchAddress("mc_er_posInNucZ", mc_er_posInNucZ, &b_mc_er_posInNucZ);
   fChain->SetBranchAddress("mc_er_Px", mc_er_Px, &b_mc_er_Px);
   fChain->SetBranchAddress("mc_er_Py", mc_er_Py, &b_mc_er_Py);
   fChain->SetBranchAddress("mc_er_Pz", mc_er_Pz, &b_mc_er_Pz);
   fChain->SetBranchAddress("mc_er_E", mc_er_E, &b_mc_er_E);
   fChain->SetBranchAddress("mc_er_FD", mc_er_FD, &b_mc_er_FD);
   fChain->SetBranchAddress("mc_er_LD", mc_er_LD, &b_mc_er_LD);
   fChain->SetBranchAddress("mc_er_mother", mc_er_mother, &b_mc_er_mother);
   fChain->SetBranchAddress("mc_fr_nNuAncestorIDs", &mc_fr_nNuAncestorIDs, &b_mc_fr_nNuAncestorIDs);
   fChain->SetBranchAddress("mc_fr_nuAncestorIDs", mc_fr_nuAncestorIDs, &b_mc_fr_nuAncestorIDs);
   fChain->SetBranchAddress("mc_fr_nuParentID", &mc_fr_nuParentID, &b_mc_fr_nuParentID);
   fChain->SetBranchAddress("mc_fr_decMode", &mc_fr_decMode, &b_mc_fr_decMode);
   fChain->SetBranchAddress("mc_fr_primProtonVtx", mc_fr_primProtonVtx, &b_mc_fr_primProtonVtx);
   fChain->SetBranchAddress("mc_fr_primProtonP", mc_fr_primProtonP, &b_mc_fr_primProtonP);
   fChain->SetBranchAddress("mc_fr_nuParentDecVtx", mc_fr_nuParentDecVtx, &b_mc_fr_nuParentDecVtx);
   fChain->SetBranchAddress("mc_fr_nuParentProdVtx", mc_fr_nuParentProdVtx, &b_mc_fr_nuParentProdVtx);
   fChain->SetBranchAddress("mc_fr_nuParentProdP", mc_fr_nuParentProdP, &b_mc_fr_nuParentProdP);
   fChain->SetBranchAddress("mc_cvweight_total", &mc_cvweight_total, &b_mc_cvweight_total);
   fChain->SetBranchAddress("wgt", &wgt, &b_wgt);
   fChain->SetBranchAddress("mc_cvweight_totalFlux", &mc_cvweight_totalFlux, &b_mc_cvweight_totalFlux);
   fChain->SetBranchAddress("mc_cvweight_totalXsec", &mc_cvweight_totalXsec, &b_mc_cvweight_totalXsec);
   fChain->SetBranchAddress("mc_cvweight_NA49", &mc_cvweight_NA49, &b_mc_cvweight_NA49);
   fChain->SetBranchAddress("mc_wgt_GENIE_sz", &mc_wgt_GENIE_sz, &b_mc_wgt_GENIE_sz);
   fChain->SetBranchAddress("mc_wgt_GENIE", mc_wgt_GENIE, &b_mc_wgt_GENIE);
   fChain->SetBranchAddress("mc_wgt_Flux_Tertiary_sz", &mc_wgt_Flux_Tertiary_sz, &b_mc_wgt_Flux_Tertiary_sz);
   fChain->SetBranchAddress("mc_wgt_Flux_Tertiary", mc_wgt_Flux_Tertiary, &b_mc_wgt_Flux_Tertiary);
   fChain->SetBranchAddress("mc_wgt_Flux_BeamFocus_sz", &mc_wgt_Flux_BeamFocus_sz, &b_mc_wgt_Flux_BeamFocus_sz);
   fChain->SetBranchAddress("mc_wgt_Flux_BeamFocus", mc_wgt_Flux_BeamFocus, &b_mc_wgt_Flux_BeamFocus);
   fChain->SetBranchAddress("mc_wgt_Flux_NA49_sz", &mc_wgt_Flux_NA49_sz, &b_mc_wgt_Flux_NA49_sz);
   fChain->SetBranchAddress("mc_wgt_Flux_NA49", mc_wgt_Flux_NA49, &b_mc_wgt_Flux_NA49);
   fChain->SetBranchAddress("n_prongs", &n_prongs, &b_n_prongs);
   fChain->SetBranchAddress("prong_nParticles", prong_nParticles, &b_prong_nParticles);
   fChain->SetBranchAddress("prong_part_score", prong_part_score, &b_prong_part_score);
   fChain->SetBranchAddress("prong_part_mass", prong_part_mass, &b_prong_part_mass);
   fChain->SetBranchAddress("prong_part_charge", prong_part_charge, &b_prong_part_charge);
   fChain->SetBranchAddress("prong_part_pid", prong_part_pid, &b_prong_part_pid);
   fChain->SetBranchAddress("prong_part_E", &prong_part_E, &b_prong_part_E);
   fChain->SetBranchAddress("prong_part_pos", &prong_part_pos, &b_prong_part_pos);
   Notify();
}


CCDeltaPlus::~CCDeltaPlus()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t CCDeltaPlus::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t CCDeltaPlus::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

Bool_t CCDeltaPlus::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

    cout<<"Done!"<<endl;
   return kTRUE;
}

void CCDeltaPlus::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t CCDeltaPlus::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

void CCDeltaPlus::initHistograms()
{
    cout<<"Initializing Histograms"<<endl;
    
    pID_purity = new TH1F( "pID_purity","Proton Purity",binList.particleScore.get_nBins(), binList.particleScore.get_min(), binList.particleScore.get_max() );
    pID_purity->GetXaxis()->SetTitle("Proton Purity = Captured Proton / Captured Total Events");
    pID_purity->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.particleScore.get_width()));
    
    pID_efficiency = new TH1F( "pID_efficiency","Proton Efficiency",binList.particleScore.get_nBins(), binList.particleScore.get_min(), binList.particleScore.get_max() );
    pID_efficiency->GetXaxis()->SetTitle("Proton Efficiency = Captured Proton / Total Protons");
    pID_efficiency->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.particleScore.get_width()));
    
    pID_piplus = new TH1F( "pID_piplus","Pi Plus",binList.particleScore.get_nBins(), binList.particleScore.get_min(), binList.particleScore.get_max() );
    pID_piplus->GetXaxis()->SetTitle("Pi Plus");
    pID_piplus->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.particleScore.get_width()));
    
    pID_piminus = new TH1F( "pID_piminus","Pi Minus",binList.particleScore.get_nBins(), binList.particleScore.get_min(), binList.particleScore.get_max() );
    pID_piminus->GetXaxis()->SetTitle("Pi Minus");
    pID_piminus->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.particleScore.get_width()));
    
    pID_proton = new TH1F( "pID_proton","Proton",binList.particleScore.get_nBins(), binList.particleScore.get_min(), binList.particleScore.get_max() );
    pID_proton->GetXaxis()->SetTitle("Proton");
    pID_proton->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.particleScore.get_width()));
    
    pID_other = new TH1F( "pID_other","Other",binList.particleScore.get_nBins(), binList.particleScore.get_min(), binList.particleScore.get_max() );
    pID_other->GetXaxis()->SetTitle("Other");
    pID_other->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.particleScore.get_width()));
    
    beamEnergy_mc = new TH1F( "beamEnergy_mc","True Beam Energy",binList.beamE.get_nBins(), binList.beamE.get_min(), binList.beamE.get_max() );
    beamEnergy_mc->GetXaxis()->SetTitle("True Beam Energy MeV");
    beamEnergy_mc->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.beamE.get_width()));
    
    beamEnergy_reco = new TH1F( "beamEnergy_reco","Reconstructed Beam Energy",binList.beamE.get_nBins(), binList.beamE.get_min(), binList.beamE.get_max() );
    beamEnergy_reco->GetXaxis()->SetTitle("Reconstructed Beam Energy MeV");
    beamEnergy_reco->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.beamE.get_width()));
    
    beamEnergy_error = new TH1F( "beamEnergy_error","Error on Beam Energy",binList.error.get_nBins(), binList.error.get_min(), binList.error.get_max() );
    beamEnergy_error->GetXaxis()->SetTitle("(Reco- True) / True");
    beamEnergy_error->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.error.get_width()));
    
    beamEnergy_reco_mc = new TH2F( "beamEnergy_reco_mc","True vs Reconstructed Beam Energy",
                                binList.beamE.get_nBins(), binList.beamE.get_min(), binList.beamE.get_max(),
                                binList.beamE.get_nBins(), binList.beamE.get_min(), binList.beamE.get_max());
    beamEnergy_reco_mc->GetXaxis()->SetTitle("Reconstructed Beam Energy MeV");
    beamEnergy_reco_mc->GetYaxis()->SetTitle("True Beam Energy MeV");
    
    q2_mc = new TH1F( "q2_mc","True Q^{2}",binList.q2.get_nBins(), binList.q2.get_min(), binList.q2.get_max() );
    q2_mc->GetXaxis()->SetTitle("True Q^{2} MeV");
    q2_mc->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.q2.get_width()));
    
    q2_reco = new TH1F( "q2_reco","Reconstructed Q^{2}",binList.q2.get_nBins(), binList.q2.get_min(), binList.q2.get_max() );
    q2_reco->GetXaxis()->SetTitle("Reconstructed Q^{2} MeV");
    q2_reco->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.q2.get_width()));
    
    q2_error = new TH1F( "q2_error","Error on Q^{2}",binList.error.get_nBins(), binList.error.get_min(), binList.error.get_max() );
    q2_error->GetXaxis()->SetTitle("(Reco- True) / True");
    q2_error->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.error.get_width()));
    
    q2_reco_mc = new TH2F( "q2_reco_mc","True vs Reconstructed Q^{2}",
                                binList.q2.get_nBins(), binList.q2.get_min(), binList.q2.get_max(),
                                binList.q2.get_nBins(), binList.q2.get_min(), binList.q2.get_max());
    q2_reco_mc->GetXaxis()->SetTitle("Reconstructed Q^{2} MeV");
    q2_reco_mc->GetYaxis()->SetTitle("True Q^{2} MeV");
    
    int_channel = new TH1F( "int_channel","Interaction Channel",binList.int_channel.get_nBins(), binList.int_channel.get_min(), binList.int_channel.get_max() );
    int_channel->GetXaxis()->SetTitle("1 = QE, 2 = Resonant, 3 = DIS, 4 = Coh pi");
    int_channel->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.int_channel.get_width()));
    
    vertex_x_y_true = new TH2F( "vertex_x_y_true","True Vertex X vs Y",   binList.vertex_x_y.get_nBins(), binList.vertex_x_y.get_min(), binList.vertex_x_y.get_max(),
                                                                binList.vertex_x_y.get_nBins(), binList.vertex_x_y.get_min(), binList.vertex_x_y.get_max());
    vertex_x_y_true->GetXaxis()->SetTitle("True Vertex X [mm]");
    vertex_x_y_true->GetYaxis()->SetTitle("True Vertex Y [mm]");

    vertex_x_y_reco = new TH2F( "vertex_x_y_reco","Reconstructed Vertex X vs Y",   binList.vertex_x_y.get_nBins(), binList.vertex_x_y.get_min(), binList.vertex_x_y.get_max(),
                                                                binList.vertex_x_y.get_nBins(), binList.vertex_x_y.get_min(), binList.vertex_x_y.get_max());
    vertex_x_y_reco->GetXaxis()->SetTitle("Reconstructed Vertex X [mm]");
    vertex_x_y_reco->GetYaxis()->SetTitle("Reconstructed Vertex Y [mm]");
    
    vertex_z_true = new TH1F( "vertex_z_true","True Vertex Z",binList.vertex_z.get_nBins(), binList.vertex_z.get_min(), binList.vertex_z.get_max() );
    vertex_z_true->GetXaxis()->SetTitle("z = 4293 Target, #bf{z = 5810 Interaction Region}, z = 8614 ECAL, z = 9088 HCAL");
    vertex_z_true->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.vertex_z.get_width()));
    
    vertex_z_reco = new TH1F( "vertex_z_reco","Reconstructed Vertex Z",binList.vertex_z.get_nBins(), binList.vertex_z.get_min(), binList.vertex_z.get_max() );
    vertex_z_reco->GetXaxis()->SetTitle("z = 4293 Target, #bf{z = 5810 Interaction Region}, z = 8614 ECAL, z = 9088 HCAL");
    vertex_z_reco->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.vertex_z.get_width()));
    
    vertex_z_error = new TH1F( "vertex_z_error","Error on Vertex Z",binList.error.get_nBins(), binList.error.get_min(), binList.error.get_max() );
    vertex_z_error->GetXaxis()->SetTitle("(Reco- True) / True");
    vertex_z_error->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.error.get_width()));
    
    vertex_z_reco_mc = new TH2F( "vertex_z_reco_mc","True vs Reconstructed Vertex Z",
                                binList.vertex_z.get_nBins(), binList.vertex_z.get_min(), binList.vertex_z.get_max(),
                                binList.vertex_z.get_nBins(), binList.vertex_z.get_min(), binList.vertex_z.get_max());
    vertex_z_reco_mc->GetXaxis()->SetTitle("Reconstructed Vertex Z");
    vertex_z_reco_mc->GetYaxis()->SetTitle("True Vertex Z");

    n_FSParticles = new TH1F( "n_FSParticles","Number of Final State Particles",binList.multiplicity.get_nBins(), binList.multiplicity.get_min(), binList.multiplicity.get_max() );
    n_FSParticles->GetXaxis()->SetTitle("Number of Final State Particles");
    n_FSParticles->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.multiplicity.get_width()));
    
    n_gammas = new TH1F( "n_gammas","Number of Gammas",binList.multiplicity.get_nBins(), binList.multiplicity.get_min(), binList.multiplicity.get_max() );
    n_gammas->GetXaxis()->SetTitle("Number of Gammas");
    n_gammas->GetYaxis()->SetTitle(Form("Candidates / %3.1f ",binList.multiplicity.get_width()));    
    

    cout<<"Done!"<<endl;
}

#endif //CCDeltaPlus_cpp
