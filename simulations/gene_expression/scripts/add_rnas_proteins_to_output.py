"""Add proteins and rnas to output molecules"""

import sys

from output_molecules import SimulationFiles, OutputEntities
from chemical_sequences import Rnas, Proteins


def main():
    for sim in sys.argv[1:]:
        files = SimulationFiles(sim)
        rnas = Rnas(files.input_path('rnas.in'))
        proteins = Proteins(files.input_path('proteins.in'))
        params = OutputEntities(files.input_path('params.in'))
        params.add(rnas.unique_elements)
        params.add(proteins.unique_elements)
        params.export(files.input_path('params_full_output.in'))


if __name__ == '__main__':
    main()
