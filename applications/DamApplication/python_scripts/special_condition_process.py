from KratosMultiphysics import *
from KratosMultiphysics.DamApplication import *

def Factory(settings, Model):
    if(type(settings) != Parameters):
        raise Exception("expected input shall be a Parameters object, encapsulating a json string")
    return SpecialConditionProcess(Model, settings["Parameters"])

## All the python processes should be derived from "python_process"

class SpecialConditionProcess(Process):
    def __init__(self, Model, settings ):
        Process.__init__(self)

        model_part = Model[settings["model_part_name"].GetString()]
        variable_name = settings["variable_name"].GetString()
