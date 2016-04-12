

/**
 * @file flyratevector.h
 * @brief Header for the FlyRateVector class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef FLY_RATE_VECTOR_H
#define FLY_RATE_VECTOR_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector
#include <numeric> // std::partial_sum
#include <cmath> // fabs

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "macros.h" // REQUIRE() ENSURE()
#include "ratecontainer.h"

/**
 * @brief Class used to store rates as a vector.
 *
 * FlyRateVector inherits RateContainer. It stores rates as a simple vector. 
 * Random drawing of a rate is done by linear search, starting at the start or
 * at the end, depending on a parameter specified at construction. It is 
 * particularly efficient when it is known that larger values are likely to be
 * stored at the start/end of the vector. The vector can be extended 
 * dynamically.
 */
class FlyRateVector : public RateContainer
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   * @param size Initial size of the vector (0 by default).
   * @param forward Search direction (true = from the start,
   *  false = from the end).
   */
  FlyRateVector (int size = 0, bool forward = true)
    : _rates (size, 0)
    , _forward (forward)
    , _total_rate (0)
    , _last_max_rate (0)
  {
  }

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // FlyRateVector (const FlyRateVector& other_fly_rate_vector);
  // /* @brief Assignment operator. */
  // FlyRateVector& operator= (const FlyRateVector& other_fly_rate_vector);
  // /* @brief Destructor. */
  // ~FlyRateVector (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from RateContainer
  void update_cumulates (void)  {}
  int random_index (void) const;

  /**
   * @brief Resize the rate vector.
   * @param number_values Size by which the vector will be extended.
   */
  void extend (int number_values)
  {
    /** @pre new_size must be positive. */
    REQUIRE (number_values > 0);
    _rates.resize (_rates.size() + number_values, 0);
  }

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // redefined from RateContainer
  double total_rate (void) const
  { 
    /** @post total rate should be positive. */
    ENSURE (_total_rate >= 0);
    /** @post total rate should be close to real rate. */
    ENSURE ((_total_rate == 0) ||
	    (fabs (std::accumulate (_rates.begin(), _rates.end(), 0.0) 
		   - _total_rate)
	    < 1e-5 * std::accumulate (_rates.begin(), _rates.end(), 0.0)));
    return _total_rate;
  }

  /**
   * @brief Accessor to vector size.
   * @return Number of rates stored in the vector.
   */
  int size (void) const { return _rates.size(); }

  /**
   * @brief Accessor to individual rates.
   * @param index Index of rate in the vector.
   * @return Rate corresponding to given index.
   */
  const double& operator[] (int index) const { return _rates [index]; }


  // ==========================
  //  Public Methods - Setters
  // ==========================
  //
  // redefined from RateContainer
  void set_rate (int index, double value)
  {
    /** @pre index must be within vector bounds. */
    REQUIRE ((index >= 0) && (index < size()));
    /** @pre value must be positive. */
    REQUIRE (value >= 0);
    _total_rate += value - _rates [index];
    _rates [index] = value;
    if ((_total_rate <= 0) || (_total_rate < 1e-5 * _last_max_rate))
      { _compute_total_rate(); }
    else if (_total_rate > _last_max_rate) { _last_max_rate = _total_rate; }
  }

private:
  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Compute total rate from scratch.
   */
  void _compute_total_rate (void) const
  {
    _last_max_rate =_total_rate = 
      std::accumulate (_rates.begin(), _rates.end(), 0.0);
  }

  // redefined from RateContainer
  std::ostream& _print (std::ostream& output) const
  {
    for (std::vector <double>::const_iterator it = _rates.begin();
	 it != _rates.end(); ++it)
      { output << *it << " "; }
    output << _total_rate << "\n";
    return output;
  }

  // ============
  //  Attributes
  // ============
  //
  /** @brief Vector of rates. */
  std::vector <double> _rates;

  /** @brief Total rate. */
  mutable double _total_rate;

  /** @brief Max rate attained since last actual computation of total rate. */
  mutable double _last_max_rate;

  /** @brief Sense of cumulation. */
  bool _forward;
};

// ======================
//  Inline declarations
// ======================
//
inline int FlyRateVector::random_index (void) const
{
  /** @pre Total rate must be strictly positive. */
  REQUIRE (total_rate() > 0);
  double u = RandomHandler::instance().draw_uniform
    (1e-16*total_rate(), total_rate());
  if (_forward)
    {
      int index_drawn = 0;
      while (index_drawn < _rates.size()) 
	{
	  u -= _rates [index_drawn]; 
	  if (u <= 0) { return index_drawn; }
	  ++index_drawn;
	}
      // problem: approximation of total rate was wrong
      double approximation = total_rate();
      _compute_total_rate();
      u = _total_rate * (1 - u / approximation);
      while (u >= 0) {--index_drawn; u -= _rates [index_drawn]; }	
      return index_drawn;
    }
  else // search backward
    {
      int index_drawn = _rates.size() - 1;
      while (index_drawn >= 0)
	{ 
	  u -= _rates [index_drawn];
	  if (u <= 0) { return index_drawn; }
	  --index_drawn;
	}
      // problem: approximation of total rate was wrong
      double approximation = total_rate();
      _compute_total_rate();
      u = _total_rate * (1 - u / approximation);
      while (u >= 0) { ++index_drawn; u -= _rates [index_drawn]; }	
      return index_drawn;
    }
}


#endif // FLY_RATE_VECTOR_H
