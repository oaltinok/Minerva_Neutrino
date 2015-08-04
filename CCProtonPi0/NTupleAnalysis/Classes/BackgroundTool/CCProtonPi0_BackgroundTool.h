/*
================================================================================
Class: CCProtonPi0_BackgroundTool
    Class Responsible for 
        Calculating Statistics for Background Events
        Creating Background Tables
    
    Main Directory:
        Classes/BackgroundTool
 
    Author:         Ozgur Altinok  - ozgur.altinok@tufts.edu
================================================================================
*/
#ifndef CCProtonPi0_BackgroundTool_h
#define CCProtonPi0_BackgroundTool_h

#include "../NTupleAnalysis/CCProtonPi0_NTupleAnalysis.h"

using namespace std;

struct Background
{ 
    string name; 
    double nAll;
    double nWithMichel;
};

class CCProtonPi0_BackgroundTool : public CCProtonPi0_NTupleAnalysis
{
    public:
        CCProtonPi0_BackgroundTool(bool isModeReduce, std::string ana_folder);
        ~CCProtonPi0_BackgroundTool();
        
        void writeBackgroundTable();
        void fillBackgroundWithPi0(bool NoPi0, bool SinglePi0, bool MultiPi0, bool withMichel);
        
        void fillBackground(bool NC,
                            bool AntiNeutrino,
                            bool QELike,
                            bool SinglePion,
                            bool DoublePion,
                            bool MultiPion,
                            bool Other,
                            bool withMichel);
    private:
        string fileName;
        ofstream textFile;
        
        vector< Background > BackgroundWithPi0Vector;
        vector< Background > BackgroundTypeVector;
       
        // Background with Pi0
        Background bckg_NoPi0;
        Background bckg_SinglePi0;
        Background bckg_MultiPi0;
        Background bckg_Total_WithPi0;
        
        // Background Types 
        Background bckg_NC;
        Background bckg_AntiNeutrino;
        Background bckg_QELike;
        Background bckg_SinglePion;
        Background bckg_DoublePion;
        Background bckg_MultiPion;
        Background bckg_Other;
        Background bckg_Total;

        void updateBackground(Background &b, bool withMichel);
        double calcPercent(double nEvents, double nBase);
        void formBackgroundVectors();
        void initBackgrounds();
        void initSingleBackground(Background &b, string name);
        void writeBackgroundTableHeader();
        void writeBackgroundTableRows(vector< Background > &bckgVector);
        void OpenTextFile(std::string ana_folder);

};


#endif

