

/**
 * @file reaction.h
 * @brief Header for the Reaction class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef REACTION_H
#define REACTION_H

// ==================
//  General Includes
// ==================
//
#include <list> // std::list
#include <set> // std::set

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "observable.h"
#include "reactionobserver.h"
#include "simulatorinput.h"

/**
 * @brief Abstract class that represent all possible reactions. 
 *
 * All reactions that happen in the cell inherit this class. It can be used to
 * access reaction rates and perform reactions.
 */
class Reaction : public Observable<ReactionObserver>, public SimulatorInput
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  Reaction (void);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // Reaction (Reaction& other_reaction);

  /**
   * @brief Destructor
   */
  virtual ~Reaction ( void );

  // ===========================
  //  Public Methods - Commands
  // ===========================
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
   * @brief Update reaction rates.
   */
  virtual void update_rates (void) = 0;

  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const = 0;


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns previously calculated forward reaction rate.
   * @return Forward reaction rate value computed at last update.
   * @sa update_rates
   */
  double forward_rate ( void ) const;


  /**
   * @brief Returns previously calculates backward reaction rate.
   * @return Forward reaction rate value computed at last update.
   * @sa update_rates
   */
  double backward_rate ( void ) const;

  /**
   * @brief Returns chemicals taking part in the reaction.
   * @return List of chemicals taking part in the reaction.
   */
  const std::list<Reactant*>& reactants ( void ) const;

  /**
   * @brief Returns whether there are enough chemicals to perform forward reaction.
   * @return True if there are enough reactant, false otherwise.
   */
  virtual bool is_forward_reaction_possible (void) const = 0;

  /**
   * @brief Returns whether there are enough chemicals to perform backward reaction.
   * @return True if there are enough reactant, false otherwise.
   */
  virtual bool is_backward_reaction_possible (void) const = 0;


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
  // Reaction& operator= (Reaction& other_reaction);
  
  /**
   * @brief Standard output.
   * @return A reference to the stream containing the output.
   * @param output Stream where output should be written.
   * @param reaction Reference to the reaction whose information should be written.
   */
  friend std::ostream& operator<< (std::ostream& output, const Reaction& reaction);
  
  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant( void ) const;


protected:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Forward reaction rate value computed at last update. */
  double _forward_rate;

  /** @brief Backward reaction rate value computed at last update. */
  double _backward_rate;

  /** @brief Rectants taking part in the reaction. */
  std::list< Reactant* > _reactants;

  // ===================
  //  Protected Methods
  // ===================
  //
  /**
   * @brief Update chemical quantities according to the forward reaction.
   */
  virtual void do_forward_reaction (void) = 0;

  /**
   * @brief Update chemical quantities according to the backward reaction.
   */
  virtual void do_backward_reaction (void) = 0;
};

// ======================
//  Inline declarations
// ======================
//
inline double Reaction::forward_rate ( void ) const
{
  return _forward_rate;
}

inline double Reaction::backward_rate ( void ) const
{
  return _backward_rate;
}

inline const std::list<Reactant*>& Reaction::reactants ( void ) const
{
  return _reactants;
}

inline void Reaction::perform_forward (void)
{
  do_forward_reaction();
  notify_change();
}

inline void Reaction::perform_backward (void)
{
  do_backward_reaction();  
  notify_change();
}


#endif // REACTION_H
