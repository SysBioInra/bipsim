"""Classes exporting metabolism files for MyBacteria."""

from __future__ import absolute_import, division, print_function
from simgenerator.lineformat import header, free_chemical, bound_chemical


class MetabolismExport(object):
    """Export metabolism files for MyBacteria."""

    def __init__(self, params):
        """Make export object from parameters."""
        self._aas = ['A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M',
                     'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'Y']
        self._nucleotides = ['A', 'C', 'G', 'U']
        self._dnucleotides = ['A', 'C', 'G', 'T']
        self.params = params

    def input(self):
        """Return input section of metabolism file."""
        lines = [header('Metabolites')]
        if sum(self.params.dNTPs) > 0:
            fmt = (free_chemical(['d{0}TP'], ['{1}'])
                   + free_chemical(['d{0}MP']))
            lines += [fmt.format(n, num)
                      for n, num in zip(self._dnucleotides, self.params.dNTPs)]
        fmt = (free_chemical(['{0}TP'], ['{1}'], True)
               + free_chemical(['{0}DP', '{0}MP'], [0, 0],
                               self.params.cut_slow_reactions))
        lines += [fmt.format(n, num)
                  for n, num in zip(self._nucleotides, self.params.NTPs)]
        lines.append(free_chemical(['aa_' + aa for aa in self._aas]))
        lines.append(free_chemical(['Pi', 'PPi', 'ppGpp'], [0]*3,
                                   self.params.cut_slow_reactions))
        lines.append('\n')
        return ''.join(lines)

    def pseudo_metabolism(self):
        """Return reaction section of metabolism file."""
        if self.params.cut_slow_reactions:
            return ''
        sto_1 = self._stoichiometry(1)
        sto_2 = self._stoichiometry(2)
        lines = [header('Pseudo metabolism')]
        lines.append('ChemicalReaction PPi -{} Pi {} rates 1 0\n'
                     .format(sto_1, sto_2))
        lines.append('ChemicalReaction GDP -{0} Pi -{0} GTP {0} rates 1 0\n'
                     .format(sto_1))
        fmt = ('ChemicalReaction {{0}}MP -{0} Pi -{1} {{0}}TP {0} rates 1 0\n'
               .format(sto_1, sto_2))
        lines += [fmt.format(n) for n in self._nucleotides]
        lines.append('\n')
        return ''.join(lines)

    def _stoichiometry(self, value):
        if self.params.aggregate_slow_reactions:
            return str(value*1000) + ' order ' + str(value)
        else:
            return str(value)
