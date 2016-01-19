

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
#include <vector> // std::vector
#include <iostream> // std::ostream

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "observable.h"
#include "simulatorinput.h"

/**
 * @brief Abstract class that represent all reactions happening in the cell. 
 *
 * All reactions can be represented by this class, either because they are
 * irreversible or because they can be decomposed in a forward and a backward
 * reaction.
 * @sa BidirectionalReaction
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

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // Not needed for this class (use of default copy constructor) !
  // /* @brief Copy constructor. */
  // Reaction (Reaction& other_reaction);
  // /* @brief Assignment operator. */
  // Reaction& operator= (Reaction& other_reaction);
  /** @brief Destructor (empty but virtual). */
  virtual ~Reaction (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Update chemical quantities.
   */
  void perform (void);

  /**
   * @brief Update reaction rate.
   */
  void update_rate (void);


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns previously calculated rate.
   * @return Reaction rate value computed at last update.
   * @sa update_rate
   */
  double rate (void) const;

  /**
   * @brief Returns chemicals consumed by reaction.
   * @return Vector of chemicals consumed by reaction.
   */
  const std::vector<Reactant*>& reactants (void) const;

  /**
   * @brief Returns chemicals created by reaction.
   * @return Vector of chemicals created by reaction.
   */
  const std::vector<Reactant*>& products (void) const;

  /**
   * @brief Returns whether there are enough chemicals to perform reaction.
   * @return True if there are enough reactants, false otherwise.
   */
  virtual bool is_reaction_possible (void) const = 0;
  
  /**
   * @brief Standard output.
   * @return A reference to the stream containing the output.
   * @param output Stream where output should be written.
   * @param reaction Reference to the reaction whose information should be written.
   */
  friend std::ostream& operator<< (std::ostream& output, const Reaction& reaction);
  
 protected:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Rectants consumed by reaction. */
  std::vector <Reactant*> _reactants;

  /** @brief Products created by reaction. */
  std::vector <Reactant*> _products;

 private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Reaction rate value computed at last update. */
  double _rate;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const = 0;

  /**
   * @brief Update chemical quantities according to reaction.
   */
  virtual void do_reaction (void) = 0;

  /**
   * @brief Compute current reaction rate.
   * @return Rate according to current product concentrations.
   */
  virtual double compute_rate (void) const = 0;
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h" // ENSURE ()
#include "reactionobserver.h"

inline Reaction::Reaction (void)
  : _rate (0)
{
}

inline void Reaction::update_rate (void)
{
  _rate = compute_rate();
  /** @post Rate must be positive. */
  ENSURE (_rate >= 0);
}

inline double Reaction::rate (void) const
{
  return _rate;
}

inline const std::vector<Reactant*>& Reaction::reactants (void) const
{
  return _reactants;
}

inline const std::vector<Reactant*>& Reaction::products (void) const
{
  return _products;
}

inline void Reaction::perform (void)
{
  do_reaction();
  notify_change();
}

inline std::ostream& operator<< (std::ostream& output, const Reaction& reaction)
{
  reaction.print (output);
  return output;
}

#endif // REACTION_H
