

/**
 * @file ratecontainerfactory.h
 * @brief Header for the RateContainerFactory, RateVectorFactory,
 *  RateTreeFactory and HybridRateContainerFactory classes.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef RATE_CONTAINER_FACTORY_H
#define RATE_CONTAINER_FACTORY_H

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
 * @brief Abstract class for RateContainer factories.
 */
class RateContainerFactory
{  
 public:
  /** @brief Destructor (empty but virtual). */
  virtual ~RateContainerFactory (void) {};

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Create a RateContainer on heap (needs to be deleted by user).
   * @param params Simulation parameter.
   * @param number_rates Number of rates to store.
   * @return RateContainer whose implementation depends on the factory used.
   */
  virtual RateContainer* create (const SimulationParams& params,
				 int number_rates) const = 0;

  /**
   * @brief Standard output.
   * @return A reference to the stream containing the output.
   * @param output Stream where output should be written.
   * @param factory Reference to the factory whose name should be written to 
   *  output.
   */
  friend std::ostream& operator<< (std::ostream& output,
				   const RateContainerFactory& factory)
    { return factory.print(output); }

 private:
  /**
   * @brief Print name of class created by factory.
   * @return A reference to the stream containing the output.
   * @param output Stream where output should be written.
   */
  virtual std::ostream& print (std::ostream& output) const = 0;
};

/**
 * @brief Factory class creating RateVector.
 */
class RateVectorFactory : public RateContainerFactory
{
 public:
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  RateContainer* create (const SimulationParams& params, int number_rates) const;

 private:
  std::ostream& print (std::ostream& output) const
    { output << "RateVector"; return output; }
};

/**
 * @brief Factory class creating RateTree.
 */
class RateTreeFactory : public RateContainerFactory
{
 public:
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  RateContainer* create (const SimulationParams& params, int number_rates) const;

 private:
  std::ostream& print (std::ostream& output) const
    { output << "RateTree"; return output; }
};

/**
 * @brief Factory class creating HybridRateContainer.
 */
class HybridRateContainerFactory : public RateContainerFactory
{
 public:
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  RateContainer* create (const SimulationParams& params, int number_rates) const;

 private:
  std::ostream& print (std::ostream& output) const
    { output << "HybridRateContainer"; return output; }
};

// ======================
//  Inline declarations
// ======================
//
#include "ratevector.h"
#include "ratetree.h"
#include "hybridratecontainer.h"

inline RateContainer* 
RateVectorFactory::create (const SimulationParams& params, 
			   int number_rates) const
{
  return new RateVector (number_rates);
}

inline RateContainer* 
RateTreeFactory::create (const SimulationParams& params, int number_rates) const
{
  return new RateTree (number_rates);
}

inline RateContainer* 
HybridRateContainerFactory::create (const SimulationParams& params,
				    int number_rates) const
{
  return new HybridRateContainer (number_rates, params.hybrid_base_rate());
}


#endif // RATE_CONTAINER_FACTORY_H
