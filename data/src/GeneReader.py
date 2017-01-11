
import csv

class Gene:
    def __init__(self, name, bsu, position, rbs, sense, TUs):
        self.name = name
        self.bsu = bsu
        self.start = position[0]
        self.end = position[1]
        self.rbs_start = rbs[0]
        self.rbs_end = rbs[1]
        self.sense = sense
        self.TUs = TUs

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
        TU_index = header.index('TUs')
        type_index = header.index('gene_category')
        self.genes = []
        no_RBS = []
        invalid_RBS = []
        invalid_start = []
        invalid_stop = []
        invalid_length = []
        for r in parser:
            if r[type_index] != 'CDS':
                continue
            name = r[name_index]
            bsu = r[bsu_index]
            position = [int(r[start_index]), int(r[end_index])]
            assert(position[0] < position[1])
            if (position[1] - position[0] + 1) % 3 != 0:
                invalid_length.append(name)
                continue
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
            rbs[1] = position[0] + 2
            
            # check sequence
            sequence = r[sequence_index]
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
            self.genes.append(Gene(name, bsu, position, rbs, sense, TUs))
            
        # log all relevant information
        if log_stream:
            msg = 'For the following genes, no RBS info was found, ' \
                  + 'RBS was set at 21 bases from start codon: '
            log_stream.write(msg + ', '.join(no_RBS) + '.\n\n')
            msg = 'For the following genes, RBS was invalid. RBS was set' \
                  + ' at 21 bases from start codon: '
            log_stream.write(msg + ', '.join(invalid_RBS) + '.\n\n')
            msg = 'Following genes had invalid stop codon (they were excluded): '
            log_stream.write(msg + ', '.join(invalid_stop) + '.\n\n')
            msg = 'Following genes had invalid start codon: '
            log_stream.write(msg + ', '.join(invalid_start) + '.\n\n')
            msg = 'Following genes had invalid length (they were excluded): '
            log_stream.write(msg + ', '.join(invalid_length) + '.\n\n')

    def _is_valid_start_codon(self, sequence):
        start_codon = sequence[:3]
        return start_codon in ['atg', 'ctg', 'gtg', 'ttg']

    def _is_valid_stop_codon(self, sequence):
        stop_codon = sequence[-3:]
        return stop_codon in ['taa', 'tag', 'tga']

    def _invert_position(self, position, dna_length):
        tmp = dna_length - position[1] + 1
        position[1] = dna_length - position[0] + 1
        position[0] = tmp
