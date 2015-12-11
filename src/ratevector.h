

/**
 * @file ratevector.h
 * @brief Header for the RateVector class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef RATE_VECTOR_H
#define RATE_VECTOR_H

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
#include "ratecontainer.h"
#include "biasedwheel.h"

/**
 * @brief Class used to store rates depending on multiple elements.
 *
 * RateVector inherits RateContainer. It stores rates as a simple vector and
 * cumulated rates as another vector that is fully updated when prompted. 
 * Random drawing of a rate is done by binary search using the cumulated rate
 * vector. Therefore, its update cost is O(N) and its random drawing O(logN),
 * where N is the number of rates stored. The vector can be extended 
 * dynamically.
 * @sa RateTree.
 */
class RateVector : public RateContainer
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   * @param size Initial size of the vector (0 by default).
   */
  RateVector (int size = 0)
    : _rates (size, 0)
    , _cumulated_rates (size, 0)
    , _biased_wheel (_cumulated_rates)
  {
  }

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // RateVector (const RateVector& other_rate_vector);

  /**
   * @brief Destructor.
   */
  ~RateVector (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // inherited (virtual)
  void update_cumulates (void)
  {
    std::partial_sum (_rates.begin(), _rates.end(), _cumulated_rates.begin());
  }

  /**
   * @brief Find reaction index corresponding to cumulated rate value.
   * @param value Cumulated rate value.
   * @return Reaction index such that the cumulated rate < index
   *  is smaller than cumulated_value while the cumulated rate <= index
   *  is greater or equal to cumulated_value. This index is computed from
   *  the cumulated rates as of last update, not current rates.
   */
  int find (double value) const
  {
    /** @pre drawn value must be consistent with total rate. */
    REQUIRE ((value > 0) && (value <= _cumulated_rates.back()));
    return _biased_wheel.find_index (value);
  }

  // inherited (virtual)
  int random_index (void) const
  {
    /** Total rate must be strictly positive. */
    ENSURE (total_rate() > 0);
    return find (RandomHandler::instance().draw_uniform
		 (1e-16*total_rate(), total_rate()));
  }

  /**
   * @brief Resize the rate vector.
   * @param number_values Size by which the vector will be extended.
   */
  void extend (int number_values)
  {
    /** @pre new_size must be positive. */
    REQUIRE (number_values > 0);
    int new_size = _rates.size() + number_values;
    _rates.resize (new_size, 0);
    int fill_value = 0;
    if (_cumulated_rates.size() > 0) fill_value = _cumulated_rates.back();
    _cumulated_rates.resize (new_size, fill_value);
  }


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // inherited (virtual)
  double total_rate (void) const
  { 
    /** @post total rate should be positive. */
    ENSURE (_cumulated_rates.back() >= 0);
    return _cumulated_rates.back();
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
  // inherited (virtual)
  void set_rate (int index, double value)
  {
    /** @pre index must be within vector bounds. */
    REQUIRE ((index >= 0) && (index < size()));
    /** @pre value must be positive. */
    REQUIRE (value >= 0);
    _rates [index] = value;
  }


  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /*
  //  * @brief Assignment operator.
  //  */
  // RateVector& operator= (const RateVector& other_rate_vector);



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
  /** @brief Vector of rates. */
  std::vector <double> _rates;

  /** @brief Vector of cumulated rates. */
  std::vector <double> _cumulated_rates;

  /** @brief Biased wheel used to find drawn index. */
  BiasedWheel <double> _biased_wheel;

  // =================
  //  Private Methods
  // =================
  //


  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//


#endif // RATE_VECTOR_H
