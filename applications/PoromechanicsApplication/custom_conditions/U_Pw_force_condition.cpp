//   
//   Project Name:        KratosPoromechanicsApplication $
//   Last Modified by:    $Author:    Ignasi de Pouplana $
//   Date:                $Date:           February 2016 $
//   Revision:            $Revision:                 1.0 $
//

// Application includes
#include "custom_conditions/U_Pw_force_condition.hpp"

namespace Kratos
{

template< unsigned int TDim, unsigned int TNumNodes >
Condition::Pointer UPwForceCondition<TDim,TNumNodes>::Create(IndexType NewId,NodesArrayType const& ThisNodes,PropertiesType::Pointer pProperties) const
{
    return Condition::Pointer(new UPwForceCondition(NewId, this->GetGeometry().Create(ThisNodes), pProperties));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

template< unsigned int TDim, unsigned int TNumNodes >
void UPwForceCondition<TDim,TNumNodes>::CalculateRHS( VectorType& rRightHandSideVector, const ProcessInfo& CurrentProcessInfo )
{    
    array_1d<double,3> ForceVector = this->GetGeometry()[0].FastGetSolutionStepValue( FORCE );
        
    for(unsigned int i = 0; i < TDim; i++)
    {
        rRightHandSideVector[i] = ForceVector[i];
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

template class UPwForceCondition<2,1>;
template class UPwForceCondition<3,1>;

} // Namespace Kratos.
