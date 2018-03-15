"""Export files needed for MyBacteria simulation of Paulsson model."""

from __future__ import absolute_import, division, print_function

from os import path
from collections import namedtuple

Rates = namedtuple('Rates', 'bsu promoter rna_deg rbs prot_deg')


def main():
    """Export files for MyBacteria."""
    import sys
    output_dir = sys.argv[1]
    rates, initial_values = read_rates()
    export_simulation(rates, initial_values, output_dir)


def read_rates():
    """Read rna and protein production and degradation rates."""
    with open('input/parametre_simulation.csv') as f:
        # skip header
        next(f)
        rates = []
        initial_values = {}
        for line in f:
            fields = line.rstrip().split(';')
            bsu = fields[0]
            initial_values[bsu] = (int(float(fields[2])),
                                   int(float(fields[3])))
            rates.append(Rates(bsu, *fields[4:]))
    return rates, initial_values


def export_simulation(rates, initial_values, output_dir):
    """Write simulation files."""
    # write chemical species
    bsus = [r.bsu for r in rates]
    lines = ['FreeChemical rna_tracker 0\n'
             'FreeChemical protein_tracker 0\n\n']
    species_fmt = ('FreeChemical {0}_rna {1}\n'
                   'FreeChemical {0}_prot {2}\n')
    for bsu in bsus:
        rna_0, prot_0 = initial_values[bsu]
        lines.append(species_fmt.format(bsu, rna_0, prot_0))
    with open(path.join(output_dir, 'species.in'), 'w') as f:
        f.write(''.join(lines))
    with open(path.join(output_dir, 'output_species.in'), 'w') as f:
        f.write(' '.join([bsu + '_rna' for bsu in bsus]
                         + [bsu + '_prot' for bsu in bsus]))
    # write reactions
    reaction_fmt = (
        'ChemicalReaction {0}_rna 1 rna_tracker 1 rates {1} 0\n'
        'ChemicalReaction {0}_rna -1 rates {2} 0\n'
        'ChemicalReaction {0}_rna -1 {0}_rna 1 {0}_prot 1'
        ' protein_tracker 1 rates {3} 0\n'
        'ChemicalReaction {0}_prot -1 rates {4} 0\n\n'
        )
    lines = [reaction_fmt.format(*r) for r in rates]
    with open(path.join(output_dir, 'reactions.in'), 'w') as f:
        f.write(''.join(lines))


if __name__ == '__main__':
    main()
