
/**
 * @file simulation.cpp
 * @brief Implementation of the Simulation class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//
#include <string> // std::string
#include <vector> // std::vector
#include <iostream> // std::cout
#include <stdexcept> // std::runtime_error

// ==================
//  Project Includes
// ==================
//
#include "simulation.h"
#include "cellstate.h"
#include "chemical.h"
#include "solverfactory.h"
#include "chemicallogger.h"
#include "reactionlogger.h"
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
  , _solver (0)
  , _next_log_time (0)
  , _next_timing (0)
  , _next (EVENT)
{
  RandomHandler::instance().set_seed (_params.seed());

  // read input files and create units, reactions and events
  InputData input_data (_params.input_files());
  Parser parser (_cell_state, _event_handler);
  parser.parse (input_data);
  std::cout << "Loaded system containing "
	    << _cell_state.number_chemicals() << " chemicals and "
	    << _cell_state.reactions().size() << " reactions."
	    << std::endl;

  // set volume parameters
  _cell_state.set_volume_parameters (_params.base_volume(),
				     _params.volume_modifiers(),
				     _params.volume_weights());
  _next_volume_time = _params.initial_time() + _params.volume_step();

  // create solver
  _solver = _params.solver_factory().create (_params, _cell_state);

  // set next log time and create loggers
  _next_log_time = _params.initial_time();
  create_loggers();
}

// Forbidden
// Simulation::Simulation (const Simulation& other_simulation);
// Simulation& Simulation::operator= (const Simulation& other_simulation);

Simulation::~Simulation (void)
{
  for (std::list<Logger*>::iterator it = _loggers.begin();
       it != _loggers.end(); ++it)
    { delete *it; }
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

  // ignore all events preceding simulation start
  while (_event_handler.next_event_time() < _params.initial_time())
    { _event_handler.ignore_event(); }

  compute_next_timing();
  while (_next_timing < _params.final_time())
    {
      if (_next == REACTION)
	{ 
	  write_logs (_next_timing);
	  _solver->perform_next_reaction(); 
	}
      else if (_next == EVENT)
	{ 
	  write_logs (_next_timing);
	  _event_handler.perform_event(); 
	  _solver->reschedule (_next_timing);
	}
      else if (_next == VOLUME)
	{
	  if (_cell_state.update_volume())
	    { _solver->reschedule (_next_timing); }
	  _next_volume_time += _params.volume_step();
	}
      compute_next_timing();
    }
  // write final logs
  if (_next_log_time <= _params.final_time()) { write_logs (_next_log_time); }

  std::cout << "\n" << _solver->number_reactions_performed() 
	    << " reactions occurred." << std::endl;
}

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
void Simulation::create_loggers (void)
{
  // create reactant and doublestrand loggers
  std::vector <const Chemical*> chemical_refs;
  std::vector <const DoubleStrand*> double_strands;
  std::vector <std::string> ds_names;
  bool dependency_errors = false;
  for (std::vector <std::string>::const_iterator 
	 it = _params.output_entities().begin();
       it != _params.output_entities().end(); ++it)
    {
      try
	{
	  const Chemical* next = &_cell_state.fetch <Chemical> (*it);
	  chemical_refs.push_back (next); 
	  const DoubleStrand* next_ds = 
	    dynamic_cast <const DoubleStrand*> (next);
	  if (next_ds != 0) 
	    { 
	      double_strands.push_back (next_ds); 
	      ds_names.push_back (*it);
	    }
	}
      catch (const DependencyException& error)
	{
	  dependency_errors = true;
	  std::cerr << "DEPENDENCY ERROR (parameter file, chemicals to log): " 
		    << error.what() << "." << std::endl;
  	}
    }
  if (dependency_errors) 
    { throw std::runtime_error ("unresolved dependencies"); }
  if (chemical_refs.size() > 0)
    {
      _loggers.push_back 
	(new ChemicalLogger (_params.concentration_file(),
			     chemical_refs, _params.output_entities()));
    }
  for (int i = 0; i < double_strands.size(); ++i)
    {
      _loggers.push_back 
	(new DoubleStrandLogger (_params.output_dir() 
				 + "/" + ds_names[i] + ".out",
				 *(double_strands[i])));
    }
  
  // create reaction logger
  // convert reactions to unmodifiable reactions
  // (logger is only supposed to access reactions, nothing more)
  std::vector <const Reaction*> reactions (_cell_state.reactions().size());
  for (int i = 0; i < reactions.size(); ++i)
    { reactions[i] = _cell_state.reactions()[i]; }
  _loggers.push_back (new ReactionLogger (_params.reaction_file(), reactions));
}

void Simulation::compute_next_timing (void)
{
  _next_timing = _event_handler.next_event_time();
  _next = EVENT;
  if (_next_volume_time < _next_timing)
    {
      _next_timing = _next_volume_time;
      _next = VOLUME;
    }
  if (_solver->next_reaction_time() < _next_timing)
    {
      _next_timing = _solver->next_reaction_time();
      _next = REACTION;
    }
}

void Simulation::write_logs (double t)
{
  while (t >= _next_log_time)
    { 
      for (std::list <Logger*>::iterator it = _loggers.begin();
	   it != _loggers.end(); ++it)
	{ (*it)->log (_next_log_time); }
      _next_log_time += _params.output_step();
    }
}
