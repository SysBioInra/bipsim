

/**
 * @file volumehandler.h
 * @brief Header for the VolumeHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef VOLUME_HANDLER_H
#define VOLUME_HANDLER_H

// ==================
//  General Includes
// ==================
//
#include <list> // std::list

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

/**
 * @brief Class storing and handling simulation volumes.
 *
 * VolumeHandler stores user-defined volumes such as
 * adding/removing a chemical at a given time.
 */
class VolumeHandler
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   * @param initial_volume Initial volume.
   */
  VolumeHandler (double initial_volume);

  // Use of compiler generated versions
  /* /\* @brief Copy constructor. *\/ */
  /* VolumeHandler (const VolumeHandler& other_volume_handler); */
  /* /\* @brief Assignment operator. *\/ */
  /* VolumeHandler& operator= ( const VolumeHandler& other_volume_handler ); */
  /* /\* @brief Destructor. *\/ */
  /* ~VolumeHandler (void); */

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Prepare next volume change. 
   */
  void step (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to initial volume.
   * @return Volume at start of simulation.
   */
  double initial_value (void) const;

  /**
   * @brief Accessor to next volume value.
   * @return Next volume value.
   */
  double next_value (void) const;

  /**
   * @brief Accessor to next volume change time.
   * @return Time at which next volume change occurs. Returns
   *  INFINITY if the volume does not change.
   */
  double next_time (void) const;

  // ==================
  //  Public Constants
  // ==================
  //
  /**
   * @brief Highest possible double value.
   */
  static const double INFINITY;

private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief Initial volume value. */
  double _initial;

  /** @brief Next volume value. */
  double _next;

  /** @brief Next volume change time. */
  double _next_time;
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h" // REQUIRE

inline double VolumeHandler::initial_value (void) const
{
  return _initial;
}

inline double VolumeHandler::next_value (void) const
{
  /** @pre Next time must be strictly smaller than INFINITY. */
  REQUIRE (_next_time != INFINITY);
  return _next;
}

inline double VolumeHandler::next_time (void) const
{
  return _next_time;
}

#endif // EVENT_HANDLER_H
