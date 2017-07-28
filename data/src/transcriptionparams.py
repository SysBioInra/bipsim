"""Transcription parameters."""

from __future__ import absolute_import, division, print_function
import math


def test_parameters():
    """Return transcription parameters for test case."""
    return TranscriptionParams()


def AN_parameters():
    """Return transcription parameters for performance simulations."""
    params = TranscriptionParams()
    params.sigma_factors = ['A']
    params.absent_sigma_factors = ['B', 'D', 'EF', 'GF', 'H',
                                   'I', 'K', 'L', 'WXY', '-']
    # initial values
    # rates
    params.k_on = 1
    params.k_off = 0.1
    params.loading = 5e-3
    params.mg_recruitment = 500
    params.translocation = 50
    params.release = 100
    params.aggregation_rate = 50
    params.degradation = -1
    return params


def paulsson_parameters():
    """Return transcription parameters for Paulsson simulations."""
    params = AN_parameters()
    # half life of 2.6min = 156s
    params.degradation = math.log(2) / 156
    return params


class TranscriptionParams(object):
    """Transcription parameters."""

    def __init__(self):
        """Build default object."""
        self.aggreegate_slow_reactions = False
        self.cut_slow_reactions = False
        self.sigma_factors = ['A', 'B']
        self.absent_sigma_factors = ['D', 'EF', 'GF', 'H', 'I',
                                     'K', 'L', 'WXY', '-']
        self.aggregated_TUs = []
        # initial values
        self.RNAP = 1000
        self.mg = 1000
        self.nuta = 100
        self.sigma = 1000
        # rates
        self.k_on = 10
        self.k_off = 1
        self.loading = 1
        self.mg_recruitment = 1
        self.translocation = 6
        self.release = 1
        self.aggregation_rate = 5
        self.degradation = 9e-6
