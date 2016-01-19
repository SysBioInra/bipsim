

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
 * ReactionFactory automatically recognizes which reaction to create and stores it
 * in a CellState object. It inherits abstract class Factory.
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
  /** @brief CellState object used to store new reactions. */
  CellState& _cell_state;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Create oading reaction from line and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_loading (const std::string& line);

  /**
   * @brief Create chemical reaction from line and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_chemical_reaction (const std::string& line);

  /**
   * @brief Create complexation from line and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_complexation (const std::string& line);

  /**
   * @brief Create translocation from line and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_translocation (const std::string& line);

  /**
   * @brief Create sequence binding from line and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_sequence_binding (const std::string& line);

  /**
   * @brief Create release from line and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_release (const std::string& line);

  /**
   * @brief Create release from line and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_degradation (const std::string& line);
};

// ======================
//  Inline declarations
// ======================
//

#endif // REACTION_FACTORY_H
