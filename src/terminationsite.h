

/**
 * @file terminationsite.h
 * @brief Header for the TerminationSite class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef TERMINATIONSITE_H
#define TERMINATIONSITE_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

/**
 * @brief Represents termination sites for processive chemicals.
 *
 * Usually associated with the ending of synthesis processes. A termination site
 * is simply a motif found on a sequence.
 */
class TerminationSite
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  TerminationSite (void);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // TerminationSite (TerminationSite& other_termination_site);

  /**
   * @brief Destructor
   */
  virtual ~TerminationSite (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //


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
  // TerminationSite& operator= (TerminationSite& other_termination_site);

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @return True if class invariant is preserved
   */
  bool check_invariant (void);

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


#endif // TERMINATIONSITE_H
