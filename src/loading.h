
/**
 * @file loading.h
 * @brief Header for the Loading class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef LOADING_H
#define LOADING_H

// ==================
//  General Includes
// ==================
//
#include <string>

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "reaction.h"
#include "templatefilter.h"

// ===================
//  Class Declaration
// ===================
//
/**
 * @brief Abstract class for loading reactions.
 *
 * Loading represents reactions in which a chemical (a Loader) matches
 * up a chemical with the template it is currently bound to.
 * This class inherits class Reaction.
 */
class Loading : public Reaction
{
public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param loader BoundChemical performing loading operation.
   * @param table LoadingTable used by the loader.
   */
  Loading (BoundChemical& loader, const LoadingTable& table);

 private:
  /** @brief Copy constructor */
  Loading (Loading& other);
  /** @brief Assignment operator */
  Loading& operator= (Loading& other);
 public:
  
  /** @brief Destructor */
  virtual ~Loading (void) = 0;
  
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // Redefined from Reaction
  void handle_volume_change (double volume);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // Redefined from Reaction
  bool is_reaction_possible (void) const;

 protected:
  // ===================
  //  Protected Methods
  // ===================
  //
  /**
   * @brief Draw random unit according to loading rates.
   * @return BoundUnit selected.
   */
  BoundUnit& random_unit (void) const;

  /**
   * @brief Perform standard loading reaction for a specific unit.
   * @param unit Unit with which to perform loading reaction.
   */
  void load_chemical (BoundUnit& unit);

  // ============
  //  Attributes
  // ============
  //
  /** @brief Chemical loading an element corresponding to current template. */
  BoundChemical& _loader;    

 private:
  // =================
  //  Private Methods
  // =================
  //
  // redefined from Reaction
  void do_reaction (void);
  double compute_rate (void) const;
  void print (std::ostream& output) const;

  // ============
  //  Attributes
  // ============
  //
  /** @brief LoadingTable used by the loader. */
  const LoadingTable& _table;    

  /** @brief Filter organizing loader units according to templates. */
  TemplateFilter _template_filter;  

  /** @brief Volume constant. */
  double _volume_constant;
};

/**
 * @brief Class for loading reactions yielding products as free chemicals.
 *
 * ProductLoading inherits Loading. It is used for the case where the loaded
 * elements are assembled into a product that will be liberated in the cytosol.
 */
class ProductLoading : public Loading
{
 public:
  /**
   * @brief Constructor
   * @param loader BoundChemical performing loading operation.
   * @param table LoadingTable used by the loader.
   */
  ProductLoading (BoundChemical& loader, const LoadingTable& table)
    : Loading (loader, table)
    {}
};

/**
 * @brief Class for loading reactions sythesizing double strands.
 *
 * DoubleStrandLoading inherits Loading. It is used when the loaded elements
 * are assembled into a strand appariated to the strand read, forming a 
 * double strand.
 */
class DoubleStrandLoading: public Loading
{
 public:
  /**
   * @brief Constructor
   * @param loader BoundChemical performing loading operation.
   * @param table LoadingTable used by the loader.
   * @param stalled_form BoundChemical appearing if loading failed because
   *  it met a previously polymerized strand.
   */
  DoubleStrandLoading (BoundChemical& loader, const LoadingTable& table, 
		       BoundChemical& stalled_form);

 private:
  // redefined from Loading
  void do_reaction (void);

  /** @brief Form reached if loading failed. */
  BoundChemical& _stalled_form;
};

// =====================
//  Inline Declarations
// =====================
//
inline void Loading::handle_volume_change (double volume)
{
  _volume_constant = 1 / (volume * volume);
}

inline BoundUnit& Loading::random_unit (void) const
{
  return _template_filter.random_unit();
}



#endif // LOADING_H
