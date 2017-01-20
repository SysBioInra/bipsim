
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
        self._sigma_factors = ['A','B']
        self._absent_sigma_factors = ['D','EF','GF','H','I','K','L','WXY','-']


    def write_translation_input(self, output_stream):
        lines = self._header('Translation input')
        lines += self._free_chemical(['30S','50S'], [1000,1000])
        lines += self._free_chemical(['IF1','IF2','IF3'], [100,400,100])
        lines += self._free_chemical(['EFTud','EFP','RF'], [60000,100,1000])
        lines += self._free_chemical(['tRNA_fM'], [300])
        lines += self._free_chemical(['tRNA_' + aa for aa in self._aas], \
                                     [1000*i for i in self._nb_codons])
        lines += '\n'
        lines += '# not a real chemical, used to count production\n'
        lines += self._free_chemical(['protein'])
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
                     + 'AMP 1 PPi 1 H+ 1 tRNA_' + aa + '_c 1 rates 1 0\n'
        lines += '\n'
        lines += 'ChemicalReaction EFTud -1 GTP -1 EFTua 1 GDP 1 rates 1 0\n'
        lines += '\n'
        for aa in self._aas:
            lines += 'ChemicalReaction tRNA_' + aa + '_c -1 EFTua -1 ' \
                     + 'EtRNA_' + aa + '_ca 1 rates 1 0\n'
        lines += '\n'
        output_stream.write(lines)

    def write_translation_pre_initiation(self, output_stream):
        lines = self._header('Translation pre-initiation')
        lines += self._free_chemical(['30S1', '30S3', '30S31'])
        lines += '\n'
        lines += 'ChemicalReaction 30S -1 IF3 -1 30S3 1 rates 1 1\n'
        lines += 'ChemicalReaction 30S3 -1 IF1 -1 30S31 1 rates 1 1\n'
        lines += '\n'
        output_stream.write(lines)

    def write_translation_initiation(self, output_stream):
        lines = self._header('Translation initiation')
        lines += self._bound_chemical(['m30S31','m30S312a','m30S312d', \
                                       'm30S12a', 'EFt70S'])
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
                 + 'EFt70S 1 IF1 1 IF2d 1 Pi 1 H2O 1 tRNA_fM 1 rates 1 0\n'
        lines += '\n'
        output_stream.write(lines)

    def write_translation_elongation(self, output_stream):        
        lines = self._header('Translation elongation')
        lines += self._bound_chemical(['stalled_ribosome', '70S'])
        lines += self._bound_chemical(['Et70S_' + aa + '_ca' for aa in self._aas])
        lines += self._bound_chemical(['t70S_' + aa + '_c' for aa in self._aas])
        lines += self._bound_chemical(['t70S_' + aa + '_cp' for aa in self._aas])
        lines += self._free_chemical(['EFPa','EFPd'])
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
        lines += 'ChemicalReaction EFP -1 GTP -1 EFPa 1 rates 1 1\n'
        lines += 'ChemicalReaction EFP -1 GDP -1 EFPd 1 rates 0.1 10\n'
        lines += '\n'
        for aa in self._aas:
            lines += 'ChemicalReaction Et70S_' + aa + '_ca -1 ' \
                     + 't70S_' + aa + '_c 1 EFTud 1 Pi 1 rates 1 0\n'
            lines += 'ChemicalReaction t70S_' + aa + '_c -1 EFPa -1 ' \
                     + 't70S_' + aa + '_cp 1 EFPd 1 Pi 1 rates 1 0 \n'
            lines += 'ChemicalReaction t70S_' + aa + '_cp -1 GTP -1 ' \
                     + 'EFt70S 1 GDP 1 Pi 1 tRNA_' + aa + ' 1 rates 1 0 \n'
        lines += '\n'
        output_stream.write(lines)

    def write_agregated_translation_elongation(self, TUs, output_stream):
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
                             + 'EFt70S ' + name_70S + '\n'
                    # translocate 70S further into protein
                    lines += '# arbitrary translocation inside cds.\n'
                    lines += 'Translocation ' + name_70S + ' ' + \
                             active_70S + ' ' + active_70S + ' 50 1\n'
                    # rest of elongation
                    lines += 'ChemicalReaction ' + active_70S + ' -1 ' \
                            + self._protein_reactants(g) + ' ' \
                            + '30S 1 50S 1 ' + name + ' 1 ' \
                            + 'protein 1 rates 1 0\n'
                lines += '\n'
                output_stream.write(lines)

    def write_translation_termination(self, output_stream):        
        lines = self._header('Translation termination')
        lines += self._bound_chemical(['empty_ribosome'])
        lines += '\n'
        lines += 'Release stalled_ribosome empty_ribosome 70S proteins 1\n'
        lines += 'ChemicalReaction empty_ribosome -1 30S 1 50S 1 protein 1 RF 1 rates 1 0\n'
        lines += '\n'
        output_stream.write(lines)

    def write_transcription_input(self, output_stream):
        lines = self._header('Transcription input')
        lines += self._free_chemical(['RNAP'], [1000])
        for s in self._sigma_factors:
            lines += self._free_chemical(['Sig'+s], [1000])
        for s in self._absent_sigma_factors:
            lines += self._free_chemical(['Sig'+s])
        lines += self._free_chemical(['Mg2+'],['1000'])
        lines += self._free_chemical(['NutA'], [100])
        lines += '# not a real chemical: only used to count production.\n'
        lines += self._free_chemical(['rna'])
        lines += '\n'
        output_stream.write(lines)
        
    def write_transcription_pre_initiation(self, output_stream):
        lines = self._header('Transcription pre-initiation')
        for s in self._sigma_factors + self._absent_sigma_factors:
            lines += self._free_chemical(['RNAP_Sig'+s])
        lines += '\n'
        for s in self._sigma_factors:
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
        lines += self._bound_chemical(['stable_RNAP'])
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
        lines += self._bound_chemical(['loaded_RNAP', 'stalled_RNAP', \
                                       'translocating_RNAP'])
        lines += '\n'
        lines += 'LoadingTable NTP_loading A ATP loaded_RNAP 1, ' \
                 + 'C CTP loaded_RNAP 1, G GTP loaded_RNAP 1, ' \
                 + 'T UTP loaded_RNAP 1\n'
        lines += 'ProductLoading stable_RNAP NTP_loading\n'
        lines += 'ChemicalReaction loaded_RNAP -1 Mg2+ -2 ' \
                 + 'translocating_RNAP 1 PPi 1 Mg2+ 2 rates 1 0\n'
        lines += 'Translocation translocating_RNAP stable_RNAP ' \
                 + 'stalled_RNAP 1 6\n'
        lines += '\n'
        output_stream.write(lines)

    def write_agregated_transcription_elongation(self, TUs, output_stream):
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
            lines += 'ChemicalReaction ' + active_RNAP + ' -1 ' \
                     + self._TU_reactants(TU) + ' ' \
                     + 'RNAP' + ' 1 ' + TU.sigma + ' 1 ' + TU.name + ' 1 '\
                     + 'rna 1 rates 1 0\n'
            lines += '\n'
            output_stream.write(lines)

    def write_transcription_termination(self, output_stream):
        lines = self._header('Transcription termination')
        lines += self._bound_chemical(['empty_RNAP','terminating_RNAP'])
        lines += '\n'
        lines += 'Switch hairpin stable_RNAP terminating_RNAP\n'
        lines += 'Release terminating_RNAP empty_RNAP stable_RNAP rnas 1\n'
        lines += 'ChemicalReaction empty_RNAP -1 NutA -1 NutA 1 RNAP 1 rna 1 rates 1 0\n'
        lines += '\n'
        output_stream.write(lines)

    def write_TUs(self, TUs, output_stream):
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
            output_stream.write(lines)

    def write_termination_sites(self, TUs, output_stream):
        output_stream.write(self._header('Termination sites'))
        for TU in TUs:
            # handle TU output
            if TU.sense == 1:
                parent = 'sensedna'
            else:
                parent = 'antisensedna'

            terminator = TU.end + 1
            lines = 'SwitchSite ' + parent + ' ' \
                    + str(terminator) + ' hairpin\n'
            output_stream.write(lines)

    def write_proteins(self, TUs, output_stream):
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
            + 'Mg2+ ' + str(-2) + ' order 0 ' \
            + 'PPi ' + str(nb_bases) + ' Mg2+ ' + str(2)

    def _protein_reactants(self, gene):
        nb_bases = sum(gene.aa_composition)
        reaction = ''
        assert(len(self._aas) == len(gene.aa_composition))
        for i, aa in enumerate(self._aas):
            sto = gene.aa_composition[i]
            reaction += 'EtRNA_' + aa + '_ca ' + str(-sto) + ' order 0 ' \
                        + 'tRNA_' + aa + ' ' + str(sto) + ' '
        reaction += 'EFP ' + str(-1) + ' order 0 ' \
                    + 'GTP ' + str(-2*nb_bases) + ' order 0 ' \
                    + 'RF -1 order 0 ' \
                    + 'EFP ' + str(1) + ' ' \
                    + 'EFTud ' + str(nb_bases) + ' order 0 ' \
                    + 'GDP ' + str(2*nb_bases) + ' ' \
                    + 'Pi ' + str(3*nb_bases) + ' ' \
                    + 'RF ' + str(1)
        return reaction
