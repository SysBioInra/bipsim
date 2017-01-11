

from src.TUreader import *
from src.GeneReader import *

log_file = open('log.txt', 'w')

# read input files
dna_file = open('input/dna.txt','r')
dna = dna_file.readline().strip()
dna_file.close()

gene_file = open('input/genes.txt','r')
gene_reader = GeneReader(gene_file, len(dna), log_file)
gene_file.close()

tu_file = open('input/TUs.txt','r')
TU_reader = TUReader(tu_file, len(dna))
tu_file.close()

# associate genes to TUs
TU_reader.associate_genes(gene_reader.genes, log_file)

# write output
rna_file = open('output/rnas.in','w')
protein_file = open('output/proteins.in','w')
TU_reader.write_simulator_input(rna_file, protein_file)
rna_file.close()
protein_file.close()
log_file.close()

