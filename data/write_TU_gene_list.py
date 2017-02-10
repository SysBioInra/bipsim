from src.TUreader import *
from src.GeneReader import *

# open global log file
log_file = open('log.txt', 'w')

# read input files
dna_file = open('input/dna.txt','r')
dna = dna_file.readline().strip()
dna_file.close()        
gene_file = open('input/genes.txt','r')
gene_reader = GeneReader(gene_file, len(dna), log_file)
gene_file.close()
tu_file = open('input/TUs.txt','r')
TU_reader = TUReader(tu_file, dna)
tu_file.close()

# associate genes to TUs and store TUs
TU_reader.associate_genes(gene_reader.genes, log_file)

# close log file
log_file.close()

f = open('gene_list.txt','w')
for TU in TU_reader.TUs:
    if TU.sigma != 'SigA': continue
    line = TU.name + ' ' + ' '.join([g.bsu for g in TU.genes]) + '\n'
    f.write(line);
