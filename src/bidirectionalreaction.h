

/**
 * @file bidirectionalreaction.h
 * @brief Header for the BidirectionalReaction class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef BIDIRECTIONAL_REACTION_H
#define BIDIRECTIONAL_REACTION_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "simulatorinput.h"

/**
 * @brief Abstract class that represent all reactions that can happen in both
 *  directions. 
 *
 * This class serves as a storage convenience, its members should really be
 * accessed by the reaction subclasses that interface it as if it were
 * composed of two distinct unidirectional reactions. It is closer to input 
 * representation and takes less space than a decomposition in two reactions, 
 * but is less convenient to use for a solver, hence the reaction interfaces.
 * @sa Reaction
 */
class BidirectionalReaction : public SimulatorInput
{
public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  // Not needed for this class (use of compiler-generated version)
  // /* @brief Default constructor. */
  // BidirectionalReaction (void);
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // BidirectionalReaction (BidirectionalReaction& other_reaction);
  // /* @brief Assignment operator. */
  // BidirectionalReaction& operator= (BidirectionalReaction& other_reaction);
  /** @brief Destructor (empty but virtual). */
  virtual ~BidirectionalReaction (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Update chemical quantities according to the forward reaction.
   */
  virtual void perform_forward (void) = 0;

  /**
   * @brief Update chemical quantities according to the backward reaction.
   */
  virtual void perform_backward (void) = 0;

  /**
   * @brief Compute and return current forward reaction rate.
   * @return Current forward reaction rate.
   */
  double forward_rate (void) const;

  /**
   * @brief Compute and return current backward reaction rate.
   * @return Current backward reaction rate..
   */
  double backward_rate (void) const;

  /**
   * @brief Update computation of rate following a change in volume.
   */
  // virtual void handle_volume_change (double volume);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns chemicals taking part in the reaction.
   * @return Vector of chemicals taking part in the reaction.
   */
  const std::vector<Reactant*>& forward_reactants (void) const;

  /**
   * @brief Returns chemicals taking part in the reaction.
   * @return Vector of chemicals taking part in the reaction.
   */
  const std::vector<Reactant*>& backward_reactants (void) const;

  /**
   * @brief Returns whether there are enough chemicals to perform forward
   *  reaction.
   * @return True if there are enough reactant, false otherwise.
   */
  virtual bool is_forward_reaction_possible (void) const = 0;

  /**
   * @brief Returns whether there are enough chemicals to perform backward
   *  reaction.
   * @return True if there are enough reactant, false otherwise.
   */
  virtual bool is_backward_reaction_possible (void) const = 0;
  
  /**
   * @brief Standard output.
   * @return A reference to the stream containing the output.
   * @param output Stream where output should be written.
   * @param reaction Reference to the reaction whose information should be 
   *  written.
   */
  friend std::ostream& operator<< (std::ostream& output,
				   const BidirectionalReaction& reaction);

 protected:
  // ============
  //  Attributes
  // ============
  //  
  /** @brief Rectants taking part in the forward reaction. */
  std::vector <Reactant*> _forward_reactants;

  /** @brief Rectants taking part in the backward reaction. */
  std::vector <Reactant*> _backward_reactants;


 private:
  // ============
  //  Attributes
  // ============
  //

  // ===================
  //  Private Methods
  // ===================
  //
  /**
   * @brief Compute current forward rate.
   * @return Current forward rate.
   */
  virtual double compute_forward_rate (void) const = 0;

  /**
   * @brief Compute current backward rate.
   * @return Current backward rate.
   */
  virtual double compute_backward_rate (void) const = 0;

  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const = 0;
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h" // ENSURE()

inline double BidirectionalReaction::forward_rate ( void ) const
{
  /** @post Rate must be positive. */
  ENSURE (compute_forward_rate() >= 0);
  return compute_forward_rate();
}

inline double BidirectionalReaction::backward_rate ( void ) const
{
  /** @post Rate must be positive. */
  ENSURE (compute_backward_rate() >= 0);
  return compute_backward_rate();
}

inline const std::vector<Reactant*>&
BidirectionalReaction::forward_reactants ( void ) const
{
  return _forward_reactants;
}

inline const std::vector<Reactant*>& 
BidirectionalReaction::backward_reactants ( void ) const
{
  return _backward_reactants;
}

inline std::ostream& operator<< (std::ostream& output,
				 const BidirectionalReaction& reaction)
{
  reaction.print (output);
  return output;
}

#endif // BIDIRECTIONAL_REACTION_H
