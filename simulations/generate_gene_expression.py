"""Generate all simulations used for Application Note article."""

from __future__ import absolute_import, division, print_function
from subprocess import call

from simgenerator.simulation_exporter import export_simulation_data, read_data


def main():
    """Generate all simulation files for article."""
    data = read_data()
    for p, t in zip(*gene_expression()):
        export_simulation_data(data, p, t)


def gene_expression():
    params = []
    target_dir = []
    full_set = ['detailed', 'stacked', 'constant']
    quick_set = ['stacked', 'constant']
    params += [('AN', 'detailed', m) for m in full_set]
    target_dir += ['gene_expression/detailed_' + m for m in full_set]
    params += [('AN', 'hybrid', m) for m in quick_set]
    target_dir += ['gene_expression/hybrid_' + m for m in quick_set]
    params += [('AN', 'aggregated', m) for m in quick_set]
    target_dir += ['gene_expression/aggregated_' + m for m in quick_set]
    return params, target_dir


if __name__ == '__main__':
    main()
