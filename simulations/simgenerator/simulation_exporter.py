"""Export files needed for MyBacteria simulation."""

from __future__ import absolute_import, division, print_function

import os.path
from collections import namedtuple

from simgenerator.TUreader import TUReader, TU
from simgenerator.GeneReader import GeneReader
from simgenerator.replicationexport import ReplicationExport
from simgenerator.transcriptionexport import TranscriptionExport
from simgenerator.translationexport import TranslationExport
from simgenerator.metabolismexport import MetabolismExport
from simgenerator import (replicationparams, transcriptionparams,
                          translationparams, metabolismparams)


Rates = namedtuple('Rates', 'bsu promoter rna_deg rbs prot_deg')
SimulationParameters = namedtuple(
    'SimulationParameters',
    'TUs aggregated_TUs replication transcription translation metabolism '
    )


def read_data():
    """Import subtilis data for dna, rnas and proteins."""
    input_dir = 'data'
    # open global log file
    log_file = open('log.txt', 'w')

    # read input files
    dna_file = open(os.path.join(input_dir, 'dna.txt'), 'r')
    dna = dna_file.readline().strip()
    dna_file.close()
    gene_file = open(os.path.join(input_dir, 'genes.txt'), 'r')
    gene_reader = GeneReader(gene_file, len(dna), log_file)
    gene_file.close()
    tu_file = open(os.path.join(input_dir, 'TUs.txt'), 'r')
    TU_reader = TUReader(tu_file, dna)
    tu_file.close()
    # associate genes to TUs and store TUs
    TU_reader.associate_genes(gene_reader.genes, log_file)
    # read production and degradation rates for paulsson simulations
    # create a set of fake TUs where a TU exactly encompasses a gene.
    rates, initial_values = read_rates()
    known_bsus = [r[0] for r in rates]
    known_genes = [g for g in gene_reader.genes if g.bsu in known_bsus]
    gene_TUs = adjust_TUs_to_genes(known_genes, dna)
    # close log file
    log_file.close()
    return TU_reader.TUs, gene_TUs, rates, initial_values


def read_rates():
    """Read rna and protein production and degradation rates."""
    input_dir = 'data'
    with open(os.path.join(input_dir, 'parametre_simulation.csv')) as f:
        # skip header
        next(f)
        rates = []
        initial_values = {}
        for line in f:
            fields = line.rstrip().split(';')
            bsu = fields[0]
            initial_values[bsu] = (int(float(fields[2])),
                                   int(float(fields[3])))
            rates.append(Rates(bsu, *fields[4:]))
    return rates, initial_values


def export_simulation_data(data, parameters, target_dir):
    """Write simulation files."""
    # get parameters
    params = simulation_parameters(data, parameters)

    # write files
    with open(os.path.join(target_dir, 'input/replication.in'), 'w') as f:
        if params.replication.export:
            replication = ReplicationExport(params.replication)
            f.write(replication.input())
            f.write(replication.initiation())
            f.write(replication.elongation())
    transcription = TranscriptionExport(params.transcription)
    with open(os.path.join(target_dir, 'input/rnas.in'), 'w') as f:
        f.write(transcription.transcription_units(
            params.TUs, params.transcription.promoters,
            ))
        f.write(transcription.degradation(
            params.TUs, params.transcription.deg_rates
            ))
        f.write(transcription.initial_values(
            params.TUs, params.transcription.initial_values
        ))
    with open(os.path.join(target_dir, 'input/transcription.in'), 'w') as f:
        f.write(transcription.input())
        f.write(transcription.pre_initiation())
        f.write(transcription.initiation())
        f.write(transcription.elongation())
        f.write(transcription.termination())
        f.write(transcription.aggregated_elongation(params.aggregated_TUs))
    translation = TranslationExport(params.translation)
    with open(os.path.join(target_dir, 'input/proteins.in'), 'w') as f:
        f.write(translation.proteins(params.TUs, params.translation.rbs))
        f.write(translation.degradation(params.TUs))
        f.write(translation.initial_values(
            params.TUs, params.translation.initial_values
        ))
    with open(os.path.join(target_dir, 'input/translation.in'), 'w') as f:
        f.write(translation.input())
        f.write(translation.tRNA_activation())
        f.write(translation.pre_initiation())
        f.write(translation.initiation())
        f.write(translation.elongation())
        f.write(translation.termination())
        f.write(translation.aggregated_elongation(params.aggregated_TUs))
    with open(os.path.join(target_dir, 'input/metabolites.in'), 'w') as f:
        metabolism = MetabolismExport(params.metabolism)
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
                    '', [gene.name])
        new_TU.associate_genes([gene])
        TUs.append(new_TU)
    return TUs


def simulation_parameters(data, parameters):
    """Return parameters corresponding to data set selected."""
    data_set, process_aggregation, metab_aggregation = parameters
    real_TUs, gene_TUs, rates, initial_values = data

    # select data set
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

    # select process aggregation level
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

    # select TU/protein parameters
    is_paulsson = (data_set.upper() == 'PAULSSON')
    TUs = gene_TUs if is_paulsson else real_TUs
    aggregated_TUs = select_aggregated_TUs(TUs, process_aggregation)
    if is_paulsson:
        transcription.promoters = {r.bsu + '_rna': (r.promoter, 0)
                                   for r in rates}
        transcription.deg_rates = {r.bsu + '_rna': r.rna_deg for r in rates}
        transcription.initial_values = {k + '_rna': v[0]
                                        for k, v in initial_values.items()}
        translation.rbs = {r.bsu: (r.rbs, 0) for r in rates}
        translation.degradation = float(rates[0].prot_deg)
        translation.initial_values = {k: v[1]
                                      for k, v in initial_values.items()}

    return SimulationParameters(TUs, aggregated_TUs, replication,
                                transcription, translation, metabolism)


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
