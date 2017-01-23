from src.replicationexport import *
from src.transcriptionexport import *
from src.translationexport import *
from src.metabolismexport import *

def write_detailed_files(TUs):
    replication_export = ReplicationExport(TUs)
    f = open('output/replication.in','w')
    replication_export.write_input(f)
    replication_export.write_initiation(f)
    replication_export.write_elongation(f)
    f.close()

    transcription_export = TranscriptionExport(TUs)
    f = open('output/rnas.in','w')
    transcription_export.write_TUs(f)
    transcription_export.write_termination_sites(f)
    f.close()
    f = open('output/transcription.in','w')
    transcription_export.write_input(f)
    transcription_export.write_pre_initiation(f)
    transcription_export.write_initiation(f)
    transcription_export.write_elongation(f)
    transcription_export.write_termination(f)
    f.close()

    translation_export = TranslationExport(TUs)
    f = open('output/proteins.in','w')
    translation_export.write_proteins(f)
    f.close()
    f = open('output/translation.in','w')
    translation_export.write_input(f)
    translation_export.write_tRNA_activation(f)
    translation_export.write_pre_initiation(f)
    translation_export.write_initiation(f)
    translation_export.write_elongation(f)
    translation_export.write_termination(f)
    f.close()

    metabolism_export = MetabolismExport()
    f = open('output/metabolites.in','w')
    metabolism_export.write_input(f)
    metabolism_export.write_pseudo_metabolism(f)
    f.close()

def write_agregated_files(TUs):
    replication_export = ReplicationExport(TUs)
    f = open('output/replication.in','w')
    replication_export.write_input(f)
    replication_export.write_initiation(f)
    replication_export.write_elongation(f)
    f.close()

    transcription_export = TranscriptionExport(TUs)
    f = open('output/rnas.in','w')
    transcription_export.write_TUs(f)
    f.close()
    f = open('output/transcription.in','w')
    transcription_export.write_input(f)
    transcription_export.write_pre_initiation(f)
    transcription_export.write_initiation(f)
    transcription_export.write_agregated_elongation(f)
    f.close()
    translation_export = TranslationExport(TUs)
    f = open('output/proteins.in','w')
    translation_export.write_proteins(f)
    f.close()
    f = open('output/translation.in','w')
    translation_export.write_input(f)
    translation_export.write_tRNA_activation(f)
    translation_export.write_pre_initiation(f)
    translation_export.write_initiation(f)
    translation_export.write_agregated_elongation(f)
    f.close()

    metabolism_export = MetabolismExport()
    f = open('output/metabolites.in','w')
    metabolism_export.write_input(f)
    metabolism_export.write_pseudo_metabolism(f)
    f.close()
    
if __name__ == '__main__':
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
    
    # associate genes to TUs
    TU_reader.associate_genes(gene_reader.genes, log_file)

    # write output
    write_agregated_files(TU_reader.TUs)

    # close log file
    log_file.close()

