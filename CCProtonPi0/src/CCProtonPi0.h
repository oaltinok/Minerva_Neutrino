/*
================================================================================
CCProtonPi0

    Reconstruction Package:
        Exclusive Channel for muon,proton,pi0 on the final state
        Uses Nightly Build
    
    Main Package:
        AnalysisFramework/Ana/CCProtonPi0/
        
    Setup:
        > getpack -u Ana/CCProtonPi0
        > cmt config
        > cmt make
        
    Usage:
        There is a corresponding Options file under Tools/SystemTests
        Use Tools/ProductionScripts/ana_scripts/ProcessAna.py

    
    Author:         Ozgur Altinok  - ozgur.altinok@tufts.edu
    Date:           2014_03_27
    Last Revision:  2014_05_30
    
================================================================================
*/
#ifndef CCPROTONPI0_H 
#define CCPROTONPI0_H 1

#include <utility>

// ineritance
#include "AnaUtils/MinervaAnalysisTool.h"

//-- Forward Declarations
#include "Event/MinervaEventFwd.h"

class TRandom3;
class IMichelTool;
class IMinervaCoordSysTool;
class IProtonUtils;
class IEnergyCorrectionTool;
class IHitTaggerTool;
class IMinervaMathTool;
class IProngClassificationTool;
class IODProngClassificationTool;
class IParticleMakerTool;
class ICCPionIncUtils;
class IRecoObjectTimeTool;
class IMinervaObjectAssociator;
class ICalorimetryUtils;
class IIDAnchoredBlobCreator;
class IParticleTool;
class IExtraEnergyTool;
class IGetDeadTime;
class IMCTrackTool;
class IGiGaGeomCnvSvc;

class IBlobCreatorUtils;
class IHoughBlob;
class IHoughTool;

namespace Minerva {
  class DeDetector;
  class DeOuterDetector;
}

//! This class is for Reconstruct Pi0 using muon match vertex
class CCProtonPi0 : public MinervaAnalysisTool
{
    private:
        typedef std::vector<Minerva::NeutrinoInt*> NeutrinoVect;
        
    public:
        
        //! Standard constructor
        CCProtonPi0( const std::string& type, const std::string& name, const IInterface* parent );
        
        //! Destructor (mandatory for inheritance)
        virtual ~CCProtonPi0(){};
        
        StatusCode initialize();
        StatusCode finalize();
        
        //! Reconstruct the event (mandatory for inheritance)
        StatusCode reconstructEvent( Minerva::PhysicsEvent* event, Minerva::GenMinInteraction* truthEvent = NULL ) const;
        
        //! Attach an interpretations to the event (mandatory for inheritance)
        StatusCode interpretEvent( const Minerva::PhysicsEvent* event, const Minerva::GenMinInteraction* truthEvent, NeutrinoVect& interaction_hyp ) const;
        
        StatusCode tagTruth( Minerva::GenMinInteraction* truthEvent ) const;
        
    private:
        // Fiducial Volume
        double m_fidHexApothem;
        double m_fidUpStreamZ;
        double m_fidDownStreamZ;
        
        // Analysable Volume
        double m_recoHexApothem;
        double m_recoUpStreamZ;
        double m_recoDownStreamZ;
        
        double m_beamAngleBias;
        
        double m_detectableGammaE;
        double m_detectablePi0KE;
        double m_detectableProtonKE;
        
        double m_minMuonScore;
        double m_minProtonScore;
        
        bool m_store_all_events;
        bool m_makeShortTracks;
        bool m_doPlausibilityCuts;
        
        // Tool Names
        std::string m_particleToolName;
        std::string m_particleToolAlias;
        std::string m_protonUtilsAlias;
        std::string m_michelTrkToolAlias;
        std::string m_michelVtxToolAlias;
        
        // Prong Colors
        int m_muonProngColor; 
        int m_protonProngColor; 
        int m_primaryVertexColor; 
        int m_secondaryVertexColor; 
        int m_endPointVertexColor; 
        int m_unattachedProngColor;
        
        // VtxBlob
        bool 	 m_sphereVertex;
        double  m_maxSearchD;
        double  m_maxStartingD;
        double  m_maxSearchGap;		
        bool	 m_filamentVertex;
        double  m_maxSearchDFila;
        double  m_maxStartingDFila;
        double  m_maxSearchGapFila;
        bool    m_filterClusterTypes;
        bool    fSkipLowEnergyClusterVtxEnergy;
        bool    fThresholdVertexEnergy;
        
        TRandom3*                 m_randomGen;
        unsigned long int         m_randomSeed;
        
        Minerva::DeDetector*        m_InnerDetector;
        Minerva::DeOuterDetector*   m_OuterDetector;
        
        IMinervaCoordSysTool*       m_coordSysTool;
        IMichelTool*                m_michelTrkTool;
        IMichelTool*                m_michelVtxTool;
        IProtonUtils*               m_protonUtils;
        IEnergyCorrectionTool*      m_energyCorrectionTool;
        IHitTaggerTool*             m_hitTagger;
        IProngClassificationTool*   m_prongIntersection;
        IODProngClassificationTool* m_odMatchTool;
        IParticleMakerTool*         m_particleMaker;
        ICCPionIncUtils*            m_ccPionIncUtils;
        IRecoObjectTimeTool*        m_recoTimeTool;
        IMinervaObjectAssociator*   m_objectAssociator;
        ICalorimetryUtils*          m_caloUtils;
        IIDAnchoredBlobCreator*     m_stopPointBlobTool;
        IExtraEnergyTool*           m_extraEnergyTool;
        IGetDeadTime*               m_getDeadTimeTool;
        IMCTrackTool*               m_MCTrackTool;
        IGiGaGeomCnvSvc*            m_gigaCnvSvc;
        IParticleTool*              m_particleTool;
        IMinervaMathTool*           m_mathTool;
        
        IBlobCreatorUtils*          m_blobUtils;
        IHoughBlob*                 m_idHoughBlob;
        IHoughTool*                 m_idHoughTool;


        
        //! Private Functions
        StatusCode interpretFailEvent( Minerva::PhysicsEvent* event ) const;
        StatusCode getNearestPlane(double z, int & module_return, int & plane_return) const;        
        bool createTrackedParticles( Minerva::ProngVect& prongs ) const;
        
        StatusCode setMuonParticleData(   Minerva::NeutrinoInt* nuInt, SmartRef<Minerva::Prong>& muonProng) const;
        
        bool getProtonProng(    Minerva::ProngVect& hadronProngs, 
                                Minerva::ProngVect& protonProngs,
                                Minerva::ParticleVect& protonParticles ) const;
                                                
        void setProtonParticleData( Minerva::NeutrinoInt* nuInt, 
                                    Minerva::ProngVect& protonProngs,
                                    Minerva::ParticleVect& protonParticles, 
                                    double vertexZ ) const;
                                                    
        void correctProtonProngEnergy(  SmartRef<Minerva::Prong>& protonProng, 
                                        double& p_calCorrection, 
                                        double& p_visEnergyCorrection ) const;

        void setTrackProngTruth( Minerva::NeutrinoInt* neutrino, Minerva::ProngVect& prongs ) const;
        
        //! CCPi0 Functions
        StatusCode VtxBlob(Minerva::PhysicsEvent *event, const SmartRef<Minerva::Vertex>& vertex ) const;
        SmartRefVector<Minerva::IDCluster> FilterInSphereClusters(  const SmartRef<Minerva::Vertex>& vertex,
                                                                    const SmartRefVector<Minerva::IDCluster>& clusters,
                                                                    const double sphereRadius,
                                                                    std::vector<double>& radii) const;
                                        
                                        

        
        
  
  
  
};

#endif // CCPROTONPI0_H 
