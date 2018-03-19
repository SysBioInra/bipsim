"""Update output molecules."""

from __future__ import absolute_import, print_function, division

import sys
import re
from os import path
from collections import Counter


def main():
    input_ = Input(sys.argv[1])
    rnas = Rnas(input_.path('rnas.in'))
    proteins = Proteins(input_.path('proteins.in'))
    test = OutputEntities(input_.path('params.in'))
    test.add(rnas.elements)
    test.add(proteins.unique_elements)
    test.export()


class Input(object):
    def __init__(self, input_dir):
        self._input_dir = input_dir

    def path(self, filename):
        return path.join(self._input_dir, 'input', filename)


class OutputEntities(object):
    PARAM_TAG = 'OUTPUT_ENTITIES'

    def __init__(self, param_file):
        self._param_file = param_file
        self._initialize_entities()

    def _initialize_entities(self):
        with open(self._param_file) as input_stream:
            for line in input_stream:
                if self._is_output_entities_line(line):
                    self._entities = line.split()[1:]

    def _is_output_entities_line(self, line):
        tag = line.split(None, 1)
        return tag and (tag[0] == self.PARAM_TAG)

    def add(self, elements):
        self._entities += elements

    def export(self):
        with open(self._param_file) as input_stream:
            lines = input_stream.readlines()
        with open(self._param_file + '.test', 'w') as output_stream:
            for line in lines:
                if not self._is_output_entities_line(line):
                    output_stream.write(line)
                else:
                    output_stream.write('# ' + line)
                    output_stream.write(self._formatted_entities() + '\n')

    def _formatted_entities(self):
        return self.PARAM_TAG + ' ' + ' '.join(self._entities)


class Rnas(object):
    parser = re.compile(r'ChemicalSequence ([\w]+) '
                        'product_of [\w]+ [0-9]+ [0-9]+ rnas')

    def __init__(self, file):
        with open(file) as input_:
            self.elements = self.parser.findall(input_.read())


class Proteins(object):
    parser = re.compile(r'ChemicalSequence ([\w]+) '
                        'product_of ([\w]+) [0-9]+ [0-9]+ proteins')

    def __init__(self, file):
        with open(file) as input_:
            self.elements = self.parser.findall(input_.read())
        self.count = Counter(p[0] for p in self.elements)
        self.unique_elements = set(p[0] for p in self.elements)


if __name__ == '__main__':
    main()
