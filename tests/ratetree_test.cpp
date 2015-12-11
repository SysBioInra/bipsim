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
#include <iostream> // std::cerr
#include <cstdlib> // EXIT_SUCCESS EXIT_FAILURE
#include <vector> // std::vector
#include <cmath> // fabs

// ==================
//  Project Includes
// ==================
//
#include "../src/ratetree.h"
#include "experimentalcumulative.h"

#define FAILURE(msg) {std::cerr << "TEST FAILED: " << msg << std::endl; return EXIT_FAILURE;}

bool test (RateTree&, double expected);

double cumulative (int index);

int main (int argc, char *argv[])
{
  RateTree tree (10);

  if (!test (tree, 0)) { FAILURE ("wrong initialization"); }
  
  tree.set_rate (0, 0); tree.set_rate (1, 0); tree.set_rate (2, 0);
  tree.set_rate (3, 0);
  tree.update_cumulates();
  if (!test (tree, 0)) { FAILURE ("wrong update."); }

  tree.set_rate (0,2);
  tree.update_cumulates();
  if (!test (tree, 2)) { FAILURE ("wrong update."); }
  if (tree.find(1) != 0) { FAILURE ("wrong lookup."); }

  tree.set_rate (1,3);
  tree.set_rate (2,3);
  tree.set_rate (4,6);
  tree.update_cumulates();
  // r0 : 2, r1 : 3, r2 : 3, r4 : 6
  // cum: r0 : 2, r1 : 5, r2 : 8, r4 : 14
  if (!test (tree, 14)) { FAILURE ("wrong update."); }
  if (tree.find(0) != 0) { FAILURE ("wrong lookup."); }
  if (tree.find(1) != 0) { FAILURE ("wrong lookup."); }
  if (tree.find(2) != 0) { FAILURE ("wrong lookup."); }
  if (tree.find(3) != 1) { FAILURE ("wrong lookup."); }
  if (tree.find(6) != 2) { FAILURE ("wrong lookup."); }
  if (tree.find(9) != 4) { FAILURE ("wrong lookup."); }
  if (tree.find(14) != 4) { FAILURE ("wrong lookup."); } 

  // test if drawing statistics are correct
  ExperimentalCumulative <int> ecf;
  for (int i = 0; i < 10000; ++i)
    ecf.add_pick (tree.random_index());
  if (ecf(3) != ecf(2)) { FAILURE ("Incorrect drawing statistics."); }
  if (distance_to_discrete_cumulative (ecf, cumulative) > 0.01)
    { FAILURE ("Incorrect drawing statistics."); }

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

bool test (RateTree& tree, double expected)
{
  double tolerance = 1e-15;
  if (fabs(tree.total_rate()-expected) < tolerance) return true;
  std::cerr << "Wrong total rate: expected " << expected
	    << ", got " << tree.total_rate() << ".\n";
  return false;
}
