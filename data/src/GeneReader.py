"""Module reading gene/protein information from subtilis data set."""

import csv


class Gene:
    def __init__(self, name, bsu, position, rbs, sense, aa_seq, TUs):
        self.name = name
        self.bsu = bsu
        self.start = position[0]
        self.end = position[1]
        self.rbs_start = rbs[0]
        self.rbs_end = rbs[1]
        self.sense = sense
        self.TUs = TUs
        aas = ['A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N',
               'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'Y']
        self.aa_composition = []
        # we do not count initial fMet
        aa_seq = aa_seq[1:]
        for aa in aas:
            self.aa_composition.append(aa_seq.count(aa))

    def name_and_size(self):
        return '{} [{}:{}]'.format(self.name, self.rbs_start, self.end)


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


class GeneReader:

    def __init__(self, input_stream, dna_length, log_stream=None):
        parser = csv.reader(input_stream, delimiter='\t')
        header = next(parser)
        indices = ColumnIndices(header)
        self.genes = []
        self._no_RBS = []
        self._invalid_RBS = []
        self._long_RBS = []
        invalid_start = []
        invalid_stop = []
        invalid_sequence = []
        invalid_aa_seq = []
        for r in parser:
            if r[indices.type] != 'CDS':
                continue
            name, bsu, position, sense = \
                self._extract_base_info(indices, dna_length)
            rbs = self._extract_rbs(indices, position, dna_length)

            # check sequence
            sequence = r[indices.sequence].strip()
            aa_seq = r[indices.aa_seq].strip()
            assert(len(sequence) == (position[1] - position[0] + 1))
            if not self._is_valid_sequence(sequence):
                invalid_sequence.append(name)
                continue
            if (len(sequence) != 3*(len(aa_seq)+1)):
                invalid_aa_seq.append(name)
                continue
            if not self._is_valid_start_codon(sequence):
                invalid_start.append(name)
            if not self._is_valid_stop_codon(sequence):
                invalid_stop.append(name)
                continue

            # read TUs
            if (r[indices.TU] != ''):
                TUs = map(str.strip, r[indices.TU].split(','))
            else:
                TUs = []

            # create gene
            self.genes.append(Gene(name, bsu, position, rbs, sense,
                                   aa_seq, TUs))

        # log all relevant information
        if log_stream:
            msg = 'Genes with no RBS info, ' \
                  + 'RBS was set at 21 bases from start codon: '
            log_stream.write(msg + ', '.join(no_RBS) + '.\n\n')
            msg = 'Genes with invalid RBS info, RBS was set' \
                  + ' at 21 bases from start codon: '
            log_stream.write(msg + ', '.join(invalid_RBS) + '.\n\n')
            msg = 'Genes with RBS far away from gene, RBS was set' \
                  + ' at 21 bases from start codon: '
            log_stream.write(msg + ', '.join(long_RBS) + '.\n\n')
            msg = 'Genes with invalid stop codon (they were excluded): '
            log_stream.write(msg + ', '.join(invalid_stop) + '.\n\n')
            msg = 'Genes with invalid start codon: '
            log_stream.write(msg + ', '.join(invalid_start) + '.\n\n')
            msg = ('Genes with invalid length or containing a stop codon '
                   '(they were excluded): ')
            log_stream.write(msg + ', '.join(invalid_sequence) + '.\n\n')
            msg = 'Genes with invalid aa sequence (they were excluded): '
            log_stream.write(msg + ', '.join(invalid_aa_seq) + '.\n\n')

    def _get_base_info(self, indices):
        name = r[indices.name]
        bsu = r[indices.bsu]
        position = [int(r[indices.start]), int(r[indices.end])]
        assert(position[0] < position[1])
        sense = int(r[indices.sense])
        if sense != 1:
            position = self._invert_position(position, dna_length)
        return name, bsu, position, sense

    def _invert_position(self, position, dna_length):
        return [dna_length - position[0] + 1, dna_length - position[1] + 1]

    def _extract_rbs(self, indices, position, dna_length):
        [int(r[indices.rbs_start]), int(r[indices.rbs_end])]
        if self._is_undefined_rbs(rbs):
            no_RBS.append(name)
            rbs[0] = self._default_rba_start(position)
        else:
            if sense != 1:
                rbs = self._invert_position(rbs, dna_length)
            if rbs[0] > position[0]:
                invalid_RBS.append(name)
                rbs[0] = self._default_rba_start(position)
            if rbs[0] < position[0] - 50:
                long_RBS.append(name)
                rbs[0] = self._default_rba_start(position)
        rbs[1] = position[0] + 2
        return rbs

    def _is_undefined_rbs(self, rbs):
        return rbs[0] == rbs[1] == 0

    def _default_rba_start(self, position):
        return position[0] - 21


    def _is_valid_start_codon(self, sequence):
        start_codon = sequence[:3]
        return start_codon in ['atg', 'ctg', 'gtg', 'ttg']

    def _is_valid_stop_codon(self, sequence):
        stop_codon = sequence[-3:]
        return stop_codon in ['taa', 'tag', 'tga']

    def _is_valid_sequence(self, sequence):
        # check sequence length
        if len(sequence) % 3 != 0:
            return False
        # we do not count the stop codon here
        nb_codons = len(sequence) / 3 - 1
        codons = [sequence[3*i:(3*i+3)] for i in range(nb_codons)]
        for c in codons:
            if self._is_valid_stop_codon(c):
                return False
        return True
