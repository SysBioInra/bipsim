/**
 * @file randomhandler_test.cpp
 * @brief Unit testing for RandomHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector
#include <list> // std::list
#include <set> // std::set
#include <iostream> // std::cerr
#include <cstdlib> // EXIT_SUCCESS EXIT_FAILURE
#include <cmath> // fabs exp log
#include <numeric> // std::partial_sum


// ==================
//  Project Includes
// ==================
//
#include "experimentalcumulative.h"
#include "../src/randomhandler.h"


#define FAILURE(msg) {std::cerr << "TEST FAILED: " << msg << std::endl; return EXIT_FAILURE;}
#define WARNING(msg) {std::cerr << "TEST WARNING: " << msg << std::endl;}

// Cumulative distribution of U(1,10)
double cumulative_uniform_1_10 (int k);

// Cumulative distribution of Poisson(1)
double cumulative_poisson_1 (int k);

// Cumulative distribution of Poisson(13.5)
double cumulative_poisson_13_5 (int k);

// Cumulative distribution of Poisson(50000)
double cumulative_poisson_2000 (int k);

// Compute supremum distance from experimental cumulative to theoretical
// cumulative distribution for discrete distributions.
double distance_to_discrete_cumulative (ExperimentalCumulative<int>& exp_cum, double (*theor_cum)(int));

// Cumulative distribution of Exp(1)
double cumulative_exp_1 (double x);

// Compute supremum distance from experimental cumulative to theoretical
// cumulative distribution for continuous distributions.
double distance_to_continuous_cumulative (ExperimentalCumulative<double>& exp_cum, double (*theor_cum)(double));

int main (int argc, char *argv[])
{ 
  int vector_size = 10;
  std::vector<int> int_zeros (vector_size, 0);
  std::vector<int> int_test;
  std::vector<double> double_zeros (vector_size, 0);
  std::vector<double> double_test;

  // draw_index should abort when total weight <= 0
  // RandomHandler::instance().draw_index (std::vector<int>(3,0));

  // draw_index should return the only non-zero weighted index
  for (int i = 0; i < vector_size; ++i)
    {
      int_test = int_zeros; int_test[i] = i + 1;
      if (RandomHandler::instance().draw_index (int_test) != i)
	FAILURE ("draw_index did not return only non-zero weighted index");
    }

  // draw_index should return the only non-zero weighted index
  for (int i = 0; i < vector_size; ++i)
    {
      double_test = double_zeros; double_test[i] = i + 1.1;
      if (RandomHandler::instance().draw_index (double_test) != i)
	FAILURE ("draw_index did not return only non-zero weighted index");
    }

  // draw_multiple_indices should return n times the only non-zero weighted index
  int n = 10;
  for (int i = 0; i < vector_size; ++i)
    {
      double_test = double_zeros; double_test[i] = i+1.1;
      std::vector <double> cumulated (double_test.size());
      std::partial_sum (double_test.begin(), double_test.end(), cumulated.begin());
      if (RandomHandler::instance().draw_multiple_indices_cumulated (cumulated,n) != std::vector<int> (n,i))
	{
	  FAILURE ("draw_multiple_indices did not return only non-zero weighted index");
	}
    }
  
  // draw_uniform with min = max = a should return a
  int a = 7;
  if (RandomHandler::instance().draw_uniform (a,a) != a)
    FAILURE ("draw_uniform (a,a) did not return a");

  // draw_uniform should abort if a 
  // RandomHandler::instance().draw_uniform (3,1);

  // should abort
  // RandomHandler::draw_exponential (0);

  // check that draw_uniform does approximately do its job
  ExperimentalCumulative<int> uniform;
  for (int i = 0; i < 10000; ++i)
      uniform.add_pick (RandomHandler::instance().draw_uniform (1,10));
  if (distance_to_discrete_cumulative (uniform, cumulative_uniform_1_10) > 0.01)
    FAILURE ("Uniform distribution really uniform?");

  // check that draw_exponential does approximately do its job
  ExperimentalCumulative<double> exponential;
  for (int i = 0; i < 10000; ++i)
    exponential.add_pick (RandomHandler::instance().draw_exponential (1));
  if (distance_to_continuous_cumulative (exponential, cumulative_exp_1) > 0.01)
    FAILURE ("Exponential distribution really exponential?");

  // check that draw_poisson does approximately do its job (parameter 1)
  ExperimentalCumulative<int> poisson;
  for (int i = 0; i < 10000; ++i)
    poisson.add_pick (RandomHandler::instance().draw_poisson (1));
  if (distance_to_discrete_cumulative (poisson, cumulative_poisson_1) > 0.01)
    FAILURE ("Poisson distribution really poisson (lambda = 1)?");

  // check that draw_poisson does approximately do its job (parameter 13.5)
  ExperimentalCumulative<int> poisson135;
  for (int i = 0; i < 10000; ++i)
    poisson135.add_pick (RandomHandler::instance().draw_poisson (13.5));
  if (distance_to_discrete_cumulative (poisson135, cumulative_poisson_13_5) > 0.01)
    FAILURE ("Poisson distribution really poisson (lambda = 13.5)?");
  
  // check that draw_poisson does approximately do its job (parameter 2000)
  ExperimentalCumulative<int> poisson2000;
  for (int i = 0; i < 10000; ++i)
    {
      int pick = RandomHandler::instance().draw_poisson (2000);
      poisson2000.add_pick (pick);
      // std::cout << pick << std::endl;
    }
  double distance = distance_to_discrete_cumulative (poisson2000, cumulative_poisson_2000);
  if (distance > 0.01)
    FAILURE ("Poisson distribution really poisson (lambda = 2000, max_distance = " << distance << ")?");

  return EXIT_SUCCESS;
}


double cumulative_poisson (int k, double lambda)
{
  // we compute the cumulative in
  double val = exp (-lambda); // starting value
  double log_fact_i = 0;
  double log_lambda = log (lambda);
  for (int i = 1; i <= k; ++i)
    {
      log_fact_i += log(i);
      val += exp (i*log_lambda - log_fact_i - lambda);
    }
  return val;
}

double cumulative_poisson_1 (int k)
{
  return cumulative_poisson (k, 1);
}

double cumulative_poisson_13_5 (int k)
{
  return cumulative_poisson (k, 13.5);
}

double cumulative_poisson_2000 (int k)
{
  return cumulative_poisson (k, 2000);
}

double cumulative_uniform_1_10 (int k)
{
  return k*0.1;
}

double distance_to_discrete_cumulative (ExperimentalCumulative<int>& exp_cum, double (*theor_cum)(int))
{
  // it suffices to compute the distance at each jump
  double max_distance = 0;
  double d = 0;

  std::set<int> jump_values = exp_cum.jump_values();
  for (std::set<int>::iterator k = jump_values.begin(); k != jump_values.end(); ++k)
    {
      // distance at jump
      d = fabs (exp_cum (*k) - (*theor_cum)(*k));
      if ( d > max_distance ) max_distance = d;
      // std::cout << exp_cum(*k) << " " << (*theor_cum)(*k) << " " << d << std::endl;
    }

  return max_distance;
}

double cumulative_exp_1 (double x)
{
  return 1 - exp (-x);
}

double distance_to_continuous_cumulative (ExperimentalCumulative<double>& exp_cum, double (*theor_cum)(double))
{
  double max_distance = 0;
  double d = 0;

  // it suffices to compute the distance before and after each jump
  double prev = 0;  // the value of the experimental distribution before the first jump is 0
  std::set<double> jump_values = exp_cum.jump_values();
  for (std::set<double>::iterator x = jump_values.begin(); x != jump_values.end(); ++x)
    {
      double theoretical = (*theor_cum) (*x);

      // distance before jump
      d = fabs (prev - theoretical);
      if ( d > max_distance ) max_distance = d;
      // std::cout << prev << " " << theoretical << " " << d << " ";

      // distance after jump
      double after = exp_cum (*x);
      d = fabs (after - theoretical);
      if ( d > max_distance ) max_distance = d;
      // std::cout << after << " " << theoretical << " " << d << std::endl;

      // prepare value before jump for next iteration
      prev = after;
    }

  return max_distance;
}
