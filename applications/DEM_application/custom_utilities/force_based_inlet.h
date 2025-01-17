// Author: Guillermo Casas (gcasas@cimne.upc.edu)

#if !defined(DEM_FORCE_BASED_INLET_H)
#define DEM_FORCE_BASED_INLET_H

// Project includes
#include "inlet.h"

namespace Kratos {

    class KRATOS_API(DEM_APPLICATION) DEM_Force_Based_Inlet: public DEM_Inlet
    {
    public:
        typedef WeakPointerVector<Element >::iterator ParticleWeakIteratorType;
        typedef WeakPointerVector<Element> ParticleWeakVectorType;
        typedef ModelPart::ElementsContainerType ElementsArrayType;
              
        /// Constructor:               
        DEM_Force_Based_Inlet(ModelPart& inlet_modelpart, array_1d<double, 3> injection_force);

        /// Destructor.
        virtual ~DEM_Force_Based_Inlet(){}

    private:
        array_1d<double, 3> mInjectionForce;
        void FixInjectionConditions(Element* p_element) override;
        void FixInjectorConditions(Element* p_element) override;
        void RemoveInjectionConditions(Element &element) override;
        virtual array_1d<double, 3> GetInjectionForce(Element* p_element);
    };
}// namespace Kratos.

#endif // DEM_FORCE_BASED_INLET_H defined
