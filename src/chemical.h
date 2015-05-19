

/**
 * @file chemical.h
 * @brief Header for the Chemical class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef CHEMICAL_H
#define CHEMICAL_H

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "macros.h"

/**
 * @brief Global class that contains all chemicals.
 *
 * If an entity has no particular characteristics for the simulation, it belongs
 * directly to the class Chemical, else it belongs to a class inheriting
 * Chemical.
 */
class Chemical
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  Chemical (void);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // Chemical (Chemical& other_chemical);

  /**
   * @brief Destructor
   */
  virtual ~Chemical (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Adds a given quantity of chemical to the existing pool.
   * @param quantity The amount to add
   */
  virtual void add (int quantity);

  /**
   * @brief Remove a given quantity of chemical to the existing pool.
   * @param quantity The amount to remove.
   */
  virtual void remove (int quantity);


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @return The number of elements in the current pool.
   */
  int number (void);

 
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
  // Chemical& operator= (Chemical& other_chemical);

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
  /** @brief The number of elements in the current pool. */
  int _number;

  // =================
  //  Private Methods
  // =================
  //

};

// ======================
//  Inline declarations
// ======================
//
inline int Chemical::number (void) { return _number; }

inline void Chemical::add (int quantity)
{
  REQUIRE( quantity >= 0 ); /** @pre Quantity is nonnegative. */
  
  _number += quantity;
}

inline void Chemical::remove(int quantity)
{ 
  /** @pre Quantity does not exceed current number. */
  REQUIRE( quantity <= _number );
  
  _number -= quantity;
}


#endif // CHEMICAL_H
