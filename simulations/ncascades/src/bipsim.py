
from __future__ import absolute_import, division, print_function
import os.path

EXE_NAME = 'bipsim'


def write_run_script(output_dir):
    filename = os.path.join(output_dir, 'run.sh')
    with open(filename, 'w') as output:
        output.write(EXE_NAME + ' params.in')


class Params(object):
    def __init__(self):
        self._data = {}
        self._data['SEED'] = '0'
        self._data['INITIAL_TIME'] = '0'
        self._data['FINAL_TIME'] = '1000'
        self._data['INPUT_FILES'] = 'input.in'
        self._data['OUTPUT_DIR'] = '.'
        self._data['OUTPUT_STEP'] = '100'
        self._data['OUTPUT_ENTITIES'] = ''
        self._data['DRAWING_ALGORITHM'] = 'hybrid'
        self._data['HYBRID_BASE_RATE'] = '1e-6'

    def set_max_time(self, time):
        self._data['FINAL_TIME'] = str(time)
        self._data['OUTPUT_STEP'] = str(time / 100)

    def set_output_entities(self, entities):
        self._data['OUTPUT_ENTITIES'] = ' '.join(entities)

    def write(self, output_dir):
        with open(os.path.join(output_dir, 'params.in'), 'w') as output:
            output.write('\n'.join([
                '{} {}'.format(name, val) for name, val in self._data.items()
            ]))
            output.write('\n')
