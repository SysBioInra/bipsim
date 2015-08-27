

/**
 * @file biasedwheel.h
 * @brief Header for the BiasedWheel class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef BIASED_WHEEL_H
#define BIASED_WHEEL_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

/**
 * @brief Template class representing a weighted vector from which values are drawn.
 *
 * BiasedWheel is a template class that is initialized with a weight vector. It does
 * NOT do any of the RNG part, it enables searching the index/indices corresponding
 * to some user-provided cumulated weight.
 */
template <typename T>
class BiasedWheel
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   * @param weights Vector of weights representing the biased wheel.
   */
  BiasedWheel (const std::vector<T>& weights);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // BiasedWheel ( const BiasedWheel& other_class_name );

  /**
   * @brief Destructor.
   */
  ~BiasedWheel (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Find the index corresponding to some cumulated weight.
   * @param drawn_weight Cumulated weight to search for in the wheel in the range
   *  (0, total_weight].
   * @return Index corresponding to cumulated weights by applying the following
   *  rules: if cw[0] >= dw, i = 0, else (a) cw[i-1] < dw and (b) cw[i] >= dw,
   *  where i is the returned index, cw the cumulated weights vector and dw the drawn weight.
   */
  int find_index (T drawn_weight);
  
  /**
   * @brief Find indices corresponding to a list of cumulated weights.
   * @param drawn_weights List of cumulated weights to search for in the wheel.
   * @return Vector of indices corresponding to list of cumulated weights by 
   *  following exactly the same rules as find_index().
   */
  std::vector<int> find_multiple_indices (const std::vector<T>& drawn_weights);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to the cumulated form of the wheel.
   * @return Vector containing cumulative form of the wheel, all 0 values in the
   *  original vector being totally skipped (i.e. the cumulative form may be shorter
   *  than the original vector).
   *
   * Mainly for maintenance purposes, as this really is an internal representation
   * of the wheel that should be of no interest for a user.
   */
  const std::vector<T>& cumulated_weights (void);

  /**
   * @brief Accessor to total of weights contained in the wheel.
   * @return Sum of weights contained in the wheel.
   */
  T total_weight (void);

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
  // BiasedWheel& operator= ( const BiasedWheel& other_class_name );

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved.
   */
  bool check_invariant (void) const;


private:

  // ============
  //  Attributes
  // ============
  //
  /**
   * @brief Vector representating the wheel in a cumulative form, all 0 in the original having been
   *  skipped.
   *
   * By skipping 0s, the values of the vector are strictly increasing which facilitates some algorithms.
   */
  std::vector<T> _cumulated_weights;

  /**
   * @brief Mapping of indices in cumulative form to indices in original form.
   *
   * Because we skip 0 values, cumulative form is smaller in general and does not preserve indices, so we
   * need a mapping that enables us to return original inidices to the user.
   */
  std::vector<int> _original_indices;

  /**
   * @brief Sum of weights contained in the biased wheel.
   */
  T _total_weight;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Create wheel by taking out all 0 values of a vector and create a cumulative form of remaining values.
   * @param vector_to_cumulate Vector used to create wheel.
   */
  void cumulate_vector_and_strip (const std::vector<T>& vector_to_cumulate);

  /**
   * @brief Check wether all value of a vector are positive.
   * @param v Weight vector to check.
   * @return true if all values are greater or equal to 0, false else.
   */
  bool check_weight_positivity (const std::vector<T>& v);

  /**
   * @brief Check wether values of a vector can effectively be drawn on current wheel..
   * @param v Drawn weight vector to check.
   * @return true if all values are in the (0, _total_weight] range, false else.
   */
  bool check_drawn_weight_validity (const std::vector<T>& v);

  /**
   * @brief Class for comparing values of a vector by inputting indices.
   *
   * Provides a way for the user to only work with indices. Useful in sorting
   * functions when a traceback to original indices is indeed.
   */
  class CompareValues
  {
  public:
    CompareValues (const std::vector<T>& vector): v (vector) {}
    bool operator() (int i, int j) { return v [i] < v [j]; }
  private:
    const std::vector<T>& v;
  };

  /**
   * @brief Provide order of vector elements by indices, leaving original vector unchanged.
   * @param vector_to_sort Vector whose sorting is needed.
   * @return Order vector giving indices of elements from the smallest to the largest
   *  (e.g. sorted_indices(v)[0] yields the index of smallest value in v).
   */
  std::vector<int> sorted_indices (const std::vector<T>& vector_to_sort);
    // ======================
  //  Forbidden Operations
  // ======================
  //
};

// ======================
//  Inline declarations
// ======================
//
template <typename T>
inline const std::vector<T>& BiasedWheel<T>::cumulated_weights (void)
{
  return _cumulated_weights;
}

template <typename T>
inline T BiasedWheel<T>::total_weight (void)
{
  return _total_weight;
}



#endif // BIASED_WHEEL_H
