
/**
 * @file reactionbuilder.h
 * @brief Header for the ReactionBuilder, ChemicalReactionBuilder, 
 *  ProductLoadingBuilder, DoubleStrandLoading, TranslocationBuilder, 
 *  SequenceBindingBuilder, ReleaseBuilder and DegradationBuilder classes.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef REACTION_BUILDER_H
#define REACTION_BUILDER_H

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "builder.h"

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Abstract class creating Reactions from text input.
 */
class ReactionBuilder : public Builder
{
 public:
  /**
   * @brief Constructor.
   * @param cell_state Object in which entities are stored and fetched from.
   */
  ReactionBuilder (CellState& cell_state);

  /**
   * @brief Store reaction in cell state and name it.
   * @param Reaction Reaction to store.
   * @param name Name of reaction.
   */
  void store_and_name (Reaction* reaction, const std::string& name)
  {
    store (reaction); reaction->set_name (name);
  }
};

/**
 * @brief Class creating ChemicalReaction from text input.
 */
class ChemicalReactionBuilder : public ReactionBuilder
{
 public:
  /**
   * @brief Constructor.
   * @param cell_state Object in which entities are stored and fetched from.
   */
  ChemicalReactionBuilder (CellState& cell_state);

  // redefined from Builder
  bool match (InputLine& text_input);

 private:
  /** @brief Affect chemicals to free, bound reactant or bound product. */
  void parse_chemicals (const std::vector <std::string>& names, 
			const std::vector <int>& stoichiometries);

  // format and values read
  Rule _format;
  std::vector <std::string> _chemical_names;
  std::vector <int> _stoichiometries_read, _stoichiometries, _orders;
  std::vector <FreeChemical*> _free_chemicals;
  double _k_1 , _k_m1;
  BoundChemical* _bound_reactant, * _bound_product;
};

/**
 * @brief Class creating ProductLoading from text input.
 */
class ProductLoadingBuilder : public ReactionBuilder
{
 public:
  /**
   * @brief Constructor.
   * @param cell_state Object in which entities are stored and fetched from.
   */
  ProductLoadingBuilder (CellState& cell_state);

  // redefined from Builder
  bool match (InputLine& text_input);

 private:
  // format and values read
  Rule _format;
  std::string _loader_name, _table_name;
};

/**
 * @brief Class creating DoubleStrandLoading from text input.
 */
class DoubleStrandLoadingBuilder : public ReactionBuilder
{
 public:
  /**
   * @brief Constructor.
   * @param cell_state Object in which entities are stored and fetched from.
   */
  DoubleStrandLoadingBuilder (CellState& cell_state);

  // redefined from Builder
  bool match (InputLine& text_input);

 private:
  // format and values read
  Rule _format;
  std::string _loader_name, _table_name, _stalled_name;
};

/**
 * @brief Class creating Translocation from text input.
 */
class TranslocationBuilder : public ReactionBuilder
{
 public:
  /**
   * @brief Constructor.
   * @param cell_state Object in which entities are stored and fetched from.
   */
  TranslocationBuilder (CellState& cell_state);

  // redefined from Builder
  bool match (InputLine& text_input);

 private:
  // format and values read
  Rule _format;
  std::string _processive_name, _step_name, _stalled_name;
  std::vector <std::string> _family_names;
  int _step_size;
  double _rate;
};

/**
 * @brief Class creating SequenceBinding from text input.
 */
class SequenceBindingBuilder : public ReactionBuilder
{
 public:
  /**
   * @brief Constructor.
   * @param cell_state Object in which entities are stored and fetched from.
   */
  SequenceBindingBuilder (CellState& cell_state);

  // redefined from Builder
  bool match (InputLine& text_input);

 private:
  // format and values read
  Rule _format;
  std::string _unit_to_bind, _bound_unit, _family;
};

/**
 * @brief Class creating Release from text input.
 */
class ReleaseBuilder : public ReactionBuilder
{
 public:
  /**
   * @brief Constructor.
   * @param cell_state Object in which entities are stored and fetched from.
   */
  ReleaseBuilder (CellState& cell_state);

  // redefined from Builder
  bool match (InputLine& text_input);

 private:
  // format and values read
  Rule _format;
  std::string _releasing_polymerase, _empty_polymerase, 
    _fail_polymerase, _product_table;
  double _rate;
};

/**
 * @brief Class creating Degradation from text input.
 */
class DegradationBuilder : public ReactionBuilder
{
 public:
  /**
   * @brief Constructor.
   * @param cell_state Object in which entities are stored and fetched from.
   */
  DegradationBuilder (CellState& cell_state);

  // redefined from Builder
  bool match (InputLine& text_input);

 private:
  // format and values read
  Rule _format;
  std::string _sequence_name, _table_name;
  double _rate;
};

/**
 * @brief Class creating DoubleStrandRecruitment from text input.
 */
class DoubleStrandRecruitmentBuilder : public ReactionBuilder
{
 public:
  /**
   * @brief Constructor.
   * @param cell_state Object in which entities are stored and fetched from.
   */
  DoubleStrandRecruitmentBuilder (CellState& cell_state);

  // redefined from Builder
  bool match (InputLine& text_input);

 private:
  // format and values read
  Rule _format;
  std::string _recruiter, _recruit, _bound_recruit;
  double _rate;
};

#endif // REACTION_BUILDER_H
