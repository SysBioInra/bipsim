
/**
 * @file freechemical.h
 * @brief Header for the ClassName class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef FREE_CHEMICAL_H
#define FREE_CHEMICAL_H

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "chemical.h"
#include "forwarddeclarations.h"

/**
 * @brief Class for chemicals diffusing freely in the cytosol.
 */
class FreeChemical : public Chemical
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  // Not needed for this class (use of compiler-generated versions)
  /** @brief Default constructor. */
  FreeChemical (void) : _constant (false) {}

  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // FreeChemical (const FreeChemical& other_chemical);
  // /* @brief Assignment operator. */
  // FreeChemical& operator= (const FreeChemical& other_chemical);
  // /* @brief Destructor. */
  // ~FreeChemical (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from Chemical  
  void add (int quantity) { if (!_constant) Chemical::add (quantity); }
  void remove (int quantity) { if (!_constant) Chemical::remove (quantity); }
  
  /**
   * @brief Specify whether chemical's concentration should change over time. 
   * @param is_constant If set to true, all add and remove 
   *  commands will be ignored.
   */
  void set_constant (bool is_constant) { _constant = is_constant; }

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to flag specifying if chemical has constant concentration. 
   * @return True if chemical has constant concentartion (i.e. it ignores all
   *  add and remove commands).
   */
  bool is_constant (void) const { return _constant; }

private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief Whether chemical has constant concentration or not. */
  bool _constant;
};

// ======================
//  Inline declarations
// ======================
//

#endif // FREE_CHEMICAL_H
