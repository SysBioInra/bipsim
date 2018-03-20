"""Add proteins and rnas to output molecules"""

import sys

from output_molecules import SimulationFiles, OutputEntities
from chemical_sequences import Rnas, Proteins


def main():
    files = SimulationFiles(sys.argv[1])
    rnas = Rnas(files.input_path('rnas.in'))
    proteins = Proteins(files.input_path('proteins.in'))
    params = OutputEntities(files.input_path('params.in'))
    params.add(rnas.elements)
    params.add(proteins.unique_elements)
    params.export()


if __name__ == '__main__':
    main()
