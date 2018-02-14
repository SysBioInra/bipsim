
from __future__ import absolute_import, division, print_function


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
