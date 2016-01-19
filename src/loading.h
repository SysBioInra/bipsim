
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
    

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // Not needed for this class (use of default copy constructor) !
  // /* @brief Copy constructor */
  // Loading (Loading& other_loading);
  // /* @brief Assignment operator */
  // Loading& operator= (Loading& other_loading);
  // /* @brief Destructor */
  // virtual ~Loading (void);
  
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  //

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // Redefined from Reaction
  bool is_reaction_possible (void) const;

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
  // redefined from Reaction
  void do_reaction (void);
  double compute_rate (void) const;
  void print (std::ostream& output) const;
};

// =====================
//  Inline Declarations
// =====================
//

#endif // LOADING_H
