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
 * @file event.h
 * @brief Header for the Event, AddEvent, SetEvent and RemoveEvent classes.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef EVENT_H
#define EVENT_H

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "macros.h" // ENSURE

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Abstract class representing simulation events.
 * @details Event is an abstract class used for user-defined events that happen
 * throughout simulation, such as adding/removing a given amount of a
 * target chemical at some point in time.
 */
class Event
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param time Time at which the event happens during simulation.
   * @param target FreeChemical impacted by the event.
   */
  Event (double time, FreeChemical& target)
    : _time (time)
    , _target (target)
  { REQUIRE (time >= 0); /** @pre time must be positive. */ }

  // Not needed for this class (use of compiler-generated versions).
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // Event (const Event& other_event);
  // /* @brief Assignment operator. */
  // Event& operator= (const Event& other_event);
  /** @brief Destructor. */
  virtual ~Event (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Perform event (implemented by children classes).
   */
  virtual void perform (void) = 0;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to event time.
   * @return Time at which event occurs.
   */
  double time (void)
  {
    /** @post Returned value is positive. */
    ENSURE (_time >= 0);
    return _time;
  }

 protected:
  // ===================
  //  Protected Methods
  // ===================
  //
  /** 
   * @brief Accessor to the target of event.
   * @return Reference to event target.
   */
  FreeChemical& target (void) { return _target; }

 private:  
  // ============
  //  Attributes
  // ============
  //
  /** @brief Time at which event occurs. */
  double _time;

  /** @brief Target impacted by event. */
  FreeChemical& _target;

  // =================
  //  Private Methods
  // =================
  //
};


/**
 * @brief Class representing events where chemicals are added.
 *
 * AddEvent is a class used for user-defined events that happen
 * throughout simulation, where a given amount of a target chemical
 * is added at some point in time. It inherits class Event.
 */
class AddEvent : public Event
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param time Time at which the event happens during simulation.
   * @param target FreeChemical impacted by the event.
   * @param quantity Amout of chemical to add.
   */
  AddEvent (double time, FreeChemical& target, int quantity)
    : Event (time, target)
    , _quantity (quantity)
  { REQUIRE (quantity > 0); /** @pre Quantity must be strictly positive. */ }


  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // AddEvent (const AddEvent& other_add_event);
  // /* @brief Assignment operator. */
  // AddEvent& operator= (const AddEvent& other_event);
  // /* @brief Destructor. */
  // ~AddEvent (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from Event
  /** Perform event (add specified amount of chemical). */
  void perform (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Quantity of chemical to add. */
  int _quantity;

  // =================
  //  Private Methods
  // =================
  //
};


/**
 * @brief Class representing events where amount of chemical is set.
 *
 * SetEvent is a class used for user-defined events that happen
 * throughout simulation, where a given amount of a target chemical
 * is set at some point in time. It inherits class Event.
 */
class SetEvent : public Event
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param time Time at which the event happens during simulation.
   * @param target FreeChemical impacted by the event.
   * @param quantity Amout of chemical to set.
   */
  SetEvent (double time, FreeChemical& target, int quantity)
    : Event (time, target)
    , _quantity (quantity)
  { REQUIRE (quantity >= 0); /** @pre Quantity must be positive. */ }

  // Not needed for this class (use of compiler-generated version)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // SetEvent (const SetEvent& other_set_event);
  // /* @brief Assignment operator. */
  // SetEvent& operator= (const SetEvent& other_set_event);
  // /* @brief Destructor. */
  // ~SetEvent (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from Event
  /** Perform event (set specified amount of chemical). */
  void perform (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Quantity of chemical to set. */
  int _quantity;

  // =================
  //  Private Methods
  // =================
  //
};

/**
 * @brief Class representing events where chemicals are removed.
 *
 * RemoveEvent is a class used for user-defined events that happen
 * throughout simulation, where a given amount of a target chemical
 * is removed at some point in time. It inherits class Event.
 */
class RemoveEvent : public Event
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param time Time at which the event happens during simulation.
   * @param target FreeChemical impacted by the event.
   * @param quantity Amout of chemical to remove.
   */
  RemoveEvent (double time, FreeChemical& target, int quantity)
    : Event (time, target), _quantity (quantity)
  { REQUIRE (quantity > 0); /** @pre Quantity must be strictly positive. */ }

  // Not needed for this class (use of compiler generated versions) !
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // RemoveEvent (const RemoveEvent& other_remove_event);
  // /* @brief Assignment operator. */
  // RemoveEvent& operator= (const RemoveEvent& other_remove_event);
  // /* @brief Destructor. */
  // ~RemoveEvent (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from Event
  /** Remove specified amount of chemical. */
  void perform (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief Quantity of chemical to remove. */
  int _quantity;
};

// ======================
//  Inline declarations
// ======================
//
#include "freechemical.h"

inline void AddEvent::perform (void)
{
  bool is_constant = target().is_constant();
  target().set_constant (false);
  target().add (_quantity);
  target().set_constant (is_constant);
}

inline void SetEvent::perform (void)
{
  bool is_constant = target().is_constant();
  target().set_constant (false);
  int difference = _quantity - target().number();
  if (difference > 0) { target().add (difference); }
  else if (difference < 0) { target().remove (-difference); }
  target().set_constant (is_constant);
}

inline void RemoveEvent::perform (void)
{
  bool is_constant = target().is_constant();
  target().set_constant (false);
  if (_quantity < target().number()) { target().remove (_quantity); }
  else { target().remove (target().number()); }
  target().set_constant (is_constant);
}


#endif // EVENT_H
