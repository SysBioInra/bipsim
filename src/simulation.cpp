

/**
 * @file simulation.cpp
 * @brief Implementation of the Simulation class.
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
#include "simulation.h"
#include "cellstate.h"
#include "chemical.h"
#include "naivesolver.h"
#include "manualdispatchsolver.h"
#include "reactionclassification.h"
#include "chemicallogger.h"
#include "eventhandler.h"
#include "inputdata.h"
#include "parser.h"

// #define CLASSIFICATION

// ==========================
//  Constructors/Destructors
// ==========================
//
Simulation::Simulation (const char* filename)
 : _t (0)
{
  // read input files and create units, reactions and events
  std::list<std::string> input_files;
  input_files.push_back ("../input/translation.in");
  input_files.push_back ("../input/events.in");
  InputData input_data (input_files);
  Parser parser (_cell_state, _event_handler);
  parser.parse (input_data);
  input_data.write_warnings (std::cerr);

  // create solver
#ifdef CLASSIFICATION
  ReactionClassification classification;
  int class_id = classification.create_new_class (0.001);
  // int class_id = classification.create_new_class (ReactionClassification::ALWAYS_UPDATED);
  classification.add_reactions_to_class (class_id, _cell_state.reactions());
  _solver = new ManualDispatchSolver (_t, _cell_state, classification);
#else
  _solver = new NaiveSolver (_t, _cell_state);
#endif

  // create logger
  std::list <std::string> chemical_names;
  chemical_names.push_back ("protein");
  chemical_names.push_back ("GTP");
  std::list <const Chemical*> chemical_refs;
  for (std::list <std::string>::iterator name_it = chemical_names.begin(); name_it != chemical_names.end(); ++name_it)
    {
      chemical_refs.push_back (_cell_state.find <Chemical> (*name_it));
    }
  _logger = new ChemicalLogger ("../output/translation.txt", chemical_refs, chemical_names);

}

// Not needed for this class (use of default copy constructor) !
// Simulation::Simulation ( const Simulation& other_simulation );

Simulation::~Simulation (void)
{
  delete _logger;
  delete _solver;
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void Simulation::run (double time_interval)
{
  int log_step = 10000;

  double final_time = _solver->time() + time_interval;
  std::cout << "Solving from t = " << _t << " to t = " << final_time << "..." << std::endl;

  double next_event_time = _event_handler.next_event_time();
  while (next_event_time < _solver->time())
    {
      _event_handler.ignore_event();
      next_event_time = _event_handler.next_event_time();
    }

  // run until next event
  while (final_time > next_event_time)
    {
      while (_solver->time() < next_event_time)
	{
	  if ((_solver->number_reactions_performed() % log_step) == 0) { _logger->log (_solver->time()); }
	  _solver->go_to_next_reaction(); 
	}
      // perform event(s)
      while (next_event_time <= _solver->time())
	{
	  _event_handler.perform_event();
	  next_event_time = _event_handler.next_event_time();	  
	}
    }
  // no event left: run until final time
  while (_solver->time() < final_time)
    {      
      if ((_solver->number_reactions_performed() % log_step) == 0) { _logger->log (_solver->time()); }
      _solver->go_to_next_reaction();
    }

  std::cout << _solver->number_reactions_performed() << " reactions occurred." << std::endl;
}

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
// Simulation& Simulation::operator= ( const Simulation& other_simulation );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool Simulation::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
