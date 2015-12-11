/**
 * @file rategroup_test.cpp
 * @brief Unit testing for ProductTable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cerr
#include <cstdlib> // EXIT_SUCCESS EXIT_FAILURE
#include <cmath> // fabs()


// ==================
//  Project Includes
// ==================
//
#include "../src/rategroup.h"
#include "experimentalcumulative.h"

#define FAILURE(msg) {std::cerr << "TEST FAILED: " << msg << std::endl; return EXIT_FAILURE;}

double cumulative_uniform_0_9 (int k) { return (k+1)/10.0; }
double cumulative_linear_0_9 (int k) { return k*(k+1)/90.0; }

int main (int argc, char *argv[])
{ 
  double err_max = 1e-14;
  RateGroup rg (10, 20);
  
  // test inserting token
  RateToken rtk (7); rg.insert (&rtk, 15);
  if (rg.size() != 1) { FAILURE ("Wrong size."); }
  if (fabs (rg.total_rate() - 15) > err_max) { FAILURE ("Wrong total rate."); }
  
  // test removing token
  rg.remove (&rtk);
  if (rg.size() != 0) { FAILURE ("Wrong size."); }
  if (fabs (rg.total_rate() - 0) > err_max) { FAILURE ("Wrong total rate."); }

  // test inserting multiple tokens
  std::vector <RateToken*> tokens;
  for (int i = 0; i < 5; ++i)
    { 
      tokens.push_back (new RateToken (i+1));
      rg.insert (tokens.back(), i+11);
    }
  if (rg.size() != 5) { FAILURE ("Wrong size."); }
  if (fabs (rg.total_rate() - 65) > err_max) { FAILURE ("Wrong total rate."); }

  // test inserting/removing
  rg.remove (tokens[2]);
  if (rg.size() != 4) { FAILURE ("Wrong size."); }
  if (fabs (rg.total_rate() - 52) > err_max) { FAILURE ("Wrong total rate."); }
  RateToken rtk30 (30); rg.insert (&rtk30, 18);
  if (rg.size() != 5) { FAILURE ("Wrong size."); }
  if (fabs (rg.total_rate() - 70) > err_max) { FAILURE ("Wrong total rate."); }
  rg.remove (tokens[0]);
  rg.remove (tokens[3]);
  if (rg.size() != 3) { FAILURE ("Wrong size."); }
  if (fabs (rg.total_rate() - 45) > err_max) { FAILURE ("Wrong total rate."); }
  rg.remove (&rtk30);
  if (rg.size() != 2) { FAILURE ("Wrong size."); }
  if (fabs (rg.total_rate() - 27) > err_max) { FAILURE ("Wrong total rate."); }
  rg.remove (tokens[4]);
  rg.remove (tokens[1]);
  if (rg.size() != 0) { FAILURE ("Wrong size."); }
  if (fabs (rg.total_rate() - 0) > err_max) { FAILURE ("Wrong total rate."); }

  // other test
  RateGroup rg2 (0, 1);
  const int num_el = 20;
  tokens.clear();
  for (int i = 0; i < num_el; ++i) 
    { 
      tokens.push_back (new RateToken (i)); 
      rg2.insert (tokens.back(), 0.1);
    }
  rg2.remove (tokens[0]); rg2.insert (tokens[0], 1);
  for (int i = 1; i < num_el; ++i) { rg2.remove (tokens[i]); }
  if (rg2.size() != 1) { FAILURE ("Wrong size."); }
  if (fabs (rg2.total_rate() - 1) > err_max) { FAILURE ("Wrong total rate."); }
  rg2.remove (tokens[0]);
  if (rg2.size() != 0) { FAILURE ("Wrong size."); }
  if (fabs (rg2.total_rate() - 0) > err_max) { FAILURE ("Wrong total rate."); }
  
  // test if drawing statistics are correct
  tokens.clear();
  for (int i = 0; i < 10; ++i)
    { 
      tokens.push_back (new RateToken (i));
      rg2.insert (tokens.back(), 1);
    }  
  ExperimentalCumulative <int> ecf;
  for (int i = 0; i < 10000; ++i) { ecf.add_pick (rg2.random_index()); }
  if (distance_to_discrete_cumulative (ecf, cumulative_uniform_0_9) > 0.01)
    { FAILURE ("Incorrect drawing statistics."); }

  // test if drawing statistics are correct
  rg2.remove (tokens[0]);
  for (int i = 1; i < 10; ++i) 
    { 
      rg2.remove (tokens[i]);
      rg2.insert (tokens[i], i/10.0);
    }  
  ExperimentalCumulative <int> ecf2;
  for (int i = 0; i < 10000; ++i) { ecf2.add_pick (rg2.random_index()); }
  if (ecf2 (0) != 0) { FAILURE ("Incorrect drawing statistics."); }
  if (distance_to_discrete_cumulative (ecf2, cumulative_linear_0_9) > 0.01)
    { FAILURE ("Incorrect drawing statistics."); }

  return EXIT_SUCCESS;
}
