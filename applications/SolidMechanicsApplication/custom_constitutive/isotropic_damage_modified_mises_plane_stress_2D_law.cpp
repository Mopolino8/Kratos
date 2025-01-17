//
//   Project Name:        KratosSolidMechanicsApplication $
//   Created by:          $Author:              IPouplana $
//   Last modified by:    $Co-Author:                     $
//   Date:                $Date:                July 2015 $
//   Revision:            $Revision:                  0.0 $
//
//

// System includes
#include <iostream>

// Project includes
#include "includes/properties.h"
#include "custom_constitutive/custom_flow_rules/isotropic_damage_flow_rule.hpp"
#include "custom_constitutive/custom_yield_criteria/modified_mises_yield_criterion.hpp"
#include "custom_constitutive/custom_hardening_laws/modified_exponential_damage_hardening_law.hpp"
#include "custom_constitutive/isotropic_damage_modified_mises_plane_stress_2D_law.hpp"

#include "solid_mechanics_application_variables.h"

namespace Kratos
{

//******************************CONSTRUCTOR*******************************************
//************************************************************************************

IsotropicDamageModifiedMisesPlaneStress2DLaw::IsotropicDamageModifiedMisesPlaneStress2DLaw()
    : LinearElasticPlasticPlaneStress2DLaw()
{
  mpHardeningLaw   = HardeningLaw::Pointer( new ModifiedExponentialDamageHardeningLaw() );
  mpYieldCriterion = YieldCriterion::Pointer( new ModifiedMisesYieldCriterion(mpHardeningLaw) );
  mpFlowRule       = FlowRule::Pointer( new IsotropicDamageFlowRule(mpYieldCriterion) );
}


//******************************CONSTRUCTOR*******************************************
//************************************************************************************

IsotropicDamageModifiedMisesPlaneStress2DLaw::IsotropicDamageModifiedMisesPlaneStress2DLaw(FlowRulePointer pFlowRule, YieldCriterionPointer pYieldCriterion, HardeningLawPointer pHardeningLaw)
    : LinearElasticPlasticPlaneStress2DLaw(pFlowRule, pYieldCriterion, pHardeningLaw)
{

}

//******************************COPY CONSTRUCTOR**************************************
//************************************************************************************

IsotropicDamageModifiedMisesPlaneStress2DLaw::IsotropicDamageModifiedMisesPlaneStress2DLaw(const IsotropicDamageModifiedMisesPlaneStress2DLaw& rOther)
    : LinearElasticPlasticPlaneStress2DLaw(rOther)
{

}

//********************************CLONE***********************************************
//************************************************************************************

ConstitutiveLaw::Pointer IsotropicDamageModifiedMisesPlaneStress2DLaw::Clone() const
{
    IsotropicDamageModifiedMisesPlaneStress2DLaw::Pointer p_clone(new IsotropicDamageModifiedMisesPlaneStress2DLaw(*this));
    return p_clone;
}

//*******************************DESTRUCTOR*******************************************
//************************************************************************************

IsotropicDamageModifiedMisesPlaneStress2DLaw::~IsotropicDamageModifiedMisesPlaneStress2DLaw()
{
}

//************* COMPUTING  METHODS
//************************************************************************************
//************************************************************************************

//******************CHECK CONSISTENCY IN THE CONSTITUTIVE LAW*************************
//************************************************************************************

int IsotropicDamageModifiedMisesPlaneStress2DLaw::Check(const Properties& rMaterialProperties,
                                                const GeometryType& rElementGeometry,
                                                const ProcessInfo& rCurrentProcessInfo)
{
    int ierr = HyperElasticPlastic3DLaw::Check(rMaterialProperties,rElementGeometry,rCurrentProcessInfo);
    if(ierr != 0) return ierr;

    if(DAMAGE_THRESHOLD.Key() == 0 || rMaterialProperties.Has( DAMAGE_THRESHOLD ) == false || rMaterialProperties[DAMAGE_THRESHOLD] <= 0.0)
        KRATOS_THROW_ERROR( std::invalid_argument,"DAMAGE_THRESHOLD has Key zero, is not defined or has an invalid value for property", rMaterialProperties.Id() )
    if(STRENGTH_RATIO.Key() == 0 || rMaterialProperties.Has( STRENGTH_RATIO ) == false || rMaterialProperties[STRENGTH_RATIO] <= 0.0)
        KRATOS_THROW_ERROR( std::invalid_argument,"STRENGTH_RATIO has Key zero, is not defined or has an invalid value for property", rMaterialProperties.Id() )
    if(RESIDUAL_STRENGTH.Key() == 0 || rMaterialProperties.Has( RESIDUAL_STRENGTH ) == false || rMaterialProperties[RESIDUAL_STRENGTH] < 0.0)
        KRATOS_THROW_ERROR( std::invalid_argument,"RESIDUAL_STRENGTH has Key zero, is not defined or has an invalid value for property", rMaterialProperties.Id() )
    if(SOFTENING_SLOPE.Key() == 0 || rMaterialProperties.Has( SOFTENING_SLOPE ) == false || rMaterialProperties[SOFTENING_SLOPE] < 0.0)
        KRATOS_THROW_ERROR( std::invalid_argument,"SOFTENING_SLOPE has Key zero, is not defined or has an invalid value for property", rMaterialProperties.Id() )
        
    return ierr;
}

} // Namespace Kratos
