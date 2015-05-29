

/**
 * @file randomhandler.h
 * @brief Header for the RandomHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef RANDOMHANDLER_H
#define RANDOMHANDLER_H

// ==================
//  General Includes
// ==================
//
#include <boost/random/mersenne_twister.hpp> // boost::mt19937
#include <vector> // std::vector

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

/**
 * @brief BRIEF DESCRIPTION OF CLASS
 *
 * DETAILED DESCRIPTION OF CLASS
 */
class RandomHandler
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  // not needed here
  // /**
  //  * @brief Default constructor
  //  */
  // RandomHandler (void);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // RandomHandler (RandomHandler& other_random_handler);

  /**
   * @brief Destructor
   */
  virtual ~RandomHandler (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Draw an index from a weight vector of integers.
   * @return Index within the array drawn according to weights within the array.
   * @param weights Vector containing weights.
   */ 
  int draw_index ( const std::vector<int>& weights );

  /**
   * @brief Draw an index from a weight vector of doubles.
   * @return Index within the array drawn according to weights within the array.
   * @param weights Array containing weights.
   */ 
  int draw_index ( const std::vector<double>& weights );

  /**
   * @brief Draw an integer uniformly from an interval [a,b].
   * @return Random integer in interval [a,b].
   * @param a First integer of the interval.
   * @param b Last integer of the interval.
   */ 
  int draw_uniform ( int a, int b );

  /**
   * @brief Draw a real value from an exponential distribution with paramter lambda.
   * @return Random non-negative double.
   * @param lambda Parameter of the exponential distribution.
   */ 
  double draw_exponential ( double lambda );

  // ============================
  //  Public Methods - Accessors
  // ============================
  //


  // ==========================
  //  Public Methods - Setters
  // ==========================
  //
  void set_seed ( int seed );

  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /*
  //  * @brief Assignment operator
  //  */
  // RandomHandler& operator= (RandomHandler& other_random_handler);

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Original pseudorandom number uniform sequence (Mersenne twister). */
  static boost::mt19937 _generator;

  // =================
  //  Private Methods
  // =================
  //
  template<typename T> int find_index ( T drawn_weight, const std::vector<T>& cumulated_weights );

  template<typename T> int cumulate_vector_and_strip ( std::vector<T>& vector_to_cumulate, std::vector<int>& original_indices );
};

// ======================
//  Inline declarations
// ======================
//
inline void RandomHandler::set_seed ( int seed )
{
  RandomHandler::_generator.seed( seed );
}

#endif // RANDOMHANDLER_H
