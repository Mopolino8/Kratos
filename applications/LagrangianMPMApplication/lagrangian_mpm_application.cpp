//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Pooyan Dadvand@{KRATOS_APP_AUTHOR}
//


// System includes


// External includes


// Project includes
#include "lagrangian_mpm_application.h"
#include "lagrangian_mpm_application_variables.h"


namespace Kratos
{

KratosLagrangianMPMApplication::KratosLagrangianMPMApplication():
    mMeshlessBaseElement(0, Element::GeometryType::Pointer(new Geometry<Node<3> >(Element::GeometryType::PointsArrayType(3)))),
    mUpdatedLagrangianMPMElement(0, Element::GeometryType::Pointer(new Geometry<Node<3> >(Element::GeometryType::PointsArrayType(4))))
{}

void KratosLagrangianMPMApplication::Register()
{
    // calling base class register to register Kratos components
    KratosApplication::Register();
    std::cout << "Initializing KratosLagrangianMPMApplication... " << std::endl;
  
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS( GAUSS_POINT_COORDINATES )
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS( MP_VOLUME_ACCELERATION )
    KRATOS_REGISTER_VARIABLE(   GAUSS_AREA )
    KRATOS_REGISTER_VARIABLE(   EFFECTIVE_RADIUS )
    KRATOS_REGISTER_VARIABLE(   SHAPE_FUNCTIONS )
    KRATOS_REGISTER_VARIABLE(   SHAPE_FUNCTIONS_DERIVATIVES )

    KRATOS_REGISTER_ELEMENT("MeshlessBaseElement",mMeshlessBaseElement); //this is the name the element should have according to the naming convention
    KRATOS_REGISTER_ELEMENT("UpdatedLagrangianMPMElement",mUpdatedLagrangianMPMElement); //this is the name the element should have according to the naming convention

}
}  // namespace Kratos.
