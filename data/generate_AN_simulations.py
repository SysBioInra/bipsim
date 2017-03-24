
from subprocess import call

from generate_simulator_files import *

params = []
target_dir = []
# base simulations
params += [('AN', 'detailed', m) for m in ['detailed', 'stacked', 'constant']]
target_dir += ['AN_detailed_' + m for m in ['detailed', 'stacked', 'constant']]
params += [('AN', 'hybrid', m) for m in ['stacked', 'constant']]
target_dir += ['AN_hybrid_' + m for m in ['stacked', 'constant']]
params += [('AN', 'agregated', m) for m in ['stacked', 'constant']]
target_dir += ['AN_agregated_' + m for m in ['stacked', 'constant']]

# paulsson simulations
params += [('paulsson', 'detailed', 'constant'),
           ('paulsson', 'agregated', 'constant')]
target_dir += ['paulsson_' + m for m in ['detailed', 'agregated']]

for p, t in zip(params, target_dir):
    # generate files
    export_simulation_data(p[0], p[1], p[2])
    # copy them
    call('cp -r output/* ../' + t + '/input', shell=True)
