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
#include <iostream> // std::cerr
#include <cstdlib> // EXIT_SUCCESS EXIT_FAILURE
#include <cmath> // fabs

// ==================
//  Project Includes
// ==================
//
#include "../src/hybridratecontainer.h"
#include "experimentalcumulative.h"


#define FAILURE(msg) {std::cerr << "TEST FAILED: " << msg << std::endl; return EXIT_FAILURE;}

double cumulative_linear_0_99 (int k) { return k*(k+1)/(99.0*100.0); }

int main (int argc, char *argv[])
{ 
  // arguments: number of rates, base rate
  HybridRateContainer hrc (200, 1);

  for (int i = 0; i < 100; ++i) { hrc.set_rate (i, i); }
  hrc.update_cumulates();

  // test if total rate is correct
  if (fabs (hrc.total_rate() - 100*99/2) > 1e-15)
    { FAILURE ("Wrong total rate."); }

  // test if drawing statistics are correct
  ExperimentalCumulative <int> ecf;
  for (int i = 0; i < 10000; ++i)
    ecf.add_pick (hrc.random_index());
  if (ecf (0) != 0) { FAILURE ("Incorrect drawing statistics."); }
  if (distance_to_discrete_cumulative (ecf, cumulative_linear_0_99) > 0.05)
    { FAILURE ("Incorrect drawing statistics."); }

  
  return EXIT_SUCCESS;
}

