"""Module defining SbmlData class."""

# global imports
import itertools
import re
import libsbml

# local imports
from .entities import Species, Reaction


class SbmlData(object):
    """
    Class used to parse BipSim-relevant SBML data.

    Attributes
    ----------
    species: 
        SBML species.
    """

    def __init__(self, input_file):
        """
        Build from file.

        Parameters
        ----------
        input_file: str
            Path to input file.

        """
        # WARNING: not storing document in a variable will result
        # in segmentation fault!
        document = self._load_document(input_file)
        model = document.getModel()
        self._parse_species(model)
        self._parse_parameters(model)
        self._parse_observables(model)
        self._parse_assignments(model)
        self._parse_reactions(model)

    def _load_document(self, input_file):
        document = libsbml.readSBML(input_file)
        if document.getNumErrors() > 0:
            document.printErrors()
            raise UserWarning('Invalid SBML.')
        return document

    def _parse_species(self, model):
        self.species = []
        for spec in model.getListOfSpecies():
            self.species.append(Species(spec.getId(), spec.getInitialConcentration()))

    def _parse_parameters(self, model):
        self.parameters = {p.id: p.value for p in model.getListOfParameters()}

    def _parse_observables(self, model):
        self.observables = itertools.chain.from_iterable(
            re.split("[+*]", r.getFormula()) for r in model.getListOfRules()
        )
        self.observables = set(o.strip() for o in self.observables)
        known_ids = set(s.id for s in self.species)
        self.observables = self.observables.intersection(known_ids)

    def _parse_assignments(self, model):
        rules = model.getListOfRules()
        self.assignments = list(
            r.variable + "=" + r.getFormula().replace("cell * ", "") for r in model.getListOfRules()
        )

    def _parse_reactions(self, model):
        self.reactions = []
        for reaction in model.getListOfReactions():
            new_reaction = self._create_reaction(reaction)
            self.reactions.append(new_reaction)

    def _create_reaction(self, reaction):
        result = Reaction()
        for r in reaction.getListOfReactants():
            result.add_reactant(r.getSpecies(), r.getStoichiometry())
        for p in reaction.getListOfProducts():
            result.add_product(p.getSpecies(), p.getStoichiometry())
        result.kon = self._parse_kon(reaction)
        return result
    
    def _parse_kon(self, reaction):
        reaction_params = reaction.getKineticLaw().getFormula().split(" * ")
        for p in reaction_params:
            if p in self.parameters:
                return self.parameters[p]
        return None
    
    def to_bipsim(self, ic_species):
        species = "\n".join(s.to_bipsim() for s in self.species if not s.id in ic_species)
        reactions = "\n".join(r.to_bipsim() for r in self.reactions)
        result = "\n".join(("# Species", species, "\n# Reactions", reactions))
        return(result)
