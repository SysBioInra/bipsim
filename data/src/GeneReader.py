"""Module reading gene/protein information from subtilis data set."""

import csv


class GeneReader(object):

    def __init__(self, input_stream, dna_length, log_stream=None):
        parser = csv.reader(input_stream, delimiter='\t')
        header = next(parser)
        indices = ColumnIndices(header)
        self.genes = []
        self._no_RBS = []
        self._invalid_RBS = []
        self._long_RBS = []
        self._invalid_start = []
        self._invalid_stop = []
        self._invalid_sequence = []
        self._invalid_aa_seq = []
        for row in parser:
            if row[indices.type] != 'CDS':
                continue
            self.gene = Gene(row, indices, dna_length)
            self._check_gene()
            if self.gene.is_usable():
                self.gene.set_rbs_at_valid_location()
                self.genes.append(self.gene)
        self._log_errors(log_stream)

    def _check_gene(self):
        self._check_rbs()
        self._check_sequence()

    def _check_rbs(self):
        if self.gene.is_undefined_rbs():
            self._no_RBS.append(self.gene.name)
            return
        if self.gene.is_rbs_after_start_codon():
            self._invalid_RBS.append(self.gene.name)
        if self.gene.is_rbs_too_far_before_stop_codon():
            self._long_RBS.append(self.gene.name)

    def _check_sequence(self):
        if not self.gene.has_valid_sequence_length():
            self._invalid_sequence.append(self.gene.name)
        if not self.gene.has_valid_start_codon():
            self._invalid_start.append(self.gene.name)
        if not self.gene.has_valid_stop_codon():
            self._invalid_stop.append(self.gene.name)
        if self.gene.contains_premature_stop_codon():
            self._invalid_sequence.append(self.gene.name)
        if not self.gene.is_aa_sequence_consistent_with_sequence():
            self._invalid_aa_seq.append(self.gene.name)

    def _log_errors(self, log_stream):
        if log_stream:
            log_stream.write(self._rbs_errors() + '\n\n'
                             + self._sequence_errors())

    def _rbs_errors(self):
        return '\n\n'.join((
            self._rbs_error_msg('no RBS info', self._no_RBS),
            self._rbs_error_msg('invalid RBS info', self._invalid_RBS),
            self._rbs_error_msg('RBS far away from gene', self._long_RBS)
            ))

    def _rbs_error_msg(self, error_type, error_list):
        return self._error_msg(
            error_type + ', RBS was set at 21 bases from start codon',
            error_list
            )

    def _error_msg(self, error_type, error_list):
        return 'Genes with ' + error_type + ': ' + ', '.join(error_list) + '.'

    def _sequence_errors(self):
        return '\n\n'.join((
            self._error_msg('invalid stop codon (they were excluded)',
                            self._invalid_stop),
            self._error_msg('invalid start codon', self._invalid_start),
            self._error_msg('invalid length or containing a stop codon '
                            '(they were excluded)', self._invalid_sequence),
            self._error_msg('invalid aa sequence (they were excluded)',
                            self._invalid_aa_seq)
            ))


class ColumnIndices(object):

    def __init__(self, header):
        self.name = header.index('name')
        self.bsu = header.index('BSU')
        self.start = header.index('start')
        self.end = header.index('end')
        self.rbs_start = header.index('RBSstart')
        self.rbs_end = header.index('RBSend')
        self.sense = header.index('brin_DNA')
        self.sequence = header.index('seq')
        self.aa_seq = header.index('aaseq')
        self.TU = header.index('TUs')
        self.type = header.index('gene_category')


class Gene(object):
    aas = ['A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N',
           'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'Y']

    def __init__(self, row, indices, dna_length):
        self._indices = indices
        self._dna_length = dna_length
        self.name = self.bsu = self.sense = None
        self.start = self.end = self.rbs_start = self.rbs_end = None
        self.sequence = self.aa_seq = self.TUs = None
        self.aa_composition = []
        self._extract_row(row)

    def _extract_row(self, row):
        self.name = row[self._indices.name]
        self.bsu = row[self._indices.bsu]
        self.sense = int(row[self._indices.sense])
        self._extract_position(row)
        self._extract_rbs(row)
        self._extract_sequence(row)
        self._extract_aa_seq(row)
        self._extract_TUs(row)

    def _extract_position(self, row):
        position = [int(row[self._indices.start]), int(row[self._indices.end])]
        assert(position[0] < position[1])
        if self.is_antisense():
            position = self._invert_position(position)
        self.start = position[0]
        self.end = position[1]

    def is_antisense(self):
        return self.sense != 1

    def _invert_position(self, position):
        return [self._dna_length - position[1] + 1,
                self._dna_length - position[0] + 1]

    def _extract_rbs(self, row):
        self.rbs_start = int(row[self._indices.rbs_start])
        self.rbs_end = int(row[self._indices.rbs_end])
        if not self.is_undefined_rbs() and self.is_antisense():
            self.rbs_start, self.rbs_end = self._invert_position(
                [self.rbs_start, self.rbs_end]
                )

    def is_undefined_rbs(self):
        return self.rbs_start == self.rbs_end == 0

    def _extract_sequence(self, row):
        self.sequence = row[self._indices.sequence].strip()
        assert(len(self.sequence) == (self.end - self.start + 1))

    def _extract_aa_seq(self, row):
        self.aa_seq = row[self._indices.aa_seq].strip()
        self.aa_composition = []
        # we skip initial fMet
        aa_seq = self.aa_seq[1:]
        for aa in self.aas:
            self.aa_composition.append(aa_seq.count(aa))

    def _extract_TUs(self, row):
        self.TUs = []
        if row[self._indices.TU]:
            self.TUs = map(str.strip, row[self._indices.TU].split(','))

    def set_rbs_at_valid_location(self):
        if (self.is_rbs_after_start_codon() or
                self.is_rbs_too_far_before_stop_codon()):
            self.set_rbs_at_default_location()

    def set_rbs_at_default_location(self):
        self.rbs_start = self.start - 21
        self.rbs_end = self.start + 2

    def is_rbs_after_start_codon(self):
        return self.rbs_start > self.start

    def is_rbs_too_far_before_stop_codon(self):
        return self.rbs_start < self.start - 50

    def is_usable(self):
        return (self.has_valid_sequence_length() and
                self.has_valid_stop_codon() and
                not self.contains_premature_stop_codon() and
                self.is_aa_sequence_consistent_with_sequence())

    def has_valid_sequence_length(self):
        return len(self.sequence) % 3 == 0

    def has_valid_start_codon(self):
        return is_valid_start_codon(self.sequence[:3])

    def has_valid_stop_codon(self):
        return is_valid_stop_codon(self.sequence[-3:])

    def contains_premature_stop_codon(self):
        nb_codons = int(len(self.sequence) / 3)
        codons = [self.sequence[3*i:(3*i+3)] for i in range(nb_codons)]
        return any(is_valid_stop_codon(c) for c in codons[:-1])

    def is_aa_sequence_consistent_with_sequence(self):
        return len(self.sequence) == 3*(len(self.aa_seq)+1)

    def name_and_size(self):
        return '{} [{}:{}]'.format(self.name, self.rbs_start, self.end)


def is_valid_start_codon(codon):
    return codon in ['atg', 'ctg', 'gtg', 'ttg']


def is_valid_stop_codon(codon):
    return codon in ['taa', 'tag', 'tga']
