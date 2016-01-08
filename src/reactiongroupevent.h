

/**
 * @file reactiongroupevent.h
 * @brief Header for the ReactionGroupEvent class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef REACTION_GROUP_EVENT_H
#define REACTION_GROUP_EVENT_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

/**
 * @brief Class that storing next scheduled event type for a ReactionGroup.
 *
 * ReactionGroupEvent stores the time of next event and its type, namely either
 * performing a reaction or expiring (end of programmed time_step) and needing
 * to be updated.
 */
class ReactionGroupEvent
{
 public:

  /**
   * @brief List of possible events.
   */
  enum EventType { PERFORM_REACTION, UPDATE_GROUP };

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   * @param time Time of the next event.
   * @param group_index Index of the targetted reaction group (provided by user).
   * @param event_type ReactionGroupEvent::PERFORM_REACTION or ReactionGroupEvent::UPDATE_GROUP.
   */
  ReactionGroupEvent (double time, int group_index, EventType event_type)
    : _time (time)
    , _group_index (group_index)
    , _event_type (event_type)
  {}

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // ReactionGroupEvent ( const ReactionGroupEvent& other_reaction_event );

  /**
   * @brief Destructor
   */
  ~ReactionGroupEvent (void)
    {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to event time.
   * @return Time of event.
   */
  double time (void) const { return _time; }

  /**
   * @brief Accessor to group index.
   * @return Integer index to a reaction group as originally provided by the user.
   */
  int group_index (void) const { return _group_index; }

  /**
   * @brief Accessor to event type.
   * @return Event type: ReactionGroupEvent::PERFORM_REACTION or ReactionGroupEvent::UPDATE_GROUP.
   */
  double event_type (void) const { return _event_type; }

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
  //  * @brief Assignment operator
  //  */
  // ReactionGroupEvent& operator= ( const ReactionGroupEvent& other_reaction_event );


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Time of the next event. */
  double _time;
  
  /** @brief Index of the targetted reaction group (provided by user). */
  int _group_index;

  /** @brief ReactionGroupEvent::PERFORM_REACTION or ReactionGroupEvent::UPDATE_GROUP.*/
  EventType _event_type;

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

#endif // REACTION_GROUP_EVENT_H
