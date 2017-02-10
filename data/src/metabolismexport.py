
from processexport import *

class MetabolismExport(ProcessExport):
    def __init__(self):
        super(MetabolismExport, self).__init__()
        self._aas = ['A','C','D','E','F','G','H','I','K','L','M','N', \
                    'P','Q','R','S','T','V','W','Y']
        self._nucleotides = ['A', 'C', 'G', 'U']
        self._dnucleotides = ['A', 'C', 'G', 'T']
        self.set_test_parameters()

    def set_test_parameters(self):
        # initial values
        self.dNTPs = [1e7]*4
        self.NTPs = [1.2e6, 1e6, 1e6, 1e6]
        # rates
        self.constant = False

    def set_AN_parameters(self):
        self.set_test_parameters()
        # initial values
        self.dNTPs = [1e8]*4
        self.NTPs = [4.2e6, 0.12e6, 3e6, 0.3e6]

    def set_paulsson_parameters(self):
        self.set_AN_parameters()
        # initial values
        self.dNTPs = [-1]*4
        
    def write_input(self, output_stream):
        lines = self._header('Metabolites')
        assert(len(self._nucleotides) == len(self.NTPs))
        if sum(self.dNTPs) > 0:
            for n, num in zip(self._dnucleotides, self.dNTPs):
                lines += self._free_chemical(['d' + n + 'TP'], [num])
                lines += self._free_chemical(['d' + n + 'MP'])
        for n, num in zip(self._nucleotides, self.NTPs):
            lines += self._free_chemical([n + 'TP'], [num], \
                                         self.cut_slow_reactions)
            lines += self._free_chemical([n + 'DP'], [0], \
                                         self.cut_slow_reactions)
            lines += self._free_chemical([n + 'MP'], [0], \
                                         self.cut_slow_reactions)
        lines += self._free_chemical(['aa_' + aa for aa in self._aas])
        lines += self._free_chemical(['Pi', 'PPi', 'ppGpp'], \
                                     [0]*3, self.cut_slow_reactions)
        lines += '\n'
        output_stream.write(lines)

    def write_pseudo_metabolism(self, output_stream):
        if self.cut_slow_reactions: return
        lines = self._header('Pseudo metabolism')
        lines += 'ChemicalReaction PPi -' + self._stoichiometry(1) \
                 + ' Pi ' + self._stoichiometry(2) + ' rates 1 0\n'
        lines += 'ChemicalReaction GDP -' + self._stoichiometry(1) \
                 + ' Pi -' + self._stoichiometry(1) \
                 + ' GTP ' + self._stoichiometry(1) + ' rates 1 0\n'
        for n in self._nucleotides:
            lines += 'ChemicalReaction ' + n + 'MP -' + self._stoichiometry(1) \
                     + ' Pi -' + self._stoichiometry(2) \
                     + ' ' + n + 'TP ' + self._stoichiometry(1) + ' rates 1 0\n'
        lines += '\n'
        output_stream.write(lines)

    def _stoichiometry(self, value):
        if self.agregate_slow_reactions:
            return str(value*1000) + ' order ' + str(value)
        else:
            return str(value)
