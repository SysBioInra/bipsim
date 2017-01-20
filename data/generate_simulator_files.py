
def write_detailed_files(TUs):
    simulation_export = SimulationExport()
    f = open('output/rnas.in','w')
    simulation_export.write_TUs(TUs, f)
    simulation_export.write_termination_sites(TUs, f)
    f.close()
    f = open('output/proteins.in','w')
    simulation_export.write_proteins(TUs, f)
    f.close()
    f = open('output/translation.in','w')
    simulation_export.write_translation_input(f)
    simulation_export.write_tRNA_activation(f)
    simulation_export.write_translation_pre_initiation(f)
    simulation_export.write_translation_initiation(f)
    simulation_export.write_translation_elongation(f)
    simulation_export.write_translation_termination(f)
    f.close()
    f = open('output/transcription.in','w')
    simulation_export.write_transcription_input(f)
    simulation_export.write_transcription_pre_initiation(f)
    simulation_export.write_transcription_initiation(f)
    simulation_export.write_transcription_elongation(f)
    simulation_export.write_transcription_termination(f)
    f.close()

def write_agregated_files(TUs):
    simulation_export = SimulationExport()
    f = open('output/rnas.in','w')
    simulation_export.write_TUs(TUs, f)
    f.close()
    f = open('output/proteins.in','w')
    simulation_export.write_proteins(TUs, f)
    f.close()
    f = open('output/translation.in','w')
    simulation_export.write_translation_input(f)
    simulation_export.write_tRNA_activation(f)
    simulation_export.write_translation_pre_initiation(f)
    simulation_export.write_translation_initiation(f)
    simulation_export.write_agregated_translation_elongation(TUs, f)
    f.close()
    f = open('output/transcription.in','w')
    simulation_export.write_transcription_input(f)
    simulation_export.write_transcription_pre_initiation(f)
    simulation_export.write_transcription_initiation(f)
    simulation_export.write_agregated_transcription_elongation(TUs, f)
    f.close()


if __name__ == '__main__':
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
    write_agregated_files(TU_reader.TUs)

    # close log file
    log_file.close()

