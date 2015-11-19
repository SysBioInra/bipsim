

/**
 * @file subratevector.h
 * @brief Header for the SubRateVector class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef SUB_RATE_VECTOR_H
#define SUB_RATE_VECTOR_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "macros.h" // REQUIRE() ENSURE()

/**
 * @brief Class used to store rates depending on multiple elements.
 *
 * SubRateVector is a convenience class that stores rates as the sum of several
 * subrates. It is typically used when a reaction can use multiple different 
 * reactants, e.g. binding sites, codons. The class is essentially a vector that
 * automatically computes the total rates as the sum of all subrates stored in 
 * the vector.
 */
class SubRateVector
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   * @param initial_size
   */
  SubRateVector (int initial_size = 0)
    : _rates (initial_size, 0)
    , _total_rate (0)
    {
      /** @pre initial_size must be positive. */
      REQUIRE (initial_size >= 0);
    }

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // SubRateVector (const SubRateVector& other_sub_rate_vector);

  /**
   * @brief Destructor.
   */
  ~SubRateVector (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Update a rate in the vector.
   * @param index Index of the entry to modify.
   * @param value New rate value.
   */
  void update (int index, double value)
  {
    /** @pre index must be within vector bounds. */
    REQUIRE ((index >= 0) && (index < size()));
    /** @pre value must be positive. */
    REQUIRE (value >= 0);

    _total_rate += (value - _rates [index]);
    _rates [index] = value;

    if (_total_rate < 0) compute_total_rate();

    /** @post total rate must remain positive. */
    ENSURE (_total_rate >= 0);
  }

  /**
   * @brief Resize the rate vector.
   * @param new_size New size of the vector.
   * @param fill_value Value used to fill vector in case new size exceeds old
   *  size.
   */
  void resize (int new_size, double fill_value = 0)
  {
    /** @pre new_size must be positive. */
    REQUIRE (new_size >= 0);
    
    if (new_size > size())
      {
	// total rate won't change because we only appends 0s
	_rates.resize (new_size, fill_value);
      }
    else
      {
	// we remove potential nonzero values: we need to recompute total rate
	_rates.resize (new_size);
	compute_total_rate();
      }
  }

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to total rate.
   * @return Total rate of the rate vector (sum of vector entries).
   */
  double total_rate (void) const { return _total_rate; }

  /**
   * @brief Accesor to size of the rate vector.
   * @return Current size of the rate vector.
   */
  int size (void) const { return _rates.size(); }

  /**
   * @brief Accessor to subrates.
   * @return Vector of subrates.
   */
  const std::vector<double>& subrates (void) const { return _rates; }

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //


  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /*
  //  * @brief Assignment operator.
  //  */
  // SubRateVector& operator= (const SubRateVector& other_sub_rate_vector);

protected:
  // ======================
  //  Protected Attributes
  // ======================
  //

  // ===================
  //  Protected Methods
  // ===================
  //


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Vector of subrates. */
  std::vector <double> _rates;

  /** @brief Total rate contained in the vector. */
  double _total_rate;

  // =================
  //  Private Methods
  // =================
  //
  /** @brief Compute total rate from scratch. */
  void compute_total_rate (void)
  {
    // recompute the total rate contribution from the contribution table
    _total_rate = 0;
    // loop through binding sites
    for (std::vector<double>::iterator rate = _rates.begin();
	 rate != _rates.end(); ++rate)
      {
	_total_rate += *rate;
      }
  }


  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//

#endif // SUB_RATE_VECTOR_H
