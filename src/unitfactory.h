

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

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // UnitFactory (const UnitFactory& other_unit_factory);
  // /* @brief Assignment operator. */
  // UnitFactory& operator= (const UnitFactory& other_unit_factory);
  // /* @brief Destructor. */
  // ~UnitFactory (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // Redefined from Factory
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
  /** @brief Create binding site from current line and store it. */
  void create_binding_site (void);

  /** @brief Create termination site from current line and store it. */
  void create_termination_site (void);

  /** @brief Create composition table from current line and store it. */
  void create_composition_table (void);

  /** @brief Create decoding table from current line and store it. */
  void create_decoding_table (void);

  /** @brief Create product table from current line and store it. */
  void create_product_table (void);

  /**  @brief Create transformation table from current line and store it. */
  void create_transformation_table (void);

  /** @brief Create free chemical from current line and store it. */
  void create_free_chemical (void);

  /** @brief Create bound chemical from current line and store it. */
  void create_bound_chemical (void);

  /** @brief Create loader from current line and store it. */
  void create_loader (void);

  /** @brief Create chemical sequence from current line and store it. */
  void create_chemical_sequence (void);

  /** @brief Create double strand sequence from current line and store it. */
  void create_double_strand_sequence (void);

  /** @brief Create processive chemical from current line and store it. */
  void create_processive_chemical (void);
};

// ======================
//  Inline declarations
// ======================
//

#endif // UNIT_FACTORY_H
