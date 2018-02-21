
from __future__ import absolute_import, division, print_function
import subprocess
import os.path

from ncascade import copasi, sbml


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

    def to_bipsim(self, output_dir):
        with open(self._bipsim_input_file(output_dir), 'w') as output:
            output.write('\n'.join(c.to_bipsim_format()
                                   for c in self.chemicals))
            output.write('\n\n')
            output.write('\n'.join(r.to_bipsim_format()
                                   for r in self.reactions))
            output.write('\n\n')

    def _bipsim_input_file(self, output_dir):
        return os.path.join(output_dir, 'input.in')

    def to_copasi(self, output_dir):
        copasi.write_run_script(output_dir, 'input.cps')
        self.to_sbml(output_dir)
        subprocess.call([copasi.EXE_NAME,
                         '-i', self._sbml_input_file(output_dir),
                         '-s', self._copasi_input_file(output_dir)])
        model = copasi.CopasiModel(self._copasi_input_file(output_dir))
        model.add_n_cascade(self.length)
        with open(self._copasi_input_file(output_dir), 'wb') as output:
            model.write(output)

    def to_sbml(self, output_dir):
        model = sbml.SbmlModel()
        for c in self.chemicals:
            model.add_default_species(c.id, c.initial_value)
        for i, r in enumerate(self.reactions):
            model.add_reaction('reaction_{}'.format(i),
                               r.reactant, r.product)
        with open(self._sbml_input_file(output_dir), 'w') as output:
            output.write(model.to_string())

    def _sbml_input_file(self, output_dir):
        return os.path.join(output_dir, 'input.xml')

    def _copasi_input_file(self, output_dir):
        return os.path.join(output_dir, 'input.cps')


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
