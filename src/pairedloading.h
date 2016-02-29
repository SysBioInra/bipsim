

/**
 * @file pairedloading.h
 * @brief Header for the PairedLoading class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef PAIRED_LOADING_H
#define PAIRED_LOADING_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "loadingbase.h"

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class used for strand extension in a double paired sequence.
 *
 * PairedLoading represents loading of a base that automatically appariates
 * to the strand bearing the polymerase.
 * This class inherits class LoadingBase.
 */
class PairedLoading : public LoadingBase
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
  PairedLoading (Loader& loader);
  
  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // PairedLoading (const PairedLoading& other);
  // /* @brief Assignment operator. */
  // PairedLoading& operator= (const PairedLoading& other);
  // /* @brief Destructor. */
  // ~PairedLoading (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //

  // ============================
  //  Public Methods - Accessors
  // ============================
  //


 protected:
  // ===================
  //  Protected Methods
  // ===================
  //
  // redefined from Reaction
  void do_reaction (void);
  void print (std::ostream& output) const;
};

// ======================
//  Inline declarations
// ======================
//

#endif // PAIRED_LOADING_H
