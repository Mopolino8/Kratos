//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Philipp Bucher, Jordi Cotela
//
// See Master-Thesis P.Bucher
// "Development and Implementation of a Parallel
//  Framework for Non-Matching Grid Mapping"

#if !defined(KRATOS_INTERFACE_GEOMETRY_OBJECT_INCLUDED_H_INCLUDED )
#define  KRATOS_INTERFACE_GEOMETRY_OBJECT_INCLUDED_H_INCLUDED

// System includes

// External includes

// Project includes
#include "interface_object.h"


namespace Kratos
{
///@addtogroup ApplicationNameApplication
///@{

///@name Kratos Globals
///@{

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
class InterfaceGeometryObject : public InterfaceObject
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of InterfaceGeometryObject
    KRATOS_CLASS_POINTER_DEFINITION(InterfaceGeometryObject);

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor.
    InterfaceGeometryObject(Geometry<Node<3>>& rGeometry, const double ApproximationTolerance, const int ConstructionIndex,
                            GeometryData::IntegrationMethod IntegrationMethod = GeometryData::NumberOfIntegrationMethods) :
        mpGeometry(&rGeometry),
        mApproximationTolerance(ApproximationTolerance),
        mConstructionIndex(ConstructionIndex),
        mIntegrationMethod(IntegrationMethod)
    {
        SetCoordinates();
        mGeometryFamily = mpGeometry->GetGeometryFamily();
        mNumPoints = mpGeometry->PointsNumber();
        KRATOS_ERROR_IF(mNumPoints == 0) << "Number of Points cannot be zero" << std::endl;
        mpPoint = &(mpGeometry->GetPoint(0));
    }

    /// Destructor.
    virtual ~InterfaceGeometryObject() { }


    ///@}
    ///@name Operators
    ///@{


    ///@}
    ///@name Operations
    ///@{

    bool EvaluateResult(const array_1d<double, 3>& rGlobalCoords,
                        double& rMinDistance, const double Distance,
                        std::vector<double>& rShapeFunctionValues) override   // I am an object in the bins
    {
        // Distance is the distance to the center and not the projection distance, therefore it is unused
        bool is_closer = false;
        bool is_inside = false;
        double projection_distance;
        array_1d<double, 3> projection_local_coords;

        if (mGeometryFamily == GeometryData::Kratos_Linear
                && mNumPoints == 2)   // I am a linear line condition
        {
            is_inside = MapperUtilities::ProjectPointToLine(mpGeometry, rGlobalCoords,
                        projection_local_coords,
                        projection_distance);
        }
        else if (mGeometryFamily == GeometryData::Kratos_Triangle
                 && mNumPoints == 3)   // I am a linear triangular condition
        {
            is_inside = MapperUtilities::ProjectPointToTriangle(mpGeometry, rGlobalCoords,
                        projection_local_coords,
                        projection_distance);
        }
        else if (mGeometryFamily == GeometryData::Kratos_Quadrilateral
                 && mNumPoints == 4)   // I am a linear quadrilateral condition
        {
            is_inside = MapperUtilities::ProjectPointToQuadrilateral(mpGeometry, rGlobalCoords,
                        projection_local_coords,
                        projection_distance);
        }
        else if (mGeometryFamily == GeometryData::Kratos_Tetrahedra ||
                 mGeometryFamily == GeometryData::Kratos_Prism ||
                 mGeometryFamily == GeometryData::Kratos_Hexahedra)   // Volume Mapping
        {
            is_inside = MapperUtilities::PointLocalCoordinatesInVolume(mpGeometry, rGlobalCoords,
                        projection_local_coords,
                        projection_distance);
        }
        else
        {
            std::cout << "MAPPER WARNING, Unsupported geometry, "
                      << "using an approximation" << std::endl;
            return false;
        }

        projection_distance = fabs(projection_distance);

        if (is_inside)
        {
            if (projection_distance < rMinDistance)
            {
                rMinDistance = projection_distance;
                rShapeFunctionValues.resize(mNumPoints);
                for (int i = 0; i < mNumPoints; ++i)
                {
                    rShapeFunctionValues[i] = mpGeometry->ShapeFunctionValue(i, projection_local_coords);
                }
                is_closer = true;
            }
        }
        return is_closer;
    }

    bool ComputeApproximation(const array_1d<double, 3>& rGlobalCoords, double& rMinDistance,
                              std::vector<double>& rShapeFunctionValues) override   // I am an object in the bins
    {
        bool is_closer = false;
        double distance_point = std::numeric_limits<double>::max();
        int closest_point_index = -1;
        // Loop over all points of the geometry and check which one is the closest
        for (int i = 0; i < mNumPoints; ++i)
        {
            distance_point = MapperUtilities::ComputeDistance(rGlobalCoords,
                             mpGeometry->GetPoint(i).Coordinates());

            if (distance_point < rMinDistance && distance_point <= mApproximationTolerance)
            {
                rMinDistance = distance_point;
                closest_point_index = i;
                is_closer = true;
            }
        }

        if (is_closer)
        {
            // Use the value of the closest point by setting its corresponding sf-value to 1
            rShapeFunctionValues.resize(mNumPoints);
            for (int i = 0; i < mNumPoints; ++i)
            {
                if (i == closest_point_index)
                {
                    rShapeFunctionValues[i] = 1.0f;
                }
                else
                {
                    rShapeFunctionValues[i] = 0.0f;
                }
            }
        }

        return is_closer;
    }


    // Scalars
    double GetObjectValue(const Variable<double>& rVariable,
                          const Kratos::Flags& rOptions) override
    {
        KRATOS_ERROR_IF_NOT(rOptions.Is(MapperFlags::NON_HISTORICAL_DATA))
                << "Only Non-Historical Variables are accessible for Conditions" << std::endl;

        return mpPoint->GetValue(rVariable);
    }

    void SetObjectValue(const Variable<double>& rVariable,
                        const double& rValue,
                        const Kratos::Flags& rOptions,
                        const double Factor) override
    {
        KRATOS_ERROR_IF_NOT(rOptions.Is(MapperFlags::NON_HISTORICAL_DATA))
                << "Only Non-Historical Variables are accessible for Conditions" << std::endl;

        if (rOptions.Is(MapperFlags::ADD_VALUES))
        {
            double old_value = mpPoint->GetValue(rVariable);
            mpPoint->SetValue(rVariable, old_value + rValue * Factor);
        }
        else
        {
            mpPoint->SetValue(rVariable, rValue * Factor);
        }
    }

    double GetObjectValueInterpolated(const Variable<double>& rVariable,
                                      const std::vector<double>& rShapeFunctionValues) override
    {
        double interpolated_value = 0.0f;

        for (int i = 0; i < mNumPoints; ++i)
        {
            interpolated_value += mpGeometry->GetPoint(i).FastGetSolutionStepValue(rVariable) * rShapeFunctionValues[i];
        }
        return interpolated_value;
    }

    // Vectors
    array_1d<double, 3> GetObjectValue(const Variable< array_1d<double, 3> >& rVariable,
                                       const Kratos::Flags& rOptions) override
    {
        KRATOS_ERROR_IF_NOT(rOptions.Is(MapperFlags::NON_HISTORICAL_DATA))
                << "Only Non-Historical Variables are accessible for Conditions" << std::endl;

        return mpPoint->GetValue(rVariable);
    }

    void SetObjectValue(const Variable< array_1d<double, 3> >& rVariable,
                        const array_1d<double, 3>& rValue,
                        const Kratos::Flags& rOptions,
                        const double Factor) override
    {
        KRATOS_ERROR_IF_NOT(rOptions.Is(MapperFlags::NON_HISTORICAL_DATA))
                << "Only Non-Historical Variables are accessible for Conditions" << std::endl;

        if (rOptions.Is(MapperFlags::ADD_VALUES))
        {
            array_1d<double, 3> old_value = mpPoint->GetValue(rVariable);
            mpPoint->SetValue(rVariable, old_value + rValue * Factor);
        }
        else
        {
            mpPoint->SetValue(rVariable, rValue * Factor);
        }
    }

    array_1d<double, 3> GetObjectValueInterpolated(const Variable< array_1d<double, 3> >& rVariable,
            const std::vector<double>& rShapeFunctionValues) override
    {
        array_1d<double, 3> interpolated_value;
        interpolated_value[0] = 0.0f;
        interpolated_value[1] = 0.0f;
        interpolated_value[2] = 0.0f;
        for (int i = 0; i < mNumPoints; ++i)
        {
            interpolated_value[0] += mpGeometry->GetPoint(i).FastGetSolutionStepValue(rVariable)[0] * rShapeFunctionValues[i];
            interpolated_value[1] += mpGeometry->GetPoint(i).FastGetSolutionStepValue(rVariable)[1] * rShapeFunctionValues[i];
            interpolated_value[2] += mpGeometry->GetPoint(i).FastGetSolutionStepValue(rVariable)[2] * rShapeFunctionValues[i];
        }
        return interpolated_value;
    }

    // Functions used for Debugging
    void PrintNeighbors(const int CommRank) override
    {
        array_1d<double, 3> neighbor_coordinates = mpPoint->GetValue(NEIGHBOR_COORDINATES);
        double neighbor_comm_rank = mpPoint->GetValue(NEIGHBOR_RANK);

        PrintMatchInfo("InterfaceGeometryObject", CommRank,
                       neighbor_comm_rank, neighbor_coordinates);
    }

    void WriteRankAndCoordinatesToVariable(const int CommRank) override
    {
        // This function writes the coordinates and the rank of the
        // InterfaceObject to the variables "NEIGHBOR_COORDINATES"
        // and "NEIGHBOR_RANK", for debugging
        array_1d<double, 3> neighbor_coordinates;
        // TODO exchange with "Coordinates()"
        neighbor_coordinates[0] = this->X();
        neighbor_coordinates[1] = this->Y();
        neighbor_coordinates[2] = this->Z();
        mpPoint->SetValue(NEIGHBOR_COORDINATES, neighbor_coordinates);
        mpPoint->SetValue(NEIGHBOR_RANK, CommRank);
    }

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
        std::stringstream buffer;
        buffer << "InterfaceGeometryObject" ;
        return buffer.str();
    }

    /// Print information about this object.
    virtual void PrintInfo(std::ostream& rOStream) const
    {
        rOStream << "InterfaceGeometryObject";
    }

    /// Print object's data.
    virtual void PrintData(std::ostream& rOStream) const {}


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

    Geometry<Node<3>>* mpGeometry;
    Node<3>* mpPoint;
    GeometryData::KratosGeometryFamily mGeometryFamily;
    int mNumPoints;
    double mApproximationTolerance = 0.0f;
    int mConstructionIndex;
    GeometryData::IntegrationMethod mIntegrationMethod;


    ///@}
    ///@name Private Operators
    ///@{


    ///@}
    ///@name Private Operations
    ///@{

    void SetCoordinates() override
    {
        if (mConstructionIndex == 0)
        {
            this->Coordinates() = mpGeometry->Center();
        }
        else
        {
            Matrix shape_functions = mpGeometry->ShapeFunctionsValues(mIntegrationMethod); // TODO "ShapeFunctionsValues" seems to not be implemented for every geometry!!!

            // const int num_gauss_points = shape_functions.size1();
            const int num_nodes = shape_functions.size2();

            array_1d<double, 3> gauss_point_global_coords;

            gauss_point_global_coords[0] = 0.0f;
            gauss_point_global_coords[1] = 0.0f;
            gauss_point_global_coords[2] = 0.0f;

            // TODO change to GlobalCoordinates()?
            // here mConstructionIndex is the number of the GP to use for this Object
            for (int n = 0; n < num_nodes; ++n)
            {
                gauss_point_global_coords[0] += shape_functions(mConstructionIndex - 1, n) * mpGeometry->GetPoint(n).X();
                gauss_point_global_coords[1] += shape_functions(mConstructionIndex - 1, n) * mpGeometry->GetPoint(n).Y();
                gauss_point_global_coords[2] += shape_functions(mConstructionIndex - 1, n) * mpGeometry->GetPoint(n).Z();
            }
            // TODO check again if this is whole computation of the GPs is correct

            this->Coordinates() = gauss_point_global_coords;
        }
    }


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
    InterfaceGeometryObject& operator=(InterfaceGeometryObject const& rOther);

    //   /// Copy constructor.
    //   InterfaceGeometryObject(InterfaceGeometryObject const& rOther){}


    ///@}

}; // Class InterfaceGeometryObject

///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{


/// input stream function
inline std::istream& operator >> (std::istream& rIStream,
                                  InterfaceGeometryObject& rThis)
{
    return rIStream;
}

/// output stream function
inline std::ostream& operator << (std::ostream& rOStream,
                                  const InterfaceGeometryObject& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}
///@}

///@} addtogroup block

}  // namespace Kratos.

#endif // KRATOS_INTERFACE_GEOMETRY_OBJECT_INCLUDED_H_INCLUDED  defined
