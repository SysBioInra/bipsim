
from __future__ import absolute_import, division, print_function

import libsbml


class NCascade(object):
    def __init__(self, length, initial_value):
        self.length = length
        self.chemicals = [
            Chemical(self.chemical_id(i), 0) for i in range(length)
            ]
        self.reactions = [
            Reaction(self.chemical_id(i), self.chemical_id(i+1))
            for i in range(length-1)
            ]
        self.set_initial_value(initial_value)

    def chemical_id(self, index):
        return 'A_{}'.format(index)

    def set_initial_value(self, initial_value):
        if self.chemicals:
            self.chemicals[0].initial_value = initial_value

    def to_bipsim_format(self, output):
        output.write('\n'.join(c.to_bipsim_format() for c in self.chemicals))
        output.write('\n\n')
        output.write('\n'.join(r.to_bipsim_format() for r in self.reactions))
        output.write('\n\n')

    def to_sbml(self, output):
        model = SbmlModel()
        for c in self.chemicals:
            model.add_default_species(c.id, c.initial_value)
        for i, r in enumerate(self.reactions):
            model.add_reaction('reaction_{}'.format(i),
                               r.reactant, r.product)
        output.write(model.to_string())


class Chemical(object):
    def __init__(self, id_, initial_value):
        self.id = id_
        self.initial_value = initial_value

    def to_bipsim_format(self):
        return 'FreeChemical {} {}'.format(self.id, self.initial_value)


class Reaction(object):
    def __init__(self, reactant, product):
        self.reactant = reactant
        self.product = product

    def to_bipsim_format(self):
        return ('ChemicalReaction {} -1 {} 1 rates 1 0'
                .format(self.reactant, self.product))


class SbmlModel(object):
    CYTOSOL_ID = 'cytosol'

    def __init__(self):
        self._create_document_and_model()
        self._add_default_compartment(self.CYTOSOL_ID)

    def _create_document_and_model(self):
        try:
            self._document = libsbml.SBMLDocument(3, 1)
        except ValueError:
            raise SystemExit('Could not create SBMLDocument object')
        model = self._document.createModel()
        model.setTimeUnits('second')
        model.setExtentUnits('mole')
        model.setSubstanceUnits('mole')

    def _add_default_compartment(self, id_):
        result = self._document.model.createCompartment()
        result.setId(id_)
        result.setConstant(True)
        result.setSize(1)
        result.setSpatialDimensions(3)
        result.setUnits('litre')

    def add_default_species(self, id_, initial_value):
        result = self._document.model.createSpecies()
        result.setId(id_)
        result.setCompartment(self.CYTOSOL_ID)
        result.setConstant(False)
        result.setInitialAmount(initial_value)
        result.setSubstanceUnits('mole')
        result.setBoundaryCondition(False)
        result.setHasOnlySubstanceUnits(False)

    def add_reaction(self, id_, reactant_id, product_id):
        result = self._document.model.createReaction()
        result.setId(id_)
        result.setReversible(False)
        result.setFast(False)
        self._init_species_reference(result.createReactant(), reactant_id)
        self._init_species_reference(result.createProduct(), product_id)

    def _init_species_reference(self, species_reference, id_):
        species_reference.setSpecies(id_)
        species_reference.setConstant(True)

    def to_string(self):
        return libsbml.writeSBMLToString(self._document)
