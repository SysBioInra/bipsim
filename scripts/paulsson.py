"""Plot figures for paulsson experiments."""

from __future__ import absolute_import, division, print_function

import numpy
import matplotlib.pyplot as plt
from collections import namedtuple


def main():
    """Plot figures for paulsson experiments."""
    import sys
    simulation_file = sys.argv[1]
    theoretical_file = sys.argv[2]
    # read data
    bsus, rnas, prots, rnas_var, prots_var = \
        read_theoretical_data(theoretical_file)
    simu_bsus, simu_rnas, simu_prots, simu_rnas_var, simu_prots_var = \
        read_simulation_data(simulation_file)
    # filter data (keep only simulated genes)
    keep = numpy.array([bsu in simu_bsus for bsu in bsus])
    rnas = rnas[keep]
    prots = prots[keep]
    rnas_var = rnas_var[keep]
    prots_var = prots_var[keep]
    # save figures
    fig = statistics_figure(rnas, prots, rnas_var, prots_var, simu_rnas,
                            simu_prots, simu_rnas_var, simu_prots_var)
    fig.savefig('results/statistics.pdf')
    fig = residuals_figure(rnas, prots, simu_rnas, simu_prots)
    fig.savefig('results/residuals_average.pdf')


def statistics_figure(rnas, prots, rnas_var, prots_var,
                      simu_rnas, simu_prots, simu_rnas_var, simu_prots_var):
    fig = plt.figure()
    ax = fig.add_subplot(221)
    plot_simulation_vs_theory(ax, rnas, simu_rnas, 'RNA average')
    ax = fig.add_subplot(222)
    plot_simulation_vs_theory(ax, prots, simu_prots, 'Protein average')
    ax = fig.add_subplot(223)
    plot_simulation_vs_theory(ax, rnas_var, simu_rnas_var, 'RNA variance')
    ax = fig.add_subplot(224)
    plot_simulation_vs_theory(ax, prots_var, simu_prots_var,
                              'Protein variance')
    fig.tight_layout()
    return fig


def plot_simulation_vs_theory(ax, theory, simulation, title=None):
    if title:
        ax.set_title(title)
    ax.set_xlabel('Theory')
    ax.set_ylabel('Simulation')
    ax.loglog(theory, simulation, 'o')
    diagonal = [numpy.min(theory), numpy.max(theory)]
    ax.plot(diagonal, diagonal, 'k')


def residuals_figure(rnas, prots, simu_rnas, simu_prots):
    fig = plt.figure()
    ax = fig.add_subplot(211)
    plot_residuals(ax, rnas, simu_rnas)
    ax = fig.add_subplot(212)
    plot_residuals(ax, prots, simu_prots)
    fig.tight_layout()
    return fig


def plot_residuals(ax, theory, simulation):
    ax.set_xlabel('Theory')
    ax.set_ylabel('Residuals')
    f_percent = five_percent_limit(theory, simulation)
    print(f_percent)
    keep = theory > f_percent
    th = theory[keep]
    ax.plot(th, (simulation[keep]-th)/th, 'o')


def five_percent_limit(theory, simulation):
    order = numpy.argsort(theory)
    residuals = ((simulation - theory) / theory)[order]
    max_ind = numpy.where(abs(residuals) > 0.1)[0][-1]
    return theory[order][max_ind]


def read_simulation_data(output):
    """Return first and second moment of simulation data."""
    min_time = 3000
    with open(output, 'r') as f:
        header = next(f)
        # skip time, nb_rnas and nb_proteins
        names = header.rstrip().split('\t')[3:]
        nb_points = 0
        moment_1 = numpy.zeros(len(names))
        moment_2 = numpy.zeros(len(names))
        var_test = []
        for line in f:
            fields = line.rstrip().split('\t')
            if fields[0] > min_time:
                nb_points += 1
                data = numpy.fromiter((d for d in fields[3:]),
                                      'float', len(names))
                moment_1 += data
                moment_2 += data**2
                var_test.append(moment_2[-1]/nb_points
                                - (moment_1[-1]/nb_points)**2)
    moment_1 /= nb_points
    moment_2 /= nb_points
    var = moment_2 - moment_1**2
    nb_genes = int(len(names) / 2)
    bsus = [n[:-4] for n in names[:nb_genes]]
    plt.plot(var_test)
    plt.savefig('estimator.pdf')
    return (bsus, moment_1[:nb_genes], moment_1[nb_genes:],
            var[:nb_genes], var[nb_genes:])


def read_theoretical_data(input):
    """Return expected averages."""
    # read rates
    with open(input, 'r') as f:
        header = next(f)
        bsus = []
        rates = []
        for line in f:
            fields = line.rstrip().split(';')
            bsus.append(fields[0])
            rates.append(fields[4:8])
    # compute average and variance
    rna_b = numpy.fromiter((r[0] for r in rates), 'float')
    rna_d = numpy.fromiter((r[1] for r in rates), 'float')
    prot_b = numpy.fromiter((r[2] for r in rates), 'float')
    prot_d = numpy.fromiter((r[3] for r in rates), 'float')
    rnas = rna_b / rna_d
    rnas_var = rnas
    prots = rnas * prot_b / prot_d
    prots_var = prots * (1 + prot_b / (rna_d + prot_d))
    return bsus, rnas, prots, rnas_var, prots_var


if __name__ == '__main__':
    main()
