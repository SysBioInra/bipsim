
/**
 * @file interpreter.h
 * @brief Header for the Interpreter, Rule, TagToken, BaseToken class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef INTERPRETER_H
#define INTERPRETER_H

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "inputline.h"

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Abstract class for interpreter used to parse InputLine.
 */
class Interpreter
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /** @brief Destructor. */
  virtual ~Interpreter (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Check whether input matches Interpreter format.
   * @param input InputLine to parse. If input matches format, the method 
   *  processes past all recognized tokens, else it is returned in its initial
   *  state.
   * @return Returns `true` if input matched internal format, `false` otherwise.
   */
  virtual bool match (InputLine& input) = 0;
  
  /**
   * @brief Create a rule from existing Interpreter by concatening formats.
   * @param other Interpreter whose format will be added to current object 
   *  format.
   * @return Rule object which matches inputs which are a concatenation
   *  of current's object format and `other`'s object format.
   */
  Rule operator+ (const Interpreter& other);

  /**
   * @brief Create a copy of current object on the heap.
   * @return Pointer to copy of current object. Memory must be freed by user.
   */
  virtual Interpreter* clone (void) const = 0;
};

/**
 * @brief Class matching inputs defined as a concatenation of two predefined 
 *  formats.
 */
class Rule : public Interpreter
{
 public:
  /**
   * @brief Constructor from concatenation of two existing formats.
   * @param first Interpreter used to define beginning of format.
   * @param second Interpreter used to define second part of format.
   */
  Rule (const Interpreter& first, const Interpreter& second)
    : _first (first.clone())
    , _second (second.clone())
    {}
  
  /**
   * @brief Copy constructor.
   * @param other Rule to copy.
   */
  Rule (const Rule& other)
    : _first (other._first->clone())
    , _second (other._second->clone())
    {}

  /**
   * @brief Assignment operator.
   * @param other Rule to copy.
   */
  Rule& operator= (const Rule& other) 
    {
      delete _first; _first = other._first->clone();
      delete _second; _second = other._second->clone();
      return *this;
    }
  
  /** @brief Destructor. */
  ~Rule (void) { delete _first; delete _second; }

  // redefined from Interpreter
  bool match (InputLine& input)
  {
    int mark = input.mark();
    bool success = _first->match (input) && _second->match (input);
    if (!success) { input.go_to (mark); return false; }
    else { return true; }
  }

  // redefined from Interpreter
  Interpreter* clone (void) const { return new Rule (*_first, *_second); }
  
 private:
  /** @brief First part of format. */
  Interpreter* _first;
  /** @brief Second part of format. */
  Interpreter* _second;
};

/**
 * @brief Class recognizing tokens matching a predefined string.
 */ 
class TagToken : public Interpreter
{
 public:
  /**
   * @brief Constructor.
   * @param tag `std::string` that the input token must match.
   */
  TagToken (const std::string& tag) : _tag (tag) {}

  // redefined from Interpreter
  bool match (InputLine& input)
  {
    if (input.end_of_line()) { return false; }
    if (input.word() == _tag) { input.step(); return true; }
    else { return false; }
  }

  // redefined from Interpreter
  Interpreter* clone (void) const { return new TagToken (*this); }

 private:
  /** @brief Tag that input token must match. */
  std::string _tag;
};

/**
 * @brief Class recognizing and storing tokens of given type.
 * @tparam T Type that input tokens should match.
 */
template <class T>
class BaseToken : public Interpreter
{
 public:
  /**
   * @brief Constructor.
   * @param data Variable where data read should be stored if it matches 
   *  predefined type.
   */
  BaseToken (T& data) : _data (data) {}

  // redefined from Interpreter
  bool match (InputLine& input)
  {
    if (input.end_of_line()) { return false; }
    std::istringstream word_stream (input.word());
    if (word_stream >> _data) { input.step(); return true; }
    else { return false; }
  }
  
  // redefined from Interpreter
  Interpreter* clone (void) const { return new BaseToken (*this); }

 private:
  /** @brief Variable where valid data read should be stored. */
  T& _data;
};

/** @brief Token recognizing and storing `int`. */
typedef BaseToken <int> IntToken;
/** @brief Token recognizing and storing `double`. */
typedef BaseToken <double> DblToken;
/** @brief Token recognizing and storing `std::string`. */
typedef BaseToken <std::string> StrToken;

/**
 * @brief Class recognizing and accumulating tokens of given type.
 * @tparam T Type that input tokens should match.
 */
template <class T>
class MemToken : public Interpreter
{
 public:
  /**
   * @brief Constructor.
   * @param data Vector where data is stored if it matches 
   *  predefined type.
   */
  MemToken (std::vector<T>& data) : _data (data) {}

  // redefined from Interpreter
  bool match (InputLine& input)
  {
    if (input.end_of_line()) { return false; }
    T value;
    std::istringstream word_stream (input.word());
    if (word_stream >> value) 
      { _data.push_back (value); input.step(); return true; }
    else { return false; }
  }
  
  // redefined from Interpreter
  Interpreter* clone (void) const { return new MemToken (*this); }

 private:
  /** @brief Vector where valid data read should be stored. */
  std::vector <T>& _data;
};

/**
 * @brief Class parsing a rule iteratively until rule cannot be matched or
 *  specified end rule is matched.
 */
class Iteration : public Interpreter
{
 public:
  /**
   * @brief Constructor from existing rule.
   * @param rule Interpreter containing rule to iterate as long as possible.
   */
  Iteration (const Interpreter& rule)
    : _rule (rule.clone())
    , _end (0)
    {}
  
  /**
   * @brief Constructor from existing rule.
   * @param rule Interpreter containing rule to iterate as long as possible
   *  or until end_rule is matched.
   * @param end_rule Interpreter containing rule that marks the end of 
   *  iteration.
   */
  Iteration (const Interpreter& rule, const Interpreter& end_rule)
    : _rule (rule.clone())
    , _end (end_rule.clone())
    {}
  
  /**
   * @brief Copy constructor.
   * @param other Iteration to copy.
   */
  Iteration (const Iteration& other)
    : _rule (other._rule->clone())
    , _end (0)
    { if (other._end) { _end = other._end->clone(); } }
      
  /**
   * @brief Assignment operator.
   * @param other Iteration to copy.
   */
  Iteration& operator= (const Iteration& other) 
    {
      delete _rule; _rule = other._rule->clone();
      if (other._end) { delete _end; _end = other._end->clone(); }
      return *this;
    }
  
  /** @brief Destructor. */
  ~Iteration (void) { delete _rule; delete _end; }

  // redefined from Interpreter
  bool match (InputLine& input)
  {
    if (_end == 0) { while (_rule->match (input)) {} }
    else { while ((!_end->match (input)) && (_rule->match (input))) {} }
    return true;
  }
  
  // redefined from Interpreter
  Interpreter* clone (void) const { return new Iteration (*this); }
  
 private:
  /** @brief Rule to iterate. */
  Interpreter* _rule;

  /** @brief Rule marking end of iteration */
  Interpreter* _end;
};

// ======================
//  Inline declarations
// ======================
//
inline Rule Interpreter::operator+ (const Interpreter& other)
{
  return Rule (*this, other);
}

#endif // INTERPRETER_H
