"""Plot number of proteins vs number of mRNAs."""

from __future__ import absolute_import, print_function, division

import sys
import pandas
import matplotlib.pyplot as plt

from output_molecules import SimulationFiles
from chemical_sequences import Proteins


def main():
    markers = ['o', 's', '^']
    stat_list = [protein_stats(dir) for dir in sys.argv[1:]]
    f = plt.figure()
    for stats, marker in zip(stat_list, markers[:len(stat_list)]):
        plt.scatter([s[1] for s in stats], [s[2] for s in stats],
                    facecolors='none', edgecolors='k', marker=marker)
    f.savefig('number_proteins.pdf', bbox_inches='tight')


def protein_stats(simulation_directory):
    files = SimulationFiles(simulation_directory)
    chemicals = pandas.read_csv(files.output_path('chemicals.out'),
                                sep='\t')
    final_numbers = chemicals.iloc[-1]
    proteins = Proteins(files.input_path('proteins.in'))
    return [(id_, rna_number, final_numbers[id_])
            for id_, rna_number in proteins.count.items()]


if __name__ == '__main__':
    main()
