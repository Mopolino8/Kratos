// KRATOS  __  __ _____ ____  _   _ ___ _   _  ____ 
//        |  \/  | ____/ ___|| | | |_ _| \ | |/ ___|
//        | |\/| |  _| \___ \| |_| || ||  \| | |  _ 
//        | |  | | |___ ___) |  _  || || |\  | |_| |
//        |_|  |_|_____|____/|_| |_|___|_| \_|\____| APPLICATION
//
//  License:		 BSD License
//                       license: MeshingApplication/license.txt
//
//  Main authors:    Vicente Mataix Ferrándiz
//

#if !defined(KRATOS_METRICS_MATH_UTILS)
#define KRATOS_METRICS_MATH_UTILS

// Project includes
#include "utilities/math_utils.h"
#include "includes/model_part.h"
#include "utilities/openmp_utils.h"

namespace Kratos
{
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

//// This class is used to compute some mathematical operations needed for the metrics computing

template<unsigned int TDim>  
class MetricsMathUtils
{
public:

    ///@name Type Definitions
    ///@{
    
    ///@}
    ///@name Life Cycle
    ///@{
    
    ///@}
    ///@name Operators
    ///@{


    ///@}
    ///@name Operations
    ///@{
 
    /**
     * This function converts tensors to vector and viceversa
     */
    
    static inline Vector TensorToVector(const boost::numeric::ublas::bounded_matrix<double, TDim, TDim> Tensor)
    {
        Vector TensorCond;
        TensorCond.resize(3 * (TDim - 1), false);
        
        // Common part
        TensorCond[0] = Tensor(0, 0);
        TensorCond[1] = Tensor(0, 1);
            
        if (TDim == 2)
        {
            TensorCond[2] = Tensor(1, 1);
        }
        else
        {
            TensorCond[2] = Tensor(0, 2);
            TensorCond[3] = Tensor(1, 1);
            TensorCond[4] = Tensor(1, 2);
            TensorCond[5] = Tensor(2, 2);
        }

        return TensorCond;
    }
    
    static inline boost::numeric::ublas::bounded_matrix<double, TDim, TDim>  VectorToTensor(const Vector TensorCond)
    {
        boost::numeric::ublas::bounded_matrix<double, TDim, TDim> Tensor;
        
        // Common part
        Tensor(0, 0) = TensorCond[0];
        Tensor(0, 1) = TensorCond[1];
        Tensor(1, 0) = TensorCond[1];
        if (TDim == 2)
        {
            Tensor(1, 1) = TensorCond[2];
        }
        else
        {
            Tensor(0, 2) = TensorCond[2];
            Tensor(2, 0) = TensorCond[2];
            Tensor(1, 1) = TensorCond[3];
            Tensor(1, 2) = TensorCond[4];
            Tensor(2, 1) = TensorCond[4];
            Tensor(2, 2) = TensorCond[5];
        }

        return Tensor;
    }
    
    /***********************************************************************************/
    /***********************************************************************************/
    
    /**
     * It computes the intersection between two metrics 
     * @param Metric1: The first metric
     * @param Metric2: The second metric
     */
        
    static inline Vector IntersectMetrics(
        const Vector Metric1,
        const Vector Metric2
    )
    {
        const boost::numeric::ublas::bounded_matrix<double, TDim, TDim> Metric1Matrix = VectorToTensor(Metric1);
        const boost::numeric::ublas::bounded_matrix<double, TDim, TDim> Metric2Matrix = VectorToTensor(Metric2);
        
        boost::numeric::ublas::bounded_matrix<double, TDim, TDim> auxmat;
        boost::numeric::ublas::bounded_matrix<double, TDim, TDim> emat;
        
        double auxdet;
        const boost::numeric::ublas::bounded_matrix<double, TDim, TDim> invMetric1Matrix = MathUtils<double>::InvertMatrix<TDim>(Metric1Matrix, auxdet);
        const boost::numeric::ublas::bounded_matrix<double, TDim, TDim> NMatrix = prod(invMetric1Matrix, Metric2Matrix);
        
        MathUtils<double>::EigenSystem<TDim>(NMatrix, emat, auxmat, 1e-18, 20);
        
        typedef boost::numeric::ublas::bounded_matrix<double, TDim, TDim> temp_type;
        const boost::numeric::ublas::bounded_matrix<double, TDim, TDim> lambdamat =  prod(trans(emat), prod<temp_type>(Metric1Matrix, emat));
        const boost::numeric::ublas::bounded_matrix<double, TDim, TDim> mumat =  prod(trans(emat), prod<temp_type>(Metric2Matrix, emat));
        
        for (unsigned int i = 0; i < TDim; i++)
        {
            auxmat(i, i) = MathUtils<double>::Max(lambdamat(i, i), mumat(i, i));
        }
        
        const boost::numeric::ublas::bounded_matrix<double, TDim, TDim> invemat = MathUtils<double>::InvertMatrix<TDim>(emat, auxdet);
        
        boost::numeric::ublas::bounded_matrix<double, TDim, TDim> IntersectionMatrix =  prod(trans(invemat), prod<temp_type>(auxmat, invemat));
        
        const Vector Intersection = TensorToVector(IntersectionMatrix);
        
        return Intersection;
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
};// class MetricsMathUtils
///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{

}// namespace Kratos.
#endif /* KRATOS_METRICS_MATH_UTILS defined */
