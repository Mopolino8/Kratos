// =============================================================================
/*
 KratosALEApllication
 A library based on:
 Kratos
 A General Purpose Software for Multi-Physics Finite Element Analysis
 (Released on march 05, 2007).

 Copyright (c) 2016: Pooyan Dadvand, Riccardo Rossi, Andreas Winterstein
                     pooyan@cimne.upc.edu
                     rrossi@cimne.upc.edu
                     a.winterstein@tum.de
- CIMNE (International Center for Numerical Methods in Engineering),
  Gran Capita' s/n, 08034 Barcelona, Spain
- Chair of Structural Analysis, Technical University of Munich
  Arcisstrasse 21 80333 Munich, Germany

 Permission is hereby granted, free  of charge, to any person obtaining
 a  copy  of this  software  and  associated  documentation files  (the
 "Software"), to  deal in  the Software without  restriction, including
 without limitation  the rights to  use, copy, modify,  merge, publish,
 distribute,  sublicense and/or  sell copies  of the  Software,  and to
 permit persons to whom the Software  is furnished to do so, subject to
 the following condition:

 Distribution of this code for  any  commercial purpose  is permissible
 ONLY BY DIRECT ARRANGEMENT WITH THE COPYRIGHT OWNERS.

 The  above  copyright  notice  and  this permission  notice  shall  be
 included in all copies or substantial portions of the Software.

 THE  SOFTWARE IS  PROVIDED  "AS  IS", WITHOUT  WARRANTY  OF ANY  KIND,
 EXPRESS OR  IMPLIED, INCLUDING  BUT NOT LIMITED  TO THE  WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT  SHALL THE AUTHORS OR COPYRIGHT HOLDERS  BE LIABLE FOR ANY
 CLAIM, DAMAGES OR  OTHER LIABILITY, WHETHER IN AN  ACTION OF CONTRACT,
 TORT  OR OTHERWISE, ARISING  FROM, OUT  OF OR  IN CONNECTION  WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//==============================================================================

/* ****************************************************************************
 *  Projectname:         $KratosALEApplication
 *  Last Modified by:    $Author: A.Winterstein@tum.de $
 *  Date:                $Date: June 2016 $
 *  Revision:            $Revision: 1.5 $
 * ***************************************************************************/

// System includes

#if defined(KRATOS_PYTHON)
// External includes
#include <boost/python.hpp>

// Project includes
#include "includes/define.h"
#include "ale_application.h"
#include "custom_python/add_custom_strategies_to_python.h"
#include "custom_python/add_custom_utilities_to_python.h"


namespace Kratos
{

namespace Python
{

using namespace boost::python;



BOOST_PYTHON_MODULE(KratosALEApplication)
{

    class_<KratosALEApplication,
           KratosALEApplication::Pointer,
           bases<KratosApplication>, boost::noncopyable >("KratosALEApplication")
           ;

    AddCustomStrategiesToPython();
    AddCustomUtilitiesToPython();

    // variables moved to core
    // MESH_VELOCITY currently put to the core since used in other applications
    // KRATOS_REGISTER_IN_PYTHON_VARIABLE(MESH_VELOCITY)
    // KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(MESH_DISPLACEMENT);
    // KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(MESH_REACTION);
    // KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS(MESH_RHS);

}

}  // namespace Python.

}  // namespace Kratos.

#endif // KRATOS_PYTHON defined
