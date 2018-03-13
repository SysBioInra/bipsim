
from io import StringIO

from src.GeneReader import GeneReader


class SampleData(object):
    def __init__(self):
        self.data = {}
        self.data['name'] = 'test_name'
        self.data['BSU'] = 'test_bsu'
        self.data['start'] = 0
        self.data['end'] = 8
        self.data['RBSstart'] = 0
        self.data['RBSend'] = 0
        self.data['brin_DNA'] = 1
        self.data['seq'] = 'atgaaatag'
        self.data['aaseq'] = 'MA'
        self.data['gene_category'] = 'CDS'
        self.data['TUs'] = 'test_tus'

    def as_stream(self):
        header = []
        line = []
        for key, value in self.data.items():
            header.append(key)
            line.append(str(value))
        return StringIO('\t'.join(header) + '\n' + '\t'.join(line) + '\n')


def test_creation_one_gene():
    output = StringIO()
    gene_reader = GeneReader(SampleData().as_stream(), 1000, output)
    assert(len(gene_reader.genes) == 1)
    assert(not any((gene_reader._invalid_RBS,
                    gene_reader._long_RBS,
                    gene_reader._invalid_start,
                    gene_reader._invalid_stop,
                    gene_reader._invalid_sequence,
                    gene_reader._invalid_aa_seq)))
