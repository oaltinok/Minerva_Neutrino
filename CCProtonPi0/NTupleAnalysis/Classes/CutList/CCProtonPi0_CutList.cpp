/*
    See CutList.h header for Class Information
*/
#ifndef CCProtonPi0_CutList_cpp
#define CCProtonPi0_CutList_cpp

#include "CCProtonPi0_CutList.h"

using namespace std;

CCProtonPi0_CutList::CCProtonPi0_CutList(int nMode) : CCProtonPi0_NTupleAnalysis(nMode)
{
    cout<<"Initializing CCProtonPi0_CutList"<<endl;
    
    SetCutNames();
    OpenTextFiles();

    cout<<"Done!"<<endl;
}

void CCProtonPi0_CutList::SetCutNames()
{
    // Common Cut Numbers
    nCut_All.set_Name("All");
    nCut_Vertex_None.set_Name("Vertex_None");
    nCut_Vertex_Not_Reconstructable.set_Name("Vertex_Not_Reconstructable"); 
    nCut_Vertex_Not_Fiducial.set_Name("Vertex_Not_Fiducial");
    nCut_Vertex_Count.set_Name("Vertex_Count");  
    nCut_nProngs.set_Name("nProngs");
    
    // nProngs == 1 Cut Numbers
    nCut_1Prong_Muon_None.set_Name("Muon_None");              
    nCut_1Prong_Muon_Not_Plausible.set_Name("Muon_Not_Plausible");
    nCut_1Prong_Muon_Score_Low.set_Name("Muon_Score_Low");
    nCut_1Prong_Muon_Charge.set_Name("Muon_Charge");
    nCut_1Prong_Vertex_Michel_Exist.set_Name("Vertex_Michel_Exist"); 
    nCut_1Prong_EndPoint_Michel_Exist.set_Name("EndPoint_Michel_Exist");
    nCut_1Prong_secEndPoint_Michel_Exist.set_Name("secEndPoint_Michel_Exist");
    nCut_1Prong_PreFilter_Pi0.set_Name("PreFilter_Pi0");
    nCut_1Prong_VtxBlob.set_Name("VtxBlob");
    nCut_1Prong_ConeBlobs.set_Name("ConeBlobs");
    nCut_1Prong_Photon1DistanceLow.set_Name("Photon1DistanceLow");
    nCut_1Prong_Photon2DistanceLow.set_Name("Photon2DistanceLow");
    nCut_1Prong_Pi0_invMass.set_Name("Pi0_invMass");
    nCut_1Prong_beamEnergy.set_Name("beamEnergy");
    nCut_1Prong_UnusedE.set_Name("UnusedE");
    nCut_1Prong_Pi0BlobCuts.set_Name("Pi0BlobCuts");
    nCut_1Prong_PreBlobCuts.set_Name("PreBlobCuts");

    // nProngs == 2 Cut Numbers
    nCut_2Prong_Muon_None.set_Name("Muon_None");              
    nCut_2Prong_Muon_Not_Plausible.set_Name("Muon_Not_Plausible");
    nCut_2Prong_Muon_Score_Low.set_Name("Muon_Score_Low");
    nCut_2Prong_Muon_Charge.set_Name("Muon_Charge");
    nCut_2Prong_Vertex_Michel_Exist.set_Name("Vertex_Michel_Exist"); 
    nCut_2Prong_EndPoint_Michel_Exist.set_Name("EndPoint_Michel_Exist");
    nCut_2Prong_secEndPoint_Michel_Exist.set_Name("secEndPoint_Michel_Exist");
    nCut_2Prong_Particle_None.set_Name("Particle_None");
    nCut_2Prong_Proton_None.set_Name("Proton_None");            
    nCut_2Prong_ProtonScore.set_Name("Proton_Score");
    nCut_2Prong_PreFilter_Pi0.set_Name("PreFilter_Pi0");
    nCut_2Prong_VtxBlob.set_Name("VtxBlob");
    nCut_2Prong_ConeBlobs.set_Name("ConeBlobs");
    nCut_2Prong_Photon1DistanceLow.set_Name("Photon1DistanceLow");
    nCut_2Prong_Photon2DistanceLow.set_Name("Photon1DistanceLow");
    nCut_2Prong_Pi0_invMass.set_Name("Pi0_invMass");
    nCut_2Prong_beamEnergy.set_Name("beamEnergy");
    nCut_2Prong_UnusedE.set_Name("UnusedE");
    nCut_2Prong_DeltaInvMass.set_Name("Delta_invMass");
    nCut_2Prong_Pi0BlobCuts.set_Name("Pi0BlobCuts");
    nCut_2Prong_PreBlobCuts.set_Name("PreBlobCuts");

}

void CCProtonPi0_CutList::OpenTextFiles()
{
    // Open Cut Files
    cutFile[0] = Folder_List::output + Folder_List::textOut + branchDir + "CutTable_1Prong.txt";
    cutFile[1] = Folder_List::output + Folder_List::textOut + branchDir + "CutTable_2Prong.txt";
    
    for (int i = 0; i < nTopologies; i++){
        cutText[i].open( cutFile[i].c_str() );
        if( !cutText[i].is_open() ){
            cerr<<"Cannot open output text file: "<<cutFile[i]<<endl;
            exit(1);
        }else{
            cout<<"\t"<<cutFile[i]<<endl;
        }
    } 
}

void CCProtonPi0_CutList::writeCutTableHeader()
{
    for (int i = 0; i < nTopologies; i++){
        cutText[i]<<std::left;
        
        cutText[i].width(35); cutText[i]<<"Cut"<<" "; 
        
        cutText[i].width(12); cutText[i]<<"N(Events)"<<" ";    
        cutText[i].width(12); cutText[i]<<"N(Signal)"<<" ";      
        cutText[i].width(12); cutText[i]<<"Efficiency"<<" ";      
        cutText[i].width(12); cutText[i]<<"Purity"<<" ";
        cutText[i].width(12); cutText[i]<<"N(Study1)"<<" "; 
        cutText[i].width(12); cutText[i]<<"N(Study2)"<<" "; 
        cutText[i]<<endl;
    }

}

double CCProtonPi0_CutList::getCutEfficiency(double nSig, double effBase)
{
    double eff;   
    eff = (nSig / effBase) * 100.0;
    return eff;   
}

double CCProtonPi0_CutList::getCutPurity(double nSig, double nEvents)
{
    double purity;   
    purity = (nSig / nEvents) * 100.0;
    return purity;   
}

void CCProtonPi0_CutList::formCutVectors()
{   
    //--------------------------------------------------------------------------
    // Form nCutVector for nProngs == 1
    //--------------------------------------------------------------------------
    // Complete List
    nCutVector_1Prong.push_back(nCut_All);
    nCutVector_1Prong.push_back(nCut_Vertex_None);
    nCutVector_1Prong.push_back(nCut_Vertex_Not_Reconstructable); 
    nCutVector_1Prong.push_back(nCut_Vertex_Not_Fiducial);
    nCutVector_1Prong.push_back(nCut_Vertex_Count); 
    nCutVector_1Prong.push_back(nCut_nProngs);
    nCutVector_1Prong.push_back(nCut_1Prong_Muon_None);              
    nCutVector_1Prong.push_back(nCut_1Prong_Muon_Not_Plausible);
    nCutVector_1Prong.push_back(nCut_1Prong_Muon_Score_Low);
    nCutVector_1Prong.push_back(nCut_1Prong_Muon_Charge);
    nCutVector_1Prong.push_back(nCut_1Prong_Vertex_Michel_Exist); 
    nCutVector_1Prong.push_back(nCut_1Prong_EndPoint_Michel_Exist);
    nCutVector_1Prong.push_back(nCut_1Prong_secEndPoint_Michel_Exist);
    nCutVector_1Prong.push_back(nCut_1Prong_PreFilter_Pi0);
    nCutVector_1Prong.push_back(nCut_1Prong_VtxBlob);
    nCutVector_1Prong.push_back(nCut_1Prong_ConeBlobs);
    nCutVector_1Prong.push_back(nCut_1Prong_PreBlobCuts);
    nCutVector_1Prong.push_back(nCut_1Prong_Pi0BlobCuts);
    nCutVector_1Prong.push_back(nCut_1Prong_Photon1DistanceLow);
    nCutVector_1Prong.push_back(nCut_1Prong_Photon2DistanceLow);
    nCutVector_1Prong.push_back(nCut_1Prong_Pi0_invMass);
    nCutVector_1Prong.push_back(nCut_1Prong_beamEnergy);
    nCutVector_1Prong.push_back(nCut_1Prong_UnusedE);
    
    //--------------------------------------------------------------------------
    // Form nCutVector for nProngs == 2
    //--------------------------------------------------------------------------
    nCutVector_2Prong.push_back(nCut_All);
    nCutVector_2Prong.push_back(nCut_Vertex_None);
    nCutVector_2Prong.push_back(nCut_Vertex_Not_Reconstructable); 
    nCutVector_2Prong.push_back(nCut_Vertex_Not_Fiducial);
    nCutVector_2Prong.push_back(nCut_Vertex_Count); 
    nCutVector_2Prong.push_back(nCut_nProngs);
    nCutVector_2Prong.push_back(nCut_2Prong_Muon_None);              
    nCutVector_2Prong.push_back(nCut_2Prong_Muon_Not_Plausible);
    nCutVector_2Prong.push_back(nCut_2Prong_Muon_Score_Low);
    nCutVector_2Prong.push_back(nCut_2Prong_Muon_Charge);
    nCutVector_2Prong.push_back(nCut_2Prong_Vertex_Michel_Exist); 
    nCutVector_2Prong.push_back(nCut_2Prong_EndPoint_Michel_Exist);
    nCutVector_2Prong.push_back(nCut_2Prong_secEndPoint_Michel_Exist);
    nCutVector_2Prong.push_back(nCut_2Prong_PreFilter_Pi0);
    nCutVector_2Prong.push_back(nCut_2Prong_VtxBlob);
    nCutVector_2Prong.push_back(nCut_2Prong_ConeBlobs);
    nCutVector_2Prong.push_back(nCut_2Prong_PreBlobCuts);
    nCutVector_2Prong.push_back(nCut_2Prong_Pi0BlobCuts);
    nCutVector_2Prong.push_back(nCut_2Prong_Photon1DistanceLow);
    nCutVector_2Prong.push_back(nCut_2Prong_Photon2DistanceLow);
    nCutVector_2Prong.push_back(nCut_2Prong_Pi0_invMass);
    nCutVector_2Prong.push_back(nCut_2Prong_Particle_None);
    nCutVector_2Prong.push_back(nCut_2Prong_Proton_None);
    nCutVector_2Prong.push_back(nCut_2Prong_ProtonScore);
    nCutVector_2Prong.push_back(nCut_2Prong_DeltaInvMass);
    nCutVector_2Prong.push_back(nCut_2Prong_beamEnergy);
    nCutVector_2Prong.push_back(nCut_2Prong_UnusedE);
    
}

void CCProtonPi0_CutList::writeCutTable()
{
    formCutVectors();
    
    for (int t = 0; t < nTopologies; t++){
        cout<<">> Writing "<<cutFile[t]<<endl;
    }
    
    writeCutTableHeader();
    writeCutTableRows(nCutVector_1Prong, 1);
    writeCutTableRows(nCutVector_2Prong, 2);
}

void CCProtonPi0_CutList::writeCutTableRows(vector<CCProtonPi0_Cut> nCutVector, int nProngs)
{
    int t; // Topology
    double efficiency;
    double purity;    
    double efficiencyBase;
    int efficiencyInd = 6;
    
    if (nProngs == 1) t = 0;
    if (nProngs == 2) t = 1;

    // Efficiency Base is MINOS Matched Muons
    efficiencyBase = nCutVector[efficiencyInd].nSignal.getCount();
      
    // Loop over Each Cut    
    for( unsigned int i = 0; i < nCutVector.size(); i++){

            efficiency = getCutEfficiency(nCutVector[i].nSignal.getCount(),efficiencyBase);
            purity = getCutPurity(nCutVector[i].nSignal.getCount(),nCutVector[i].nEvent.getCount());
            
            cutText[t].unsetf( std::ios::floatfield ); 
            cutText[t].width(35); cutText[t]<<nCutVector[i].get_Name()<<" ";
            cutText[t].width(12); cutText[t]<<nCutVector[i].nEvent.getCount()<<" ";
            
            // Total Signal
            cutText[t].width(12); cutText[t]<<nCutVector[i].nSignal.getCount()<<" ";

            cutText[t].precision(4); 
        
            if ( efficiency <= 100){
                cutText[t].width(12); cutText[t]<<efficiency<<" ";
            }else{
                cutText[t].width(12); cutText[t]<<"N/A"<<" ";    
            }

            if ( efficiency <= 100){
                cutText[t].width(12); cutText[t]<<purity<<" ";
            }else{
                cutText[t].width(12);  cutText[t]<<"N/A"<<" ";    
            }
            
            // Number of Events which are studied
            cutText[t].width(12); cutText[t]<<nCutVector[i].nStudy1.getCount()<<" ";
            cutText[t].width(12); cutText[t]<<nCutVector[i].nStudy2.getCount()<<" ";
            
            cutText[t]<<endl;
    }
}


CCProtonPi0_CutList::~CCProtonPi0_CutList()
{
    for (int i = 0; i < nTopologies; i++){
        cutText[i].close(); 
    }
}


#endif