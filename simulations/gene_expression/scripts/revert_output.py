"""Restore original output entities"""

import sys

from output_molecules import Input, OutputEntities


def main():
    input_ = Input(sys.argv[1])
    params = OutputEntities(input_.path('params.in'))
    params.revert()


if __name__ == '__main__':
    main()
