
import csv

class TU:
    def __init__(self, name, start, end, sense, sigma, genes):
        self.name = name
        self.start = start
        self.end = end
        self.sense = sense
        self.sigma = sigma
        self.annotation_genes = genes
        self.genes = []

    def associate_genes(self, genes, log_stream):
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
    def __init__(self, input_stream, dna_length):
        parser = csv.reader(input_stream, delimiter = '\t')
        header = parser.next()
        name_index = header.index('name')
        start_index = header.index('start')
        end_index = header.index('end')
        sense_index = header.index('brin_DNA')
        sigma_index = header.index('sigma')
        gene_index = header.index('genes')
        self.TUs = []
        for r in parser:
            name = r[name_index]
            start = int(r[start_index])
            end = int(r[end_index])
            assert(start<end)
            sense = int(r[sense_index])
            if sense != 1:
                tmp = dna_length-end+1
                end = dna_length-start+1
                start = tmp
            assert(start<end)
            sigma = r[sigma_index]
            if (r[gene_index] != ''):
                genes = map(str.strip, r[gene_index].split(','))
            else:
                genes = []
            self.TUs.append(TU(name, start, end, sense, sigma, genes))

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
                
    def write_simulator_input(self, TU_stream, protein_stream):
        k_on = '10'
        k_off = '1'
        for TU in self.TUs:
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
            TU_stream.write(lines)
            # handle protein output
            for g in TU.genes:
                name = g.name + '_' + g.bsu
                lines = 'ChemicalSequence ' + name + ' ' \
                        + 'product_of ' + TU.name + ' ' \
                        + str(g.start) + ' ' + str(g.end-3) + ' proteins\n'
                lines += 'BindingSite RBS ' + TU.name + ' ' \
                         + str(g.rbs_start) + ' ' + str(g.rbs_end) + ' ' \
                         + k_on + ' ' + k_off + ' ' \
                         + str(g.start) + '\n'
                protein_stream.write(lines)
