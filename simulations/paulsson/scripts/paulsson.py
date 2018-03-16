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
    bsus, rna_b, rna_d, protein_b, protein_d = \
        read_theoretical_data(theoretical_file)
    rnas, prots, rnas_var, prots_var = \
        theoretical_statistics(rna_b, rna_d, protein_b, protein_d)
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
    fig = residuals_figure(rnas, prots, rnas_var, prots_var, simu_rnas,
                           simu_prots, simu_rnas_var, simu_prots_var)
    fig.savefig('results/residuals.pdf')


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
    data_range = [numpy.min(theory), numpy.max(theory)]
    ax.plot(data_range, data_range, 'k')


def residuals_figure(rnas, prots, rnas_var, prots_var,
                     simu_rnas, simu_prots, simu_rnas_var, simu_prots_var):
    fig = plt.figure()
    ax = fig.add_subplot(221)
    plot_residuals(ax, rnas, simu_rnas)
    ax = fig.add_subplot(222)
    plot_residuals(ax, prots, simu_prots)
    ax = fig.add_subplot(223)
    plot_residuals(ax, rnas_var, simu_rnas_var)
    ax = fig.add_subplot(224)
    plot_residuals(ax, prots_var, simu_prots_var)
    fig.tight_layout()
    return fig


def plot_residuals(ax, theory, simulation, title=None):
    if title:
        ax.set_title(title)
    ax.set_xlabel('Theory')
    ax.set_ylabel('Residuals')
    frac = 0.1
    limit = relative_limit(theory, simulation, frac)
    data_range = [numpy.min(theory), numpy.max(theory)]
    ax.semilogx(theory, (simulation-theory)/theory, 'o')
    ax.axhline(-frac, ls=':')
    ax.axhline(frac, ls=':')
    ax.axvline(limit, ls=':')
    ax.text(limit, 0.5, '{0:.1f}'.format(limit))


def relative_limit(theory, simulation, fraction):
    order = numpy.argsort(theory)
    residuals = ((simulation - theory) / theory)[order]
    max_ind = numpy.where(abs(residuals) > fraction)[0][-1]
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
            if float(fields[0]) > min_time:
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
    plt.savefig('results/estimator.pdf')
    return (bsus, moment_1[:nb_genes], moment_1[nb_genes:],
            var[:nb_genes], var[nb_genes:])


def theoretical_statistics(rna_b, rna_d, prot_b, prot_d):
    """Return expected averages and variances."""
    rnas = rna_b / rna_d
    rnas_var = rnas
    prots = rnas * prot_b / prot_d
    prots_var = prots * (1 + prot_b / (rna_d + prot_d))
    return rnas, prots, rnas_var, prots_var


def read_theoretical_data(input_file):
    """Return birth/death rates for RNAs and proteins."""
    # read rates
    with open(input_file, 'r') as f:
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
    return bsus, rna_b, rna_d, prot_b, prot_d


if __name__ == '__main__':
    main()
