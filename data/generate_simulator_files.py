from src.TUreader import *
from src.GeneReader import *
from src.replicationexport import *
from src.transcriptionexport import *
from src.translationexport import *
from src.metabolismexport import *

def export_simulation_data(data_set, process_agregation, metab_agregation):
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
    TUs = TU_reader.TUs
    
    # close log file
    log_file.close()

    # create export objects
    replication_export = ReplicationExport()
    transcription_export = TranscriptionExport()
    translation_export = TranslationExport()
    metabolism_export = MetabolismExport()
    exporters = [replication_export, transcription_export,
                 translation_export, metabolism_export]

    # set parameters
    write_replication = True
    if data_set.upper() == "TEST":
        for obj in exporters: obj.set_test_parameters()
    elif data_set.upper() == "AN":
        for obj in exporters: obj.set_AN_parameters()
    elif data_set.upper() == "PAULSSON":
        for obj in exporters: obj.set_paulsson_parameters()
        write_replication = False
    else:
        print 'UNKNOWN DATA SET: please choose one of the following:'
        print '\tTEST, AN, PAULSSON,'
        return

    agregated_TUs = []
    if process_agregation.upper() == "AGREGATED":
        agregated_TUs = TUs
    elif process_agregation.upper() == "HYBRID":
        # pick last 95% of TUs and agregate them
        agregated_TUs = TUs[(5*len(TUs)/100):]
    elif process_agregation.upper() == "DETAILED":
        pass
    else:
        print('UNKNWON AGREGATION LEVEL FOR PROCESSES: ' + process_agregation
              + ', please choose one of the following:\n'
              '\tDETAILED, HYBRID, AGREGATED')
        return

    if metab_agregation.upper() == 'CONSTANT':
        for obj in exporters: obj.cut_slow_reactions = True
    elif metab_agregation.upper() == 'STACKED':
        for obj in exporters: obj.agregate_slow_reactions = True
    elif metab_agregation.upper() == 'DETAILED':
        pass
    else:
        print('UNKNWON AGREGATION METHOD FOR METABOLISM: ' + metab_agregation
              + ', please choose one of the following:\n'
              '\tCONSTANT, STACKED, DETAILED')
        return
    
    # write files
    f = open('output/replication.in','w')
    if write_replication:
        replication_export.write_input(f)
        replication_export.write_initiation(f)
        replication_export.write_elongation(f)
    f.close()
    f = open('output/rnas.in','w')
    transcription_export.write_TUs(f, TUs)
    transcription_export.write_degradation(f, TUs)
    f.close()
    f = open('output/transcription.in','w')
    transcription_export.write_input(f)
    transcription_export.write_pre_initiation(f)
    transcription_export.write_initiation(f)
    transcription_export.write_elongation(f)
    transcription_export.write_termination(f)
    transcription_export.write_agregated_elongation(f, agregated_TUs)
    f.close()
    f = open('output/proteins.in','w')
    translation_export.write_proteins(f, TUs)
    translation_export.write_degradation(f, TUs)
    f.close()
    f = open('output/translation.in','w')
    translation_export.write_input(f)
    translation_export.write_tRNA_activation(f)
    translation_export.write_pre_initiation(f)
    translation_export.write_initiation(f)
    translation_export.write_elongation(f)
    translation_export.write_termination(f)
    translation_export.write_agregated_elongation(f, agregated_TUs)
    f.close()
    f = open('output/metabolites.in','w')
    metabolism_export.write_input(f)
    metabolism_export.write_pseudo_metabolism(f)
    f.close()
    
if __name__ == '__main__':
    import sys
    if len(sys.argv) == 4:
        export_simulation_data(sys.argv[1], sys.argv[2], sys.argv[3])
    else:
        print('ERROR: run program with arguments: '
              '\tDATA_SET PROCESS_AGREGATION_LEVEL METABOLISM_AGREGATION_LEVEL')


