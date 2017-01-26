
from processexport import *

class TranscriptionExport(ProcessExport):
    def __init__(self):
        self.set_test_parameters()

    def set_test_parameters(self):
        self._sigma_factors = ['A','B']
        self._absent_sigma_factors = ['D','EF','GF','H','I','K','L','WXY','-']
        self.agregated_TUs = []
        # initial values
        self.RNAP = 1000
        self.mg = 1000
        self.nuta = 100
        self.sigma = 1000
        # rates
        self.k_on = 10
        self.k_off = 1
        self.loading = 1
        self.mg_recruitment = 1
        self.translocation = 6
        self.release = 1
        self.agregation_rate = 5
        self.degradation = 9e-6
        
    def set_AN_parameters(self):
        self.set_test_parameters()
        self._sigma_factors = ['A']
        self._absent_sigma_factors = ['B','D','EF','GF','H','I','K','L','WXY','-']
        # initial values
        # rates
        self.k_on = 1
        self.k_off = 0.1
        self.loading = 5e-3
        self.mg_recruitment = 500
        self.translocation = 50
        self.release = 100
        self.agregation_rate = 50
        self.degradation = -1

    def set_paulsson_parameters(self):
        self.set_AN_parameters()
        # half life of 2.6min = 156s
        self.degradation = log(2) / 156

    def write_input(self, output_stream):
        lines = self._header('Transcription input')
        lines += self._free_chemical(['RNAP'], [self.RNAP])
        for s in self._sigma_factors:
            lines += self._free_chemical(['Sig'+s], [self.sigma])
        for s in self._absent_sigma_factors:
            lines += self._free_chemical(['Sig'+s])
        lines += self._free_chemical(['Mg2+'],[self.mg])
        lines += self._free_chemical(['NutA'], [self.nuta])
        lines += '# not a real chemical: only used to count production.\n'
        lines += self._free_chemical(['rna_tracker'])
        lines += '\n'
        output_stream.write(lines)
        
    def write_pre_initiation(self, output_stream):
        lines = self._header('Transcription pre-initiation')
        for s in self._sigma_factors + self._absent_sigma_factors:
            lines += self._free_chemical(['RNAP_Sig'+s])
        lines += '\n'
        for s in self._sigma_factors:
            lines += 'ChemicalReaction RNAP -1 Sig' + s + ' -1 ' \
                     + 'RNAP_Sig' + s + ' 1 rates 1 1\n'
        lines += '\n'
        output_stream.write(lines)
            
    def write_initiation(self, output_stream):
        lines = self._header('Transcription initiation')
        for s in self._sigma_factors:
            lines += self._bound_chemical(['bound_RNAP_Sig'+s, \
                                           'stable_RNAP_Sig'+s,])
        lines += self._bound_chemical(['stable_RNAP'])
        lines += '\n'
        for s in self._sigma_factors:
            lines += 'SequenceBinding RNAP_Sig' + s + ' bound_RNAP_Sig' + s \
                     + ' promoter_Sig' + s + '\n'
            lines += 'ChemicalReaction bound_RNAP_Sig' + s + ' -1 ' \
                     + 'stable_RNAP_Sig' + s + ' 1 rates 1 0\n'
            lines += 'ChemicalReaction stable_RNAP_Sig' + s + ' -1 ' \
                     + 'stable_RNAP 1 Sig' + s + ' 1 rates 1 0\n'
        lines += '\n'
        output_stream.write(lines)

    def write_elongation(self, output_stream):
        lines = self._header('Transcription elongation')
        lines += self._bound_chemical(['loaded_RNAP', 'stalled_RNAP', \
                                       'translocating_RNAP'])
        lines += '\n'
        templates = ['A', 'C', 'G', 'T']
        to_load = ['UTP', 'GTP', 'CTP', 'ATP']
        loading_cases = [t + ' ' + l + ' loaded_RNAP ' + str(self.loading) \
                         for t, l in zip(templates, to_load)]
        lines += 'LoadingTable NTP_loading ' + ', '.join(loading_cases) + '\n'
        lines += 'ProductLoading stable_RNAP NTP_loading\n'
        lines += 'ChemicalReaction loaded_RNAP -1 Mg2+ -2 ' \
                 + 'translocating_RNAP 1 PPi 1 Mg2+ 2 rates ' \
                 + str(self.mg_recruitment) + ' 0\n'
        lines += 'Translocation translocating_RNAP stable_RNAP ' \
                 + 'stalled_RNAP 1 ' + str(self.translocation) + '\n'
        lines += '\n'
        output_stream.write(lines)

    def write_agregated_elongation(self, output_stream, TUs):
        if len(TUs) == 0: return
        lines = self._header('Agregated transcription elongation')
        output_stream.write(lines)
        for TU in TUs:
            if TU.sense == 1:
                parent = 'sensedna'
            else:
                parent = 'antisensedna'
            name = TU.name
            name_RNAP = 'RNAP_' + name
            active_RNAP = 'transcribing_' + name_RNAP
            lines = self._bound_chemical([name_RNAP, active_RNAP])
            # switch once sigma_factor is released
            switch_name = name + '_translation'
            lines += 'Switch ' +  switch_name + ' ' \
                     + 'stable_RNAP ' + name_RNAP + '\n'
            lines += 'SwitchSite ' + parent + ' ' + str(TU.start) + ' ' \
                     + switch_name + '\n'
            # translocate RNAP further into cds
            lines += '# arbitrary translocation inside TU.\n'
            lines += 'Translocation ' + name_RNAP + ' ' + \
                     active_RNAP + ' ' + active_RNAP + ' 50 1\n'
            rate = float(self.agregation_rate) / sum(TU.composition)
            lines += 'ChemicalReaction ' + active_RNAP + ' -1 ' \
                     + self._TU_reactants(TU) + ' ' \
                     + 'RNAP' + ' 1 ' + TU.sigma + ' 1 ' + TU.name + ' 1 '\
                     + 'rna_tracker 1 rates ' + str(rate) + ' 0\n'
            lines += '\n'
            output_stream.write(lines)

    def write_termination(self, output_stream):
        lines = self._header('Transcription termination')
        lines += self._bound_chemical(['empty_RNAP','terminating_RNAP'])
        lines += self._bound_chemical(['continuing_RNAP'])
        lines += '\n'
        lines += 'Switch hairpin stable_RNAP terminating_RNAP\n'
        lines += 'Release terminating_RNAP empty_RNAP continuing_RNAP rnas ' \
                 + str(self.release) + '\n'
        lines += 'ChemicalReaction empty_RNAP -1 NutA -1 NutA 1 RNAP 1 ' \
                 + 'rna_tracker 1 rates ' + str(self.release) + ' 0\n'
        lines += 'ProductLoading continuing_RNAP NTP_loading\n'
        lines += '\n'
        output_stream.write(lines)

    def write_TUs(self, output_stream, TUs):
        lines = self._header('General information')
        lines += 'TransformationTable dna2rna A U, C G, G C, T A\n'
        lines += 'ProductTable rnas dna2rna\n'
        lines += '\n'
        lines += self._header('Sequence information')
        output_stream.write(lines)
        for TU in TUs:
            # handle TU output
            if TU.sense == 1:
                parent = 'sensedna'
            else:
                parent = 'antisensedna'
            lines = 'ChemicalSequence ' + TU.name \
                    + ' product_of ' + parent + ' ' \
                    + str(TU.start) + ' ' + str(TU.end) \
                    + ' rnas\n'
            promoter_start = TU.start - 35
            promoter_end = TU.start + 1
            reading_frame = TU.start
            lines += 'BindingSite promoter_' + TU.sigma \
                     + ' ' + parent + ' ' + str(promoter_start) \
                     + ' ' + str(promoter_end) + ' ' + str(self.k_on) \
                     + ' ' + str(self.k_off) + ' ' + str(reading_frame) + '\n'
            terminator = str(TU.end + 1)
            lines += 'SwitchSite ' + parent + ' ' + terminator + ' hairpin\n'
            output_stream.write(lines)
        lines = '\n'
        output_stream.write(lines)

    def write_degradation(self, output_stream, TUs):
        if self.degradation <= 0: return
        lines = self._header('Degradation')
        lines += 'CompositionTable rna_composition A AMP, C CMP, G GMP, U UMP\n'
        lines += '\n'
        for TU in TUs:
            lines += 'Degradation ' + TU.name + ' rna_composition ' \
                     + str(self.degradation) + '\n'
        lines += '\n'
        output_stream.write(lines)

    def _TU_reactants(self, TU):
        nb_bases = sum(TU.composition)
        return 'ATP ' + str(-TU.composition[0]) + ' order 0 ' \
            + 'CTP ' + str(-TU.composition[1]) + ' order 0 ' \
            + 'GTP ' + str(-TU.composition[2]) + ' order 0 ' \
            + 'UTP ' + str(-TU.composition[3]) + ' order 0 ' \
            + 'Mg2+ ' + str(-2) + ' order 0 ' \
            + 'PPi ' + str(nb_bases) + ' Mg2+ ' + str(2)
