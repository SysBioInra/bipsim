

/**
 * @file randomhandler.cpp
 * @brief Implementation of the RandomHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout
#include <boost/random/uniform_int.hpp> // boost::uniform_int

// ==================
//  Project Includes
// ==================
//
#include "randomhandler.h"
#include "macros.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
boost::mt19937 RandomHandler::_generator;


// Not needed for this class (use of default copy constructor) !
// RandomHandler::RandomHandler (RandomHandler& other_random_handler);

RandomHandler::~RandomHandler (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
int RandomHandler::draw_index ( const std::vector<int>& weights )
{
  // we compute the total weight
  int number_items = weights.size();
  int total_weight = 0;
  for ( int i = 0; i < number_items; i++ ) { total_weight += weights[ i ]; }
  REQUIRE( total_weight > 0 ); /** @pre The sum of weights must be positive. */
  
  // we draw a number in the weight distribution
  boost::uniform_int<int> distribution ( 1, total_weight );
  int drawn_weight = distribution( RandomHandler::_generator );

  // we retrieve the corresponding index
  int drawn_index = 0;
  int current_weight = weights[ 0 ];
  while ( current_weight < drawn_weight )
    {
      drawn_index++;
      current_weight += weights[ drawn_index ];
    }

  return drawn_index;
}

int RandomHandler::draw_uniform ( int a, int b )
{
  REQUIRE( a <= b ); /** @pre a must be smaller or equal to b. */

  // we create the distribution and draw a number
  boost::uniform_int<int> distribution ( a, b );
  return distribution( RandomHandler::_generator );
}


// ============================
//  Public Methods - Accessors
// ============================
//


// ==========================
//  Public Methods - Setters
// ==========================
//


// =======================================
//  Public Methods - Operator overloading
// =======================================
//
// Not needed for this class (use of default overloading) !
// RandomHandler& RandomHandler::operator= (RandomHandler& other_random_handler);

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool RandomHandler::check_invariant ( void ) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
