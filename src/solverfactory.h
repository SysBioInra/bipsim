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
 * @file solverfactory.h
 * @brief Header for the SolverFactory, NaiveSolverFactory and
 *  ManualDispatchSolver classes.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */



// Multiple include protection
//
#ifndef SOLVER_FACTORY_H
#define SOLVER_FACTORY_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//


// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Abstract class for Solver factories.
 */
class SolverFactory
{
 public:
  /** @brief Destructor (empty but virtual). */
  virtual ~SolverFactory (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Create a Solver on the heap (needs to be destroyed by user).
   * @param params Simulation parameters.
   * @param cell_state Reference to a cell state describing current state and
   *  reactions within the cell.
   * @return New Solver whose type is determined by the factory used.
   */
  virtual Solver* create (const SimulationParams& params,
			  CellState& cell_state) const = 0;  
};

/**
 * @brief Class used to create NaiveSolver.
 */
class NaiveSolverFactory : public SolverFactory
{
 public:
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  Solver* create (const SimulationParams& params, CellState& cell_state) const;
};

/**
 * @brief Class used to create ManualDispatchSolver.
 */
class ManualDispatchSolverFactory : public SolverFactory
{
 public:
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  Solver* create (const SimulationParams& params, CellState& cell_state) const;
};


// ======================
//  Inline declarations
// ======================
//
#include "naivesolver.h"
#include "manualdispatchsolver.h"
#include "cellstate.h"
#include "reactionclassification.h"

inline Solver*
NaiveSolverFactory::create (const SimulationParams& params,
			    CellState& cell_state) const
{
  return new NaiveSolver (params, cell_state.reactions());
}

inline Solver*
ManualDispatchSolverFactory::create (const SimulationParams& params,
				     CellState& cell_state) const
{
  ReactionClassification classification;
  int class_id = classification.create_new_class (0.001);
  classification.add_reactions_to_class (class_id, cell_state.reactions());
  return new ManualDispatchSolver (params, classification);
}

#endif // SOLVER_FACTORY_H
