
/**
 * @file randomhandler_test.cpp
 * @brief Unit testing for RandomHandler class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE RandomHandler
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <vector> // std::vector
#include <list> // std::list
#include <iostream> // std::cerr
#include <cmath> // fabs exp log
#include <numeric> // std::partial_sum


// ==================
//  Project Includes
// ==================
//
#include "experimentalcumulative.h"
#include "randomhandler.h"

class VectorsSize10
{
public:
  VectorsSize10 (void)
    : int_zeros (10, 0)
    , double_zeros (10, 0)
  {}

  std::vector<int> int_zeros;
  std::vector<double> double_zeros;
};

BOOST_FIXTURE_TEST_SUITE (BaseTestsDrawIndex, VectorsSize10)
 
BOOST_AUTO_TEST_CASE (draw_index_oneNoneZeroIntegerValue_returnsIndexNonZeroValue)
{
  int_zeros [0] = 1;
  BOOST_CHECK_EQUAL (RandomHandler::instance().draw_index (int_zeros), 0);
}

BOOST_AUTO_TEST_CASE (draw_index_oneNoneZeroIntegerValue_returnsIndexNonZeroValue2)
{
  int_zeros [5] = 5;
  BOOST_CHECK_EQUAL (RandomHandler::instance().draw_index (int_zeros), 5);
}

BOOST_AUTO_TEST_CASE (draw_index_oneNoneZeroIntegerValue_returnsIndexNonZeroValue3)
{
  int_zeros [9] = 9;
  BOOST_CHECK_EQUAL (RandomHandler::instance().draw_index (int_zeros), 9);
}

BOOST_AUTO_TEST_CASE (draw_index_oneNoneZeroDoubleValue_returnsIndexNonZeroValue)
{
  double_zeros [0] = 1.1;
  BOOST_CHECK_EQUAL (RandomHandler::instance().draw_index (double_zeros), 0);
}

BOOST_AUTO_TEST_CASE (draw_index_oneNoneZeroDoubleValue_returnsIndexNonZeroValue2)
{
  double_zeros [9] = 9.9;
  BOOST_CHECK_EQUAL (RandomHandler::instance().draw_index (double_zeros), 9);
}
 
BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE (BaseTestsDistributions)

BOOST_AUTO_TEST_CASE (draw_uniform_singleValue_returnsSingleValue)
{
  BOOST_CHECK_EQUAL (RandomHandler::instance().draw_uniform (10, 10), 10);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE (AdvancedTestsDistributions)

double cumulative_uniform_1_10 (int k) { return k*0.1; }

BOOST_AUTO_TEST_CASE (draw_uniform_tenThousandDraws_statisticsCheckOut)
{
  ExperimentalCumulative <int> uniform;
  for (int i = 0; i < 10000; ++i)
    { uniform.add_pick (RandomHandler::instance().draw_uniform (1,10)); }
  BOOST_CHECK_SMALL (distance_to_discrete_cumulative (uniform, 
						      cumulative_uniform_1_10),
		     0.01);
}

double cumulative_exp (double x, double lambda) { return 1 - exp (-lambda*x);}
double cumulative_exp_1 (double x) { return cumulative_exp (x, 1);}

BOOST_AUTO_TEST_CASE (draw_exponential_tenThousandDrawsLambdaOne_statisticsCheckOut)
{
  ExperimentalCumulative<double> exponential;
  for (int i = 0; i < 10000; ++i)
    { exponential.add_pick (RandomHandler::instance().draw_exponential (1)); }
  BOOST_CHECK_SMALL (distance_to_continuous_cumulative (exponential, 
							cumulative_exp_1),
		     0.01);
}

double cumulative_exp_01 (double x) { return cumulative_exp (x, 0.1);}

BOOST_AUTO_TEST_CASE (draw_exponential_tenThousandDrawsLambdaZeroOne_statisticsCheckOut)
{
  ExperimentalCumulative<double> exponential;
  for (int i = 0; i < 10000; ++i)
    { exponential.add_pick (RandomHandler::instance().draw_exponential (0.1)); }
  BOOST_CHECK_SMALL (distance_to_continuous_cumulative (exponential, 
							cumulative_exp_01),
		     0.01);
}


BOOST_AUTO_TEST_SUITE_END()
