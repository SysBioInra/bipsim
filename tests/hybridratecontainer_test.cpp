/**
 * @file hybridratecontainer_test.cpp
 * @brief Unit testing for HybridRateContainer class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE HybridRateContainer
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <iostream> // std::cerr

// ==================
//  Project Includes
// ==================
//
#include "../src/hybridratecontainer.h"
#include "experimentalcumulative.h"

class HRCSize100BaseRate1
{
public:
  HRCSize100BaseRate1 (void)
    : zero_container (100, 1)
    , linear_rates_container (100, 1)
  {
    for (int i = 0; i < 100; ++i) { linear_rates_container.set_rate (i, i); }  
    linear_rates_container.update_cumulates();
  }

  HybridRateContainer zero_container;
  HybridRateContainer linear_rates_container;
};

BOOST_FIXTURE_TEST_SUITE (BaseTests, HRCSize100BaseRate1)
 
BOOST_AUTO_TEST_CASE (total_rate_linearlyIncreasingRates_returnsSumOfRates)
{
  BOOST_CHECK_CLOSE (linear_rates_container.total_rate(), 100*99/2, 1e-15);
}

double cumulative_linear_0_99 (int k) { return k*(k+1)/(99.0*100.0); }

BOOST_AUTO_TEST_CASE (random_index_linearlyIncreasingRates_drawingStatisticsAreCorrect)
{
  ExperimentalCumulative <int> ecf;
  for (int i = 0; i < 10000; ++i) 
    { ecf.add_pick (linear_rates_container.random_index()); }
  BOOST_CHECK_EQUAL (ecf (0), 0);
  double d = distance_to_discrete_cumulative (ecf, cumulative_linear_0_99);
  BOOST_CHECK_SMALL (d, 0.05);
}
 
BOOST_AUTO_TEST_SUITE_END()


