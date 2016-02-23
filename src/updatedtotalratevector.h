

/**
 * @file updatedtotalratevector.h
 * @brief Header for the UpdatedTotalRateVector class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef UPDATED_TOTAL_RATE_VECTOR_H
#define UPDATED_TOTAL_RATE_VECTOR_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector
#include <numeric> // std::partial_sum

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "macros.h" // REQUIRE() ENSURE()
#include "ratevector.h"

/**
 * @brief Class used to store rates whose total is often accessed.
 *
 * UpdatedTotalRateVector inherits RateVector. It does essentially the same
 * as its parent, except the total rate is always updated. Other cumulated rates
 * are still computed at user's request. This class is especially useful when
 * the total rate is often accessed compared to operations that need the whole
 * distribution of cumulated rates (i.e. multinomial drawing).
 */
class UpdatedTotalRateVector : public RateVector
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param size Initial size of the vector (0 by default).
   */
  UpdatedTotalRateVector (int size = 0)
    : RateVector (size)
    , _total_approximation (0)
  {
  }

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // UpdatedTotalRateVector (const UpdatedTotalRateVector& other_rate_vector);
  // /* @brief Assignment operator. */
  // UpdatedTotalRateVector& operator= (const UpdatedTotalRateVector& other_rate_vector);
  // /* @brief Destructor. */
  // ~UpdatedTotalRateVector (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from RateVector
  void update_cumulates (void)
  {
    RateVector::update_cumulates();
    _total_approximation = RateVector::total_rate();
  }

  // redefined from RateVector
  void set_rate (int index, double value)
  {
    /** @pre index must be within vector bounds. */
    REQUIRE ((index >= 0) && (index < size()));
    /** @pre value must be positive. */
    REQUIRE (value >= 0);
    _total_approximation = 
      _total_approximation - RateVector::operator[] (index) + value;
    RateVector::set_rate (index, value);
    if (_total_approximation < 0) { update_cumulates(); }
    /** @post Total rate approximation should stay positive (invariant). */
    ENSURE (_total_approximation >= 0);
  }

  // redefined from RateVector
  void pop_back (void)
  {
    _total_approximation -= RateVector::operator[] (size()-1);
    RateVector::pop_back();
    if (_total_approximation < 0) { update_cumulates(); }
    /** @post Total rate approximation should stay positive (invariant). */
    ENSURE (_total_approximation >= 0);
  }

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  double total_rate (void) const
  { 
    /** @post total rate should be positive. */
    ENSURE (_total_approximation >= 0);
    return _total_approximation;
  }

private:
  // ============
  //  Attributes
  // ============
  //
  double _total_approximation;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//


#endif // UPDATED_TOTAL_RATE_VECTOR_H
