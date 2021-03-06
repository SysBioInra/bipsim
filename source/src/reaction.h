// 
// Copyright 2017 INRA
// Authors: M. Dinh, S. Fischer
// Last modification: 2017-09-19
// 
// 
// Licensed under the GNU General Public License.
// You should have received a copy of the GNU General Public License
// along with BiPSim.  If not, see <http://www.gnu.org/licenses/>.
// 



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
#include "simulatorinput.h"

/**
 * @brief Abstract class that represent all reactions happening in the cell. 
 *
 * All reactions can be represented by this class, either because they are
 * irreversible or because they can be decomposed in a forward and a backward
 * reaction.
 * @sa BidirectionalReaction
 */
class Reaction : public SimulatorInput
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

  /**
   * @brief Update computation of rate following a change in volume.
   * @param volume New volume value.
   */
  virtual void handle_volume_change (double volume) = 0;

  /**
   * @brief Set reaction name.
   * @param name New name of reaction.
   */
  void set_name (const std::string& name);

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
   * @brief Returns whether there are enough chemicals to perform reaction.
   * @return True if there are enough reactants, false otherwise.
   */
  virtual bool is_reaction_possible (void) const = 0;

  /**
   * @brief Accessor to number of times the reaction has been performed.
   * @return Number of times the reaction has been performed.
   */
  long long int number_performed (void) const;

  /**
   * @brief Accessor to name of reaction
   * @return Name of reaction (empty string by default).
   */
  const std::string& name (void) const;
  
  /**
   * @brief Standard output.
   * @return A reference to the stream containing the output.
   * @param output Stream where output should be written.
   * @param reaction Reference to the reaction whose information should be written.
   */
  friend std::ostream& operator<< (std::ostream& output, 
				   const Reaction& reaction);
  
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

  // ============
  //  Attributes
  // ============
  //
  /** @brief Reaction rate value computed at last update. */
  double _rate;
  /** @brief Number of times the reaction has been performed. */
  long long int _number_performed;
  /** @brief Name of reaction (optional). */
  std::string _name;
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h" // ENSURE ()

inline Reaction::Reaction (void)
  : _rate (0), _number_performed (0)
{
}

inline void Reaction::update_rate (void)
{
  _rate = compute_rate();
  /** @post Rate must be positive. */
  ENSURE (_rate >= 0);
}

inline void Reaction::set_name (const std::string& name)
{
  _name = name;
}

inline double Reaction::rate (void) const
{
  return _rate;
}

inline const std::vector<Reactant*>& Reaction::reactants (void) const
{
  return _reactants;
}

inline void Reaction::perform (void)
{
  ++_number_performed;
  do_reaction();
}

inline std::ostream& operator<< (std::ostream& output, const Reaction& reaction)
{
  reaction.print (output);
  return output;
}

inline long long int Reaction::number_performed (void) const
{
  return _number_performed;
}

inline const std::string& Reaction::name (void) const
{
  return _name;
}

#endif // REACTION_H
