
from __future__ import absolute_import, division, print_function
import os.path

EXE_NAME = '~/biosys/BioNetGen-2.5.0/BNG2.pl'


def write_run_script(output_dir):
    filename = os.path.join(output_dir, 'run.sh')
    with open(filename, 'w') as output:
        output.write(EXE_NAME + ' model.bngl > output.txt')

