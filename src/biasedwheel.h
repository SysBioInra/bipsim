// 
// Copyright 2017 INRA
// Authors: M. Dinh, S. Fischer
// Last modification: 2017-09-19
// 
// 
// Licensed under the GNU General Public License.
// You should have received a copy of the GNU General Public License
// along with BiPSim.  If not, see <http://www.gnu.org/licenses/>.
// 



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
 * @brief Generic class representing a weighted vector from which values are
 *  drawn.
 *
 * BiasedWheel is a template class that is initialized with a weight vector.
 * It does NOT do any of the RNG part, it enables searching the index/indices
 * corresponding to some user-provided cumulated weight.
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
   * @param cumulated_weights Vector of cumulated weights representing the
   *  biased wheel.
   */
  BiasedWheel (const std::vector<T>& cumulated_weights);

  // Not needed for this class (use of default compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // BiasedWheel (const BiasedWheel& other_class_name);
  // /* @brief Assignment operator. */
  // BiasedWheel& operator= (const BiasedWheel& other_class_name);
  // /* @brief Destructor. */
  // ~BiasedWheel (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Find the index corresponding to some cumulated weight.
   * @param drawn_weight Cumulated weight to search for in the wheel in the
   *  range (0, total_weight].
   * @return Index corresponding to cumulated weights by applying the following
   *  rules: if cw[0] >= dw, i = 0, else (a) cw[i-1] < dw and (b) cw[i] >= dw,
   *  where i is the returned index, cw the cumulated weights vector and dw the
   *  drawn weight.
   */
  int find_index (T drawn_weight) const;
  
  /**
   * @brief Find indices corresponding to a list of cumulated weights.
   * @param drawn_weights List of cumulated weights to search for in the wheel.
   * @return Vector of indices corresponding to list of cumulated weights by 
   *  following exactly the same rules as find_index().
   */
  std::vector<int> find_multiple_indices (const std::vector<T>& drawn_weights) const;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:
  // ============
  //  Attributes
  // ============
  //
  /**
   * @brief Vector representating the wheel in a cumulative form.
   */
  const std::vector<T>& _cumulated_weights;


  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Check wether all value of a vector are positive.
   * @param v Weight vector to check.
   * @return true if all values are greater or equal to 0, false else.
   */
  bool check_weight_positivity (const std::vector<T>& v) const;

  /**
   * @brief Check wether values of a vector can effectively be drawn on current
   *  wheel.
   * @param v Drawn weight vector to check.
   * @return true if all values are in the (0, _total_weight] range, false else.
   */
  bool check_drawn_weight_validity (const std::vector<T>& v) const;

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
   * @brief Provide order of vector elements by indices, leaving original vector
   *  unchanged.
   * @param vector_to_sort Vector whose sorting is needed.
   * @return Order vector giving indices of elements from the smallest to the
   *  largest (e.g. sorted_indices(v)[0] yields the index of smallest value in
   *  v).
   */
  std::vector<int> sorted_indices (const std::vector<T>& vector_to_sort) const;
};

// ======================
//  Inline declarations
// ======================
//



#endif // BIASED_WHEEL_H
