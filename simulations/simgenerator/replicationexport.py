"""Classes exporting replication files for MyBacteria."""

from __future__ import absolute_import, division, print_function
from simgenerator.lineformat import header, free_chemical, bound_chemical


class ReplicationExport(object):
    """Export replication files for MyBacteria."""

    def __init__(self, params):
        """Make export object from parameters."""
        self.params = params

    def input(self):
        """Return input section of replication file."""
        lines = [header('Replication input')]
        lines.append(free_chemical(['DnaA_GTP'], [2]))
        lines.append(free_chemical(['free_DNAP'], [self.params.DNAP]))
        lines.append('\n')
        return ''.join(lines)

    def initiation(self):
        """Return initiation section of replication file."""
        lines = [header('Replication initiation')]
        lines.append(free_chemical(['DnaA_GDP']))
        lines.append(bound_chemical(['bound_DnaA', 'DNAP_fork']))
        lines.append('\n')
        lines.append('BindingSite DnaA_BS sensedna 1 1 100000 0 1\n')
        lines.append('BindingSite DnaA_BS antisensedna 1 1 100000 0 1\n')
        lines.append('\n')
        lines.append('SequenceBinding DnaA_GTP bound_DnaA DnaA_BS\n')
        lines.append('ChemicalReaction free_DNAP -1 bound_DnaA -1 '
                     'DNAP_fork 1 DnaA_GDP 1 Pi 1 rates 1 0\n')
        lines.append('\n')
        return ''.join(lines)

    def elongation(self):
        """Return elongation section of replication file."""
        lines = [header('Replication elongation')]
        lines.append(bound_chemical(['DNAP_lagging']))
        for end in ['fork', 'lagging']:
            for start in ['loaded', 'translocating', 'stalled']:
                lines.append(bound_chemical([start + '_DNAP_' + end]))
        lines.append('\n')
        lines.append('# Leading strand\n')
        lines.append(self._elongation_reactions('fork'))
        lines.append('\n')
        lines.append('# Lagging strand\n')
        lines.append('DoubleStrandRecruitment translocating_DNAP_fork '
                     'free_DNAP DNAP_lagging {}\n'
                     .format(self.params.recruitment))
        lines.append(self._elongation_reactions('lagging'))
        lines.append('\n')
        return ''.join(lines)

    def _elongation_reactions(self, tag):
        """Return elongation reactions with specific tag on DNAP."""
        templates = ['A', 'C', 'G', 'T']
        to_load = ['dTTP', 'dGTP', 'dCTP', 'dATP']
        fmt = '{{}} {{}} loaded_DNAP_{} {}'.format(tag, self.params.loading)
        loading_cases = [fmt.format(t, l) for t, l in zip(templates, to_load)]
        lines = ['LoadingTable dNTP_loading_' + tag + ' '
                 + ', '.join(loading_cases) + '\n']
        lines.append('DoubleStrandLoading DNAP_{0} dNTP_loading_{0} '
                     'stalled_DNAP_{0}\n'.format(tag))
        lines.append('ChemicalReaction loaded_DNAP_{0} -1 PPi 1 '
                     'translocating_DNAP_{0} 1 rates {1} 0\n'
                     .format(tag, self.params.ligation))
        lines.append('Translocation translocating_DNAP_{0} DNAP_{0} '
                     'stalled_DNAP_{0} 1 {1}\n'
                     .format(tag, self.params.translocation))
        lines.append('ChemicalReaction stalled_DNAP_{} -1 free_DNAP 1 '
                     'rates {} 0\n'.format(tag, self.params.release))
        return ''.join(lines)
