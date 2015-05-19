
/**
 * @file binding.h
 * @brief Header for the Binding class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef BINDING_H
#define BINDING_H

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
 * @brief Class for representing binding reactions.
 *
 * The Binding class can represent the binding of chemical elements onto
 * binding sites. A binding reaction is represented by an element to bind, the
 * same element in its bound form and the type of binding sites it can bind
 * onto.
 * This class inherits class Reaction.
 * @sa Reaction
 */
class Binding: public Reaction
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  Binding (void);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // Binding (Binding& other_binding);

  /**
   * @brief Destructor
   */
  virtual ~Binding (void);
  
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  //
  /**
   * @brief Update chemical quantities according to the forward reaction.
   */
  void perform_forward( void );

  /**
   * @brief Update chemical quantities according to the backward reaction.
   */
  void perform_backward( void );


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns the forward reaction rate.
   * @return The forward reaction rate.
   */
  double forward_rate( void );


  /**
   * @brief Returns the backward reaction rate.
   * @return The backward reaction rate.
   */
  double backward_rate( void );


  // ==========================
  //  Public Methods - Setters
  // ==========================
  //
  /**
   * @brief Defines the chemical that binds to a specific binding site.
   * @param unit_to_bind
   *  Chemical element that can potentially bind to a binding site.
   */
  void set_unit_to_bind (Chemical& unit_to_bind);

  /**
   * @brief Defines the chemical in its bound form.
   * @param binding_result
   *  A bound chemical element that corresponds to the original chemical in its
   *  bound form.
   */
  void set_binding_result (BoundChemical& binding_result);

  /**
   * @brief Defines the type of binding sites that the chemical is able to
   *  recognize.
   * @param  binding_site_type
   *  The type (not a specific instance) of binding sites the chemical can bind
   *  onto.
   * @sa BindingSite
   */
  void set_binding_site_family ( int binding_site_family );

  /**
   * @brief Sets binding site handler used by binding reactions.
   * @param  binding_site_handler
   *  A binding site handler unique to all binding instances, used for 
   * communication with binding sites.
   */
  static void set_binding_site_handler ( BindingSiteHandler& binding_site_handler);

  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /**
  //  * @brief Assignment operator
  //  */
  // Binding& operator= (Binding& other_binding);

  
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
  /** @brief Chemical element that can potentially bind to a binding site. */
  Chemical *_unit_to_bind;
  
  /**
   * @brief A bound chemical element that corresponds to the original chemical
   * in its bound form.
   */
  BoundChemical *_binding_result;
  
  /**
   * @brief The type (not a specific instance) of binding sites the chemical can
   *  bind onto.
   * @sa BindingSite
   */
  int _binding_site_family;

  /**
   * @brief Binding site handler used for communication with binding sites.
   *  Unique to all binding instances.
   */
  static BindingSiteHandler *_binding_site_handler;
  
  // =================
  //  Private Methods
  // =================
  //

};

// =====================
//  Inline Declarations
// =====================
//
inline void Binding::set_unit_to_bind ( Chemical& unit_to_bind )
{ _unit_to_bind = &unit_to_bind; }

inline void Binding::set_binding_result ( BoundChemical& binding_result )
{ _binding_result = &binding_result; }

inline void Binding::set_binding_site_family ( int binding_site_family )
{ _binding_site_family = binding_site_family; }

inline void Binding::set_binding_site_handler ( BindingSiteHandler& binding_site_handler )
{ _binding_site_handler = &binding_site_handler; }

#endif // BINDING_H
