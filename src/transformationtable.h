

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
#include <vector> // std::vector
#include <string> // std::string
#include <map> // std::map

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "macros.h"
#include "simulatorinput.h"

/**
 * @brief Class storing string motif transformations.
 *
 * TransformationTable stores transformations taking a string motif
 * as an input and yielding another string motif as an output. It can
 * transform a sequence of input motifs into a sequence of the corresponding
 * output motifs.
 */
class TransformationTable : public SimulatorInput
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param input_motifs Vector of strings containing input motifs.
   * @param output_motifs Vector of strings containing output motifs.
   */
  TransformationTable (const std::vector <std::string>& input_motifs, 
		       const std::vector <std::string>& output_motifs);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // TransformationTable (const TransformationTable& other_table);
  // /* @brief Assignment operator. */
  // TransformationTable& operator= (const TransformationTable& other_table);
  // /* @brief Destructor. */
  // ~TransformationTable (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Transform sequence motif by motif using existing rules.
   * @param sequence Sequence to transform.
   * @return String containing transformed sequence or empty string if
   * transformation failed (at least one input motifs was unknown).
   */
  std::string transform (const std::string& sequence) const;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to input motif length.
   * @return Input motif length as defined at construction.
   */
  int input_motif_length (void) const;

private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief Map containing the transformation rules. */
  std::map <std::string, std::string> _rules;

  /** @brief Input motif length. */
  int _motif_length;
};

// ======================
//  Inline declarations
// ======================
//
inline int TransformationTable::input_motif_length (void) const
{
  return _motif_length;
}


#endif // TRANSFORMATION_TABLE_H
