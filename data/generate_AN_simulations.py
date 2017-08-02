"""Generate all simulations used for Application Note article."""

from __future__ import absolute_import, division, print_function
from subprocess import call

from generate_simulator_files import export_simulation_data, read_data
from generate_minimal_paulsson import read_rates, export_simulation


def main():
    """Generate all simulation files for article."""
    params = []
    target_dir = []
    # base simulations
    full_set = ['detailed', 'stacked', 'constant']
    quick_set = ['stacked', 'constant']
    params += [('AN', 'detailed', m) for m in full_set]
    target_dir += ['AN_detailed_' + m for m in full_set]
    params += [('AN', 'hybrid', m) for m in quick_set]
    target_dir += ['AN_hybrid_' + m for m in quick_set]
    params += [('AN', 'aggregated', m) for m in quick_set]
    target_dir += ['AN_aggregated_' + m for m in quick_set]
    # paulsson simulations
    params += [('paulsson', 'detailed', 'constant'),
               ('paulsson', 'aggregated', 'constant')]
    target_dir += ['paulsson_' + m for m in ['detailed', 'aggregated']]

    # get data
    data = read_data()
    for p, t in zip(params, target_dir):
        # generate files
        export_simulation_data(data, p)
        # copy them
        call('cp -r output/* ../' + t + '/input', shell=True)

    # generate minimal paulsson
    rates, initial_values = read_rates()
    export_simulation(rates, initial_values, '../paulsson_minimal/input')


if __name__ == '__main__':
    main()
