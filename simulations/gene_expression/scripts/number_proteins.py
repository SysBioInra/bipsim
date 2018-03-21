"""Plot number of proteins vs number of mRNAs."""

from __future__ import absolute_import, print_function, division

import sys
import pandas
import matplotlib.pyplot as plt

from output_molecules import SimulationFiles
from chemical_sequences import Proteins


def main():
    stat_list = [ProteinStats(dir) for dir in sys.argv[1:]]
    number_vs_rnas(stat_list).savefig(
        'number_vs_rnas.pdf', bbox_inches='tight'
    )
    protein_statistics(stat_list).savefig(
        'protein_statistics.pdf', bbox_inches='tight'
    )


def number_vs_rnas(stat_list):
    fig = plt.figure()
    markers = ['o', 's', '^']
    for stats, marker in zip(stat_list, markers[:len(stat_list)]):
        plt.scatter(stats.rnas, stats.final_numbers,
                    facecolors='none', edgecolors='k', marker=marker)
    return fig


def protein_statistics(stat_list):
    fig = plt.figure()
    violin_pos = range(1, len(stat_list)+1, 1)
    plt.violinplot([s.proteins_per_rna() for s in stat_list],
                   violin_pos, points=100, widths=0.6,
                   showmeans=True, showextrema=True, showmedians=True)
    return fig


class ProteinStats(object):
    def __init__(self, simulation_directory):
        files = SimulationFiles(simulation_directory)
        chemicals = pandas.read_csv(files.output_path('chemicals.out'),
                                    sep='\t')
        final_numbers = chemicals.iloc[-1]
        self.proteins = Proteins(files.input_path('proteins.in'))
        self.ids = list(self.proteins.count.keys())
        self.rnas = list(self.proteins.count.values())
        self.final_numbers = [final_numbers[p] for p in self.ids]

    def proteins_per_rna(self):
        return [n / rna for n, rna in zip(self.final_numbers, self.rnas)]


if __name__ == '__main__':
    main()
