"""Restore original output entities"""

import sys

from output_molecules import SimulationFiles, OutputEntities


def main():
    files = SimulationFiles(sys.argv[1])
    params = OutputEntities(files.input_path('params.in'))
    params.revert()


if __name__ == '__main__':
    main()
