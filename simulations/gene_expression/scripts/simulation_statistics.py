"""Compute simulation statistics."""

from __future__ import absolute_import, print_function, division
import numpy
import re


def main():
    import sys
    sim_set = ['aggregated']*2 + ['hybrid']*2 + ['detailed']*9
    metab = (['constant', 'stacked'] * 2 + ['constant'] * 3 +
             ['stacked'] * 3 + ['detailed'] * 3)
    tcn = ['no_tcn'] * 4 + ['no_tcn', 'tcn', 'tcn'] * 3
    sim_names = ['AN_{}_{}_{}'.format(s, m, t)
                 for s, m, t in zip(sim_set, metab, tcn)]
    reactions, times = parse_results(sys.argv[1])
    lines = []
    fmt = '{}:\n\t{} reactions, {}h {}m {}s, ({} reactions/s)\n\n'
    for n, r, t in zip(sim_names, reactions, times):
        m, s = divmod(int(t), 60)
        h, m = divmod(int(m), 60)
        lines.append(fmt.format(n, r, h, m, s, r / t))
    print(''.join(lines))


def parse_results(file_name):
    with open(file_name, 'r') as f:
        data = f.read()
    parser = re.compile(r'(\d+) reactions occurred.\n'
                        r'CPU runtime: (\d+) clicks')
    reactions, times = zip(*parser.findall(data))
    return ([int(r) for r in reactions], [int(t)/1000000 for t in times])


if __name__ == '__main__':
    main()
