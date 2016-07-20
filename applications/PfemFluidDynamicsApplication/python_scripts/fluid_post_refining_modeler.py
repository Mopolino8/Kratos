from __future__ import print_function, absolute_import, division  # makes KratosMultiphysics backward compatible with python 2.6 and 2.7
#import kratos core and applications
import KratosMultiphysics
import KratosMultiphysics.PfemBaseApplication as KratosPfemBase
import KratosMultiphysics.PfemFluidDynamicsApplication as KratosPfemFluid

# Check that KratosMultiphysics was imported in the main script
KratosMultiphysics.CheckForPreviousImport()

# Import the mesh modeler (the base class for the modeler derivation)
import fluid_mesh_modeler

def CreateMeshModeler(main_model_part, meshing_parameters, mesh_id):
    return PostRefiningModeler(main_model_part, meshing_parameters, mesh_id)

class PostRefiningModeler(fluid_mesh_modeler.FluidMeshModeler):
    
    #
    def __init__(self, main_model_part, meshing_parameters, mesh_id): 
        
        self.echo_level        = 1
        self.mesh_id           = mesh_id
        self.main_model_part   = main_model_part 
        self.MeshingParameters = meshing_parameters

        print("Construction of the Post Refining Modeler finished")

           
    #
    def InitializeMeshing(self):
        
        print("::[fluid_post_refining_modeler]:: -START InitializeMeshing-")

        self.MeshingParameters.InitializeMeshing()

        # set modeler flags: to set options for the mesher (triangle 2D, tetgen 3D)
        # REFINE

        refining_parameters = self.MeshingParameters.GetRefiningParameters()
        refining_options = refining_parameters.GetRefiningOptions()

        modeler_flags = ""
        modeler_info  = "Refine the domain"

        meshing_options = self.MeshingParameters.GetOptions()

        if( self.domain_size == 2 ):
           
            if( refining_options.Is(KratosPfemBase.ModelerUtilities.REFINE_ADD_NODES) ):
                #"YYJaqrn" "YJq1.4arn" "Jq1.4arn"
                if( meshing_options.Is(KratosPfemBase.ModelerUtilities.CONSTRAINED) ):
                    modeler_flags = "pYJq1.4arnCQ"  
                else:
                    modeler_flags = "YJq1.4arnQ"

            if( refining_options.Is(KratosPfemBase.ModelerUtilities.REFINE_INSERT_NODES) ):
                #"riYYJQ" "riYYJQ" "riJQ" "riQ"
                if( meshing_options.Is(KratosPfemBase.ModelerUtilities.CONSTRAINED) ):
                    modeler_flags = "rinYYJQ"  
                else:
                    #modeler_flags = "ns"
                    modeler_flags = "nQP"
                    #modeler_flags = "rinYQ"
                    #modeler_flags = "rnYYJrQNP"
                    #modeler_flags = "YJnrQ"
                    #modeler_flags = "inJQ"
                    #modeler_flags = "riYYJnQ"
                    #modeler_flags = "riYYPJQ"
            
        elif( self.domain_size == 3 ):

            if( refining_options.Is(KratosPfemBase.ModelerUtilities.REFINE_ADD_NODES) ):
                if( meshing_options.Is(KratosPfemBase.ModelerUtilities.CONSTRAINED) ):
                    modeler_flags = "pMYJq1.4arnCBQF"
                else:
                    modeler_flags = "YJq1.4arnBQF"

            if( refining_options.Is(KratosPfemBase.ModelerUtilities.REFINE_INSERT_NODES) ):
                if( meshing_options.Is(KratosPfemBase.ModelerUtilities.CONSTRAINED) ):
                    modeler_flags = "rinYYJBQF"  
                else:
                    modeler_flags = "rinJBQF"

        self.MeshingParameters.SetTessellationFlags(modeler_flags)
        self.MeshingParameters.SetTessellationInfo(modeler_info)


    #
    def SetPreMeshingProcesses(self):
        # no process to start
        pass
     
