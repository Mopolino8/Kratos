//
// Authors: 
// Miguel Angel Celigueta maceli@cimne.upc.edu
//

#if !defined(DEM_INLET_H)
#define DEM_INLET_H

// System includes
#include <string>
#include <iostream>

// External includes
// Project includes
//#include "boost/smart_ptr.hpp"

// Project includes
#include "includes/define.h"
#include "includes/node.h"
#include "includes/element.h"
#include "geometries/geometry.h"
#include "includes/properties.h"
#include "includes/process_info.h"
#include "utilities/indexed_object.h"
#include "containers/weak_pointer_vector.h"
#include "includes/constitutive_law.h"
#include "includes/condition.h"
#include "../custom_elements/discrete_element.h"
#include "../custom_utilities/AuxiliaryFunctions.h"
#include "../applications/DEM_application/custom_utilities/properties_proxies.h"
#include "custom_elements/spheric_particle.h"

namespace Kratos {

    class ParticleCreatorDestructor;
    
    class  KRATOS_API(DEM_APPLICATION) DEM_Inlet
    {
    public:

        typedef WeakPointerVector<Element >::iterator ParticleWeakIteratorType;
        typedef WeakPointerVector<Element> ParticleWeakVectorType;
        typedef ModelPart::ElementsContainerType ElementsArrayType;
        
        
        /// Constructor:               
        DEM_Inlet(ModelPart& inlet_modelpart);

        /// Destructor.
        virtual ~DEM_Inlet(){}
        
        virtual void InitializeDEM_Inlet(ModelPart& r_modelpart, ParticleCreatorDestructor& creator, const bool using_strategy_for_continuum = false);
        virtual void InitializeStep(ModelPart&){}
        void DettachElements(ModelPart& r_modelpart, unsigned int& max_Id);
        void DettachClusters(ModelPart& r_clusters_modelpart, unsigned int& max_Id);
        virtual void CreateElementsFromInletMesh(ModelPart& r_modelpart, ModelPart& r_clusters_modelpart, ParticleCreatorDestructor& creator);
        ModelPart& GetInletModelPart();
        void SetNormalizedMaxIndentationForRelease(const double value);
        void SetNormalizedMaxIndentationForNewParticleCreation(const double value);
        int GetNumberOfParticlesInjectedSoFar();
        double GetMassInjectedSoFar();

    private:
        virtual void FixInjectorConditions(Element* p_element);
        virtual void FixInjectionConditions(Element* p_element);
        virtual void RemoveInjectionConditions(Element &element);
        virtual void UpdateThroughput(SphericParticle& r_spheric_particle);

        Vector mPartialParticleToInsert; //array of doubles, must be resized in the constructor to the number of meshes
        Vector mLastInjectionTimes; //array of doubles, must be resized in the constructor to the number of meshes

        bool mFirstTime;
        bool mBallsModelPartHasSphericity;
        bool mBallsModelPartHasRotation;
        bool mStrategyForContinuum;
        int mNumberOfParticlesInjected;
        double mMassInjected;
        // The following two ratios mark the limit indentation (normalized by the radius) for releasing a particle
        // and for allowing a new one to be injected. admissible_indentation_ratio_for_release should be smaller
        // (more strict), since we want to make sure that the particle is taken far enough to avoid interferences
        // with the newly created ones to come. Otherwise, an initial huge indentation could easily happen.
        double mNormalizedMaxIndentationForRelease;
        double mNormalizedMaxIndentationForNewParticleCreation;
        std::vector<PropertiesProxy> mFastProperties;
        boost::numeric::ublas::vector<bool> mLayerRemoved;
        std::vector<int> mTotalNumberOfDetachedParticles;
        ModelPart& mInletModelPart; //The model part used to insert elements
    };
}// namespace Kratos.

#endif // DEM_INLET_H defined
