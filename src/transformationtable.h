

/**
 * @file transformationtable.h
 * @brief Header for the TransformationTable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef TRANSFORMATION_TABLE_H
#define TRANSFORMATION_TABLE_H

// ==================
//  General Includes
// ==================
//
#include <string> // std::string
#include <map> // std::map

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "macros.h"

/**
 * @brief Class storing string motif transformations.
 *
 * TransformationTable stores transformations taking a string motif
 * as an input and yielding another string motif as an output. It can
 * transform a sequence of input motifs into a sequence of the corresponding
 * output motifs.
 */
class TransformationTable
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param input_motif_length Length of input motifs.
   */
  TransformationTable (int input_motif_length);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // TransformationTable (const TransformationTable& other_transformation_table);

  /**
   * @brief Destructor.
   */
  ~TransformationTable (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Add new transformation rule.
   * @param input String containing input motif.
   * @param output string containing output motif.
   */
  void add_rule (const std::string& input, const std::string& output);

  /**
   * @brief Transform sequence motif by motif using existing rules.
   * @param sequence Sequence to transform.
   * @return String containing transfromed sequence or empty string if
   * transformation failed (at least one input motifs was unknown).
   */
  std::string transform (const std::string& sequence) const;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //


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
  // TransformationTable& operator= (const TransformationTable& other_transformation_table);

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
  /** @brief Map containing the transformation rules. */
  std::map <std::string, std::string> _rules;

  /** @brief Input motif length. */
  int _motif_length;

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
inline void TransformationTable::add_rule (const std::string& input, const std::string& output)
{
  /** @pre Input motif length must match length specified at construction. */
  REQUIRE (input.length() == _motif_length);
  
  _rules [input] = output;
}


#endif // TRANSFORMATION_TABLE_H
