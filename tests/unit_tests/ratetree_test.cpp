/**
 * @file ratetree_test.cpp
 * @brief Unit testing for Ratetree class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE RateTree
#include <boost/test/unit_test.hpp>
// #include <boost/test/floating_point_comparison.hpp>

#include <vector> // std::vector

// ==================
//  Project Includes
// ==================
//
#include "ratetree.h"
#include "experimentalcumulative.h"


class TreeSize10
{
public:
  TreeSize10 (void)
    : zero_tree (10)
  {}

  RateTree zero_tree;
};

BOOST_FIXTURE_TEST_SUITE (BaseTests, TreeSize10)
 
BOOST_AUTO_TEST_CASE (total_rate_zeroTree_returnsZero)
{
  BOOST_CHECK_SMALL (zero_tree.total_rate(), 1e-15);
}

BOOST_AUTO_TEST_CASE (total_rate_oneNonZeroRate_returnsRate)
{
  zero_tree.set_rate (0, 2); zero_tree.update_cumulates();
  BOOST_CHECK_CLOSE (zero_tree.total_rate(), 2, 1e-15);
}

BOOST_AUTO_TEST_CASE (find_oneNonZeroRate_returnsIndexNonZeroRate)
{
  zero_tree.set_rate (5, 2); zero_tree.update_cumulates();
  BOOST_CHECK_EQUAL (zero_tree.find (1), 5);
  BOOST_CHECK_EQUAL (zero_tree.find (2), 5);
}

BOOST_AUTO_TEST_CASE (find_MultipleRates_returnsAppropriateIndex)
{
  zero_tree.set_rate (0, 2); zero_tree.set_rate (1, 3);
  zero_tree.set_rate (2, 3); zero_tree.set_rate (4, 6);
  zero_tree.update_cumulates();
  BOOST_CHECK_EQUAL (zero_tree.find (1), 0);
  BOOST_CHECK_EQUAL (zero_tree.find (2), 0);
  BOOST_CHECK_EQUAL (zero_tree.find (3), 1);
  BOOST_CHECK_EQUAL (zero_tree.find (6), 2);
  BOOST_CHECK_EQUAL (zero_tree.find (9), 4);
  BOOST_CHECK_EQUAL (zero_tree.find (14), 4);
}

double cumulative (int index)
{
  // cum: r0 : 2, r1 : 5, r2 : 8, r4 : 14
  switch (index)
    {
    case 0: return 2.0/14;
    case 1: return 5.0/14;
    case 2: return 8.0/14;
    case 3: return 8.0/14;
    case 4: return 1;
    }
}

BOOST_AUTO_TEST_CASE (random_index_tenThousandDraws_statisticsAreCorrect)
{
  zero_tree.set_rate (0, 2); zero_tree.set_rate (1, 3);
  zero_tree.set_rate (2, 3); zero_tree.set_rate (4, 6);
  zero_tree.update_cumulates();
  ExperimentalCumulative <int> ecf;
  for (int i = 0; i < 10000; ++i) { ecf.add_pick (zero_tree.random_index()); }
  BOOST_CHECK_EQUAL (ecf(2), ecf(3));
  BOOST_CHECK_SMALL (distance_to_discrete_cumulative (ecf, cumulative), 0.01);
}

BOOST_AUTO_TEST_SUITE_END()
