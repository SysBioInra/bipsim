

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
#include <string> // std::string
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
Simulation::Simulation (const std::string& filename)
  : _params (filename)
{
  // read input files and create units, reactions and events
  InputData input_data (_params.input_files());
  Parser parser (_cell_state, _event_handler);
  parser.parse (input_data);
  input_data.write_warnings (std::cerr);

  // create solver
#ifdef CLASSIFICATION
  ReactionClassification classification;
  int class_id = classification.create_new_class (0.001);
  // int class_id = classification.create_new_class (ReactionClassification::ALWAYS_UPDATED);
  classification.add_reactions_to_class (class_id, _cell_state.reactions());
  _solver = new ManualDispatchSolver (_params.initial_time(),
				      _cell_state, classification);
#else
  _solver = new NaiveSolver (_params.initial_time(), _cell_state);
#endif

  // create logger
  std::list <std::string> chemical_names = _params.output_entities();
  std::list <const Chemical*> chemical_refs;
  std::list <std::string>::iterator name_it = chemical_names.begin();
  while (name_it != chemical_names.end())
    {
      const Chemical* next_chemical = _cell_state.find <Chemical> (*name_it);
      if (next_chemical != 0)
	{ chemical_refs.push_back (next_chemical); ++name_it; }
      else
	{
	  std::cerr << "WARNING: unknown chemical " << *name_it
		    << " will not be logged!\n";
	  name_it = chemical_names.erase (name_it);
	}
    }
  _logger = new ChemicalLogger (_params.output_file(),
				chemical_refs, chemical_names);

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
void Simulation::run (void)
{
  std::cout << "Solving from t = " << _params.initial_time()
	    << " to t = "<< _params.final_time() << "..." << std::endl;

  double next_event_time = _event_handler.next_event_time();
  while (next_event_time < _solver->time())
    {
      _event_handler.ignore_event();
      next_event_time = _event_handler.next_event_time();
    }

  // run until next event
  while (_params.final_time() > next_event_time)
    {
      while (_solver->time() < next_event_time)
	{
	  if ((_solver->number_reactions_performed() % _params.output_step()) == 0)
	    { _logger->log (_solver->time()); }
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
  while (_solver->time() < _params.final_time())
    {      
      if ((_solver->number_reactions_performed() % _params.output_step()) == 0)
	{ _logger->log (_solver->time()); }
      _solver->go_to_next_reaction();
    }

  std::cout << _solver->number_reactions_performed() << " reactions occurred."
	    << std::endl;
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



// =================
//  Private Methods
// =================
//
