"""Plot number of proteins vs number of mRNAs."""

from __future__ import absolute_import, print_function, division

import sys
import pandas
import matplotlib.pyplot as plt

from output_molecules import SimulationFiles
from chemical_sequences import Proteins


def main():
    files = SimulationFiles(sys.argv[1])
    chemicals = pandas.read_csv(files.output_path('chemicals.out'), sep='\t')
    final_numbers = chemicals.iloc[-1]
    proteins = Proteins(files.input_path('proteins.in'))
    stats = [(id_, rna_number, final_numbers[id_])
             for id_, rna_number in proteins.count.items()]
    plt.plot([s[1] for s in stats], [s[2] for s in stats], 'bs')
    plt.show()


if __name__ == '__main__':
    main()
