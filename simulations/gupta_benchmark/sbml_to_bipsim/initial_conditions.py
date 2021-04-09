
from .entities import Species


class InitialConditions(object):
    def __init__(self, initial_conditions):
        self.initial_conditions = initial_conditions
        
    def to_bipsim(self):
        species = (Species(k, v) for k, v in self.initial_conditions.items())
        result = "\n".join(s.to_bipsim() for s in species)
        return result