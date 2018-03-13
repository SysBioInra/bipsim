"""Metabolism parameters."""

from __future__ import absolute_import, division, print_function


def test_parameters():
    """Return metabolism parameters for test case."""
    return MetabolismParams()


def AN_parameters():
    """Return metabolism parameters for performance simulations."""
    params = MetabolismParams()
    # initial values
    params.dNTPs = [1e8]*4
    params.NTPs = [4.2e6, 0.12e6, 3e6, 0.3e6]
    return params


def paulsson_parameters():
    """Return metabolism parameters for Paulsson simulations."""
    params = AN_parameters()
    params.dNTPs = [-1]*4
    return params


class MetabolismParams(object):
    """Metabolism parameters."""

    def __init__(self):
        """Build default object."""
        self.aggregate_slow_reactions = False
        self.cut_slow_reactions = False
        # initial values
        self.dNTPs = [1e7]*4
        self.NTPs = [1.2e6, 1e6, 1e6, 1e6]
        # rates
        self.constant = False
