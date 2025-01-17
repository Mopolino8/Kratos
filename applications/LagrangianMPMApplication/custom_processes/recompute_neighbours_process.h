//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ \.
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Zhiming Guo
//                   Riccardo Rossi
//





#if !defined(RECOMPUTE_NEIGHBOURS_PROCESS)
#define RECOMPUTE_NEIGHBOURS_PROCESS



/* Project includes */
#include "includes/define.h"
#include "includes/model_part.h"
#include "includes/node.h"
#include "includes/variables.h"


#include "lagrangian_mpm_application_variables.h"
#include "utilities/math_utils.h"
#include <cmath>
#include <algorithm>



namespace Kratos
{



class RecomputeNeighboursProcess : public Process
{

public:


    typedef ModelPart::NodesContainerType NodesArrayType;
    typedef ModelPart::ElementsContainerType ElementsArrayType;
    typedef ModelPart::ConditionsContainerType ConditionsArrayType;



    RecomputeNeighboursProcess( ModelPart& model_part ) : Process(), mr_model_part( model_part )
    {
        /////////////

    }

    ~RecomputeNeighboursProcess() {}



    virtual void Execute()

    {

        KRATOS_TRY;


        typedef Node<3>                                         NodeType;
        typedef Node<3>::Pointer                                NodePointerType;
        typedef ModelPart::NodesContainerType::ContainerType    NodesContainerType;
        ModelPart::ElementsContainerType                        temp_container;
        ModelPart::ConditionsContainerType                      temp_condition_container;

        BinsDynamic<3, NodeType, NodesContainerType> bins(mr_model_part.NodesArray().begin(),mr_model_part.NodesArray().end());

        //creating an auxiliary node for searching purposes
        Node<3> aux_node(0,0.0,0.0,0.0);

        for(ModelPart::ElementsContainerType::iterator iel = mr_model_part.ElementsBegin();
                iel!=mr_model_part.ElementsEnd(); iel++)
        {
            unsigned int MaximumNumberOfResults = 1000;
            std::vector<NodePointerType>                            results(MaximumNumberOfResults);
            std::vector<double>                                     distances(MaximumNumberOfResults);

            //calculating shape functions values
            Geometry< Node<3> >::Pointer pgeom = iel->pGetGeometry();
            pgeom->clear();

            double search_radius = iel->GetValue(SEARCH_RADIUS);//0.02;

            //get the coordinates of the gauss point as  the sum of N[i]*xnode
            const array_1d<double,3>& coordinates_of_gauss = iel->GetValue(GAUSS_POINT_COORDINATES);

            aux_node.Coordinates() =  coordinates_of_gauss;
            unsigned int nresults = bins.SearchInRadius(aux_node,search_radius,results.begin(),distances.begin(),MaximumNumberOfResults);

            unsigned int counter=0;
            for(NodesContainerType::iterator it = results.begin(); it != results.end(); ++it)
            {
                if(counter< nresults)
                {
                    pgeom->push_back( *(it.base()));
                }
                counter += 1;
            }

        }

        KRATOS_CATCH("");
    }





private:

    ModelPart& mr_model_part;


};
}

#endif

