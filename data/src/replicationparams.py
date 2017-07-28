"""Replication parameters."""

from __future__ import absolute_import, division, print_function


def test_parameters():
    """Return replication parameters for test case."""
    return ReplicationParams()


def AN_parameters():
    """Return replication parameters for performance simulations."""
    params = ReplicationParams()
    # initial values
    # rates
    params.loading = 1e-3
    params.ligation = 10000
    params.translocation = 750
    params.recruitment = 5
    params.release = 1000
    return params


def paulsson_parameters():
    """Return replication parameters for Paulsson simulations."""
    params = ReplicationParams()
    params.export = False
    return params


class ReplicationParams(object):
    """Replication parameters."""

    def __init__(self):
        """Build default object."""
        self.aggregate_slow_reactions = False
        self.cut_slow_reactions = False
        self.export = True
        # initial values
        self.DNAP = 10
        # rates
        self.loading = 1
        self.ligation = 1
        self.translocation = 10
        self.recruitment = 1000
        self.release = 1
