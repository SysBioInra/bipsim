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
#include "macros.h"

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

  // ignore all events preceding simulation start
  while (_event_handler.next_event_time() < _params.initial_time())
    { _event_handler.ignore_event(); }

  // initialize run
  compute_next_timing();
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
  std::cout << "Solving from t = " << _solver->time()
	    << " to t = "<< _params.final_time() << "..." << std::endl;

  while (_next_timing < _params.final_time())
    {
      perform_next_reaction();
    }
  // write final logs
  write_logs (_params.final_time());

  std::cout << "\n" << _solver->number_reactions_performed()
	    << " reactions occurred." << std::endl;
}

void Simulation::perform_next_reaction (void)
{
  while ((_next != REACTION) && (_next_timing < _params.final_time()))
  {
    if (_next == EVENT)
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
  if (_next_timing < _params.final_time())
    {
      write_logs (_next_timing);
      _solver->perform_next_reaction();
      compute_next_timing();
    }
  else
    {
      write_logs (_params.final_time());
    }
}

void Simulation::skip (double time_)
{
  /** @pre time_ must be larger than current simulation time. */
  REQUIRE(time_ > time());
  if (time_ > _params.final_time())
    { time_ = _params.final_time(); }
  while (_event_handler.next_event_time() < time_)
    { _event_handler.perform_event(); }
  while (_next_volume_time < time_)
    { _next_volume_time += _params.volume_step(); }
  _cell_state.update_volume();
  _solver->reschedule (time_);
  compute_next_timing();
}

void Simulation::set_free_chemicals (
  const std::vector<std::string>& names, const std::vector<int>& values
)
{
  for (int i = 0; i < names.size(); ++i)
  {
    FreeChemical& chemical = _cell_state.free_chemical (names[i]);
    bool is_constant = chemical.is_constant();
    int diff = values[i] - chemical.number();
    chemical.set_constant (false);
    if (diff > 0)
      { chemical.add (diff); }
    else
      { chemical.remove (-diff); }
    chemical.set_constant (is_constant);
  }
  _solver->reschedule (time());
  compute_next_timing();
}

void Simulation::reset_ignored_variation (const std::string& name)
{
  FreeChemical& chemical = _cell_state.free_chemical (name);
  chemical.reset_ignored_variation();
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
  for (std::size_t i = 0; i < double_strands.size(); ++i)
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
  for (std::size_t i = 0; i < reactions.size(); ++i)
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
