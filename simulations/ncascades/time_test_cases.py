
from __future__ import absolute_import, division, print_function
import sys
import os
import subprocess

from src import test_case


def main():
    print('N I bipsim Copasi')
    for case in test_case.read_file(sys.argv[1]):
        bipsim_time = time_case(case.bipsim_directory())
        copasi_time = time_case(case.copasi_directory())
        print('{} {} {} {}'.format(case._length, case._initial_value,
                                   bipsim_time, copasi_time))


def time_case(directory):
    current_dir = os.getcwd()
    os.chdir(directory)
    process = subprocess.Popen(['time', 'sh', 'run.sh'],
                               stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    time_output = process.communicate()[1]
    os.chdir(current_dir)
    return float(time_output.strip().split()[2])


if __name__ == '__main__':
    main()
