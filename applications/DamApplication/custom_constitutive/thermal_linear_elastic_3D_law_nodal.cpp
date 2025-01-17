//
//   Project Name:   
//   Last modified by:    $Author:     
//   Date:                $Date:     
//   Revision:            $Revision:     
//

/* Project includes */
#include "custom_constitutive/thermal_linear_elastic_3D_law_nodal.hpp"


namespace Kratos
{

//Default Constructor
ThermalLinearElastic3DLawNodal::ThermalLinearElastic3DLawNodal() : LinearElastic3DLaw() {}

//----------------------------------------------------------------------------------------

//Copy Constructor
ThermalLinearElastic3DLawNodal::ThermalLinearElastic3DLawNodal(const ThermalLinearElastic3DLawNodal& rOther) : LinearElastic3DLaw(rOther) {}

//----------------------------------------------------------------------------------------

//Destructor
ThermalLinearElastic3DLawNodal::~ThermalLinearElastic3DLawNodal() {}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

ConstitutiveLaw::Pointer ThermalLinearElastic3DLawNodal::Clone() const
{
    ThermalLinearElastic3DLawNodal::Pointer p_clone(new ThermalLinearElastic3DLawNodal(*this));
    return p_clone;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void ThermalLinearElastic3DLawNodal::CalculateMaterialResponseKirchhoff (Parameters& rValues)
{
    KRATOS_TRY
    
    const Properties& MaterialProperties  = rValues.GetMaterialProperties();
    const ProcessInfo& CurrentProcessInfo = rValues.GetProcessInfo();
    Flags& Options = rValues.GetOptions();
    
    Vector& StrainVector = rValues.GetStrainVector();
    Vector& StressVector = rValues.GetStressVector();
    Matrix& ConstitutiveMatrix = rValues.GetConstitutiveMatrix();

    //0.- Initialize parameters
    MaterialResponseVariables ElasticVariables;

	ElasticVariables.SetShapeFunctionsValues(rValues.GetShapeFunctionsValues());
	ElasticVariables.SetElementGeometry(rValues.GetElementGeometry());

    //1.- Lame constants
    double YoungModulus;
    this->CalculateNodalYoungModulus( ElasticVariables, YoungModulus);
    const double& PoissonCoefficient = MaterialProperties[POISSON_RATIO];

    //Used for thermal strain in plane strain case
    ElasticVariables.LameMu = 1.0+PoissonCoefficient;

    //2.- Thermal constants    
    ElasticVariables.ThermalExpansionCoefficient = MaterialProperties[THERMAL_EXPANSION]; 
    ElasticVariables.ReferenceTemperature = CurrentProcessInfo[REFERENCE_TEMPERATURE];

    if(Options.Is( ConstitutiveLaw::COMPUTE_CONSTITUTIVE_TENSOR ))
    {
		this->CalculateLinearElasticMatrix( ConstitutiveMatrix, YoungModulus, PoissonCoefficient );
		
		if( Options.Is( ConstitutiveLaw::COMPUTE_STRESS ) ) //TOTAL STRESS
		{
			double Temperature;
			this->CalculateDomainTemperature( ElasticVariables, Temperature);

			Vector ThermalStrainVector;
			this->CalculateThermalStrain(ThermalStrainVector,ElasticVariables,Temperature);

			StressVector = prod(ConstitutiveMatrix,(StrainVector - ThermalStrainVector));
		}
	}
	else if( Options.Is( ConstitutiveLaw::COMPUTE_STRESS ) ) //TOTAL STRESS
    {        
        this->CalculateLinearElasticMatrix( ConstitutiveMatrix, YoungModulus, PoissonCoefficient );

        double Temperature;
        this->CalculateDomainTemperature( ElasticVariables, Temperature);

        Vector ThermalStrainVector;
        this->CalculateThermalStrain(ThermalStrainVector,ElasticVariables,Temperature);

        StressVector = prod(ConstitutiveMatrix,(StrainVector - ThermalStrainVector));
    }
    else if( Options.Is( ConstitutiveLaw::TOTAL_TENSOR ) ) //This should be COMPUTE_MECHANICAL_STRESS
    {        
        this->CalculateLinearElasticMatrix( ConstitutiveMatrix, YoungModulus, PoissonCoefficient );

        StressVector = prod(ConstitutiveMatrix,StrainVector);
    }
    else if( Options.Is( ConstitutiveLaw::VOLUMETRIC_TENSOR_ONLY ) ) //This should be COMPUTE_THERMAL_STRESS
    {
        this->CalculateLinearElasticMatrix( ConstitutiveMatrix, YoungModulus, PoissonCoefficient );

        double Temperature;
        this->CalculateDomainTemperature( ElasticVariables, Temperature);

        this->CalculateThermalStrain(StrainVector,ElasticVariables,Temperature);

        StressVector = prod(ConstitutiveMatrix,StrainVector);
    }
    else if(Options.Is( ConstitutiveLaw::COMPUTE_STRAIN ) ) //This should be COMPUTE_THERMAL_STRAIN
    {
        double Temperature;
        this->CalculateDomainTemperature( ElasticVariables, Temperature);
        
        this->CalculateThermalStrain(StrainVector,ElasticVariables,Temperature);
    }
    
    KRATOS_CATCH( "" )
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

double&  ThermalLinearElastic3DLawNodal::CalculateNodalYoungModulus (const MaterialResponseVariables & rElasticVariables, double & rYoungModulus)
{
    KRATOS_TRY
    
    //1.-Young Modulus from nodes 
    const GeometryType& DomainGeometry = rElasticVariables.GetElementGeometry();
    const Vector& ShapeFunctionsValues = rElasticVariables.GetShapeFunctionsValues();
    const unsigned int number_of_nodes = DomainGeometry.size();
    
    rYoungModulus = 0.0;
    
    for ( unsigned int j = 0; j < number_of_nodes; j++ )
    {
      rYoungModulus += ShapeFunctionsValues[j] * DomainGeometry[j].GetSolutionStepValue(NODAL_YOUNG_MODULUS);
    }

    return rYoungModulus;
    
    KRATOS_CATCH( "" )
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

double&  ThermalLinearElastic3DLawNodal::CalculateDomainTemperature (const MaterialResponseVariables & rElasticVariables, double & rTemperature)
{
    KRATOS_TRY
    
    //1.-Temperature from nodes 
    const GeometryType& DomainGeometry = rElasticVariables.GetElementGeometry();
    const Vector& ShapeFunctionsValues = rElasticVariables.GetShapeFunctionsValues();
    const unsigned int number_of_nodes = DomainGeometry.size();
    
    rTemperature = 0.0;
    
    for ( unsigned int j = 0; j < number_of_nodes; j++ )
    {
      rTemperature += ShapeFunctionsValues[j] * DomainGeometry[j].GetSolutionStepValue(TEMPERATURE);
    }

    return rTemperature;
    
    KRATOS_CATCH( "" )
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void ThermalLinearElastic3DLawNodal::CalculateThermalStrain( Vector& rThermalStrainVector, const MaterialResponseVariables& rElasticVariables, double & rTemperature)
{
    KRATOS_TRY
    
    //Identity vector
    rThermalStrainVector.resize(6,false);
    rThermalStrainVector[0] = 1.0;
    rThermalStrainVector[1] = 1.0;
    rThermalStrainVector[2] = 1.0;
    rThermalStrainVector[3] = 0.0;
    rThermalStrainVector[4] = 0.0;
    rThermalStrainVector[5] = 0.0;

    // Delta T
    double DeltaTemperature = rTemperature - rElasticVariables.ReferenceTemperature;

    //Thermal strain vector
    for(unsigned int i = 0; i < 6; i++)
        rThermalStrainVector[i] *= rElasticVariables.ThermalExpansionCoefficient * DeltaTemperature;
    
    KRATOS_CATCH( "" )
}

} // Namespace Kratos
