from __future__ import print_function, absolute_import, division

# import Kratos
from KratosMultiphysics import *
from KratosMultiphysics.mpi import *
from KratosMultiphysics.TrilinosApplication import *


# Import Kratos "wrapper" for unittests
import KratosMultiphysics.KratosUnittest as KratosUnittest

# Import the tests o test_classes to create the suites

import test_trilinos_linear_solvers

def AssambleTestSuites():
    ''' Populates the test suites to run.

    Populates the test suites to run. At least, it should pupulate the suites:
    "small", "nighlty" and "all"

    Return
    ------

    suites: A dictionary of suites
        The set of suites with its test_cases added.
    '''

    suites = KratosUnittest.KratosSuites

    # Create a test suite with the selected tests (Small tests):
    smallSuite = suites['small']
    smallSuite.addTests(KratosUnittest.TestLoader().loadTestsFromTestCases([test_trilinos_linear_solvers.TestLinearSolvers]))
    
    # Create a test suite with the selected tests plus all small tests
    nightSuite = suites['nightly']
    nightSuite.addTests(KratosUnittest.TestLoader().loadTestsFromTestCases([test_trilinos_linear_solvers.TestLinearSolvers]))
    
    # Create a test suite that contains all the tests:
    allSuite = suites['all']
    allSuite.addTests(
        KratosUnittest.TestLoader().loadTestsFromTestCases([
            test_trilinos_linear_solvers.TestLinearSolvers
        ])
    )

    return suites

if __name__ == '__main__':
    KratosUnittest.runTests(AssambleTestSuites())