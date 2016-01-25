

/**
 * @file freechemical.h
 * @brief Header for the ClassName class.
 * 
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

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class for chemicals diffusing freely in the cytosol.
 *
 *
 */
class FreeChemical : public Chemical
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  // Not needed for this class (use of compiler-generated versions)
  // /* @brief Default constructor. */
  // FreeChemical (void);
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


  // ============================
  //  Public Methods - Accessors
  // ============================
  //


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

#endif // FREE_CHEMICAL_H
