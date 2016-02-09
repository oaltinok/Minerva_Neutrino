/*
================================================================================
main.cpp
    Main Function that controls the Analysis Package
    
    Classes Used:
        CCProtonPi0_Analyzer core class for the package
        CCProtonPi0_Plotter class includes functions specific to generate Plots
    
    Input and Output folders assigned in this function
    Edit isTest variable to run a Test job or all playlist
    
    Build Package using "make"
    
    Usage:
    Reduce NTuple to a Single File
            > ./main.exe reduce
    Analyze Reduced File
            > ./main.exe run
    Plot Analyzed File
            > ./main.exe plot

    Author:        Ozgur Altinok  - ozgur.altinok@tufts.edu
================================================================================
*/

// Include Required Classes
#include "Classes/Analyzer/CCProtonPi0_Analyzer.h"
#include "Classes/CrossSection/CCProtonPi0_CrossSection.h"
#include "Classes/Plotter/CCProtonPi0_Plotter.h"
#include "Cintex/Cintex.h"

#include <string>
#include <ctime>

using namespace std;

const string runOption_Run = "run";
const string runOption_Plot = "plot";
const string runOption_Reduce = "reduce";
const string runOption_CrossSection = "calc";

const string typeOption_mc = "mc";
const string typeOption_data = "data";

int GetMode(int argc, char* argv[]);
void showInputError(char *argv[]);
void Plot();
void Reduce(string playlist, bool isMC);
void Analyze(string playlist, bool isMC);
void Calculate_CrossSection();

int main(int argc, char *argv[] )
{
    time_t timeStart; time(&timeStart);
    time_t timeEnd;
    double timeDiff;
    int timeDiff_m;
    int timeDiff_s;
    string pl_reduce;
    string pl_analyze;
    bool isMC;

    // Check User Command
    int nMode = GetMode(argc,argv);
    if (nMode == -1){
        showInputError(argv);
        return 0;
    }

    if (nMode == 0 || nMode == 2) isMC = true;
    else isMC = false;

    if (isMC && nMode < 4){
        cout<<"MC Playlists Selected!\n"<<endl;
        pl_reduce = "Input/Playlists/pl_MC_Merged.dat"; 
        pl_analyze = "Input/Playlists/pl_MC_Reduced.dat"; 
    }else if (nMode < 4){
        cout<<"Data Playlists Selected!\n"<<endl;
        pl_reduce = "Input/Playlists/pl_Data_Merged.dat"; 
        pl_analyze = "Input/Playlists/pl_Data_Reduced.dat"; 
    }
   
    ROOT::Cintex::Cintex::Enable();

    if (nMode == 0 || nMode == 1) Reduce(pl_reduce, isMC);
    else if (nMode == 2 || nMode == 3) Analyze(pl_analyze, isMC);
    else if (nMode == 4) Plot();
    else if (nMode == 5) Calculate_CrossSection();
    else{
        cout<<"Problem on Mode!, Returning"<<endl;
        return 0;
    }

    time(&timeEnd);
    timeDiff = ( timeEnd - timeStart );
    
    timeDiff_m = (int)timeDiff / 60;
    timeDiff_s = (int)timeDiff % 60;
    
    cout<<"Time to Complete = "<<timeDiff_m<<"\' "<<timeDiff_s<<"\" or "<<timeDiff<<" seconds"<<endl;
    return 0;
}

void Reduce(string playlist, bool isMC)
{
    bool isModeReduce = true;
    cout<<"\n"<<endl;
    cout<<"======================================================================"<<endl;
    cout<<"Reducing NTuples..."<<endl;
    cout<<"======================================================================"<<endl;
    CCProtonPi0_Analyzer t(isModeReduce, isMC);
    t.reduce(playlist);
}

void Analyze(string playlist, bool isMC)
{
    bool isModeReduce = false;
    cout<<"\n"<<endl;
    cout<<"======================================================================"<<endl;
    cout<<"Analyzing NTuples, Creating Histograms..."<<endl;
    cout<<"======================================================================"<<endl;
    CCProtonPi0_Analyzer analyzer(isModeReduce, isMC);
    analyzer.analyze(playlist);
}

void Calculate_CrossSection()
{
    cout<<"\n"<<endl;
    cout<<"======================================================================"<<endl;
    cout<<"Calculating Cross Section..."<<endl;
    cout<<"======================================================================"<<endl;
    CCProtonPi0_CrossSection crossSection;
    crossSection.Calc_Crossections();
}


void Plot()
{
    cout<<"======================================================================"<<endl;
    cout<<"Plotting Histograms..."<<endl;
    cout<<"======================================================================"<<endl;
    CCProtonPi0_Plotter plotter;
    plotter.plotHistograms();
}

/*
 *  -1   Error
 *  0   Reduce MC
 *  1   Reduce Data
 *  2   Analyze MC
 *  3   Analyze Data
 *  4   Plot
 */
int GetMode(int argc, char* argv[])
{
    // argc can only be 2 or 3
    if (argc != 2 && argc != 3) return -1;

    std::string runSelect = argv[1];
    if (argc == 2){
        if (runSelect.compare(runOption_Plot) == 0) return 4;
        else if (runSelect.compare(runOption_CrossSection) == 0) return 5;
        else return -1;
    }
     
    std::string typeSelect = argv[2];
    // First check for ERROR
    if (runSelect.compare(runOption_Reduce) != 0 && runSelect.compare(runOption_Run) != 0) return -1;
    if (typeSelect.compare(typeOption_mc) != 0 && typeSelect.compare(typeOption_data) != 0) return -1;

    // Passed ERROR Check - Valid Input    
    if (runSelect.compare(runOption_Reduce) == 0){
        if (typeSelect.compare(typeOption_mc) == 0) return 0;
        else if (typeSelect.compare(typeOption_data) == 0) return 1;
        else return -1;
    }

    if (runSelect.compare(runOption_Run) == 0){
        if (typeSelect.compare(typeOption_mc) == 0) return 2;
        else if (typeSelect.compare(typeOption_data) == 0) return 3;
        else return -1;
    }

    return -1;
}

void showInputError(char *argv[])
{
    cout<<std::left;
    cout<<"----------------------------------------------------------------------"<<endl;
    cout<<"Not a valid syntax!"<<endl;
    cout<<"----------------------------------------------------------------------"<<endl;
    cout<<"Correct Syntax for NTuple Reduce"<<endl;
    cout<<"\t"<<argv[0]<<" "<<runOption_Reduce<<" "<<typeOption_mc<<endl;
    cout<<"\t"<<argv[0]<<" "<<runOption_Reduce<<" "<<typeOption_data<<"\n"<<endl;
    cout<<"Correct Syntax for NTuple Analysis"<<endl;
    cout<<"\t"<<argv[0]<<" "<<runOption_Run<<" "<<typeOption_mc<<endl;
    cout<<"\t"<<argv[0]<<" "<<runOption_Run<<" "<<typeOption_data<<"\n"<<endl;
    cout<<"Correct Syntax for Calculating Cross Section"<<endl;
    cout<<"\t"<<argv[0]<<" "<<runOption_CrossSection<<"\n"<<endl;
    cout<<"Correct Syntax for Plotting"<<endl;
    cout<<"\t"<<argv[0]<<" "<<runOption_Plot<<"\n"<<endl;
    cout<<"----------------------------------------------------------------------"<<endl;
}



