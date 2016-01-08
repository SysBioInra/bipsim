

/**
 * @file ratemanagerfactory.h
 * @brief Header for the RateManagerFactory, NaiveRateManagerFactory,
 *  DependencyRateManagerFactory and GraphRateManagerFactory classes.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef RATE_MANAGER_FACTORY_H
#define RATE_MANAGER_FACTORY_H

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
 * @brief Abstract class for RateManager factories.
 */
class RateManagerFactory
{
 public:
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Create a RateManager on heap (needs to be deleted by user).
   * @param params Simulation parameter.
   * @param reactions Vector of reactions that need to be handled by the rate 
   *  manager.
   * @param dependencygraph DependencyGraph used to determine dependencies
   *  between relations.
   * @return RateManager whose implementation depends on the factory used.
   */
  virtual RateManager* create (const SimulationParams& params,
			       const std::vector <Reaction*>& reactions,
			       const DependencyGraph& dependency_graph) const = 0;
};

/**
 * @brief Factory class creating NaiveRateManager.
 */
class NaiveRateManagerFactory : public RateManagerFactory
{
 public:
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  RateManager* create (const SimulationParams& params,
		       const std::vector <Reaction*>& reactions,
		       const DependencyGraph& dependency_graph) const;
};

/**
 * @brief Factory class creating DependencyRateManager.
 */
class DependencyRateManagerFactory : public RateManagerFactory
{
 public:
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  RateManager* create (const SimulationParams& params,
		       const std::vector <Reaction*>& reactions,
		       const DependencyGraph& dependency_graph) const;
};

/**
 * @brief Factory class creating GraphRateManager.
 */
class GraphRateManagerFactory : public RateManagerFactory
{
 public:
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  RateManager* create (const SimulationParams& params,
		       const std::vector <Reaction*>& reactions,
		       const DependencyGraph& dependency_graph) const;
};

// ======================
//  Inline declarations
// ======================
//
#include "naiveratemanager.h"
#include "dependencyratemanager.h"
#include "graphratemanager.h"

inline RateManager* 
NaiveRateManagerFactory::create (const SimulationParams& params,
				 const std::vector <Reaction*>& reactions,
				 const DependencyGraph& dependency_graph) const
{
  return new NaiveRateManager (params, reactions);
}

inline RateManager* 
DependencyRateManagerFactory::create (const SimulationParams& params,
				      const std::vector <Reaction*>& reactions,
				      const DependencyGraph& dependency_graph) const
{
  return new DependencyRateManager (params, reactions);
}

inline RateManager* 
GraphRateManagerFactory::create (const SimulationParams& params,
				 const std::vector <Reaction*>& reactions,
				 const DependencyGraph& dependency_graph) const
{
  return new GraphRateManager (params, reactions, dependency_graph);
}


#endif // RATE_MANAGER_FACTORY_H
