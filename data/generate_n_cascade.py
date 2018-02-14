"""Generate n cascades."""

from __future__ import absolute_import, division, print_function
import sys

from ncascade.ncascade import NCascade


def main():
    script_name, n_chemicals, initial_value = sys.argv
    n_cascade = NCascade(int(n_chemicals), int(initial_value))
    with open('test.in', 'w') as bipsim_output:
        n_cascade.to_bipsim_format(bipsim_output)
    with open('test.xml', 'w') as sbml_output:
        n_cascade.to_sbml(sbml_output)


if __name__ == '__main__':
    main()
