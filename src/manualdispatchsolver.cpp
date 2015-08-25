

/**
 * @file manualdispatchsolver.cpp
 * @brief Implementation of the ManualDispatchSolver class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream>

// ==================
//  Project Includes
// ==================
//
#include "manualdispatchsolver.h"
#include "reactionclassification.h"
#include "updatedrategroup.h"
#include "constantrategroup.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ManualDispatchSolver::ManualDispatchSolver (double initial_time, CellState& cell_state, const ReactionClassification& classification)
  : Solver (initial_time, cell_state)
{
  int number_groups = classification.number_classes();

  // create a group for every class in the reaction
  for (int i = 0; i < number_groups; ++i)
    {
      if (classification.time_step (i) == ReactionClassification::ALWAYS_UPDATED)
	{
	  // if the rates need to be alwas updated, we need to use UpdatedRateGroup
	  _updated_rate_group_indices.push_back (i);
	  _reaction_groups.push_back (new UpdatedRateGroup (classification.reactions (i), time()));
	}
      else
	{
	  // else we use ConstantRateGroup with the provided time step
	  _reaction_groups.push_back (new ConstantRateGroup (classification.reactions (i), time(), classification.time_step(i)));
	}
      // log next event of the newly created group in the event list
      add_event_for_group (i);
    }
}

// Not needed for this class (use of default copy constructor) !
// ManualDispatchSolver::ManualDispatchSolver ( const ManualDispatchSolver& other_manual_dispatch_solver );

ManualDispatchSolver::~ManualDispatchSolver (void)
{
  int number_groups = _reaction_groups.size();
  
  // delete all allocated reaction groups
  for (int i = 0; i < number_groups; ++i)
    {
      delete _reaction_groups [i];
    }
}

// ===========================
//  Public Methods - Commands
// ===========================
//


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
// ManualDispatchSolver& ManualDispatchSolver::operator= ( const ManualDispatchSolver& other_manual_dispatch_solver );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool ManualDispatchSolver::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//

double ManualDispatchSolver::compute_next_reaction (void)
{
  // we loop through event list until the next event is indeed a reaction
  while (_event_list.begin()->event_type() == ReactionGroupEvent::UPDATE_GROUP)
    {
      // no reaction: group needs to be reinitialized (so it is necessarily a ConstantReactionGroup)
      // we proceed to the update: we reinitialize the same group with a start time corresponding to the previous final time
      int group_index = _event_list.begin()->group_index();
      ConstantRateGroup& group = *static_cast<ConstantRateGroup*> (_reaction_groups [group_index]);
      group.reinitialize (group.final_time());

      // update event list
      _event_list.pop_front();
      add_event_for_group (group_index);
    }
  
  // next event is now a reaction to perform
  int group_index = _event_list.begin()->group_index();
  double reaction_time = _event_list.begin()->time();
  _reaction_groups [group_index]->perform_next_reaction();

  // update event_list
  _event_list.pop_front();
  add_event_for_group (group_index);

  // update events generated by variable rate groups
  update_variable_rate_events (group_index, reaction_time);

  return reaction_time;
}

void ManualDispatchSolver::add_event_for_group (int group_index)
{
  ReactionGroup& group = *_reaction_groups [group_index];
  
  // determine next event type and time
  double next_time;
  if (group.next_reaction_time() == ReactionGroup::OVERTIME)
    {
      // next event type = groups needs to be reinitialized because it is reaching its final time
      next_time = static_cast<ConstantRateGroup&> (group).final_time();
      // temporarily insert the event at the start of the list
      _event_list.push_front (ReactionGroupEvent (next_time, group_index, ReactionGroupEvent::UPDATE_GROUP));
    }
  else
    {
      // next event is a reaction
      next_time = group.next_reaction_time();
      // temporarily insert the event at the start of the list
      _event_list.push_front (ReactionGroupEvent (next_time, group_index, ReactionGroupEvent::PERFORM_REACTION));
    }

  // now move the event at its right position in the list (sorted by event time)
  std::list<ReactionGroupEvent>::iterator event = _event_list.begin();
  // first event is the element we just created so we ignore it
  ++event;
  // we look for an event that has a larger event time than the one just created
  while ((event != _event_list.end()) && (event->time() < next_time)) { ++event; }
  // move element from start to the right position
  _event_list.splice (event, _event_list, _event_list.begin());
}

void ManualDispatchSolver::update_variable_rate_events (int group_already_updated, double current_time)
{
  // we update the groups successively
  for (std::list<int>::iterator it_group_index = _updated_rate_group_indices.begin();
       it_group_index != _updated_rate_group_indices.end();
       ++it_group_index)
    {
      int group_index = *it_group_index;
      // ignore group if it has already updated (e.g. because reaction was perfom in that group)
      if (group_index != group_already_updated)
	{
	  // we remove the previous event from that group as it is now outdated
	  std::list<ReactionGroupEvent>::iterator event = _event_list.begin();
	  while (event->group_index() != group_index) { ++event; }

	  // update rates and next reaction for the group
	  static_cast<UpdatedRateGroup*> (_reaction_groups [group_index])->reschedule_next_reaction (current_time);

	  // add to event list
	  add_event_for_group (group_index);
	}
    }
}
