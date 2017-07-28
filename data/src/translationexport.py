"""Classes exporting translation files for MyBacteria."""

from __future__ import absolute_import, division, print_function
from src.lineformat import header, free_chemical, bound_chemical


def genetic_code():
    """Return genetic code as a dictionary."""
    code = {}
    code['F'] = ['UU' + n for n in ['C', 'U']]
    code['L'] = (['UU' + n for n in ['A', 'G']]
                 + ['CU' + n for n in ['A', 'C', 'G', 'U']])
    code['I'] = ['AU' + n for n in ['A', 'C', 'U']]
    code['M'] = ['AUG']
    code['V'] = ['GU' + n for n in ['A', 'C', 'G', 'U']]
    code['S'] = ['UC' + n for n in ['A', 'C', 'G', 'U']]
    code['P'] = ['CC' + n for n in ['A', 'C', 'G', 'U']]
    code['T'] = ['AC' + n for n in ['A', 'C', 'G', 'U']]
    code['A'] = ['GC' + n for n in ['A', 'C', 'G', 'U']]
    code['Y'] = ['UA' + n for n in ['C', 'U']]
    code['H'] = ['CA' + n for n in ['C', 'U']]
    code['Q'] = ['CA' + n for n in ['A', 'G']]
    code['N'] = ['AA' + n for n in ['C', 'U']]
    code['K'] = ['AA' + n for n in ['A', 'G']]
    code['D'] = ['GA' + n for n in ['C', 'U']]
    code['E'] = ['GA' + n for n in ['A', 'G']]
    code['C'] = ['UG' + n for n in ['C', 'U']]
    code['W'] = ['UGG']
    code['R'] = (['CG' + n for n in ['A', 'C', 'G', 'U']]
                 + ['AG' + n for n in ['A', 'G']])
    code['S'] += ['AG' + n for n in ['C', 'U']]
    code['G'] = ['GG' + n for n in ['A', 'C', 'G', 'U']]
    return code


class TranslationExport(object):
    """Export translation files for MyBacteria."""

    def __init__(self, params):
        """Build export object from translation parameters."""
        super(TranslationExport, self).__init__()
        self._aas = ['A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M',
                     'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'Y']
        self._genetic_code = genetic_code()
        self._nb_codons = [len(self._genetic_code[aa]) for aa in self._aas]
        self._stop_codons = ['UAA', 'UAG', 'UGA']
        self.params = params

    def input(self):
        """Return input section of translation file."""
        lines = [header('Translation input')]
        lines.append(free_chemical(['30S', '50S'],
                                   [self.params.nb_ribosomes]*2))
        lines.append(free_chemical(['IF1', 'IF2', 'IF3'], self.params.IF_123))
        lines.append(free_chemical(['EFG', 'RF'], self.params.EF_G_RF))
        if self.params.cut_slow_reactions:
            lines.append(free_chemical(['tRNA_fM_c'], [0], True))
        else:
            lines.append(free_chemical(['tRNA_fM_c'], [self.params.fMet]))
        lines.append(free_chemical(
            ['EtRNA_' + aa + '_ca' for aa in self._aas],
            [self.params.nb_tRNAs*i for i in self._nb_codons],
            self.params.cut_slow_reactions
            ))
        lines.append('\n')
        lines.append('# not a real chemical, used to count production\n')
        lines.append(free_chemical(['protein_tracker']))
        lines.append('\n')
        return ''.join(lines)

    def tRNA_activation(self):
        """Return tRNA activation section of translation file."""
        is_constant = self.params.cut_slow_reactions
        is_aggregated = self.params.aggregate_slow_reactions
        aas_fMet = ['fM'] + self._aas
        lines = [header('tRNA activation')]
        lines.append(free_chemical(['tRNA_' + aa for aa in aas_fMet],
                                   [0] * len(aas_fMet),
                                   is_constant))
        if not is_constant:
            lines.append(free_chemical(
                ['tRNA_' + aa + '_c' for aa in self._aas]
                ))
        lines.append('\n')
        lines.append(free_chemical(['EFTud'], [0], is_constant))
        if not is_constant:
            lines.append(free_chemical(['EFTua'], [self.params.EFTu]))
        lines.append('\n')
        if is_constant:
            lines.append('# RNAs were set to act as constant chemicals,\n')
            lines.append('# we skip tRNA activation reactions.\n')
            lines.append('\n')
            return ''.join(lines)

        stoichio = '100 order 1' if is_aggregated else '1'
        # watch out for {{0}} vs {0}:
        # {0} is going to be replaced by stoichio
        # {{0}} is going to be replaced by {0} and used in next .format call
        fmt = ('ChemicalReaction tRNA_{{0}} -{0} ATP -{0} AMP {0} PPi {0} '
               'tRNA_{{0}}_c {0} rates 1 0\n'.format(stoichio))
        lines += [fmt.format(aa) for aa in aas_fMet]
        lines.append('\n')

        stoichio = '1000 order 1' if is_aggregated else '1'
        lines.append('ChemicalReaction EFTud -{0} GTP -{0} EFTua {0} GDP {0} '
                     'rates 1 0\n'.format(stoichio))
        lines.append('\n')

        stoichio = '100 order 1' if is_aggregated else '1'
        fmt = ('ChemicalReaction tRNA_{{0}}_c -{0} EFTua -{0} EtRNA_{{0}}_ca '
               '{0} rates 1 0\n'.format(stoichio))
        lines += [fmt.format(aa) for aa in self._aas]
        lines.append('\n')
        return ''.join(lines)

    def pre_initiation(self):
        """Return pre-initiation section of translation file."""
        lines = [header('Translation pre-initiation')]
        lines.append(free_chemical(['30S1', '30S3', '30S31']))
        lines.append('\n')
        lines.append('ChemicalReaction 30S -1 IF3 -1 30S3 1 rates 1 1\n')
        lines.append('ChemicalReaction 30S3 -1 IF1 -1 30S31 1 rates 1 1\n')
        lines.append('\n')
        return ''.join(lines)

    def initiation(self):
        """Return initiation section of translation file."""
        lines = [header('Translation initiation')]
        if self.params.cut_slow_reactions:
            lines.append(free_chemical(['IF2d', 'fMet2a', 'fMet2d'],
                                       [0, self.params.fMet, 0], True))
        else:
            lines.append(free_chemical(['IF2a', 'IF2d', 'IF2p']))
            lines.append(free_chemical(['fMet2a', 'fMet2d', 'fMet2p']))
        lines.append(bound_chemical(['m30S31', 'm30S312a', 'm30S312d',
                                     'm30S12a', 'translocating_70S']))
        lines.append('\n')
        lines.append('SequenceBinding 30S31 m30S31 RBS\n')
        if not self.params.cut_slow_reactions:
            lines.append('ChemicalReaction IF2 -1 GTP -1 IF2a 1 rates 1 1\n')
            lines.append('ChemicalReaction IF2a -1 tRNA_fM_c -1 fMet2a 1 '
                         'rates 1 1\n')
            lines.append('ChemicalReaction IF2 -1 GDP -1 IF2d 1 rates 1 10\n')
            lines.append('ChemicalReaction IF2d -1 tRNA_fM_c -1 fMet2d 1 '
                         'rates 1 10\n')
            lines.append('ChemicalReaction IF2 -1 ppGpp -1 IF2p 1 rates 1 1\n')
            lines.append('ChemicalReaction IF2p -1 tRNA_fM_c -1 fMet2p 1 '
                         'rates 1 1\n')
        lines.append('ChemicalReaction fMet2a -1 m30S31 -1 m30S312a 1 '
                     'rates 1 0\n')
        lines.append('ChemicalReaction fMet2d -1 m30S31 -1 m30S312d 1 '
                     'rates 1 10\n')
        lines.append('ChemicalReaction m30S312a -1 IF3 1 m30S12a 1 '
                     'rates 1 0\n')
        lines.append('ChemicalReaction m30S12a -1 50S -1 '
                     'translocating_70S 1 IF1 1 IF2d 1 Pi 1 tRNA_fM 1 '
                     'rates 1 0\n')
        lines.append('\n')
        return ''.join(lines)

    def elongation(self):
        """Return elongation section of translation file."""
        lines = [header('Translation elongation')]
        lines.append(bound_chemical(['stalled_ribosome', '70S',
                                     'pre_translocation_70S']))
        lines.append(bound_chemical(['Et70S_' + aa for aa in self._aas]))
        lines.append('\n')
        lines.append('# translocation\n')
        lines.append('ChemicalReaction pre_translocation_70S -1 EFG -1 GTP -1 '
                     'translocating_70S 1 EFG 1 GDP 1 Pi 1 rates {} 0 \n'
                     .format(self.params.translocation))
        lines.append('Translocation translocating_70S 70S stalled_ribosome '
                     '3 {}\n'.format(self.params.translocation))
        lines.append('\n')
        lines.append('# tRNA loading\n')
        lines.append('LoadingTable EtRNA_loading ')
        for aa in self._aas:
            rates = (' EtRNA_{0}_ca Et70S_{0} {1}, '
                     .format(aa, self.params.loading))
            lines += [codon + rates for codon in self._genetic_code[aa]]
        lines.append(', '.join([c + ' RF stalled_ribosome 1'
                                for c in self._stop_codons])
                     + '\n')
        lines.append('ProductLoading 70S EtRNA_loading\n')
        lines.append('\n')
        lines.append('# transpeptidation\n')
        fmt = ('ChemicalReaction Et70S_{0} -1 pre_translocation_70S 1 '
               'tRNA_{0} 1 EFTud 1 Pi 1 rates '
               + str(self.params.transpeptidation) + ' 0\n')
        lines += [fmt.format(aa) for aa in self._aas]
        lines.append('\n')
        return ''.join(lines)

    def termination(self):
        """Return termination section of translation file."""
        lines = [header('Translation termination')]
        lines.append(bound_chemical(['empty_ribosome']))
        lines.append('\n')
        lines.append('Release stalled_ribosome empty_ribosome 70S proteins '
                     '{}\n'.format(self.params.release))
        lines.append('ChemicalReaction empty_ribosome -1 30S 1 50S 1 '
                     'protein_tracker 1 RF 1 rates {} 0\n'
                     .format(self.params.release))
        lines.append('\n')
        return ''.join(lines)

    def aggregated_elongation(self, TUs):
        """Return aggregated elongation section of translation file."""
        if not TUs:
            return ''
        lines = [header('Agregated translation elongation')]
        known_70S = []
        s_site_fmt = 'SwitchSite {} {} {}\n'
        switch_fmt = 'Switch {} translocating_70S {}\n'
        trans_fmt = 'Translocation {0} {1} {1} 50 1\n'
        elongation_fmt = ('ChemicalReaction {} -1 {} 30S 1 50S 1 '
                          '{} 1 protein_tracker 1 rates {} 0\n')
        for TU in TUs:
            parent = TU.name
            for g in TU.genes:
                name = g.name + '_' + g.bsu
                name_70S = '70S_' + name
                active_70S = 'translating_' + name_70S
                switch_name = name + '_translation'
                # every protein has one switch per TU
                lines.append(s_site_fmt.format(parent, g.start, switch_name))
                # however, because a protein may be on several TUs,
                # its reaction may already have been written before.
                if name_70S not in known_70S:
                    known_70S.append(name_70S)
                    lines.append(bound_chemical([name_70S, active_70S]))
                    # switch once fMet loading is completed
                    lines.append(switch_fmt.format(switch_name, name_70S))
                    # translocate 70S further into protein
                    lines.append('# arbitrary translocation inside cds.\n')
                    lines.append(trans_fmt.format(name_70S, active_70S))
                    # rest of elongation
                    rate = self.params.aggregation_rate / sum(g.aa_composition)
                    lines.append(elongation_fmt.format(
                        active_70S, self._protein_reactants(g), name, rate
                        ))
                lines.append('\n')
        return ''.join(lines)

    def proteins(self, TUs):
        """Return declaration of proteins carried by TUs."""
        lines = [header('General information')]
        codon_aa = []
        for aa in self._aas:
            for codon in self._genetic_code[aa]:
                codon_aa.append(codon + ' ' + aa)
        lines.append('TransformationTable rna2protein '
                     + ', '.join(codon_aa) + '\n')
        lines.append('ProductTable proteins rna2protein\n')
        lines.append('\n')
        lines.append(header('Sequence information'))
        seq_fmt = 'ChemicalSequence {} product_of {} {} {} proteins\n'
        rbs_fmt = 'BindingSite RBS {} {} {} {} {} {}\n'
        for TU in TUs:
            for g in TU.genes:
                name = g.name + '_' + g.bsu
                lines.append(seq_fmt.format(name, TU.name, g.start, g.end-3))
                lines.append(rbs_fmt.format(
                    TU.name, g.rbs_start, g.rbs_end,
                    self.params.k_on, self.params.k_off, g.start
                    ))
        lines.append('\n')
        return ''.join(lines)

    def degradation(self, TUs):
        """Return degradation of proteins carried by TUs."""
        if self.params.degradation <= 0:
            return ''
        lines = [header('Degradation')]
        lines.append('CompositionTable protein_composition '
                     + ', '.join([aa + ' aa_' + aa for aa in self._aas])
                     + '\n')
        lines.append('\n')
        already_treated = []
        deg_fmt = ('Degradation {} protein_composition '
                   + str(self.params.degradation) + '\n')
        for TU in TUs:
            for g in TU.genes:
                name = g.name + '_' + g.bsu
                if name not in already_treated:
                    already_treated.append(name)
                    lines.append(deg_fmt.format(name))
        lines.append('\n')
        return ''.join(lines)

    def _protein_reactants(self, gene):
        """Return aggregated reactants/byproducts for given protein."""
        nb_bases = sum(gene.aa_composition)
        reaction = ['EtRNA_{0}_ca -{1} order 0 tRNA_{0} {1} '.format(aa, sto)
                    for aa, sto in zip(self._aas, gene.aa_composition)]
        reaction.append('EFG -{0} order 0 GTP -{1} order 0 RF -1 order 0 '
                        'EFG {0} EFTud {1} GDP {1} Pi {2} RF 1'
                        .format(1, nb_bases, 2*nb_bases))
        return ''.join(reaction)
