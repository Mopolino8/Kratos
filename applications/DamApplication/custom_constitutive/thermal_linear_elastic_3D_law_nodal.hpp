//
//   Project Name:   
//   Last modified by:    $Author:   
//   Date:                $Date:  
//   Revision:            $Revision:  
//

#if !defined (KRATOS_THERMAL_LINEAR_ELASTIC_3D_LAW_NODAL_H_INCLUDED)
#define  KRATOS_THERMAL_LINEAR_ELASTIC_3D_LAW_NODAL_H_INCLUDED

// Project includes
#include "includes/serializer.h"
#include "custom_constitutive/linear_elastic_3D_law.hpp"

#include "dam_application_variables.h"

namespace Kratos
{

class ThermalLinearElastic3DLawNodal : public LinearElastic3DLaw
{

public:

    KRATOS_CLASS_POINTER_DEFINITION(ThermalLinearElastic3DLawNodal);

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // Default Constructor
    ThermalLinearElastic3DLawNodal();

    // Copy Constructor
    ThermalLinearElastic3DLawNodal (const ThermalLinearElastic3DLawNodal& rOther);

    // Destructor
    virtual ~ThermalLinearElastic3DLawNodal();

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
   
    ConstitutiveLaw::Pointer Clone() const;
    
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    /**
     * Computes the material response:
     * Kirchhoff stresses and algorithmic ConstitutiveMatrix
     * @param rValues
     * @see   Parameters
     */
    void CalculateMaterialResponseKirchhoff (Parameters & rValues);
    
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

protected:

    // Member Variables
    
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    double& CalculateNodalYoungModulus ( const MaterialResponseVariables & rElasticVariables, double & rYoungModulus);

    double& CalculateDomainTemperature ( const MaterialResponseVariables & rElasticVariables, double & rTemperature);

    virtual void CalculateThermalStrain( Vector& rThermalStrainVector, const MaterialResponseVariables & rElasticVariables, double & rTemperature);

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

private:
    
    // Serialization
    
    friend class Serializer;

    virtual void save(Serializer& rSerializer) const
    {
        KRATOS_SERIALIZE_SAVE_BASE_CLASS( rSerializer, LinearElastic3DLaw )
    }

    virtual void load(Serializer& rSerializer)
    {
        KRATOS_SERIALIZE_LOAD_BASE_CLASS( rSerializer, LinearElastic3DLaw )
    }

}; // Class ThermalLinearElastic3DLawNodal
}  // namespace Kratos.
#endif // KRATOS_THERMAL_LINEAR_ELASTIC_3D_LAW_NODAL_H_INCLUDED  defined 
