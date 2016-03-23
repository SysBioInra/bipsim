

/**
 * @file reactant.h
 * @brief Header for the Reactant class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef REACTANT_H
#define REACTANT_H

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "observable.h"

/**
 * @brief Abstract class that contains all possible reactants.
 */
class Reactant : public Observable
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  // Not needed for this class (use of compiler-generated versions)
  // /* @brief Constructor. */
  // Reactant (void);
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // Reactant (const Reactant& other);
  // /* @brief Assignment operator. */
  // Reactant& operator= (const Reactant& other);

  /** @brief Destructor. */
  virtual ~Reactant (void) = 0;

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //  
};

inline Reactant::~Reactant (void) {}

#endif // REACTANT_H
