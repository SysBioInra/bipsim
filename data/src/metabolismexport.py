
from processexport import *

class MetabolismExport(ProcessExport):
    def __init__(self):
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
        self.gdp_recycling = 1
        self.nmp_recycling = 1

    def set_AN_detailed_parameters(self):
        # initial values
        self.dNTPs = [1e8]*4
        self.NTPs = [4.2e6, 0.12e6, 3e6, 0.3e6]
        # rates

    def write_input(self, output_stream):
        lines = self._header('Metabolites')
        assert(len(self._nucleotides) == len(self.NTPs))
        for n, num in zip(self._dnucleotides, self.dNTPs):
            lines += self._free_chemical(['d' + n + 'TP'], [num])
            lines += self._free_chemical(['d' + n + 'MP'])
        for n, num in zip(self._nucleotides, self.NTPs):
            lines += self._free_chemical([n + 'TP'], [num])
            lines += self._free_chemical([n + 'DP'])
            lines += self._free_chemical([n + 'MP'])
        lines += self._free_chemical(['aa_' + aa for aa in self._aas])
        lines += self._free_chemical(['Pi', 'PPi', 'ppGpp'])
        lines += '\n'
        output_stream.write(lines)

    def write_pseudo_metabolism(self, output_stream):
        lines = self._header('Pseudo metabolism')
        lines += 'ChemicalReaction PPi -1 Pi 2 rates 1 0\n'
        lines += 'ChemicalReaction GDP -1 Pi -1 GTP 1 rates ' \
                 + str(self.gdp_recycling) + ' 0\n'
        for n in self._nucleotides:
            lines += 'ChemicalReaction ' + n + 'MP -1 Pi -2 ATP 1 rates ' \
                     + str(self.nmp_recycling) + ' 0\n'
        lines += '\n'
        output_stream.write(lines)
