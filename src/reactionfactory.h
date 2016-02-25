

/**
 * @file reactionfactory.h
 * @brief Header for the ReactionFactory class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef REACTION_FACTORY_H
#define REACTION_FACTORY_H

// ==================
//  General Includes
// ==================
//


// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "factory.h"

/**
 * @brief Class used to create reactions used by the simulator.
 *
 * ReactionFactory automatically recognizes which reaction to create and 
 * stores it in a CellState object. It inherits abstract class Factory.
 */
class ReactionFactory : public Factory
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param cell_state CellState object used to store new objects.
   */
  ReactionFactory (CellState& cell_state);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // ReactionFactory (const ReactionFactory& other_factory);
  // Not needed for this class (use of default overloading) !
  // /* @brief Assignment operator. */
  // ReactionFactory& operator= (const ReactionFactory& other_factory);
  // /* @brief Destructor. */
  // ~ReactionFactory (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // Redefined from Factory.
  bool handle (const std::string& line);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Stream of current line. */
  std::istringstream _line_stream;

  // =================
  //  Private Methods
  // =================
  //
  /** @brief Create oading reaction from current line and store it. */
  void create_loading (void);

  /** @brief Create chemical reaction from current line and store it. */
  void create_chemical_reaction (void);

  /** @brief Create complexation from current line and store it. */
  void create_complexation (void);

  /** @brief Create translocation from current line and store it. */
  void create_translocation (void);

  /** @brief Create sequence binding from current line and store it. */
  void create_sequence_binding (void);

  /** @brief Create release from current line and store it. */
  void create_release (void);

  /** @brief Create release from current line and store it. */
  void create_degradation (void);
};

// ======================
//  Inline declarations
// ======================
//

#endif // REACTION_FACTORY_H
