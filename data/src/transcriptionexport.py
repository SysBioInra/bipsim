"""Classes exporting transcription files for MyBacteria."""

from __future__ import absolute_import, division, print_function
from src.lineformat import header, free_chemical, bound_chemical


class TranscriptionExport(object):
    """Export transcription files for MyBacteria."""

    def __init__(self, params):
        """Build export object from parameters."""
        self.params = params

    def input(self):
        """Return input section of transcription file."""
        lines = [header('Transcription input')]
        lines.append(free_chemical(['RNAP'], [self.params.RNAP]))
        fmt = free_chemical(['Sig{}'], [self.params.sigma])
        lines += [fmt.format(s) for s in self.params.sigma_factors]
        fmt = free_chemical(['Sig{}'])
        lines += [fmt.format(s) for s in self.params.absent_sigma_factors]
        lines.append(free_chemical(['Mg2+'], [self.params.mg]))
        lines.append(free_chemical(['NutA'], [self.params.nuta]))
        lines.append('# not a real chemical: only used to count production.\n')
        lines.append(free_chemical(['rna_tracker']))
        lines.append('\n')
        return ''.join(lines)

    def pre_initiation(self):
        """Return pre-initiation section of transcription file."""
        lines = [header('Transcription pre-initiation')]
        fmt = free_chemical(['RNAP_Sig{}'])
        for s in self.params.sigma_factors + self.params.absent_sigma_factors:
            lines.append(fmt.format(s))
        lines.append('\n')
        fmt = 'ChemicalReaction RNAP -1 Sig{0} -1 RNAP_Sig{0} 1 rates 1 1\n'
        lines += [fmt.format(s) for s in self.params.sigma_factors]
        lines.append('\n')
        return ''.join(lines)

    def initiation(self):
        """Return initiation section of transcription file."""
        lines = [header('Transcription initiation')]
        fmt = bound_chemical(['bound_RNAP_Sig{0}', 'stable_RNAP_Sig{0}'])
        lines += [fmt.format(s) for s in self.params.sigma_factors]
        lines.append(bound_chemical(['stable_RNAP']))
        lines.append('\n')
        fmt = (
            'SequenceBinding RNAP_Sig{0} bound_RNAP_Sig{0} promoter_Sig{0}\n'
            'ChemicalReaction bound_RNAP_Sig{0} -1 stable_RNAP_Sig{0} 1 '
            'rates 1 0\n'
            'ChemicalReaction stable_RNAP_Sig{0} -1 stable_RNAP 1 Sig{0} 1 '
            'rates 1 0\n'
            )
        lines += [fmt.format(s) for s in self.params.sigma_factors]
        lines.append('\n')
        return ''.join(lines)

    def elongation(self):
        """Return elongation section of transcription file."""
        lines = [header('Transcription elongation')]
        lines.append(bound_chemical(['loaded_RNAP', 'stalled_RNAP',
                                     'translocating_RNAP']))
        lines.append('\n')
        templates = ['A', 'C', 'G', 'T']
        to_load = ['UTP', 'GTP', 'CTP', 'ATP']
        fmt = '{} {} loaded_RNAP ' + str(self.params.loading)
        loading_cases = [fmt.format(t, l) for t, l in zip(templates, to_load)]
        lines.append('LoadingTable NTP_loading '
                     + ', '.join(loading_cases) + '\n')
        lines.append('ProductLoading stable_RNAP NTP_loading\n')
        lines.append('ChemicalReaction loaded_RNAP -1 Mg2+ -2 '
                     'translocating_RNAP 1 PPi 1 Mg2+ 2 rates {} 0\n'
                     .format(self.params.mg_recruitment))
        lines.append('Translocation translocating_RNAP stable_RNAP '
                     'stalled_RNAP 1 {}\n'.format(self.params.translocation))
        lines.append('\n')
        return ''.join(lines)

    def aggregated_elongation(self, TUs):
        """Return aggregated elongation section of transcription file."""
        if not TUs:
            return ''
        lines = [header('Agregated transcription elongation')]
        switch_fmt = 'Switch {} stable_RNAP {}\n'
        s_site_fmt = 'SwitchSite {} {} {}\n'
        trans_fmt = 'Translocation {0} {1} {1} 50 1\n'
        release_fmt = ('ChemicalReaction {} -1 {} RNAP 1 {} 1 {} 1 '
                       'rna_tracker 1 rates {} 0\n')
        for TU in TUs:
            # retrieve TU information
            parent = 'sensedna' if TU.sense == 1 else 'antisensedna'
            name = TU.name
            name_RNAP = 'RNAP_' + name
            active_RNAP = 'transcribing_' + name_RNAP
            lines.append(bound_chemical([name_RNAP, active_RNAP]))
            # switch once sigma_factor is released
            switch_name = name + '_translation'
            lines.append(switch_fmt.format(switch_name, name_RNAP))
            lines.append(s_site_fmt.format(parent, TU.start, switch_name))
            # translocate RNAP further into cds
            lines.append('# arbitrary translocation inside TU.\n')
            lines.append(trans_fmt.format(name_RNAP, active_RNAP))
            rate = self.params.aggregation_rate / sum(TU.composition)
            lines.append(release_fmt.format(
                active_RNAP, self._TU_reactants(TU), TU.sigma,
                TU.name, rate
                ))
            lines.append('\n')
        return ''.join(lines)

    def termination(self):
        """Return termination section of transcription file."""
        lines = [header('Transcription termination')]
        lines.append(bound_chemical(['empty_RNAP', 'terminating_RNAP']))
        lines.append(bound_chemical(['continuing_RNAP']))
        lines.append('\n')
        lines.append('Switch hairpin stable_RNAP terminating_RNAP\n')
        lines.append('Release terminating_RNAP empty_RNAP continuing_RNAP '
                     'rnas {}\n'.format(self.params.release))
        lines.append('ChemicalReaction empty_RNAP -1 NutA -1 NutA 1 RNAP 1 '
                     'rna_tracker 1 rates {} 0\n'.format(self.params.release))
        lines.append('ProductLoading continuing_RNAP NTP_loading\n')
        lines.append('\n')
        return ''.join(lines)

    def transcription_units(self, TUs):
        """Return declation of transcription units."""
        lines = [header('General information')]
        lines.append('TransformationTable dna2rna A U, C G, G C, T A\n')
        lines.append('ProductTable rnas dna2rna\n')
        lines.append('\n')
        lines.append(header('Sequence information'))
        tu_fmt = 'ChemicalSequence {} product_of {} {} {} rnas\n'
        prom_fmt = 'BindingSite promoter_{} {} {} {} {} {} {}\n'
        s_site_fmt = 'SwitchSite {} {} hairpin\n'
        for TU in TUs:
            # handle TU output
            parent = 'sensedna' if TU.sense == 1 else 'antisensedna'
            promoter_start = TU.start - 35
            promoter_end = TU.start + 1
            reading_frame = TU.start
            terminator = TU.end + 1
            lines.append(tu_fmt.format(TU.name, parent, TU.start, TU.end))
            lines.append(prom_fmt.format(
                TU.sigma, parent, promoter_start, promoter_end,
                self.params.k_on, self.params.k_off, reading_frame
                ))
            lines.append(s_site_fmt.format(parent, terminator))
        lines.append('\n')
        return ''.join(lines)

    def degradation(self, TUs):
        """Return degradation section of transcription units."""
        if self.params.degradation <= 0:
            return ''
        lines = [header('Degradation')]
        lines.append('CompositionTable rna_composition A AMP, C CMP, '
                     'G GMP, U UMP\n')
        lines.append('\n')
        fmt = 'Degradation {} rna_composition {}\n'
        lines += [fmt.format(TU.name, self.params.degradation) for TU in TUs]
        lines.append('\n')
        return ''.join(lines)

    def _TU_reactants(self, TU):
        """Return reactants/byproducts of TU aggregated synthesis."""
        nb_bases = sum(TU.composition)
        return ('ATP ' + str(-TU.composition[0]) + ' order 0 '
                'CTP ' + str(-TU.composition[1]) + ' order 0 '
                'GTP ' + str(-TU.composition[2]) + ' order 0 '
                'UTP ' + str(-TU.composition[3]) + ' order 0 '
                'Mg2+ ' + str(-2) + ' order 0 '
                'PPi ' + str(nb_bases) + ' Mg2+ ' + str(2))
