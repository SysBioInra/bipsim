"""Plot figures for paulsson experiments."""

from __future__ import absolute_import, division, print_function

import numpy
import matplotlib.pyplot as plt


def main():
    """Plot figures for paulsson experiments."""
    import sys
    simulation_file = sys.argv[1]
    theoretical_file = sys.argv[2]
    # read data
    bsus, rnas, prots = read_theoretical_data(theoretical_file)
    simu_bsus, simu_rnas, simu_prots, simu_rnas_std, simu_prots_std = \
        read_simulation_data(simulation_file)
    # filter data (keep only simulated genes)
    keep = numpy.array([bsu in simu_bsus for bsu in bsus])
    rnas = rnas[keep]
    prots = prots[keep]
    # plot data
    plt.figure(100)
    plt.subplot(211)
    plt.title('Average number of rnas')
    plt.xlabel('Theory')
    plt.ylabel('Simulation')
    plt.plot(rnas, simu_rnas, 'o')
    M = numpy.max(rnas)
    plt.plot([0, M], [0, M], 'k')
    plt.subplot(212)
    plt.title('Average number of proteins')
    plt.xlabel('Theory')
    plt.ylabel('Simulation')
    plt.plot(prots, simu_prots, 'o')
    M = numpy.max(prots)
    plt.plot([0, M], [0, M], 'k')
    plt.tight_layout()
    plt.savefig('results/simulation_results.pdf')


def read_simulation_data(output):
    """Return first and second moment of simulation data."""
    with open(output, 'r') as f:
        header = next(f)
        # skip time, nb_rnas and nb_proteins
        names = header.rstrip().split('\t')[3:]
        nb_points = 0
        moment_1 = numpy.zeros(len(names))
        moment_2 = numpy.zeros(len(names))
        for line in f:
            nb_points += 1
            data = numpy.array([int(d) for d in line.rstrip().split('\t')[3:]])
            moment_1 += data
            moment_2 += data**2
    moment_1 /= nb_points
    moment_2 /= nb_points
    std = moment_2 - moment_1**2
    nb_genes = int(len(names) / 2)
    bsus = [n[:-4] for n in names[:nb_genes]]
    return (bsus, moment_1[:nb_genes], moment_1[nb_genes:],
            std[:nb_genes], std[nb_genes:])


def read_theoretical_data(input):
    """Return expected averages."""
    with open(input, 'r') as f:
        header = next(f)
        bsus = []
        rnas = []
        prots = []
        for line in f:
            bsu, name, rna, prot = line.split(';', 5)[:4]
            bsus.append(bsu)
            rnas.append(float(rna))
            prots.append(float(prot))
    return bsus, numpy.array(rnas), numpy.array(prots)


if __name__ == '__main__':
    main()
