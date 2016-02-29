
/**
 * @file loadingbase.h
 * @brief Header for the LoadingBase class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef LOADING_BASE_H
#define LOADING_BASE_H

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
 * @brief Base class for loading reactions.
 *
 * LoadingBase represents reaction in which a chemical (a Loader) matches
 * up a chemical with the template it is currently bound to.
 * This class inherits class Reaction.
 * @sa Reaction
 */
class LoadingBase : public Reaction
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param loader Element that recognizes and loads a chemical corresponding 
   *  to the template it is reading.
   */
  LoadingBase (Loader& loader);    

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // Not needed for this class (use of default copy constructor) !
  // /* @brief Copy constructor */
  // LoadingBase (LoadingBase& other_loading);
  // /* @brief Assignment operator */
  // LoadingBase& operator= (LoadingBase& other_loading);
  // /* @brief Destructor */
  // virtual ~LoadingBase (void);
  
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

 protected:
  // ===================
  //  Protected Methods
  // ===================
  //
  // redefined from Reaction
  double compute_rate (void) const;

  // ============
  //  Attributes
  // ============
  //
  /** @brief Chemical loading an element corresponding to current template. */
  Loader& _loader;    
};

// =====================
//  Inline Declarations
// =====================
//

#endif // LOADING_BASE_H
