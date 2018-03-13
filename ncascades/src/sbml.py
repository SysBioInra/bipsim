
from __future__ import absolute_import, division, print_function

import libsbml


class SbmlModel(object):
    CYTOSOL_ID = 'cytosol'

    def __init__(self):
        self._create_document_and_model()
        self._define_units()
        self._add_default_compartment(self.CYTOSOL_ID)
        self._add_kinetic_parameter()

    def _create_document_and_model(self):
        try:
            self._document = libsbml.SBMLDocument(3, 1)
        except ValueError:
            raise SystemExit('Could not create SBMLDocument object')
        model = self._document.createModel()
        model.setTimeUnits('second')
        model.setExtentUnits('dimensionless')
        model.setSubstanceUnits('dimensionless')

    def _define_units(self):
        per_second = self._document.model.createUnitDefinition()
        per_second.setId('per_second')
        unit = per_second.createUnit()
        unit.setKind(libsbml.UNIT_KIND_SECOND)
        unit.setExponent(-1)
        unit.setScale(0)
        unit.setMultiplier(1)

    def _add_default_compartment(self, id_):
        result = self._document.model.createCompartment()
        result.setId(id_)
        result.setConstant(True)
        result.setSize(1)
        result.setSpatialDimensions(3)
        result.setUnits('litre')

    def _add_kinetic_parameter(self):
        result = self._document.model.createParameter()
        result.setId('k')
        result.setConstant(True)
        result.setValue(1)
        result.setUnits('per_second')

    def add_default_species(self, id_, initial_value):
        result = self._document.model.createSpecies()
        result.setId(id_)
        result.setCompartment(self.CYTOSOL_ID)
        result.setConstant(False)
        result.setInitialAmount(initial_value)
        result.setSubstanceUnits('dimensionless')
        result.setBoundaryCondition(False)
        result.setHasOnlySubstanceUnits(False)

    def add_reaction(self, id_, reactant_id, product_id):
        result = self._document.model.createReaction()
        result.setId(id_)
        result.setReversible(False)
        result.setFast(False)
        self._init_species_reference(result.createReactant(), reactant_id)
        self._init_species_reference(result.createProduct(), product_id)
        kinetic_law = result.createKineticLaw()
        kinetic_law.setMath(libsbml.parseL3Formula('k * {}'.format(reactant_id)))

    def _init_species_reference(self, species_reference, id_):
        species_reference.setSpecies(id_)
        species_reference.setConstant(True)

    def to_string(self):
        return libsbml.writeSBMLToString(self._document)
