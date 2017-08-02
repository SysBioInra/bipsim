"""Translation parameters."""

from __future__ import absolute_import, division, print_function


def test_parameters():
    """Return translation parameters for test case."""
    return TranslationParams()


def AN_parameters():
    """Return translation parameters for performance simulations."""
    params = TranslationParams()
    # initial values
    params.nb_ribosomes = 18000
    params.IF_123 = [5500, 6600, 4400]
    params.EFTu = 118000
    params.EF_G_RF = [15000, 4000]
    params.fMet = 3000
    params.nb_tRNAs = 4000
    # rates
    params.k_on = 1
    params.k_off = 0.1
    params.loading = 2e-3
    params.translocation = 150
    params.transpeptidation = 150
    params.release = 150
    params.aggregation_rate = 20  # 8
    params.degradation = -1
    return params


def paulsson_parameters():
    """Return translation parameters for Paulsson simulations."""
    params = AN_parameters()
    params.constant_initiation = True
    params.rbs_clearance = 100
    # equal to growth rate 40min-1 = 2400s-1
    params.degradation = 1.0/2400
    return params


class TranslationParams(object):
    """Translation parameters."""

    def __init__(self):
        """Build default object."""
        self.aggregate_slow_reactions = False
        self.cut_slow_reactions = False
        # initial values
        self.constant_initiation = False
        self.nb_ribosomes = 1000
        self.IF_123 = [100, 400, 100]
        self.EFTu = 60000
        self.EF_G_RF = [100, 1000]
        self.fMet = 300
        self.nb_tRNAs = 1000
        # rates
        self.k_on = 10
        self.k_off = 1
        self.loading = 1
        self.translocation = 200
        self.transpeptidation = 1
        self.release = 1
        self.aggregation_rate = 1
        self.rbs_clearance = 1
        self.degradation = -1
        # individual rates
        self.rbs = None
        self.initial_values = None
