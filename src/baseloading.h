
/**
 * @file baseloading.h
 * @brief Header for the BaseLoading class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef BASELOADING_H
#define BASELOADING_H

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
 * @brief Class for representing base loading reactions.
 *
 * BaseLoading represents reaction in which a chemical (a BaseLoader) matches
 * up a base with the template it is currently bound to.
 * This class inherits class Reaction.
 * @sa Reaction
 */
class BaseLoading: public Reaction
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param base_loader
   *  Element that recognizes and loads a chemical corresponding to the template
   *  it is reading.
   */
  BaseLoading (BaseLoader& base_loader);
    

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // BaseLoading (BaseLoading& other_base_loading);

  /**
   * @brief Destructor
   */
  virtual ~BaseLoading (void);
  
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  //
  /**
   * @brief Update chemical quantities according to the forward reaction.
   */
  void perform_forward (void);

  /**
   * @brief Update chemical quantities according to the backward reaction.
   */
  void perform_backward (void);

  /**
   * @return Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const;

  /**
   * @brief Update reaction rates.
   */
  virtual void update_rates ( void );


  // ============================
  //  Public Methods - Accessors
  // ============================
  //


  // ==========================
  //  Public Methods - Setters
  // ==========================
  //

  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /**
  //  * @brief Assignment operator
  //  */
  // BaseLoading& operator= (BaseLoading& other_base_loading);

  
  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;

private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Chemical loading a base corresponding to current template. */
  BaseLoader& _base_loader;  
  
  // =================
  //  Private Methods
  // =================
  //

};

// =====================
//  Inline Declarations
// =====================
//

#endif // BASELOADING_H
