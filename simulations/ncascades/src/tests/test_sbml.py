
from __future__ import absolute_import, division, print_function
from io import StringIO

from src.sbml import SbmlModel


def test_empty_sbml_model():
    model = SbmlModel()
    assert(model.to_string() == empty_sbml_model())


def test_sbml_model_one_species():
    model = SbmlModel()
    model.add_default_species('species', 1)
    assert(model.to_string() == sbml_model_one_species())


def empty_sbml_model():
    return sbml_header() + sbml_footer()


def sbml_header():
    return """<?xml version="1.0" encoding="UTF-8"?>
<sbml xmlns="http://www.sbml.org/sbml/level3/version1/core" level="3" version="1">
  <model substanceUnits="dimensionless" timeUnits="second" extentUnits="dimensionless">
    <listOfUnitDefinitions>
      <unitDefinition id="per_second">
        <listOfUnits>
          <unit kind="second" exponent="-1" scale="0" multiplier="1"/>
        </listOfUnits>
      </unitDefinition>
    </listOfUnitDefinitions>
    <listOfCompartments>
      <compartment id="cytosol" spatialDimensions="3" size="1" units="litre" constant="true"/>
    </listOfCompartments>
"""

def sbml_footer():
    return """    <listOfParameters>
      <parameter id="k" value="1" units="per_second" constant="true"/>
    </listOfParameters>
  </model>
</sbml>
"""

def sbml_model_one_species():
    species_part = """    <listOfSpecies>
      <species id="species" compartment="cytosol" initialAmount="1" substanceUnits="dimensionless" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
    </listOfSpecies>
"""
    return sbml_header() + species_part + sbml_footer()
