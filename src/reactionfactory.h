

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

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // ReactionFactory (const ReactionFactory& other_reaction_factory);

  /**
   * @brief Destructor.
   */
  ~ReactionFactory (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Create reaction from line.
   * @param line Text line to create entity from.
   * @return True if an entity was successfully created.
   */
  bool handle (const std::string& line);

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
  // /*
  //  * @brief Assignment operator.
  //  */
  // ReactionFactory& operator= (const ReactionFactory& other_reaction_factory);

protected:
  // ======================
  //  Protected Attributes
  // ======================
  //

  // ===================
  //  Protected Methods
  // ===================
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
   * @brief Create base loading reaction from stream and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_base_loading (const std::string& line);

  /**
   * @brief Create chemical reaction from stream and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_chemical_reaction (const std::string& line);

  /**
   * @brief Create complexation from stream and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_complexation (const std::string& line);

  /**
   * @brief Create elongation from stream and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_elongation (const std::string& line);

  /**
   * @brief Create binding from stream and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_binding (const std::string& line);

  /**
   * @brief Create release from stream and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_release (const std::string& line);


  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//

#endif // REACTION_FACTORY_H
