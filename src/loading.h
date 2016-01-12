
/**
 * @file loading.h
 * @brief Header for the Loading class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef LOADING_H
#define LOADING_H

// ==================
//  General Includes
// ==================
//
#include <string>

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "reaction.h"

// ===================
//  Class Declaration
// ===================
//
/**
 * @brief Class for representingloading reactions.
 *
 * Loading represents reaction in which a chemical (a Loader) matches
 * up a chemical with the template it is currently bound to.
 * This class inherits class Reaction.
 * @sa Reaction
 */
class Loading : public Reaction
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param loader
   *  Element that recognizes and loads a chemical corresponding to the template
   *  it is reading.
   */
  Loading (Loader& loader);
    

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // Loading (Loading& other_loading);

  /**
   * @brief Destructor
   */
  virtual ~Loading (void);
  
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  //

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns whether there are enough chemicals to perform reaction.
   * @return True if there are enough reactant, false otherwise.
   */
  virtual bool is_reaction_possible (void) const;

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
  // Loading& operator= (Loading& other_loading);


 protected:
  // ===================
  //  Protected Methods
  // ===================
  //  


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Chemical loading an element corresponding to current template. */
  Loader& _loader;  
  
  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Update chemical quantities according to the reaction.
   */
  virtual void do_reaction (void);

  /**
   * @brief Compute current reaction rates.
   * @return Current reaction rate.
   */
  virtual double compute_rate (void) const;

  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const;
};

// =====================
//  Inline Declarations
// =====================
//

#endif // LOADING_H
