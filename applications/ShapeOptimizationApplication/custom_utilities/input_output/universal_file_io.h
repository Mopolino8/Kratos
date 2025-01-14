// ==============================================================================
//  KratosShapeOptimizationApplication
//
//  License:         BSD License
//                   license: ShapeOptimizationApplication/license.txt
//
//  Main authors:    Baumgärtner Daniel, https://github.com/dbaumgaertner
//
// ==============================================================================

#ifndef UNIVERSAL_FILE_IO_H
#define UNIVERSAL_FILE_IO_H

// ------------------------------------------------------------------------------
// System includes
// ------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>      // for setprecision

// ------------------------------------------------------------------------------
// Project includes
// ------------------------------------------------------------------------------
#include "../../kratos/includes/define.h"
#include "../../kratos/processes/process.h"
#include "../../kratos/includes/node.h"
#include "../../kratos/includes/element.h"
#include "../../kratos/includes/model_part.h"
#include "../../kratos/includes/kratos_flags.h"
#include "shape_optimization_application.h"

// ==============================================================================

namespace Kratos
{

///@name Kratos Globals
///@{

using namespace std;

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

Follows the universal I-deas file convetion so that results can be imported in SIEMENS NX: 
https://docs.plm.automation.siemens.com/tdoc/nx/10/nx_help/#uid:index_advanced:xid602249:id625716:id625821

*/

class UniversalFileIO
{
public:
    ///@name Type Definitions
    ///@{

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor.
    UniversalFileIO( ModelPart& designSurface, Parameters& optimizationSettings )
        : mrDesignSurface( designSurface ),
          mrOptimizationSettings( optimizationSettings )
    {
        mOutputFilename = createCompleteOutputFilenameWithPath( optimizationSettings );
    }

    /// Destructor.
    virtual ~UniversalFileIO()
    {
    }


    ///@}
    ///@name Operators
    ///@{


    ///@}
    ///@name Operations
    ///@{

    // ==============================================================================
    string createCompleteOutputFilenameWithPath( Parameters& optimizationSettings  )
    {
        string outputDirectory = optimizationSettings["output"]["output_directory"].GetString();
        string outputFilename = outputDirectory + "/" + optimizationSettings["output"]["design_history_filename"].GetString() + ".unv";
        return outputFilename;
    }
    // --------------------------------------------------------------------------
    void initializeLogging()
    {
        writeMeshToResultFile();
    }

    // --------------------------------------------------------------------------
    void writeMeshToResultFile()
    {
        initializeOutputFile();
        writeUnits();
        writeNodes(); 
        writeElements();  
    }

    // --------------------------------------------------------------------------
    void initializeOutputFile()
    {
        ofstream outputFile;
        outputFile.open(mOutputFilename, ios::out | ios::trunc );
        outputFile.close();     
    }    
    // --------------------------------------------------------------------------
    void writeUnits()
    {
        ofstream outputFile;
        outputFile.open(mOutputFilename, ios::out | ios::app );
        outputFile << scientific;
        outputFile << setprecision(15);

        const int dataSetNumberForNodes = 164;
        const int unitCode = 5;
        const int temperatureMode = 2;
        const double unitFactorLenght = 1.0;
        const double unitFactorForce = 1.0;
        const double unitFactorTemperature = 1.0;
        const double temperatureOffset = 273.15;

        outputFile << setw(6) << "-1" << "\n";
        outputFile << setw(6) << dataSetNumberForNodes << "\n";
        outputFile << setw(10) << unitCode << setw(30) << temperatureMode << "\n";
        outputFile << setw(25) << unitFactorLenght << setw(25) <<  unitFactorForce << setw(25) << unitFactorTemperature << "\n";
        outputFile << setw(25) << temperatureOffset << "\n";
        outputFile << setw(6) << "-1" << "\n";

        outputFile.close();     
    }   

    // --------------------------------------------------------------------------
    void writeNodes()
    {
        ofstream outputFile;
        outputFile.open(mOutputFilename, ios::out | ios::app );
        outputFile << scientific;
        outputFile << setprecision(15);

        const int dataSetNumberForNodes = 2411;
        const int exportCoordinateSystemNumber = 0;
        const int displacementCoordinateSystemNumber = 0;
        const int color = 0;

        outputFile << setw(6) << "-1" << "\n";
        outputFile << setw(6) << dataSetNumberForNodes << "\n";
        for (ModelPart::NodeIterator node_i = mrDesignSurface.NodesBegin(); node_i != mrDesignSurface.NodesEnd(); ++node_i)
        {
            int node_label = node_i->Id();
            double x_coordinate = node_i->X();
            double y_coordinate = node_i->Y();
            double z_coordinate = node_i->Z();
            outputFile << setw(10) << node_label << setw(10) << exportCoordinateSystemNumber << setw(10) << displacementCoordinateSystemNumber << setw(10) << color << "\n";
            outputFile << setw(25) << x_coordinate << setw(25) <<  y_coordinate << setw(25) << z_coordinate << "\n";
        }
        outputFile << setw(6) << "-1" << "\n";

        outputFile.close();     
    }

    // --------------------------------------------------------------------------
    void writeElements()
    {
        // All conditions given in the design surface are translated to dummy elements with the same geometry
        ofstream outputFile;
        outputFile.open(mOutputFilename, ios::out | ios::app );

        const int dataSetNumberForElements = 2412;
        const int physicalPropertyTableNumber = 1;
        const int materialPropertyTableNumber = 1;
        const int color = 0;

        outputFile << setw(6) << "-1" << "\n";
        outputFile << setw(6) << dataSetNumberForElements << "\n";
        for (ModelPart::ConditionIterator condition_i = mrDesignSurface.ConditionsBegin(); condition_i != mrDesignSurface.ConditionsEnd(); ++condition_i)
        {
            const int element_label = condition_i->Id();
            ModelPart::ConditionType::GeometryType condition_geometry = condition_i->GetGeometry();

            if( condition_geometry.size()==3 )
            {
                const int feDescriptorId = 41; // Plane Stress Linear Triangle
                const int numberOfNodes = 3; 
                outputFile << setw(10) << element_label;
                outputFile << setw(10) << feDescriptorId;
                outputFile << setw(10) << physicalPropertyTableNumber;
                outputFile << setw(10) << materialPropertyTableNumber ;
                outputFile << setw(10) << color;
                outputFile << setw(10) << numberOfNodes << "\n";
                outputFile << setw(10) << condition_geometry[0].Id(); 
                outputFile << setw(10) << condition_geometry[1].Id(); 
                outputFile << setw(10) << condition_geometry[2].Id() << "\n";                
            }
            else if( condition_geometry.size()==4 )
            {
                const int feDescriptorId = 44; // Plane Stress Linear Quadrilateral
                const int numberOfNodes = 4;
                outputFile << setw(10) << element_label;
                outputFile << setw(10) << feDescriptorId; 
                outputFile << setw(10) << physicalPropertyTableNumber; 
                outputFile << setw(10) << materialPropertyTableNumber;
                outputFile << setw(10) << color;
                outputFile << setw(10) << numberOfNodes << "\n";
                outputFile << setw(10) << condition_geometry[0].Id(); 
                outputFile << setw(10) << condition_geometry[1].Id();
                outputFile << setw(10) << condition_geometry[2].Id();
                outputFile << setw(10) << condition_geometry[3].Id() << "\n";                     
            } 
            else 
                KRATOS_THROW_ERROR(std::runtime_error,"Design surface contains conditions with geometries for which no UNV-output is implemented!","" )
        }
        outputFile << setw(6) << "-1" << "\n";

        outputFile.close();     
    }    

    // --------------------------------------------------------------------------
    void logNodalResults( const int optimizationIteration )
    {
        ofstream outputFile;
        outputFile.open(mOutputFilename, ios::out | ios::app );
        outputFile << scientific;
        outputFile << setprecision(5);

        const int dataSetNumberForResults = 2414;

        const int analysisDataSetLabel = 1;
        const int dataSetLocation = 1; // Data at nodes
        string analysisDataSetName = "KratosShapeOptimization";
        const int expressionNameKey = 1;

        const int modelType = 1; // Structural
        const int analysisType = 4; // Transient analysis
        const int resultType = 95; // Unknown 3DOF Vector  
        const int displacementResultType = 8; // Displacement
        const int dataType = 4; // Double precision floating point
        const int numberOfDataValuesForTheDataComponent = 3;

        const int DesignSetId = 1;
        const int iterationNumber = 0;
        const int solutionSetId = 1;
        const int boundaryCondition = 0;
        const int loadSet = 1;
        const int modeNumber = 0;
        const int timeStepNumber = optimizationIteration;
        const int frequencyNumber = 0;

        const int creationOption = 0;
        const int numberRetained = 0; 

        const double givenTime = 0.0;
        const double frequency = 0.0;
        const double eigenvalue = 0.0;
        const double modalMass = 0.0;
        const double viscousDampingRation = 0.0;
        const double hystereticDampingRatio = 0.0;
        const double realPartEigenvalue = 0.0;
        const double imaginaryPartEigenvalue = 0.0;
        const double realPartOfModalA = 0.0;
        const double realPartOfMass = 0.0;
        const double imaginaryPartOfModalA = 0.0;
        const double imaginaryPartOfMass = 0.0;   

        // Loop over all nodal result variables
        Parameters nodalResults = mrOptimizationSettings["output"]["nodal_results"];
        for(unsigned int entry = 0; entry<nodalResults.size(); entry++)
        {
            string nodalResultName = nodalResults[entry].GetString();

            unsigned int dataCharacteristic = 0; // 0: unknown, 1: Scalar value, 2: 3 DOF global translation vector
            if( KratosComponents<Variable<double>>::Has(nodalResultName))
                dataCharacteristic = 1;
            else if( KratosComponents<Variable< array_1d<double,3>>>::Has(nodalResultName))
                dataCharacteristic = 2;

            outputFile << setw(6) << "-1" << "\n";
            outputFile << setw(6) << dataSetNumberForResults << "\n";

            outputFile << setw(10) << analysisDataSetLabel << "\n";
            outputFile << "LOADCASE_NAME_KEY " << analysisDataSetName << "\n";
            outputFile << setw(10) << dataSetLocation << "\n";
            outputFile << "RESULT_NAME_KEY " << nodalResultName << "\n";
            outputFile << "NONE" << "\n";
            outputFile << "EXPRESSION_NAME_KEY " << expressionNameKey << "\n";
            outputFile << "NONE" << "\n";
            outputFile << "NONE" << "\n";    

            outputFile << setw(10) << modelType; 
            outputFile << setw(10) << analysisType; 
            outputFile << setw(10) << dataCharacteristic;
            if (nodalResultName.compare("SHAPE_CHANGE_ABSOLUTE") == 0)
                outputFile << setw(10) << displacementResultType;
            else
                outputFile << setw(10) << resultType;
            outputFile << setw(10) << dataType;
            outputFile << setw(10) << numberOfDataValuesForTheDataComponent << "\n";

            outputFile << setw(10) << DesignSetId; 
            outputFile << setw(10) << iterationNumber; 
            outputFile << setw(10) << solutionSetId;
            outputFile << setw(10) << boundaryCondition;
            outputFile << setw(10) << loadSet;
            outputFile << setw(10) << modeNumber;
            outputFile << setw(10) << timeStepNumber;
            outputFile << setw(10) << frequencyNumber << "\n";

            outputFile << setw(10) << creationOption;
            outputFile << setw(10) << numberRetained << "\n";

            outputFile << setw(13) << givenTime; 
            outputFile << setw(13) << frequency; 
            outputFile << setw(13) << eigenvalue;
            outputFile << setw(13) << modalMass;
            outputFile << setw(13) << viscousDampingRation;
            outputFile << setw(13) << hystereticDampingRatio << "\n";      

            outputFile << setw(13) << realPartEigenvalue; 
            outputFile << setw(13) << imaginaryPartEigenvalue; 
            outputFile << setw(13) << realPartOfModalA;
            outputFile << setw(13) << realPartOfMass;
            outputFile << setw(13) << imaginaryPartOfModalA;
            outputFile << setw(13) << imaginaryPartOfMass << "\n";    

            for (ModelPart::NodeIterator node_i = mrDesignSurface.NodesBegin(); node_i != mrDesignSurface.NodesEnd(); ++node_i)
            {
                outputFile << setw(10) << node_i->Id() << "\n"; 
                if(dataCharacteristic==1)
                {
                    Variable<double> nodalResultVariable = KratosComponents<Variable<double>>::Get(nodalResultName);
                    double& nodalResult = node_i->FastGetSolutionStepValue(nodalResultVariable);
                    outputFile << setw(13) << nodalResult << "\n"; 
                }
                else if(dataCharacteristic==2)
                {
                    Variable< array_1d<double,3>> nodalResultVariable = KratosComponents<Variable< array_1d<double,3>>>::Get(nodalResultName);
                    array_1d<double,3>& nodalResult = node_i->FastGetSolutionStepValue(nodalResultVariable);
                    outputFile << setw(13) << nodalResult[0];
                    outputFile << setw(13) << nodalResult[1];
                    outputFile << setw(13) << nodalResult[2] << "\n"; 
                }
            }
            outputFile << setw(6) << "-1" << "\n";
        }

        outputFile.close(); 
    }  

    // ==============================================================================

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
    virtual string Info() const
    {
        return "UniversalFileIO";
    }

    /// Print information about this object.
    virtual void PrintInfo(ostream& rOStream) const
    {
        rOStream << "UniversalFileIO";
    }

    /// Print object's data.
    virtual void PrintData(ostream& rOStream) const
    {
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

    // ==============================================================================
    // Initialized by class constructor
    // ==============================================================================
    ModelPart& mrDesignSurface;
    Parameters& mrOptimizationSettings;
    string mOutputFilename;

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
//      UniversalFileIO& operator=(UniversalFileIO const& rOther);

    /// Copy constructor.
//      UniversalFileIO(UniversalFileIO const& rOther);


    ///@}

}; // Class UniversalFileIO

///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{

///@}


}  // namespace Kratos.

#endif // UNIVERSAL_FILE_IO_H
