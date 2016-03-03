

/**
 * @file freeendhandler.h
 * @brief Header for the FreeEndHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef FREE_END_HANDLER_H
#define FREE_END_HANDLER_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "bindingsite.h"

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class used to generate binding sites from free ends.
 *
 * FreeEndHandler offers a simplified interface to binding site creation and
 * destruction due to free ends. If there is no opposite sequence, no binding
 * sites are actually created.
 */

class FreeEndHandler
{
 public:
  /**
   * @brief Constructor.
   * @param opposite_sequence ChemicalSequence on which binding sites should 
   *  be created. 0 by default, meaning that there is no appariated sequence
   *  and no binding sites should actually be created.
   *  
   */
  FreeEndHandler (ChemicalSequence* opposite_sequence = 0);

  /**
   * @brief Create binding site on base opposite of given position.
   * @param position Position immediatly preceding a left end, 
   *  facing binding site to create.
   * @return BindingSite created or 0 if no binding site was created (e.g. if
   *  there is no appariated sequence).
   */
  BindingSite* create_left (int position) const;
  
  /**
   * @brief Create binding site on base opposite of given position.
   * @param position Position immediatly following a right end, facing binding 
   *  site to create.
   * @return BindingSite created or 0 if no binding site was created (e.g. if
   *  there is no appariated sequence).
   */
  BindingSite* create_right (int position) const;
  
  /**
   * @brief Remove previously created binding site.
   * @param site BindingSite to remove. Nothing happens is there is no opposite
   *  sequence or provided site is the 0 pointer.
   */
  void remove (BindingSite* site) const;

  /**
   * @brief Define a pairing with an opposite sequence.
   * @param opposite_sequence ChemicalSequence on which binding sites should 
   *  be created.
   */
  void set_opposite (ChemicalSequence& opposite_sequence);

  /**
   * @brief Break pairing defined at construction. No more binding sites will
   *  be created.
   */
  void break_pairing (void);

 private:
  /** @brief Sequence on which binding sites should be created (0 if none). */
  ChemicalSequence* _opposite;
};

// ======================
//  Inline declarations
// ======================
//

#endif // FREE_END_HANDLER_H
