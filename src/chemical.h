

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
#include <iostream>

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "macros.h"
#include "reactant.h"
#include "simulatorinput.h"

/**
 * @brief Global class that contains all chemicals.
 *
 * If an entity has no particular characteristics for the simulation, it belongs
 * directly to the class Chemical, else it belongs to a class inheriting
 * Chemical.
 */
class Chemical : public Reactant, public SimulatorInput
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

  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Number of elements in current pool.
   * @return The number of elements in the current pool.
   */
  int number (void) const;

 
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
  
  /**
   * @brief Standard output.
   * @return A reference to the stream containing the output.
   * @param output Stream where output should be written.
   * @param chemical Reference to the chemical whose information should be written.
   */
  friend std::ostream& operator<< (std::ostream& output, const Chemical& chemical);

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
inline int Chemical::number (void) const { return _number; }

inline void Chemical::add (int quantity)
{
  REQUIRE( quantity >= 0 ); /** @pre Quantity is nonnegative. */
  
  _number += quantity;
  notify_change();
}

inline void Chemical::remove(int quantity)
{ 
  /** @pre Quantity does not exceed current number. */
  REQUIRE( quantity <= _number );
  
  _number -= quantity;
  notify_change();
}

inline std::ostream& operator<< (std::ostream& output, const Chemical& chemical)
{
  chemical.print (output);
  return output;
}

#endif // CHEMICAL_H
