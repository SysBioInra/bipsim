

/**
 * @file processivechemical.h
 * @brief Header for the ProcessiveChemical class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef PROCESSIVECHEMICAL_H
#define PROCESSIVECHEMICAL_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "boundchemical.h"

/**
 * @brief Class that represents elements that can process a chemical sequence.
 *
 * This means that, in addition to binding a chemical sequence, a processive 
 * chemical can move along it and, possibly, use the sequence as a template to
 * perform an elongation reaction.
 * Inherits class BoundChemical.
 * @sa BoundChemical Elongation
 */
class ProcessiveChemical : public BoundChemical
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  ProcessiveChemical (void);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // ProcessiveChemical (ProcessiveChemical& other_processive_chemical);

  /**
   * @brief Destructor
   */
  virtual ~ProcessiveChemical (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Actions to perform when moving beyond sequence limits.
   */
  void handle_out_of_bounds (void);


  /**
   * @brief Actions to perform when moving onto a termination site.
   */
  void handle_termination_site (void);


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
  //  * @brief Assignment operator
  //  */
  // ProcessiveChemical& operator= (ProcessiveChemical& other_processive_chemical);


  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void);


private:

  // ============
  //  Attributes
  // ============
  //
  

  // =================
  //  Private Methods
  // =================
  //

};

// ======================
//  Inline declarations
// ======================
//




#endif // PROCESSIVECHEMICAL_H
