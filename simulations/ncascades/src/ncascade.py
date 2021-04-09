
from __future__ import absolute_import, division, print_function
import subprocess
import os.path

from src import copasi, sbml, bipsim, bionetgen


class NCascade(object):
    def __init__(self, length, initial_value):
        self.length = length
        self.chemicals = [
            Chemical(self.chemical_id(i), 0) for i in range(length)
            ]
        self.reactions = [
            Reaction(self.chemical_id(i), self.chemical_id(i+1), i)
            for i in range(length-1)
            ]
        self.set_initial_value(initial_value)

    def chemical_id(self, index):
        return 'A_{}'.format(index)

    def set_initial_value(self, initial_value):
        if self.chemicals:
            self.chemicals[0].initial_value = initial_value

    def to_bipsim(self, output_dir, method):
        bipsim.write_run_script(output_dir)
        params = bipsim.Params(method)
        params.set_max_time(self.length)
        params.set_output_entities([self.chemicals[0].id,
                                    self.chemicals[-1].id])
        params.write(output_dir)
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

    def to_bionetgen(self, output_dir, method, observables=True):
        bionetgen.write_run_script(output_dir)
        with open(self._bionetgen_input_file(output_dir), 'w') as output:
            output.write('begin model\n')
            output.write('begin parameters\n')
            output.write('\tk 1\n')
            output.write('end parameters\n')
            output.write('begin molecule types\n')
            output.write('\n'.join(c.to_bng_species()
                                   for c in self.chemicals))
            output.write('\nend molecule types\n')
            output.write('begin seed species\n')
            output.write('\n'.join(c.to_bng_seed_species()
                                   for c in self.chemicals
                                   if c.initial_value>0))
            output.write('\nend seed species\n')
            if observables:
                output.write('begin observables\n')
                output.write('\n'.join(c.to_bng_observable()
                                       for c in self.chemicals))            
                output.write('\nend observables\n')
            output.write('begin reaction rules\n')
            output.write('\n'.join(r.to_bng() for r in self.reactions))
            output.write('\nend reaction rules\n')
            output.write('end model\n\n')
            fmt = 'simulate({{method=>"{}",t_start=>0,t_end=>{},n_steps=>100}})\n'
            output.write(fmt.format(method, self.length))

    def _bionetgen_input_file(self, output_dir):
        return os.path.join(output_dir, 'model.bngl')


class Chemical(object):
    def __init__(self, id_, initial_value):
        self.id = id_
        self.initial_value = initial_value

    def to_bipsim_format(self):
        return 'FreeChemical {} {}'.format(self.id, self.initial_value)
    
    def to_bng_species(self):
        return '\t{}()'.format(self.id)
    
    def to_bng_seed_species(self):
        return '\t{}() {}'.format(self.id, self.initial_value)
    
    def to_bng_observable(self):
        return '\tSpecies {} {}()'.format(self.id, self.id)
    

class Reaction(object):
    def __init__(self, reactant, product, id_):
        self.reactant = reactant
        self.product = product
        self.id = id_

    def to_bipsim_format(self):
        return ('ChemicalReaction {} -1 {} 1 rates 1 0'
                .format(self.reactant, self.product))

    def to_bng(self):
        return ('\treaction_{}: {}() -> {}() k'
                .format(self.id, self.reactant, self.product))
