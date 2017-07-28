
import csv

class TU:
    def __init__(self, name, position, sequence, sense, sigma, genes):
        self.name = name
        self.start = position[0]
        self.end = position[1]
        self.sense = sense
        self.sigma = sigma
        self.annotation_genes = genes
        self.genes = []
        self.composition = [sequence.count('a'), sequence.count('c'), \
                            sequence.count('g'), sequence.count('t')]

    def associate_genes(self, genes, log_stream=None):
        not_found = self.annotation_genes[:]
        for g in genes:
            if (g.name in not_found) and self._overlaps(g):
                self.genes.append(g)
                not_found.remove(g.name)
                if not(self._contains_gene(g)):
                    if log_stream:
                        msg = self.name_and_size() + ': gene ' \
                              + g.name_and_size() \
                              + ' is not fully included on TU. Extending TU' \
                              + ' to match gene size.\n'
                        log_stream.write(msg)
                    if (g.rbs_start < self.start):
                        self.start = g.rbs_start
                    if (g.end > self.end):
                        self.end = g.end

        # remove False if you want missing genes appearing in the log file
        # (should not be necessary as 'missing genes' are actually generally
        # RNAs, it does not seem that any 'real' gene is missing)
        if False and (len(not_found) > 0) and log_stream:
            msg = self.name_and_size() + ': WARNING: Following genes ' \
                    + 'could not be retrieved: ' + ', '.join(not_found) \
                    + '\n'
            log_stream.write(msg)

    def _contains_gene(self, gene):
        return (gene.sense == self.sense) and \
            (gene.rbs_start >= self.start) and (gene.end <= self.end)

    def _overlaps(self, gene):
        return (gene.sense == self.sense) \
            and (gene.end >= self.start) and (self.end >= gene.rbs_start)

    def name_and_size(self):
        return self.name + ' [' + str(self.start) + ':' \
            + str(self.end) + ']'

class TUReader:
    def __init__(self, input_stream, dna):
        parser = csv.reader(input_stream, delimiter = '\t')
        header = parser.next()
        name_index = header.index('name')
        start_index = header.index('start')
        end_index = header.index('end')
        sense_index = header.index('brin_DNA')
        sigma_index = header.index('sigma')
        gene_index = header.index('genes')
        self.TUs = []
        dna_length = len(dna)
        c_dna = ''
        for l in reversed(dna):
            if l == 'a': c_dna += 't'
            elif l == 'c': c_dna += 'g'
            elif l == 'g': c_dna += 'c'
            elif l == 't': c_dna += 'a'
            else:
                print 'DNA invalid...'
        for r in parser:
            name = r[name_index]
            position = [int(r[start_index]), int(r[end_index])]
            assert(position[0] < position[1])
            sense = int(r[sense_index])
            if sense == 1:
                sequence = dna[position[0]-1:position[1]-1]
            else:
                tmp = dna_length - position[1] + 1
                position[1] = dna_length - position[0] + 1
                position[0] = tmp
                sequence = c_dna[position[0]-1:position[1]-1]
            sigma = r[sigma_index]
            if (r[gene_index] != ''):
                genes = map(str.strip, r[gene_index].split(','))
            else:
                genes = []
            self.TUs.append(TU(name, position, sequence, sense, sigma, genes))

    def associate_genes(self, genes, log_stream = None):
        # remove orphan genes
        if log_stream:
            log_stream.write('Removing orphan genes: ')
        orphans = self._find_orphan_genes(genes)
        if log_stream:
            log_stream.write(', '.join([g.name for g in orphans]))
            log_stream.write('.\nRemoved ' + str(len(orphans)) + ' orphans.\n\n')

        # associate remaining genes
        if log_stream:
            log_stream.write('Adding remaining genes to TUs...\n')
        for TU in self.TUs:
            TU.associate_genes(genes, log_stream)
        if log_stream:
            log_stream.write('Done.\n')


    def check_TU_overlap(self):
        for TU_1 in self.TUs:
            for TU_2 in self.TUs:
                if self._overlaps(TU_1, TU_2):
                    print TU_1.name_and_size() + ' overlaps ' \
                        + TU_2.name_and_size() + '.'

    def _overlaps(self, TU_1, TU_2):
        # we consider that two TUs overlap if they share some bases
        # AND do not end at the same terminator (which would be a 'valid'
        # overlap)
        if (TU_1.sense != TU_2.sense): return False
        if (TU_1.start < TU_2.start):
            return (TU_1.end >= TU_2.start) and (TU_1.end != TU_2.end)
        else:
            return (TU_1.start <= TU_2.end) and (TU_1.end != TU_2.end)

    def _find_orphan_genes(self, genes):
        orphans = []
        for g in genes:
            found = False
            for TU in self.TUs:
                if (g.name in TU.annotation_genes):
                    found = True
                    break
            if not(found):
                orphans.append(g)
        return orphans
