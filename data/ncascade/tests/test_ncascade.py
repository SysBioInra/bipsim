
from __future__ import absolute_import, division, print_function
from io import StringIO

from ncascade.ncascade import NCascade, Chemical, Reaction


def test_empty_ncascade():
    n_cascade = NCascade(0, 10)
    assert(len(n_cascade.chemicals) == 0)
    assert(len(n_cascade.reactions) == 0)


def test_ncascade_lenth_2_bipsim_format():
    assert_bipsim_format(
        NCascade(2, 10),
        'FreeChemical A_0 10\nFreeChemical A_1 0\n\n'
        'ChemicalReaction A_0 -1 A_1 1 rates 1 0\n\n'
        )


def assert_bipsim_format(n_cascade, expected_output):
    output = StringIO()
    n_cascade.to_bipsim_format(output)
    assert(output.getvalue() == expected_output)


def test_ncascade_lenth_3_bipsim_format():
    assert_bipsim_format(
        NCascade(3, 10),
        'FreeChemical A_0 10\nFreeChemical A_1 0\nFreeChemical A_2 0\n\n'
        'ChemicalReaction A_0 -1 A_1 1 rates 1 0\n'
        'ChemicalReaction A_1 -1 A_2 1 rates 1 0\n\n'
       )


def test_chemical_creation():
    chemical = sample_chemical()
    assert(chemical.id == 'identifier')
    assert(chemical.initial_value == 1)


def sample_chemical():
    return Chemical('identifier', 1)


def test_chemical_bipsim_format():
    chemical = sample_chemical()
    assert(chemical.to_bipsim_format() == 'FreeChemical identifier 1')


def test_reaction_creation():
    reaction = sample_reaction()
    assert(reaction.reactant == 'reactant')
    assert(reaction.product == 'product')


def sample_reaction():
    return Reaction('reactant', 'product')


def test_reaction_bipsim_format():
    reaction = sample_reaction()
    assert(reaction.to_bipsim_format() ==
           'ChemicalReaction reactant -1 product 1 rates 1 0')
