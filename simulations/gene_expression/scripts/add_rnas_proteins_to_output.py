"""Add proteins and rnas to output molecules"""

import sys

from output_molecules import Input, OutputEntities
from chemical_sequences import Rnas, Proteins


def main():
    input_ = Input(sys.argv[1])
    rnas = Rnas(input_.path('rnas.in'))
    proteins = Proteins(input_.path('proteins.in'))
    params = OutputEntities(input_.path('params.in'))
    params.add(rnas.elements)
    params.add(proteins.unique_elements)
    params.export()


if __name__ == '__main__':
    main()
