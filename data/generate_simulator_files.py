

from src.TUreader import *
from src.GeneReader import *
from src.simulationexport import *

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

# associate genes to TUs
TU_reader.associate_genes(gene_reader.genes, log_file)

# write output
simulation_export = SimulationExport()
f = open('output/rnas.in','w')
simulation_export.write_detailed_TUs(TU_reader.TUs, f)
f.close()
f = open('output/proteins.in','w')
simulation_export.write_detailed_proteins(TU_reader.TUs, f)
f.close()
f = open('output/agregated_rnas.in','w')
simulation_export.write_agregated_TUs(TU_reader.TUs, f)
f.close()
f = open('output/agregated_proteins.in','w')
simulation_export.write_agregated_proteins(TU_reader.TUs, f)
f.close()
f = open('output/translation.in','w')
simulation_export.write_tRNA_activation(f)
simulation_export.write_translation_pre_initiation(f)
simulation_export.write_translation_initiation(f)
simulation_export.write_translation_elongation(f)
simulation_export.write_translation_termination(f)
f.close()
f = open('output/transcription.in','w')
simulation_export.write_transcription_pre_initiation(f)
simulation_export.write_transcription_initiation(f)
simulation_export.write_transcription_elongation(f)
simulation_export.write_transcription_termination(f)
f.close()

# close log file
log_file.close()

