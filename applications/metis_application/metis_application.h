//
//   Project Name:        Kratos
//   Last Modified by:    $Author: pooyan $
//   Date:                $Date: 2008-04-21 13:33:42 $
//   Revision:            $Revision: 1.1 $
//
//


#if !defined(KRATOS_KRATOS_METIS_APPLICATION_H_INCLUDED )
#define  KRATOS_KRATOS_METIS_APPLICATION_H_INCLUDED



// System includes
#include <string>
#include <iostream>


// External includes


// Project includes
#include "includes/define.h"
#include "includes/kratos_application.h"

#include "includes/variables.h"
#include "includes/condition.h"


namespace Kratos
{

///@name Kratos Globals
///@{

// Variables definition
/*	KRATOS_DEFINE_VARIABLE(double, NODAL_AREA)
	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(NORMAL_TO_WALL)
*/

///@}
///@name Type Definitions
///@{

///@}
///@name  Enum's
///@{

///@}
///@name  Functions
///@{

///@}
///@name Kratos Classes
///@{

/// Short class definition.
/** Detail class definition.
*/
class KratosMetisApplication : public KratosApplication
{
public:
    ///@name Type Definitions
    ///@{


    /// Pointer definition of KratosMetisApplication
    KRATOS_CLASS_POINTER_DEFINITION(KratosMetisApplication);

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor.
    KratosMetisApplication();

    /// Destructor.
    virtual ~KratosMetisApplication() {}


    ///@}
    ///@name Operators
    ///@{


    ///@}
    ///@name Operations
    ///@{

    virtual void Register();



    ///@}
    ///@name Access
    ///@{


    ///@}
    ///@name Inquiry
    ///@{


    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    virtual std::string Info() const
    {
        return "KratosMetisApplication";
    }

    /// Print information about this object.
    virtual void PrintInfo(std::ostream& rOStream) const
    {
        rOStream << Info();
        PrintData(rOStream);
    }

    ///// Print object's data.
    virtual void PrintData(std::ostream& rOStream) const
    {
        KRATOS_WATCH("in KratosMetisApplication");
        KRATOS_WATCH(KratosComponents<VariableData>::GetComponents().size() );
        rOStream << "Variables:" << std::endl;
        KratosComponents<VariableData>().PrintData(rOStream);
        rOStream << std::endl;
        rOStream << "Elements:" << std::endl;
        KratosComponents<Element>().PrintData(rOStream);
        rOStream << std::endl;
        rOStream << "Conditions:" << std::endl;
        KratosComponents<Condition>().PrintData(rOStream);
    }


    ///@}
    ///@name Friends
    ///@{


    ///@}

protected:
    ///@name Protected static Member Variables
    ///@{


    ///@}
    ///@name Protected member Variables
    ///@{


    ///@}
    ///@name Protected Operators
    ///@{


    ///@}
    ///@name Protected Operations
    ///@{


    ///@}
    ///@name Protected  Access
    ///@{


    ///@}
    ///@name Protected Inquiry
    ///@{


    ///@}
    ///@name Protected LifeCycle
    ///@{


    ///@}

private:
    ///@name Static Member Variables
    ///@{


    ///@}
    ///@name Member Variables
    ///@{



    ///@}
    ///@name Private Operators
    ///@{


    ///@}
    ///@name Private Operations
    ///@{


    ///@}
    ///@name Private  Access
    ///@{


    ///@}
    ///@name Private Inquiry
    ///@{


    ///@}
    ///@name Un accessible methods
    ///@{

    /// Assignment operator.
    KratosMetisApplication& operator=(KratosMetisApplication const& rOther);

    /// Copy constructor.
    KratosMetisApplication(KratosMetisApplication const& rOther);


    ///@}

}; // Class KratosMetisApplication

///@}


///@name Type Definitions
///@{


///@}
///@name Input and output
///@{

///@}


}  // namespace Kratos.

#endif // KRATOS_KRATOS_METIS_APPLICATION_H_INCLUDED  defined 


