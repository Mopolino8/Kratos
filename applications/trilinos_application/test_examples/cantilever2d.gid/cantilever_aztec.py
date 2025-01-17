from __future__ import print_function, absolute_import, division #makes KratosMultiphysics backward compatible with python 2.6 and 2.7
# -*- coding: utf-8 -*-
#
#
# setting the domain size for the problem to be solved
domain_size = 2

import sys
kratos_path = '../../../..'
kratos_benchmarking_path = '../../../../benchmarking'
sys.path.append(kratos_path)
sys.path.append(kratos_benchmarking_path)

from KratosMultiphysics import *
from KratosMultiphysics.mpi import *
from KratosMultiphysics.StructuralApplication import *
from KratosMultiphysics.TrilinosApplication import *
from KratosMultiphysics.MetisApplication import *

print("i am ", mpi.rank, " of ", mpi.size)

#
#

# defining a model part
model_part = ModelPart("StructurePart")

# adding of Variables to Model Part should be here when the "very fix container will be ready"
import trilinos_structural_solver_static
trilinos_structural_solver_static.AddVariables(model_part)
model_part.AddNodalSolutionStepVariable(PARTITION_INDEX)

# reading a model
gid_mode = GiDPostMode.GiD_PostBinary
multifile = MultiFileFlag.MultipleFiles
deformed_mesh_flag = WriteDeformedMeshFlag.WriteUndeformed
write_conditions = WriteConditionsFlag.WriteElementsOnly
gid_io = GidIO("cantilever2d", gid_mode, multifile, deformed_mesh_flag, write_conditions)
# gid_io.ReadModelPart(model_part)

number_of_partitions = mpi.size  # we set it equal to the number of processors
print("number_of_partitions", number_of_partitions)
partitioner = MetisPartitioningProcess(model_part, gid_io, number_of_partitions, domain_size)
partitioner.Execute()
# print "GetRank()",GetRank()

mesh_name = mpi.rank
gid_io.InitializeMesh(mesh_name)
gid_io.WriteMesh((model_part).GetMesh())
gid_io.FinalizeMesh()


print("mesh_name =", mesh_name)
print(model_part)
print(model_part.Properties)

# writing the mesh
# gid_io.WriteUndeformedMesh(model_part.GetMesh(),domain_size,GiDPostMode.GiD_PostBinary);

# the buffer size should be set up here after the mesh is read for the first time
model_part.SetBufferSize(2)

# importing the solver files
trilinos_structural_solver_static.AddDofs(model_part)

# creating a fluid solver object
solver = trilinos_structural_solver_static.StaticStructuralSolver(model_part, domain_size)

#
# defining the linear solver
aztec_parameters = ParameterList()
aztec_parameters.set("AZ_solver", "AZ_gmres")
aztec_parameters.set("AZ_kspace", 100)
aztec_parameters.set("AZ_output", 32);

preconditioner_type = "Amesos"
preconditioner_parameters = ParameterList()
preconditioner_parameters.set("amesos: solver type", "Amesos_Klu");

# preconditioner_type = "ILU"
# preconditioner_parameters = ParameterList()

overlap_level = 1
nit_max = 300
tol = 1e-6

solver.structure_linear_solver = AztecSolver(aztec_parameters, preconditioner_type, preconditioner_parameters, tol, nit_max, overlap_level);
solver.structure_linear_solver.SetScalingType(AztecScalingType.LeftScaling)
#

model_part.Properties[1].SetValue(CONSTITUTIVE_LAW, Isotropic2D())
print("Linear elastic model selected")

solver.Initialize()
(solver.solver).SetEchoLevel(2);

import benchmarking
import math


def BenchmarkCheck(time, model_part):
    # find the largest displacement
    local_max_disp = 0.0
    for node in model_part.Nodes:
        disp = node.GetSolutionStepValue(DISPLACEMENT, 0)
        disp_value = disp[0] ** 2 + disp[1] ** 2 + disp[2] ** 2
        if(disp_value > local_max_disp):
            local_max_disp = disp_value

    max_values = mpi.gather(mpi.world, local_max_disp, 0)  # all_gather would gather to all process .. this only gathers to the root
    mpi.world.barrier()

    if(mpi.rank == 0):
        tot_max = 0.0
        for i in range(0, len(max_values)):
            if(max_values[i] > tot_max):
                tot_max = max_values[i]

        tot_max = math.sqrt(tot_max)
        print(tot_max)

        if (benchmarking.InBenchmarkingMode()):
            benchmarking.Output(time, "Time")
            benchmarking.Output(tot_max, "maximum displacement in the model", 0.00001)

    mpi.world.barrier()


Dt = 0.001
nsteps = 10

gid_io.InitializeResults(mesh_name, (model_part).GetMesh())

for step in range(0, nsteps):
    print("line49")

    time = Dt * step
    model_part.CloneTimeStep(time)

    print(time)
    # print model_part.ProcessInfo()[TIME]

    # solving the fluid problem
    if(step > 3):

        solver.Solve()

        BenchmarkCheck(time, model_part)

#    if(step > 4):

        # print the results
        gid_io.WriteNodalResults(DISPLACEMENT, model_part.Nodes, time, 0)
        gid_io.WriteNodalResults(REACTION, model_part.Nodes, time, 0)
#        gid_io.PrintOnGaussPoints(PK2_STRESS_TENSOR,model_part,time,domain_size)

gid_io.FinalizeResults()
