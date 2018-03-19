"""Handle output molecules."""

from __future__ import absolute_import, print_function, division

import re
from os import path
from collections import Counter


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
        lines = self._file_content()
        with open(self._param_file, 'w') as output_stream:
            for line in lines:
                if not self._is_output_entities_line(line):
                    output_stream.write(line)
                else:
                    output_stream.write('# ' + line)
                    output_stream.write(self._formatted_entities() + '\n')

    def _file_content(self):
        with open(self._param_file) as input_stream:
            return input_stream.readlines()

    def _formatted_entities(self):
        return self.PARAM_TAG + ' ' + ' '.join(self._entities)

    def revert(self):
        lines = self._file_content()
        with open(self._param_file, 'w') as output_stream:
            for line in lines:
                if self._is_output_entities_line(line):
                    pass
                elif self._is_commented_output_entities_line(line):
                    output_stream.write(line[2:])
                else:
                    output_stream.write(line)

    def _is_commented_output_entities_line(self, line):
        return line.startswith('# ' + self.PARAM_TAG)


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
