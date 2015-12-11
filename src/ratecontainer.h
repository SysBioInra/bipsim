

/**
 * @file ratecontainer.h
 * @brief Header for the RateContainer class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef RATE_CONTAINER_H
#define RATE_CONTAINER_H

// ==================
//  General Includes
// ==================
//


// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "randomhandler.h"
#include "macros.h"

/**
 * @brief Abstract class for reaction rate containers.
 *
 * RateContainer is a base class for containers that store reaction rates.
 * Inheriting classes should define an implicit structure that represents
 * cumulated rates, enabling efficient random drawing of a reaction (or 
 * searching if a cumulated rate value was drawn by user). By default, this
 * structure should only be updated when prompted by user.
 */
class RateContainer
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  RateContainer (void) {}

  // Not needed for this rate (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // RateContainer (const RateContainer& other_rate_container);

  /**
   * @brief Destructor.
   */
  virtual ~RateContainer (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Update cumulated rates and total rate.
   */
  virtual void update_cumulates (void) = 0;

  /**
   * @brief Draw random reaction index (weighted by rates as of last update).
   * @return Reaction index obtained by multinomial drawing.
   */
  virtual int random_index (void) const = 0;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Total reaction rate as of last update.
   * @return Sum of reaction rates as of last update.
   */
  virtual double total_rate (void) const = 0;

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //
  /**
   * @brief Set reaction rate for a specific index.
   * @param index Index of the rate to modify.
   * @param value New reaction rate.
   */
  virtual void set_rate (int index, double value) = 0;


  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this rate (use of default overloading) !
  // /*
  //  * @brief Assignment operator.
  //  */
  // RateContainer& operator= (const RateContainer& other_rate_container);

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

  // =================
  //  Private Methods
  // =================
  //

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//

#endif // RATE_CONTAINER_H
