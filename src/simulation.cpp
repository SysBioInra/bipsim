

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
#include "solverfactory.h"
#include "chemicallogger.h"
#include "doublestrandlogger.h"
#include "eventhandler.h"
#include "inputdata.h"
#include "parser.h"
#include "randomhandler.h"
#include "doublestrand.h"


// ==========================
//  Constructors/Destructors
// ==========================
//
Simulation::Simulation (const std::string& filename)
  : _params (filename)
  , _logger (0)
  , _replication_logger (0)
  , _solver (0)
{
  RandomHandler::instance().set_seed (_params.seed());

  // read input files and create units, reactions and events
  InputData input_data (_params.input_files());
  Parser parser (_cell_state, _event_handler);
  parser.parse (input_data);
  input_data.write_warnings (std::cerr);
  std::cout << "Loaded system containing "
	    << _cell_state.number_chemicals() << " chemicals and "
	    << _cell_state.reactions().size() << " reactions."
	    << std::endl;

  // create solver
  _solver = _params.solver_factory().create (_params, _cell_state);

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
  _logger = new ChemicalLogger (_params.concentration_file(),
				chemical_refs, chemical_names);
  const DoubleStrand*  double_strand = 
    _cell_state.find <DoubleStrand> (_params.output_double_strand());
  _replication_logger = new DoubleStrandLogger (_params.replication_file(),
						*double_strand);
}

// Forbidden
// Simulation::Simulation (const Simulation& other_simulation);
// Simulation& Simulation::operator= (const Simulation& other_simulation);

Simulation::~Simulation (void)
{
  delete _logger;
  delete _replication_logger;
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
	{ write_logs(); _solver->go_to_next_reaction(); }

      // perform event(s)
      while (next_event_time <= _solver->time())
	{
	  _event_handler.perform_event();
	  next_event_time = _event_handler.next_event_time();	  
	}
    }

  // no event left: run until final time
  while (_solver->time() < _params.final_time())
    { write_logs(); _solver->go_to_next_reaction(); }

  std::cout << _solver->number_reactions_performed() << " reactions occurred."
	    << std::endl;
}

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
void Simulation::write_logs (void)
{
  if ((_solver->number_reactions_performed() % _params.output_step()) == 0)
    { 
      _logger->log (_solver->time()); 
      if (_replication_logger != 0) 
	{ _replication_logger->log (_solver->time()); }
    }
}
