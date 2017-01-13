
class SimulationExport:
    def __init__(self):
        self._aas = ['A','C','D','E','F','G','H','I','K','L','M','N', \
                    'P','Q','R','S','T','V','W','Y']
        self._aa_abundance = [4,2,2,2,4,4,2,3,4,4,1,2,4,2,6,6,4,4,1,2]
        assert(len(self._aas) == len(self._aa_abundance))
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

    def write_tRNA_activation(self, output_stream):
        aas_fMet = ['fM'] + self._aas
        lines = self._header('tRNA activation')
        lines += self._free_chemical(['tRNA_' + aa for aa in aas_fMet]) + '\n'
        lines += self._free_chemical(['tRNA_' + aa + '_c' for aa in aas_fMet]) + '\n'
        lines += self._free_chemical(['EtRNA_' + aa + '_ca' for aa in self._aas], \
                                     [1000*i for i in self._nb_codons]) + '\n'
        for aa in aas_fMet:
            lines += 'ChemicalReaction tRNA_' + aa + ' -1 ATP -1 ' \
                     + 'AMP 1 PPi 1 H+ 1 tRNA_' + aa + '_c 1 rates 1 0\n'
        lines += '\n'
        for aa in self._aas:
            lines += 'ChemicalReaction tRNA_' + aa + '_c -1 EFTua -1 ' \
                     + 'EtRNA_' + aa + '_ca 1 rates 1 0\n'
        lines += '\n'
        output_stream.write(lines)

    def write_translation_pre_initiation(self, output_stream):
        lines = self._header('Translation pre-initiation')
        lines += self._free_chemical(['30S'], [1000])
        lines += self._free_chemical(['30S1', '30S3', '30S31'])
        lines += self._free_chemical(['IF1','IF3'], [100,100])
        lines += '\n'
        lines += 'ChemicalReaction 30S -1 IF3 -1 30S3 1 rates 1 1\n'
        lines += 'ChemicalReaction 30S3 -1 IF1 -1 30S31 1 rates 1 1\n'
        lines += '\n'
        output_stream.write(lines)

    def write_translation_initiation(self, output_stream):
        lines = self._header('Translation initiation')
        lines += self._free_chemical(['50S'], [1000])
        lines += self._bound_chemical(['m30S31','m30S312a','m30S312d', \
                                       'm30S12a', 'EFt70S'])
        lines += self._free_chemical(['IF2'], [100])
        lines += self._free_chemical(['IF2a', 'IF2d', 'IF2p'])
        lines += self._free_chemical(['fMet2a'], [300])
        lines += self._free_chemical(['fMet2d', 'fMet2p'])
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
                 + 'EFt70S 1 IF1 1 IF2d 1 Pi 1 H2O 1 tRNA_fM 1 rates 1 0\n'
        lines += '\n'
        output_stream.write(lines)

    def write_translation_elongation(self, output_stream):        
        lines = self._header('Translation elongation')
        lines += self._bound_chemical(['stalled_ribosome', '70S'])
        lines += self._bound_chemical(['Et70S_' + aa + '_ca' for aa in self._aas])
        lines += self._bound_chemical(['t70S_' + aa + '_c' for aa in self._aas])
        lines += self._bound_chemical(['t70S_' + aa + '_cp' for aa in self._aas])
        lines += self._free_chemical(['EFP','RF'], [100,1000])
        lines += self._free_chemical(['EFTua','EFTud','EFPa','EFPd'])
        lines += '\n'
        lines += '# translocation\n'
        lines += 'Translocation EFt70S 70S stalled_ribosome 3 200\n'
        lines += '\n'
        lines += '# tRNA loading\n'
        lines += 'LoadingTable EtRNA_loading '
        for aa in self._aas:
            for codon in self._genetic_code[aa]:
                lines += codon + ' EtRNA_' + aa + '_ca ' \
                         + 'Et70S_' + aa + '_ca 1, '
        lines += ', '.join([c + ' RF stalled_ribosome 1' for c in self._stop_codons]) + '\n'
        lines += 'ProductLoading 70S EtRNA_loading\n'
        lines += '\n'
        lines += '# transpeptidation\n'
        for aa in self._aas:
            lines += 'ChemicalReaction Et70S_' + aa + '_ca -1 ' \
                     + 't70S_' + aa + '_c 1 EFTud 1 Pi 1 rates 1 0\n'
            lines += 'ChemicalReaction t70S_' + aa + '_c -1 EFPa -1 ' \
                     + 't70S_' + aa + '_cp 1 EFPd 1 Pi 1 rates 1 0 \n'
            lines += 'ChemicalReaction t70S_' + aa + '_cp -1 GTP -1 ' \
                     + 'EFt70S 1 GDP 1 Pi 1 tRNA_' + aa + ' 1 rates 1 0 \n'
        lines += '\n'
        lines += '# elongation factor turnover\n'
        lines += 'ChemicalReaction EFP -1 GTP -1 EFPa 1 rates 1 1\n'
        lines += 'ChemicalReaction EFP -1 GDP -1 EFPd 1 rates 0.1 10\n'
        lines += 'ChemicalReaction EFTud -1 GTP -1 EFTua 1 GDP 1 rates 1 0\n'
        lines += '\n'
        output_stream.write(lines)

    def write_translation_termination(self, output_stream):        
        lines = self._header('Translation termination')
        lines += self._bound_chemical(['empty_ribosome'])
        lines += '# not a real chemical, just used to count proteins produced\n'
        lines += self._free_chemical(['protein'])
        lines += '\n'
        lines += 'Release stalled_ribosome empty_ribosome 70S proteins 1\n'
        lines += 'ChemicalReaction empty_ribosome -1 30S 1 50S 1 protein 1 RF 1 rates 1 0\n'
        lines += '\n'
        output_stream.write(lines)

    def write_transcription_pre_initiation(self, output_stream):
        sigma_factors = ['A','B']
        lines = self._header('Transcription pre-initiation')
        lines += self._free_chemical(['RNAP'], [1000])
        for s in sigma_factors:
            lines += self._free_chemical(['Sig'+s], [1000])
            lines += self._free_chemical(['RNAP_Sig'+s])
        lines += '\n'
        for s in sigma_factors:
            lines += 'ChemicalReaction RNAP -1 Sig' + s + ' -1 ' \
                     + 'RNAP_Sig' + s + ' 1 rates 1 1\n'
        lines += '\n'
        output_stream.write(lines)
            
    def write_transcription_initiation(self, output_stream):
        sigma_factors = ['A','B']
        lines = self._header('Transcription initiation')
        for s in sigma_factors:
            lines += self._bound_chemical(['bound_RNAP_Sig'+s, \
                                           'stable_RNAP_Sig'+s,])
        lines += '\n'
        for s in sigma_factors:
            lines += 'SequenceBinding RNAP_Sig' + s + ' bound_RNAP_Sig' + s \
                     + ' promoter_Sig' + s + '\n'
            lines += 'ChemicalReaction bound_RNAP_Sig' + s + ' -1 ' \
                     + 'stable_RNAP_Sig' + s + ' 1 rates 1 0\n'
            lines += 'ChemicalReaction stable_RNAP_Sig' + s + ' -1 ' \
                     + 'stable_RNAP 1 Sig' + s + ' 1 rates 1 0\n'
        lines += '\n'
        output_stream.write(lines)

    def write_transcription_elongation(self, output_stream):
        lines = self._header('Transcription elongation')
        lines += self._bound_chemical(['loaded_RNAP','stable_RNAP', \
                                       'stalled_RNAP', 'translocating_RNAP'])
        lines += self._free_chemical(['Mg2+'],['1000'])
        lines += '\n'
        lines += 'LoadingTable NTP_loading A ATP loaded_RNAP 1, ' \
                 + 'C CTP loaded_RNAP 1, G GTP loaded_RNAP 1, ' \
                 + 'T UTP loaded_RNAP 1\n'
        lines += 'ProductLoading stable_RNAP NTP_loading\n'
        lines += 'ChemicalReaction loaded_RNAP -1 Mg2+ -2 ' \
                 + 'translocating_RNAP 1 PPi 1 Mg2+ 2 rates 1 0\n'
        lines += 'Translocation translocating_RNAP stable_RNAP ' \
                 + 'stalled_RNAP 1 6 hairpin\n'
        lines += '\n'
        output_stream.write(lines)

    def write_transcription_termination(self, output_stream):
        lines = self._header('Transcription termination')
        lines += self._bound_chemical(['empty_RNAP'])
        lines += self._free_chemical(['NutA'], [100])
        lines += '# not a real chemical: only used to count production.\n'
        lines += self._free_chemical(['rna'])
        lines += '\n'
        lines += 'Release stalled_RNAP empty_RNAP stable_RNAP rnas 1\n'
        lines += 'ChemicalReaction empty_RNAP -1 NutA -1 NutA 1 RNAP 1 rna 1 rates 1 0\n'
        lines += '\n'
        output_stream.write(lines)

    def write_detailed_TUs(self, TUs, output_stream):
        k_on = '10'
        k_off = '1'
        for TU in TUs:
            # handle TU output
            if TU.sense == 1:
                parent = 'sensedna'
            else:
                parent = 'antisensedna'
            lines = 'ChemicalSequence ' + TU.name + \
                    ' product_of ' + parent + ' ' + \
                    str(TU.start) + ' ' + str(TU.end) + \
                    ' rnas\n'
            promoter_start = TU.start - 35
            promoter_end = TU.start + 1
            reading_frame = TU.start
            lines += 'BindingSite promoter_' + TU.sigma + \
                     ' ' + parent + ' ' + str(promoter_start) + \
                     ' ' + str(promoter_end) + ' 10 1 ' + \
                     str(reading_frame) + '\n'
            terminator = TU.end + 1
            lines += 'TerminationSite hairpin ' + parent + \
                     ' ' + str(terminator) + ' ' + str(terminator) + '\n'
            output_stream.write(lines)

    def write_detailed_proteins(self, TUs, output_stream):
        k_on = '10'
        k_off = '1'
        for TU in TUs:
            for g in TU.genes:
                name = g.name + '_' + g.bsu
                lines = 'ChemicalSequence ' + name + ' ' \
                        + 'product_of ' + TU.name + ' ' \
                        + str(g.start) + ' ' + str(g.end-3) + ' proteins\n'
                lines += 'BindingSite RBS ' + TU.name + ' ' \
                         + str(g.rbs_start) + ' ' + str(g.rbs_end) + ' ' \
                         + k_on + ' ' + k_off + ' ' \
                         + str(g.start) + '\n'
                output_stream.write(lines)

    def write_agregated_TUs(self, TUs, output_stream):
        k_on = '10'
        k_off = '1'
        for TU in TUs:
            # handle TU output
            if TU.sense == 1:
                parent = 'sensedna'
            else:
                parent = 'antisensedna'
            lines = 'ChemicalSequence ' + TU.name + \
                    ' product_of ' + parent + ' ' + \
                    str(TU.start) + ' ' + str(TU.end) + \
                    ' rnas\n'
            promoter_start = TU.start - 35
            promoter_end = TU.start + 1
            reading_frame = TU.start
            promoter = 'promoter_' + TU.name
            lines += 'BindingSite ' + promoter + \
                     ' ' + parent + ' ' + str(promoter_start) + \
                     ' ' + str(promoter_end) + ' 10 1\n'
            free_pol = 'RNAP_' + TU.sigma
            bound_pol = 'RNAP_' + TU.name
            lines += 'BoundChemical ' + bound_pol + '\n'
            lines += 'SequenceBinding ' + free_pol + ' ' + bound_pol + ' ' \
                     + promoter + '\n'
            lines += 'ChemicalReaction ' + bound_pol + ' -1 ' \
                     + self._TU_reactants(TU) + ' ' \
                     + 'RNAP' + ' 1 ' + TU.sigma + ' 1 ' + TU.name + ' 1 '\
                     + 'rates 1 0\n'
            output_stream.write(lines)
            
    def write_agregated_proteins(self, TUs, output_stream):
        k_on = '10'
        k_off = '1'
        for TU in TUs:
            for g in TU.genes:
                name = g.name + '_' + g.bsu
                lines = 'ChemicalSequence ' + name + \
                        ' product_of ' + TU.name + ' ' + \
                        str(g.start) + ' ' + str(g.end) + \
                        ' proteins\n'
                rbs = 'RBS_' + name
                name_30S = '30S_' + name
                lines += 'BindingSite ' + rbs + \
                         ' ' + TU.name + ' ' + str(g.rbs_start) + \
                         ' ' + str(g.rbs_end) + ' 10 1\n'
                lines += 'BoundChemical ' + name_30S + '\n'
                lines += 'SequenceBinding ' + '30S31 ' + name_30S + ' ' \
                         + rbs + '\n'
                # fmet loading and stuff
                lines += 'ChemicalReaction ' + name_30S + ' -1 '\
                         + 'IF2 -1 order 0 ' \
                         + 'GTP -1 order 0 ' \
                         + 'tRNA_fM_c -1 order 0' \
                         + '50S -1 order 0 ' \
                         + 'IF1 1 IF2 1 IF3 1 GDP 1 Pi 1 H2O 1 tRNA_fM 1 '
                # rest of elongation
                lines += self._protein_reactants(g) \
                         + '30S 1 50S 1 ' + name + ' 1 ' \
                         + 'rates 1 0\n'
                output_stream.write(lines)

    def _header(self, title):
        line = '#' * (len(title)+4) + '\n'
        return line + '# ' + title + ' #\n' + line

    def _free_chemical(self, molecules, numbers = None):
        if numbers:
            assert(len(molecules) == len(numbers))
        lines = ''
        for i in range (len(molecules)):
            lines += 'FreeChemical ' + molecules[i]
            if numbers:
                lines += ' ' + str(numbers[i])
            lines += '\n'
        return lines

    def _bound_chemical(self, molecules):
        lines = ''
        for mol in molecules:
            lines += 'BoundChemical ' + mol + '\n'
        return lines

    def _TU_reactants(self, TU):
        nb_bases = sum(TU.composition)
        return 'ATP ' + str(-TU.composition[0]) + ' order 0 ' \
            + 'CTP ' + str(-TU.composition[1]) + ' order 0 ' \
            + 'UTP ' + str(-TU.composition[3]) + ' order 0 ' \
            + 'GTP ' + str(-TU.composition[2]) + ' order 0 ' \
            + 'Mg2+ ' + str(-2*nb_bases) + ' order 0 ' \
            + 'PPi ' + str(nb_bases) + ' Mg2+ ' + str(2*nb_bases)

    def _protein_reactants(self, gene):
        nb_bases = sum(gene.aa_composition)
        reaction = ''
        assert(len(self._aas) == len(gene.aa_composition))
        for i, aa in enumerate(self._aas):
            sto = gene.aa_composition[i]
            reaction += 'EtRNA_' + aa + '_ca ' + str(-sto) + ' order 0 ' \
                        + 'tRNA_' + aa + ' ' + str(sto) + ' '
        reaction += 'EFPa ' + str(-nb_bases) + ' order 0 ' \
                    + 'GTP ' + str(-nb_bases) + ' order 0 ' \
                    + 'RF -1 order 0 ' \
                    + 'EFTud ' + str(nb_bases) + ' ' \
                    + 'EFPd ' + str(nb_bases) + ' ' \
                    + 'GDP ' + str(nb_bases) + ' ' \
                    + 'Pi ' + str(3*nb_bases) + ' ' \
                    + 'RF 1'
        return reaction
