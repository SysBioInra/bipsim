

/**
 * @file unitfactory.h
 * @brief Header for the UnitFactory class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef UNIT_FACTORY_H
#define UNIT_FACTORY_H

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
 * @brief Class used to create units used by the simulator.
 *
 * UnitFactory automatically recognizes which unit to create and stores it
 * in a CellState object. It inherits abstract class Factory.
 */
class UnitFactory : public Factory
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
  UnitFactory (CellState& cell_state);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // UnitFactory (const UnitFactory& other_unit_factory);

  /**
   * @brief Destructor.
   */
  ~UnitFactory (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Create unit from line.
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
  // UnitFactory& operator= (const UnitFactory& other_unit_factory);

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
  /** @brief CellState object used to store new units. */
  CellState& _cell_state;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Create binding site from stream and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_binding_site (const std::string& line);

  /**
   * @brief Create termination site from stream and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_termination_site (const std::string& line);

  /**
   * @brief Create composition table from stream and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_composition_table (const std::string& line);

  /**
   * @brief Create decoding table from stream and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_decoding_table (const std::string& line);

  /**
   * @brief Create product table from stream and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_product_table (const std::string& line);

  /**
   * @brief Create transformation table from stream and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_transformation_table (const std::string& line);

  /**
   * @brief Create chemical from stream and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_chemical (const std::string& line);

  /**
   * @brief Create bound chemical from stream and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_bound_chemical (const std::string& line);

  /**
   * @brief Create base loader from stream and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_base_loader (const std::string& line);

  /**
   * @brief Create chemical sequence from stream and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_chemical_sequence (const std::string& line);

  /**
   * @brief Create processive chemical from stream and store it in cell state.
   * @param line Line to read creation information from.
   * @return True if entity was successfully created.
   */
  bool create_processive_chemical (const std::string& line);

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//

#endif // UNIT_FACTORY_H
