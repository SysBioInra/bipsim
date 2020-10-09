"""Generate n cascades."""

from __future__ import absolute_import, division, print_function
import sys
import os
import errno

from src import test_case


def main():
    for case in test_case.read_file(sys.argv[1]):
        n_cascade = case.generate_n_cascade()
        for method in ["vector", "tree", "hybrid"]:
            create_dir(case.bipsim_directory(method))
            n_cascade.to_bipsim(case.bipsim_directory(method), method)
        #create_dir(case.copasi_directory())
        #n_cascade.to_copasi(case.copasi_directory())


def create_dir(dirname):
    try:
        os.makedirs(dirname, exist_ok=True)
    except TypeError:
        create_dir_python2(dirname)


def create_dir_python2(dirname):
    if not os.path.exists(dirname):
        try:
            os.makedirs(dirname)
        except OSError as exc:  # Guard against race condition
            if exc.errno != errno.EEXIST:
                raise


if __name__ == '__main__':
    main()
