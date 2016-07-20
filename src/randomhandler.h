
/**
 * @file randomhandler.h
 * @brief Header for the RandomHandler class.
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
#include "config.h"
#ifdef HAVE_BOOST
#include <boost/version.hpp> // BOOST_VERSION
#if BOOST_VERSION >= 104700
#define CURRENT_BOOST_RANDOM
#endif // BOOST_VERSION >= 1047000
#include <boost/random/mersenne_twister.hpp> // mt19937
#endif // HAVE_BOOST

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
 private:
  // Forbidden (singleton pattern)
  /** @brief Default constructor. */
  RandomHandler (void);
  /** @brief Copy constructor. */
  RandomHandler (RandomHandler& other_handler);
  /** @brief Assignment operator. */
  RandomHandler& operator= (RandomHandler& other_handler);

 public:
  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule does not apply)
  // /* @brief Destructor */
  // ~RandomHandler (void);

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
   * @brief Draw an integer uniformly from an interval [a,b].
   * @return Random integer in interval [a,b].
   * @param a First integer of the interval.
   * @param b Last integer of the interval.
   */ 
  int draw_uniform (int a, int b);

  /**
   * @brief Draw a value uniformly from a real interval [a,b).
   * @return Random value in interval [a,b).
   * @param a First value of the interval.
   * @param b Last value of the interval.
   */ 
  double draw_uniform (double a, double b);

  /**
   * @brief Draw a real value from an exponential distribution.
   * @return Random non-negative double.
   * @param lambda Parameter of the exponential distribution.
   */ 
  double draw_exponential (double lambda);

  /**
   * @brief Change seed of the random generator.
   * @param seed Integer used to seed the generator.
   */
  void set_seed (int seed);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Return the only instance of the generator.
   */  
  static RandomHandler& instance (void);

private:

  // ============
  //  Attributes
  // ============
  //
#ifdef HAVE_BOOST
  /** @brief Pseudorandom number uniform sequence (Mersenne twister). */
#ifdef CURRENT_BOOST_RANDOM
  boost::random::mt19937 _generator;
#else // OBSOLETE BOOST
  boost::mt19937 _generator;
#endif // CURRENT_BOOST_RANDOM
#endif // HAVE_BOOST 

  /** @brief Only instance of the RandomHandler (singleton pattern). */
  static RandomHandler _instance;

  // =================
  //  Private Methods
  // =================
  //  
};

// ======================
//  Inline declarations
// ======================
//
#ifdef HAVE_BOOST
#ifndef CURRENT_BOOST_RANDOM
#include <boost/random/variate_generator.hpp>
#endif // CURRENT_BOOST_RANDOM
#include <boost/random/uniform_int.hpp> // uniform_int
#include <boost/random/uniform_real.hpp> // uniform_real
#include <boost/random/exponential_distribution.hpp> // exponential_distribution

#else // not HAVE_BOOST
#include <cstdlib>
#include <cmath>
#endif // HAVE_BOOST

#include "macros.h" // REQUIRE() ENSURE()

inline void RandomHandler::set_seed (int seed)
{
#ifdef HAVE_BOOST
  _generator.seed (seed);
#else
  srand (seed);
#endif
}

inline RandomHandler& RandomHandler::instance (void)
{
  return _instance;
}

inline int RandomHandler::draw_uniform (int a, int b)
{
  REQUIRE (a <= b); /** @pre a must be smaller or equal to b. */

#ifdef HAVE_BOOST
#ifdef CURRENT_BOOST_RANDOM
  boost::random::uniform_int_distribution<> distribution (a, b);
  return distribution (RandomHandler::_generator);
#else // OBSOLETE BOOST
  boost::uniform_int<> distribution (a, b);
  boost::variate_generator<boost::mt19937&, boost::uniform_int<> > 
    dice (_generator, distribution);
  return dice();
#endif // CURRENT_BOOST_RANDOM
#else // not HAVE_BOOST
  // or try: return a + rand() % (b-a);
  return floor (draw_uniform ((double) a, (double) (b+1)));
#endif // HAVE_BOOST
}

inline double RandomHandler::draw_uniform (double a, double b)
{
  REQUIRE (a <= b); /** @pre a must be smaller or equal to b. */

#ifdef HAVE_BOOST
#ifdef CURRENT_BOOST_RANDOM
  boost::random::uniform_real_distribution<> distribution (a, b);
  return distribution (RandomHandler::_generator);
#else // OBSOLETE BOOST
  boost::uniform_real<> distribution (a, b);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<> > 
    dice (_generator, distribution);
  return dice ();
#endif // CURRENT_BOOST_RANDOM
#else // not HAVE_BOOST
  return a + (b-a) * rand() / ((double) RAND_MAX + 1);  
#endif // HAVE_BOOST
}

inline double RandomHandler::draw_exponential (double lambda)
{
  REQUIRE (lambda > 0); /** @pre lambda must be positive. */

#ifdef HAVE_BOOST
#ifdef CURRENT_BOOST_RANDOM
  boost::random::exponential_distribution<> distribution (lambda);
  return distribution (_generator);
#else // OBSOLETE BOOST
  boost::exponential_distribution<> distribution (lambda);
  boost::variate_generator<boost::mt19937&, boost::exponential_distribution<> > 
    dice (_generator, distribution);
  return dice ();
#endif // CURRENT_BOOST_RANDOM
#else // not HAVE_BOOST
  return (-log (1 - rand() / ((double) RAND_MAX + 1)) / lambda);
#endif // HAVE_BOOST
}


#endif // RANDOMHANDLER_H
