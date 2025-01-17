//   
//   Project Name:           
//   Last modified by:    $Author:  $
//   Date:                $Date:  $
//   Revision:            $Revision: $
//

// System includes 
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

// Project includes
#include "includes/define.h"
#include "includes/model_part.h"
#include "processes/process.h"
#include "custom_python/add_custom_processes_to_python.h"
#include "spaces/ublas_space.h"
#include "includes/kratos_parameters.h"

// Processes
#include "custom_processes/bofang_condition_temperature_process.hpp"
#include "custom_processes/dam_hydro_condition_load_process.hpp"
#include "custom_processes/dam_uplift_condition_load_process.hpp"
#include "custom_processes/dam_uplift_circular_condition_load_process.hpp"
#include "custom_processes/dam_westergaard_condition_load_process.hpp"
#include "custom_processes/dam_nodal_young_modulus_process.hpp"
#include "custom_processes/dam_construction_process.hpp"



namespace Kratos
{
	
namespace Python
{

using namespace boost::python;

void  AddCustomProcessesToPython() 
{    
    // Bofang Process
    class_< BofangConditionTemperatureProcess, bases< Process >, boost::noncopyable > ( "BofangConditionTemperatureProcess",
        init < ModelPart&, Parameters>());
        
    // Hydrostatic condition
    class_< DamHydroConditionLoadProcess, bases< Process >, boost::noncopyable > ( "DamHydroConditionLoadProcess",
        init < ModelPart&, Parameters>());
        
    // Uplift Condition
    class_< DamUpliftConditionLoadProcess, bases< Process >, boost::noncopyable > ( "DamUpliftConditionLoadProcess",
        init < ModelPart&, Parameters>());
    
    // Uplift Condition for arch dams   
    class_< DamUpliftCircularConditionLoadProcess, bases< Process >, boost::noncopyable > ( "DamUpliftCircularConditionLoadProcess",
        init < ModelPart&, Parameters>());
   
   // Westergaard Condition (for hydrostatic + hydrodynamic pressure)     
    class_< DamWestergaardConditionLoadProcess, bases< Process >, boost::noncopyable > ( "DamWestergaardConditionLoadProcess",
        init < ModelPart&, Parameters>());

    // Nodal Young Modulus Process     
    class_< DamNodalYoungModulusProcess, bases< Process >, boost::noncopyable > ( "DamNodalYoungModulusProcess",
        init < ModelPart&, Parameters>());

    // Construction Process     
    class_< DamConstructionProcess, bases< Process >, boost::noncopyable > ( "DamConstructionProcess",
        init < ModelPart&, Parameters>());


}

}  // namespace Python.
} // Namespace Kratos

