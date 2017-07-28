"""Export files needed for MyBacteria simulation."""

from __future__ import absolute_import, division, print_function
from src.TUreader import TUReader, TU
from src.GeneReader import GeneReader
from src.replicationexport import ReplicationExport
from src.transcriptionexport import TranscriptionExport
from src.translationexport import TranslationExport
from src.metabolismexport import MetabolismExport
from src import (replicationparams, transcriptionparams,
                 translationparams, metabolismparams)


def main():
    """Export files for MyBacteria."""
    import sys
    if len(sys.argv) == 4:
        TUs, gene_TUs = read_data()
        export_simulation_data(TUs, gene_TUs,
                               sys.argv[1], sys.argv[2], sys.argv[3])
    else:
        print('ERROR: run program with arguments:\n'
              '\tDATA_SET PROCESS_AGGREGATION_LEVEL '
              'METABOLISM_AGGREGATION_LEVEL')


def read_data():
    """Import subtilis data for dna, rnas and proteins."""
    # open global log file
    log_file = open('log.txt', 'w')

    # read input files
    dna_file = open('input/dna.txt', 'r')
    dna = dna_file.readline().strip()
    dna_file.close()
    gene_file = open('input/genes.txt', 'r')
    gene_reader = GeneReader(gene_file, len(dna), log_file)
    gene_file.close()
    tu_file = open('input/TUs.txt', 'r')
    TU_reader = TUReader(tu_file, dna)
    tu_file.close()
    # associate genes to TUs and store TUs
    TU_reader.associate_genes(gene_reader.genes, log_file)
    # create a set of fake TUs where a TU exactly encompasses a gene.
    gene_TUs = adjust_TUs_to_genes(gene_reader.genes, dna)
    # close log file
    log_file.close()
    return TU_reader.TUs, gene_TUs


def export_simulation_data(
        real_TUs, gene_TUs, data_set, process_aggregation, metab_aggregation
        ):
    """Write simulation files."""
    # get parameters
    (replication_params, transcription_params,
     translation_params, metabolism_params) \
        = parameters(data_set, metab_aggregation)
    TUs = gene_TUs if data_set.upper() == 'PAULSSON' else real_TUs
    aggregated_TUs = select_aggregated_TUs(TUs, process_aggregation)

    # write files
    with open('output/replication.in', 'w') as f:
        if replication_params.export:
            replication = ReplicationExport(replication_params)
            f.write(replication.input())
            f.write(replication.initiation())
            f.write(replication.elongation())
    transcription = TranscriptionExport(transcription_params)
    with open('output/rnas.in', 'w') as f:
        f.write(transcription.transcription_units(TUs))
        f.write(transcription.degradation(TUs))
    with open('output/transcription.in', 'w') as f:
        f.write(transcription.input())
        f.write(transcription.pre_initiation())
        f.write(transcription.initiation())
        f.write(transcription.elongation())
        f.write(transcription.termination())
        f.write(transcription.aggregated_elongation(aggregated_TUs))
    translation = TranslationExport(translation_params)
    with open('output/proteins.in', 'w') as f:
        f.write(translation.proteins(TUs))
        f.write(translation.degradation(TUs))
    with open('output/translation.in', 'w') as f:
        f.write(translation.input())
        f.write(translation.tRNA_activation())
        f.write(translation.pre_initiation())
        f.write(translation.initiation())
        f.write(translation.elongation())
        f.write(translation.termination())
        f.write(translation.aggregated_elongation(aggregated_TUs))
    with open('output/metabolites.in', 'w') as f:
        metabolism = MetabolismExport(metabolism_params)
        f.write(metabolism.input())
        f.write(metabolism.pseudo_metabolism())


def adjust_TUs_to_genes(genes, dna):
    """Create exactly one TU for each gene."""
    TUs = []
    dna_converter = {'a': 't', 'c': 'g', 'g': 'c', 't': 'a'}
    c_dna = ''.join(dna_converter[l] for l in reversed(dna))
    for gene in genes:
        position = [gene.rbs_start, gene.end]
        assert(position[0] < position[1])
        if gene.sense == 1:
            sequence = dna[position[0]-1:position[1]-1]
        else:
            sequence = c_dna[position[0]-1:position[1]-1]
        new_TU = TU(gene.bsu + '_rna', position, sequence, gene.sense,
                    'SigA', [gene.name])
        new_TU.associate_genes([gene])
        TUs.append(new_TU)
    return TUs


def parameters(data_set, metab_aggregation):
    """Return parameters corresponding to data set selected."""
    # get general parameters
    if data_set.upper() == "TEST":
        replication = replicationparams.test_parameters()
        transcription = transcriptionparams.test_parameters()
        translation = translationparams.test_parameters()
        metabolism = metabolismparams.test_parameters()
    elif data_set.upper() == "AN":
        replication = replicationparams.AN_parameters()
        transcription = transcriptionparams.AN_parameters()
        translation = translationparams.AN_parameters()
        metabolism = metabolismparams.AN_parameters()
    elif data_set.upper() == "PAULSSON":
        replication = replicationparams.paulsson_parameters()
        transcription = transcriptionparams.paulsson_parameters()
        translation = translationparams.paulsson_parameters()
        metabolism = metabolismparams.paulsson_parameters()
    else:
        print('UNKNOWN DATA SET: please choose one of the following:')
        print('\tTEST, AN, PAULSSON,')
        raise UserWarning('Unknown data set.')

    params = [replication, transcription, translation, metabolism]
    if metab_aggregation.upper() == 'CONSTANT':
        for obj in params:
            obj.cut_slow_reactions = True
    elif metab_aggregation.upper() == 'STACKED':
        for obj in params:
            obj.aggregate_slow_reactions = True
    elif metab_aggregation.upper() == 'DETAILED':
        pass
    else:
        print('UNKNWON AGGREGATION METHOD FOR METABOLISM: ' + metab_aggregation
              + ', please choose one of the following:\n'
              '\tCONSTANT, STACKED, DETAILED')
        raise UserWarning('Unknown aggregation method.')

    return replication, transcription, translation, metabolism


def select_aggregated_TUs(TUs, process_aggregation):
    """Select TUs that need to be aggregated."""
    if process_aggregation.upper() == "AGGREGATED":
        aggregated_TUs = TUs
    elif process_aggregation.upper() == "HYBRID":
        # pick last 95% of TUs and aggregate them
        aggregated_TUs = TUs[int(5*len(TUs)/100):]
    elif process_aggregation.upper() == "DETAILED":
        aggregated_TUs = []
    else:
        print('UNKNWON AGGREGATION LEVEL FOR PROCESSES: '
              + process_aggregation
              + ', please choose one of the following:\n'
              '\tDETAILED, HYBRID, AGGREGATED')
        raise UserWarning('Invalid aggregation level.')
    return aggregated_TUs


if __name__ == '__main__':
    main()
