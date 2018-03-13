
from __future__ import absolute_import, division, print_function

from src.ncascade import NCascade


def read_file(filename):
    with open(filename) as input_:
        next(input_)  # skip header
        return [TestCase(l) for l in input_]


class TestCase(object):
    def __init__(self, line):
        self._length, self._initial_value = line.strip().split(' ')

    def generate_n_cascade(self):
        return NCascade(int(self._length), int(self._initial_value))

    def copasi_directory(self):
        return 'copasi_{}_{}'.format(self._length, self._initial_value)

    def bipsim_directory(self):
        return 'bipsim_{}_{}'.format(self._length, self._initial_value)
