
from processexport import *

class ReplicationExport(ProcessExport):
    def __init__(self):
        self.set_test_parameters()

    def set_test_parameters(self):
        # initial values
        self.DNAP = 10
        # rates
        self.loading = 1
        self.ligation = 1
        self.translocation = 10
        self.recruitment = 1000
        self.release = 1

    def set_AN_parameters(self):
        self.set_test_parameters()
        # initial values
        # rates
        self.loading = 1e-3
        self.ligation = 10000
        self.translocation = 750
        self.recruitment = 5
        self.release = 1000

    def write_input(self, output_stream):
        lines = self._header('Replication input')
        lines += self._free_chemical(['DnaA_GTP'], [2])
        lines += self._free_chemical(['free_DNAP'], [self.DNAP])
        lines += '\n'
        output_stream.write(lines)

    def write_initiation(self, output_stream):
        lines = self._header('Replication initiation')
        lines += self._free_chemical(['DnaA_GDP'])
        lines += self._bound_chemical(['bound_DnaA', 'DNAP_fork'])
        lines += '\n'
        lines += 'BindingSite DnaA_BS sensedna 1 1 100000 0 1\n'
        lines += 'BindingSite DnaA_BS antisensedna 1 1 100000 0 1\n'
        lines += '\n'
        lines += 'SequenceBinding DnaA_GTP bound_DnaA DnaA_BS\n'
        lines += 'ChemicalReaction free_DNAP -1 bound_DnaA -1 ' \
                 + 'DNAP_fork 1 DnaA_GDP 1 Pi 1 rates 1 0\n'
        lines += '\n'
        output_stream.write(lines)

    def write_elongation(self, output_stream):
        lines = self._header('Replication elongation')
        lines += self._bound_chemical(['DNAP_lagging'])
        for end in ['fork', 'lagging']:
            for start in ['loaded', 'translocating', 'stalled']:
                lines += self._bound_chemical([start + '_DNAP_' + end])
        lines += '\n'
        lines += '# Leading strand\n'
        lines += self._elongation_reactions('fork')
        lines += '\n'
        lines += '# Lagging strand\n'
        lines += 'DoubleStrandRecruitment DNAP_fork free_DNAP DNAP_lagging ' \
                 + str(self.recruitment) + '\n'
        lines += self._elongation_reactions('lagging')
        lines += '\n'
        output_stream.write(lines)

    def _elongation_reactions(self, tag):
        templates = ['A', 'C', 'G', 'T']
        to_load = ['dTTP', 'dGTP', 'dCTP', 'dATP']
        loading_cases = [t + ' ' + l + ' loaded_DNAP_' + tag \
                         + ' ' + str(self.loading) \
                         for t, l in zip(templates, to_load)]
        lines = 'LoadingTable dNTP_loading_' + tag + ' ' \
                 + ', '.join(loading_cases) + '\n'
        lines += 'DoubleStrandLoading DNAP_' + tag + ' ' \
                 + 'dNTP_loading_' + tag + ' ' + 'stalled_DNAP_' + tag + '\n'
        lines += 'ChemicalReaction loaded_DNAP_' + tag + ' -1 PPi 1 ' \
                 + 'translocating_DNAP_' + tag + ' 1 rates ' \
                 + str(self.ligation) + ' 0\n'
        lines += 'Translocation translocating_DNAP_' + tag \
                 + ' DNAP_' + tag + ' ' + 'stalled_DNAP_' + tag + ' ' \
                 + '1 ' + str(self.translocation) + '\n'
        lines += 'ChemicalReaction stalled_DNAP_' + tag + ' -1 free_DNAP 1 ' \
                 + 'rates ' + str(self.release) + ' 0\n'
        return lines
