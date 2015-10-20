/**
 * @file compositiontable_test.cpp
 * @brief Unit testing for CompositionTable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cerr
#include <cstdlib> // EXIT_SUCCESS EXIT_FAILURE

#include "../src/compositiontable.h"
#include "../src/chemical.h"

// ==================
//  Project Includes
// ==================
//
#define FAILURE(msg) {std::cerr << "TEST FAILED: " << msg << std::endl; return EXIT_FAILURE;}

template <typename Map>
bool map_compare (Map const &lhs, Map const &rhs) {
    // No predicate needed because there is operator== for pairs already.
    return lhs.size() == rhs.size()
        && std::equal(lhs.begin(), lhs.end(),
                      rhs.begin());
}

int main (int argc, char *argv[])
{ 
  // test with random composition
  Chemical A1, A2, B1, B2, C1, C2, D1;
  CompositionTable test_table;
  std::list <Chemical*> A_comp; A_comp.push_back(&A1); A_comp.push_back(&A2);
  test_table.add_rule ('A', A_comp);
  std::list <Chemical*> B_comp; B_comp.push_back(&B1);
  B_comp.push_back(&B2); B_comp.push_back(&B1);
  test_table.add_rule ('B', B_comp);
  std::list <Chemical*> C_comp; C_comp.push_back(&C1); C_comp.push_back(&C2);
  test_table.add_rule ('C', C_comp);
  std::list <Chemical*> D_comp; D_comp.push_back(&D1);
  test_table.add_rule ('D', D_comp);
 
  // simple test
  {
    std::string sequence = "A";
    std::map <Chemical*, int> expected_result;
    expected_result [&A1] = expected_result [&A2] = 1;
    if (!map_compare (test_table.composition (sequence), expected_result))
      { FAILURE ("could not compute composition of simple sequence successfully..."); }
  }
  
  {
    std::string sequence = "B";
    std::map <Chemical*, int> expected_result;
    expected_result [&B2] = 1; expected_result [&B1] = 2; 
    if (!map_compare (test_table.composition (sequence), expected_result))
      { FAILURE ("could not compute composition of simple sequence successfully..."); }
  }

  {
    std::string sequence = "C";
    std::map <Chemical*, int> expected_result;
     expected_result [&C1] = expected_result [&C2] = 1;
   if (!map_compare (test_table.composition (sequence), expected_result))
      { FAILURE ("could not compute composition of simple sequence successfully..."); }
  }

  {
    std::string sequence = "D";
    std::map <Chemical*, int> expected_result;
    expected_result [&D1] = 1;
    if (!map_compare (test_table.composition (sequence), expected_result))
      { FAILURE ("could not compute composition of simple sequence successfully..."); }
  }

  {
    std::string sequence = "DCBA";
    std::map <Chemical*, int> expected_result;
    expected_result [&A1] = expected_result [&A2] = 1;
    expected_result [&B2] = 1; expected_result [&B1] = 2; 
    expected_result [&C1] = expected_result [&C2] = 1;
    expected_result [&D1] = 1;
    if (!map_compare (test_table.composition (sequence), expected_result))
      { FAILURE ("could not compute composition of simple sequence successfully..."); }
  }

  {
    std::string sequence1 = "AAAABBBCCDCCBBBAAAA";
    std::string sequence2 = "DCCCCBBBBBBAAAAAAAA";
    if (!map_compare (test_table.composition (sequence1),
		      test_table.composition (sequence1)))
      { FAILURE ("could not compute composition of simple sequence successfully..."); }
  }

  return EXIT_SUCCESS;
}
