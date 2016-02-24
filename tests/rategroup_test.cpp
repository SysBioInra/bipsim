/**
 * @file rategroup_test.cpp
 * @brief Unit testing for RateGroup class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE RateGroup
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

// ==================
//  Project Includes
// ==================
//
#include "../src/rategroup.h"
#include "experimentalcumulative.h"

class RGBaseTests
{
public:
  RGBaseTests (void)
    : group_10_20 (10, 20)
    , group_0_1 (0, 1)
    , token_10 (0)
    , tolerance (1e-13)
    , token_0_9 (10)
  {
    for (int i = 0; i < token_0_9.size(); ++i)
      { token_0_9 [i] = new RateToken (i); }
  }

  ~RGBaseTests (void)
  {
    for (int i = 0; i < token_0_9.size(); ++i) { delete token_0_9 [i]; }
  }

  double tolerance;
  RateGroup group_10_20;
  RateGroup group_0_1;
  RateToken token_10;
  std::vector <RateToken*> token_0_9;
};

BOOST_FIXTURE_TEST_SUITE (BaseTests, RGBaseTests)
 
BOOST_AUTO_TEST_CASE (size_insertOneToken_returnsOne)
{
  group_10_20.insert (&token_10, 15);
  BOOST_CHECK_EQUAL (group_10_20.size(), 1);
}

BOOST_AUTO_TEST_CASE (size_insertAndRemoveToken_returnsZero)
{
  group_10_20.insert (&token_10, 15);
  group_10_20.remove (&token_10);
  BOOST_CHECK_EQUAL (group_10_20.size(), 0);
}

BOOST_AUTO_TEST_CASE (total_rate_insertOneToken_returnsTokenRate)
{
  group_10_20.insert (&token_10, 15);
  BOOST_CHECK_CLOSE (group_10_20.total_rate(), 15, tolerance);
}

BOOST_AUTO_TEST_CASE (total_rate_insertAndRemoveToken_returnsZero)
{
  group_10_20.insert (&token_10, 15);
  group_10_20.remove (&token_10);
  BOOST_CHECK_SMALL (group_10_20.total_rate(), tolerance);
}

BOOST_AUTO_TEST_CASE (total_rate_insertRemoveFiveToken_returnsSumOfRates)
{
  for (int i = 0; i < 5; ++i) 
    { group_10_20.insert (token_0_9[i], i+11); }
  BOOST_CHECK_CLOSE (group_10_20.total_rate(), 65, tolerance);
  group_10_20.remove (token_0_9[2]);
  BOOST_CHECK_CLOSE (group_10_20.total_rate(), 52, tolerance);
  group_10_20.insert (&token_10, 18);
  BOOST_CHECK_CLOSE (group_10_20.total_rate(), 70, tolerance);
}

BOOST_AUTO_TEST_CASE (total_rate_insertRemoveSmallRates_returnsSumOfRates)
{
  for (int i = 0; i < 10; ++i) { group_0_1.insert (token_0_9[i], 0.1); }
  group_0_1.remove (token_0_9[0]); 
  group_0_1.insert (token_0_9[0], 1);
  for (int i = 1; i < 10; ++i) { group_0_1.remove (token_0_9[i]); }
  BOOST_CHECK_CLOSE (group_0_1.total_rate(), 1, tolerance);  
  group_0_1.remove (token_0_9[0]); 
  BOOST_CHECK_CLOSE (group_0_1.total_rate(), 0, tolerance);  
}

double cumulative_uniform_0_9 (int k) { return (k+1)/10.0; }

BOOST_AUTO_TEST_CASE (random_index_tenThousandUniformDraws_statisticsAreCorrect)
{
  for (int i = 0; i < 10; ++i) { group_0_1.insert (token_0_9 [i], 1); }
  ExperimentalCumulative <int> ecf;
  for (int i = 0; i < 10000; ++i) 
    { ecf.add_pick (group_0_1.random_index()); }
  BOOST_CHECK_SMALL (distance_to_discrete_cumulative (ecf, 
						      cumulative_uniform_0_9)
		     , 0.01);
}

double cumulative_linear_0_9 (int k) { return k*(k+1)/90.0; }

BOOST_AUTO_TEST_CASE (random_index_tenThousandLinearDraws_statisticsAreCorrect)
{
  for (int i = 1; i < 10; ++i) { group_0_1.insert (token_0_9 [i], i/10.0); }
  ExperimentalCumulative <int> ecf;
  for (int i = 0; i < 10000; ++i) 
    { ecf.add_pick (group_0_1.random_index()); }
  BOOST_CHECK_EQUAL (ecf (0), 0);
  BOOST_CHECK_SMALL (distance_to_discrete_cumulative (ecf, 
						      cumulative_linear_0_9)
		     , 0.01);
} 

BOOST_AUTO_TEST_SUITE_END()
