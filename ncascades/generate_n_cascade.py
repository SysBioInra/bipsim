"""Generate n cascades."""

from __future__ import absolute_import, division, print_function
import sys
import os
import errno

from ncascade.ncascade import NCascade


def main():
    script_name, target_dir, n_chemicals, initial_value = sys.argv
    n_cascade = NCascade(int(n_chemicals), int(initial_value))
    copasi_dir = os.path.join(
        target_dir, 'copasi_{}_{}'.format(n_chemicals, initial_value)
    )
    create_dirs(copasi_dir)
    n_cascade.to_copasi(copasi_dir)
    bipsim_dir = os.path.join(
        target_dir, 'bipsim_{}_{}'.format(n_chemicals, initial_value)
    )
    create_dirs(bipsim_dir)
    n_cascade.to_bipsim(bipsim_dir)


def create_dirs(filename):
    os.makedirs(os.path.dirname(filename), exist_ok=True)


def create_dirs_python2(filename):
    if not os.path.exists(os.path.dirname(filename)):
        try:
            os.makedirs(os.path.dirname(filename))
        except OSError as exc:  # Guard against race condition
            if exc.errno != errno.EEXIST:
                raise

if __name__ == '__main__':
    main()
