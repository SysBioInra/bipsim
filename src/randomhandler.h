

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
 * a random number engine from boost.
 */
class RandomHandler
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  RandomHandler (void);

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
   * @brief Draw several indices from a weight vector of doubles.
   * @return Vector of indices within the array drawn according to weights within the array.
   * @param weights Array containing weights.
   * @param number_indices Number of indices wanted.
   */ 
  std::vector<int> draw_multiple_indices (const std::vector<double>& weights, int number_indices);

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
   * @brief Check class invariant.
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

  template<typename T> std::vector<int> find_multiple_indices ( const std::vector<T>& drawn_weights, const std::vector<T>& cumulated_weights );

  template<typename T> int cumulate_vector_and_strip ( std::vector<T>& vector_to_cumulate, std::vector<int>& original_indices );
  
  // class for comparing values of a vector if only given indices. Needed by sorted_indices.
  template<typename T> class CompareValues
    {
    public:
      CompareValues (const std::vector<T>& vector): v (vector) {}
      bool operator() (int i, int j) { return v [i] < v [j]; }
    private:
      const std::vector<T>& v;
    };

  template<typename T> std::vector<int> sorted_indices (const std::vector<T>& vector_to_sort);
};

// ======================
//  Inline declarations
// ======================
//
inline void RandomHandler::set_seed ( int seed )
{
  _generator.seed( seed );
}

#endif // RANDOMHANDLER_H
