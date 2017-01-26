
from processexport import *

class TranslationExport(ProcessExport):
    def __init__(self):
        self._aas = ['A','C','D','E','F','G','H','I','K','L','M','N', \
                    'P','Q','R','S','T','V','W','Y']
        self._genetic_code = {}
        self._genetic_code['F'] = ['UU' + n for n in ['C','U']]
        self._genetic_code['L'] = ['UU' + n for n in ['A','G']] \
                                  + ['CU' + n for n in ['A','C','G','U']]
        self._genetic_code['I'] = ['AU' + n for n in ['A','C','U']]
        self._genetic_code['M'] = ['AUG']
        self._genetic_code['V'] = ['GU' + n for n in ['A','C','G','U']]
        self._genetic_code['S'] = ['UC' + n for n in ['A','C','G','U']]
        self._genetic_code['P'] = ['CC' + n for n in ['A','C','G','U']]
        self._genetic_code['T'] = ['AC' + n for n in ['A','C','G','U']]
        self._genetic_code['A'] = ['GC' + n for n in ['A','C','G','U']]
        self._genetic_code['Y'] = ['UA' + n for n in ['C','U']]
        self._genetic_code['H'] = ['CA' + n for n in ['C','U']]
        self._genetic_code['Q'] = ['CA' + n for n in ['A','G']]
        self._genetic_code['N'] = ['AA' + n for n in ['C','U']]
        self._genetic_code['K'] = ['AA' + n for n in ['A','G']]
        self._genetic_code['D'] = ['GA' + n for n in ['C','U']]
        self._genetic_code['E'] = ['GA' + n for n in ['A','G']]        
        self._genetic_code['C'] = ['UG' + n for n in ['C','U']]
        self._genetic_code['W'] = ['UGG']
        self._genetic_code['R'] = ['CG' + n for n in ['A','C','G','U']] \
                                  + ['AG' + n for n in ['A','G']]
        self._genetic_code['S'] += ['AG' + n for n in ['C','U']]
        self._genetic_code['G'] = ['GG' + n for n in ['A','C','G','U']]
        self._nb_codons = [len(self._genetic_code[aa]) for aa in self._aas]
        self._stop_codons = ['UAA','UAG','UGA']
        self.set_test_parameters()

    def set_test_parameters(self):
        # initial values
        self.nb_ribosomes = 1000
        self.IF_123 = [100, 400, 100]
        self.EF_Tu_G_RF = [60000, 100, 1000]
        self.fMet = 300
        self.nb_tRNAs = 1000
        # rates
        self.EFTu_activation = 1
        self.k_on = 10
        self.k_off = 1
        self.loading = 1
        self.translocation = 200
        self.transpeptidation = 1
        self.release = 1
        self.agregation_rate = 1
        self.degradation = -1
        
    def set_AN_parameters(self):
        self.set_test_parameters()
        # initial values
        self.nb_ribosomes = 22000
        self.IF_123 = [5500, 6600, 4400]
        self.EF_Tu_G_RF = [118000, 15000, 4000]
        self.fMet = 3000
        self.nb_tRNAs = 4000
        # rates
        self.EFTu_activation = 1
        self.k_on = 1
        self.k_off = 0.1
        self.loading = 1e-3
        self.translocation = 150
        self.transpeptidation = 150
        self.release = 150
        self.agregation_rate = 8
        self.degradation = -1

    def set_paulsson_parameters(self):
        self.set_AN_parameters()
        # equal to growth rate 40min-1 = 2400s-1
        self.degradation = 2400

    def write_input(self, output_stream):
        lines = self._header('Translation input')
        lines += self._free_chemical(['30S','50S'], \
                                     [self.nb_ribosomes]*2)
        lines += self._free_chemical(['IF1','IF2','IF3'], self.IF_123)
        lines += self._free_chemical(['EFTud','EFG','RF'], \
                                     self.EF_Tu_G_RF)
        lines += self._free_chemical(['tRNA_fM'], [self.fMet])
        lines += self._free_chemical(['tRNA_' + aa for aa in self._aas], \
                                     [self.nb_tRNAs*i for i in self._nb_codons])
        lines += '\n'
        lines += '# not a real chemical, used to count production\n'
        lines += self._free_chemical(['protein_tracker'])
        lines += '\n'
        output_stream.write(lines)

    def write_tRNA_activation(self, output_stream):
        aas_fMet = ['fM'] + self._aas
        lines = self._header('tRNA activation')
        lines += self._free_chemical(['tRNA_' + aa + '_c' for aa in aas_fMet])
        lines += '\n'
        lines += self._free_chemical(['EFTua'])
        lines += '\n'
        lines += self._free_chemical(['EtRNA_' + aa + '_ca' for aa in self._aas])
        lines += '\n'
        for aa in aas_fMet:
            lines += 'ChemicalReaction tRNA_' + aa + ' -1 ATP -1 ' \
                     + 'AMP 1 PPi 1 tRNA_' + aa + '_c 1 rates 1 0\n'
        lines += '\n'
        lines += 'ChemicalReaction EFTud -1 GTP -1 EFTua 1 GDP 1 rates ' \
                 + str(self.EFTu_activation) + ' 0\n'
        lines += '\n'
        for aa in self._aas:
            lines += 'ChemicalReaction tRNA_' + aa + '_c -1 EFTua -1 ' \
                     + 'EtRNA_' + aa + '_ca 1 rates 1 0\n'
        lines += '\n'
        output_stream.write(lines)

    def write_pre_initiation(self, output_stream):
        lines = self._header('Translation pre-initiation')
        lines += self._free_chemical(['30S1', '30S3', '30S31'])
        lines += '\n'
        lines += 'ChemicalReaction 30S -1 IF3 -1 30S3 1 rates 1 1\n'
        lines += 'ChemicalReaction 30S3 -1 IF1 -1 30S31 1 rates 1 1\n'
        lines += '\n'
        output_stream.write(lines)

    def write_initiation(self, output_stream):
        lines = self._header('Translation initiation')
        lines += self._bound_chemical(['m30S31','m30S312a','m30S312d', \
                                       'm30S12a', 'translocating_70S'])
        lines += self._free_chemical(['IF2a', 'IF2d', 'IF2p'])
        lines += self._free_chemical(['fMet2a','fMet2d', 'fMet2p'])
        lines += '\n'
        lines += 'SequenceBinding 30S31 m30S31 RBS\n'
        lines += 'ChemicalReaction IF2 -1 GTP -1 IF2a 1 rates 1 1\n'
        lines += 'ChemicalReaction IF2a -1 tRNA_fM_c -1 fMet2a 1 rates 1 1\n'
        lines += 'ChemicalReaction IF2 -1 GDP -1 IF2d 1 rates 1 10\n'
        lines += 'ChemicalReaction IF2d -1 tRNA_fM_c -1 fMet2d 1 rates 1 10\n'
        lines += 'ChemicalReaction IF2 -1 ppGpp -1 IF2p 1 rates 1 1\n'
        lines += 'ChemicalReaction IF2p -1 tRNA_fM_c -1 fMet2p 1 rates 1 1\n'
        lines += 'ChemicalReaction fMet2a -1 m30S31 -1 m30S312a 1 rates 1 0\n'
        lines += 'ChemicalReaction fMet2d -1 m30S31 -1 m30S312d 1 rates 1 10\n'
        lines += 'ChemicalReaction m30S312a -1 IF3 1 m30S12a 1 rates 1 0\n'
        lines += 'ChemicalReaction m30S12a -1 50S -1 ' \
                 + 'translocating_70S 1 IF1 1 IF2d 1 Pi 1 tRNA_fM 1 ' \
                 + 'rates 1 0\n'
        lines += '\n'
        output_stream.write(lines)

    def write_elongation(self, output_stream):        
        lines = self._header('Translation elongation')
        lines += self._bound_chemical(['stalled_ribosome', '70S', \
                                       'pre_translocation_70S'])
        lines += self._bound_chemical(['Et70S_' + aa for aa in self._aas])
        lines += '\n'
        lines += '# translocation\n'
        lines += 'ChemicalReaction pre_translocation_70S -1 EFG -1 GTP -1 ' \
                 + 'translocating_70S 1 EFG 1 GDP 1 Pi 1 rates ' \
                 + str(self.translocation) + ' 0 \n'
        lines += 'Translocation translocating_70S 70S stalled_ribosome 3 ' \
                 + str(self.translocation) + '\n'
        lines += '\n'
        lines += '# tRNA loading\n'
        lines += 'LoadingTable EtRNA_loading '
        for aa in self._aas:
            for codon in self._genetic_code[aa]:
                lines += codon + ' EtRNA_' + aa + '_ca ' \
                         + 'Et70S_' + aa + ' ' + str(self.loading) + ', '
        lines += ', '.join([c + ' RF stalled_ribosome 1' for c in self._stop_codons]) + '\n'
        lines += 'ProductLoading 70S EtRNA_loading\n'
        lines += '\n'
        lines += '# transpeptidation\n'
        for aa in self._aas:
            lines += 'ChemicalReaction Et70S_' + aa + ' -1 ' \
                     + 'pre_translocation_70S 1 tRNA_' + aa + ' 1 ' \
                     + 'EFTud 1 Pi 1 rates ' \
                     + str(self.transpeptidation) + ' 0\n'
        lines += '\n'
        output_stream.write(lines)

    def write_termination(self, output_stream):        
        lines = self._header('Translation termination')
        lines += self._bound_chemical(['empty_ribosome'])
        lines += '\n'
        lines += 'Release stalled_ribosome empty_ribosome 70S proteins ' \
                 + str(self.release) + '\n'
        lines += 'ChemicalReaction empty_ribosome -1 30S 1 50S 1 ' \
                 + 'protein_tracker 1 RF 1 rates ' + str(self.release) + ' 0\n'
        lines += '\n'
        output_stream.write(lines)

    def write_agregated_elongation(self, output_stream, TUs):
        if len(TUs) == 0: return
        lines = self._header('Agregated translation elongation')
        output_stream.write(lines)
        known_70S = []
        for TU in TUs:
            parent = TU.name
            for g in TU.genes:
                name = g.name + '_' + g.bsu
                name_70S = '70S_' + name
                active_70S = 'translating_' + name_70S
                switch_name = name + '_translation'
                # every protein has one switch per TU
                lines = 'SwitchSite ' + parent + ' ' + str(g.start) + ' ' \
                         + switch_name + '\n'
                # however, because a protein may be on several TUs, its reaction
                # may already have been written before.
                if not (name_70S in known_70S):
                    known_70S.append(name_70S)
                    lines += self._bound_chemical([name_70S, active_70S])
                    # switch once fMet loading is completed
                    lines += 'Switch ' +  switch_name + ' ' \
                             + 'translocating_70S ' + name_70S + '\n'
                    # translocate 70S further into protein
                    lines += '# arbitrary translocation inside cds.\n'
                    lines += 'Translocation ' + name_70S + ' ' + \
                             active_70S + ' ' + active_70S + ' 50 1\n'
                    # rest of elongation
                    rate = float(self.agregation_rate) / sum(g.aa_composition)
                    lines += 'ChemicalReaction ' + active_70S + ' -1 ' \
                            + self._protein_reactants(g) + ' ' \
                            + '30S 1 50S 1 ' + name + ' 1 ' \
                            + 'protein_tracker 1 rates ' + str(rate) + ' 0\n'
                lines += '\n'
                output_stream.write(lines)

    def write_proteins(self, output_stream, TUs):
        lines = self._header('General information')
        codon_aa = []
        for aa in self._aas:
            for codon in self._genetic_code[aa]:
                codon_aa.append(codon + ' ' + aa)
        lines += 'TransformationTable rna2protein ' + ', '.join(codon_aa) + '\n'
        lines += 'ProductTable proteins rna2protein\n'
        lines += '\n'
        lines += self._header('Sequence information')
        output_stream.write(lines)
        for TU in TUs:
            for g in TU.genes:
                name = g.name + '_' + g.bsu
                lines = 'ChemicalSequence ' + name + ' ' \
                        + 'product_of ' + TU.name + ' ' \
                        + str(g.start) + ' ' + str(g.end-3) + ' proteins\n'
                lines += 'BindingSite RBS ' + TU.name + ' ' \
                         + str(g.rbs_start) + ' ' + str(g.rbs_end) + ' ' \
                         + str(self.k_on) + ' ' + str(self.k_off) + ' ' \
                         + str(g.start) + '\n'
                output_stream.write(lines)            
        lines = '\n'
        output_stream.write(lines)

    def write_degradation(self, output_stream, TUs):
        if self.degradation <= 0: return
        lines = self._header('Degradation')
        lines += 'CompositionTable protein_composition ' \
                 + ', '.join([aa + ' aa_' + aa for aa in self._aas]) \
                 + '\n'
        lines += '\n'
        already_treated = []
        for TU in TUs:
            for g in TU.genes:
                name = g.name + '_' + g.bsu
                if not(name in already_treated):
                    already_treated.append(name)
                    lines += 'Degradation ' + name + ' protein_composition ' \
                             + str(self.degradation) + '\n'
        lines += '\n'
        output_stream.write(lines)

                
    def _protein_reactants(self, gene):
        nb_bases = sum(gene.aa_composition)
        reaction = ''
        assert(len(self._aas) == len(gene.aa_composition))
        for i, aa in enumerate(self._aas):
            sto = gene.aa_composition[i]
            reaction += 'EtRNA_' + aa + '_ca ' + str(-sto) + ' order 0 ' \
                        + 'tRNA_' + aa + ' ' + str(sto) + ' '
        reaction += 'EFG ' + str(-1) + ' order 0 ' \
                    + 'GTP ' + str(-nb_bases) + ' order 0 ' \
                    + 'RF -1 order 0 ' \
                    + 'EFG ' + str(1) + ' ' \
                    + 'EFTud ' + str(nb_bases) + ' order 0 ' \
                    + 'GDP ' + str(nb_bases) + ' ' \
                    + 'Pi ' + str(2*nb_bases) + ' ' \
                    + 'RF ' + str(1)
        return reaction
