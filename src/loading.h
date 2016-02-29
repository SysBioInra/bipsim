
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
#include "loadingbase.h"

// ===================
//  Class Declaration
// ===================
//
/**
 * @brief Class for representing cytosolic loading reactions.
 *
 * Loading represents reaction in which a chemical (a Loader) matches
 * up a chemical with the template it is currently bound to, eventually
 * leading to a cytosolic product.
 * This class inherits class BaseLoading.
 */
class Loading : public LoadingBase
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

 private:
  // =================
  //  Private Methods
  // =================
  //
  // redefined from LoadingBase
  void do_reaction (void);
  void print (std::ostream& output) const;
};

// =====================
//  Inline Declarations
// =====================
//

#endif // LOADING_H
