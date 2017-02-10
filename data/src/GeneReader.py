
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
        aas = ['A','C','D','E','F','G','H','I','K','L','M','N', \
               'P','Q','R','S','T','V','W','Y']
        self.aa_composition = []
        # we do not count initial fMet
        aa_seq = aa_seq[1:]
        for aa in aas:
            self.aa_composition.append(aa_seq.count(aa))

    def name_and_size(self):
        return self.name + ' [' + str(self.rbs_start) + ':' \
            + str(self.end) + ']'                

class GeneReader:
    def __init__(self, input_stream, dna_length, log_stream = None):
        parser = csv.reader(input_stream, delimiter = '\t')
        header = parser.next()
        name_index = header.index('name')
        bsu_index = header.index('BSU')
        start_index = header.index('start')
        end_index = header.index('end')
        rbs_start_index = header.index('RBSstart')
        rbs_end_index = header.index('RBSend')
        sense_index = header.index('brin_DNA')
        sequence_index = header.index('seq')
        aa_seq_index = header.index('aaseq')
        TU_index = header.index('TUs')
        type_index = header.index('gene_category')
        self.genes = []
        no_RBS = []
        invalid_RBS = []
        long_RBS = []
        invalid_start = []
        invalid_stop = []
        invalid_sequence = []
        invalid_aa_seq = []
        for r in parser:
            if r[type_index] != 'CDS':
                continue
            name = r[name_index]
            bsu = r[bsu_index]
            position = [int(r[start_index]), int(r[end_index])]
            assert(position[0] < position[1])
            sense = int(r[sense_index])
            if sense != 1:
                self._invert_position(position, dna_length)
                
            # read RBS info
            rbs = [int(r[rbs_start_index]), int(r[rbs_end_index])]
            if rbs[0] == rbs[1] == 0:
                no_RBS.append(name)
                rbs[0] = position[0] - 21
            else:
                if sense != 1:
                    self._invert_position(rbs, dna_length)
                if rbs[0] > position[0]:
                    invalid_RBS.append(name)
                    rbs[0] = position[0] - 21
                if rbs[0] < position[0] - 50:
                    long_RBS.append(name)
                    rbs[0] = position[0] - 21
            rbs[1] = position[0] + 2
            
            # check sequence
            sequence = r[sequence_index].strip()
            aa_seq = r[aa_seq_index].strip()
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
            if (r[TU_index] != ''):
                TUs = map(str.strip, r[TU_index].split(','))
            else:
                TUs = []

            # create gene
            self.genes.append(Gene(name, bsu, position, rbs, sense, \
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
            msg = 'Genes with invalid length or containing a stop codon (they were excluded): '
            log_stream.write(msg + ', '.join(invalid_sequence) + '.\n\n')
            msg = 'Genes with invalid aa sequence (they were excluded): '
            log_stream.write(msg + ', '.join(invalid_aa_seq) + '.\n\n')

    def _is_valid_start_codon(self, sequence):
        start_codon = sequence[:3]
        return start_codon in ['atg', 'ctg', 'gtg', 'ttg']

    def _is_valid_stop_codon(self, sequence):
        stop_codon = sequence[-3:]
        return stop_codon in ['taa', 'tag', 'tga']

    def _is_valid_sequence(self, sequence):
        # check sequence length
        if (len(sequence)) % 3 != 0:
            return False
        # we do not count the stop codon here
        nb_codons = len(sequence) / 3 - 1
        codons = [sequence[3*i:(3*i+3)] for i in range(nb_codons)]
        for c in codons:
            if self._is_valid_stop_codon(c):
                return False
        return True

    def _invert_position(self, position, dna_length):
        tmp = dna_length - position[1] + 1
        position[1] = dna_length - position[0] + 1
        position[0] = tmp
