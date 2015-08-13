/*
================================================================================
Class: CCProtonPi0_BinList
    List of the Bins used in Analysis
    Uses CCProtonPi0_SingleBin Class as Member Variable
    
    Main Directory:
        Classes/BinList
    
            
    Author:         Ozgur Altinok  - ozgur.altinok@tufts.edu
    Last Revision: 2015_05_07
================================================================================
*/
#ifndef CCProtonPi0_BinList_h
#define CCProtonPi0_BinList_h

#include "../SingleBin/CCProtonPi0_SingleBin.h"

class CCProtonPi0_BinList
{
    public:
        CCProtonPi0_BinList();
        
        // Standard Bins
        CCProtonPi0_SingleBin true_false;
        CCProtonPi0_SingleBin error;
        CCProtonPi0_SingleBin angle;
        CCProtonPi0_SingleBin particleScore;
        CCProtonPi0_SingleBin particleScore_LLR;
        CCProtonPi0_SingleBin particleScoreSum;
        CCProtonPi0_SingleBin particleScoreDiff;
        CCProtonPi0_SingleBin particleStatus;
        CCProtonPi0_SingleBin multiplicity;
        CCProtonPi0_SingleBin fraction;
        CCProtonPi0_SingleBin fraction2;
        CCProtonPi0_SingleBin vertex_z;
        CCProtonPi0_SingleBin vertex_x_y;
        CCProtonPi0_SingleBin deltaInvMass;
        CCProtonPi0_SingleBin preFilter_Status;
        CCProtonPi0_SingleBin preFilter_RejectedEnergy;
        
        // Analysis
        CCProtonPi0_SingleBin beamE;
        CCProtonPi0_SingleBin q2;
        CCProtonPi0_SingleBin wSq;
        CCProtonPi0_SingleBin w;
        
        CCProtonPi0_SingleBin UsedE;
        CCProtonPi0_SingleBin UnusedE;
        
        // Cut Histograms
        CCProtonPi0_SingleBin eVis_nuclearTarget;
        CCProtonPi0_SingleBin eVis_other;
        CCProtonPi0_SingleBin pi0_invMass;
        CCProtonPi0_SingleBin bin_photonConvLength;
        
        // Michel Tool
        CCProtonPi0_SingleBin michelMuon_P;
        CCProtonPi0_SingleBin michelMuon_end_dist_vtx;
        CCProtonPi0_SingleBin michelMuon_length;
        CCProtonPi0_SingleBin michelMuon_Z_vtx;
        CCProtonPi0_SingleBin michelPion_P;
        CCProtonPi0_SingleBin michelPion_begin_dist_vtx;
        CCProtonPi0_SingleBin michelPion_length;
        CCProtonPi0_SingleBin michel_time_diff;
        
    
    private:
        

};

#endif
