

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
 * @brief Class handling Random Number Generation.
 *
 * ReactionHandler initiates a random number engine and provides various services
 * such as drawing of classical distributions and biased-wheel drawing. It uses
 * a random number engine from boost. It also uses the singleton pattern: only 
 * one copy of the handler exists at all times and must be accessed through
 * the instance() member.
 */
class RandomHandler
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //

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
  int draw_index (const std::vector<int>& weights);

  /**
   * @brief Draw an index from a weight vector of doubles.
   * @return Index within the array drawn according to weights within the array.
   * @param weights Array containing weights.
   */ 
  int draw_index (const std::vector<double>& weights);

  /**
   * @brief Draw an index from a weight vector of integers.
   * @return Index within the array drawn according to weights within the array.
   * @param cumulated_weights Vector containing cumulated weights.
   */ 
  int draw_index_cumulated (const std::vector<int>& cumulated_weights);

  /**
   * @brief Draw an index from a weight vector of doubles.
   * @return Index within the array drawn according to weights within the array.
   * @param cumulated_weights Array containing cumulated_weights.
   */ 
  int draw_index_cumulated (const std::vector<double>& cumulated_weights);

  /**
   * @brief Draw several indices from a weight vector of doubles.
   * @return Vector of indices within the array drawn according to weights within the array.
   * @param cumulated_weights Array containing cumulated_weights.
   * @param number_indices Number of indices wanted.
   */ 
  std::vector<int> draw_multiple_indices_cumulated (const std::vector<double>& cumulated_weights, int number_indices);

  /**
   * @brief Draw an integer uniformly from an interval [a,b].
   * @return Random integer in interval [a,b].
   * @param a First integer of the interval.
   * @param b Last integer of the interval.
   */ 
  int draw_uniform ( int a, int b );

  /**
   * @brief Draw a value uniformly from a real interval [a,b].
   * @return Random value in interval [a,b].
   * @param a First value of the interval.
   * @param b Last value of the interval.
   */ 
  double draw_uniform (double a, double b);

  /**
   * @brief Draw a real value from an exponential distribution with paramter lambda.
   * @return Random non-negative double.
   * @param lambda Parameter of the exponential distribution.
   */ 
  double draw_exponential ( double lambda );

  /**
   * @brief Draw an integer value from a Poisson distribution with parameter lambda.
   * @return Random non-negative integer.
   * @param lambda Parameter of the Poisson distribution.
   */ 
  int draw_poisson (double lambda);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Return the only instance of the generator.
   */  
  static RandomHandler& instance (void);

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //
  /**
   * @brief Change seed of the random generator.
   * @param seed Integer used to seed the generator.
   */
  void set_seed ( int seed );

  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Original pseudorandom number uniform sequence (Mersenne twister). */
  boost::mt19937 _generator;

  /** @brief Original pseudorandom number uniform sequence (Mersenne twister). */
  static RandomHandler _instance;

  // =================
  //  Private Methods
  // =================
  //

  // ======================
  //  Forbidden operations
  // ======================
  //
  /**
   * @brief Default constructor
   */
  RandomHandler (void);

  /**
   * @brief Copy constructor
   */
  RandomHandler (RandomHandler& other_random_handler);
  
  /**
   * @brief Assignment operator
   */
  RandomHandler& operator= (RandomHandler& other_random_handler);
  

};

// ======================
//  Inline declarations
// ======================
//
inline void RandomHandler::set_seed ( int seed )
{
  _generator.seed( seed );
}

inline RandomHandler& RandomHandler::instance (void)
{
  return _instance;
}

#endif // RANDOMHANDLER_H
