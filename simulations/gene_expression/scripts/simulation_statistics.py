"""Compute simulation statistics."""

from __future__ import absolute_import, print_function, division

import sys
from os import path
import numpy
import re


def main():
    print('\n'.join(s.to_string() for s in simulations()))


def simulations():
    sim_set = ['aggregated']*2 + ['hybrid']*2 + ['detailed']*3
    metab = ['constant', 'stacked'] * 3 + ['detailed']
    return [SimulationTimes(s, m) for s, m in zip(sim_set, meta)]


class SimulationTimes(object):
    def __init__(self, processes, metabolism):
        self.processes = processes
        self.metabolism = metabolism
        self._parse_results()

    def _parse_results(self):
        with open(self._log_file(), 'r') as f:
            data = f.read()
        parser = re.compile(r'(\d+) reactions occurred.\n'
                            r'CPU runtime: (\d+) clicks')
        reactions, times = zip(*parser.findall(data))
        self.reactions = [int(r) for r in reactions]
        self.times = [int(t)/1000000 for t in times]

    def _log_file(self):
        return path('{}_{}'.format(self.processes, self.metabolism), 'log.txt')

    def name(self):
        return '{}_{}'.format(self.processes, self.metabolism)

    def to_string(self):
        return ''.join(
            self._string_line(r, t) for r, t in zip(self.reactions, self.times)
        )

    def _string_line(self, nb_reaction, total_time):
        min, sec = divmod(int(total_time), 60)
        hour, min = divmod(int(min), 60)
        return ('{}:\n\t{} reactions, {}h {}m {}s, ({} reactions/s)\n\n'
                .format(self.name(), nb_reaction, hour, min, sec,
                        nb_reaction / total_time))


if __name__ == '__main__':
    main()
