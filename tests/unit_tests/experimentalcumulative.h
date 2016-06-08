

#ifndef EXPERIMENTAL_CUMULATIVE
#define EXPERIMENTAL_CUMULATIVE

#include <set> // std::set
#include <list> // std::list
#include <iostream> // std::cout
#include <cmath> // fabs

/**
 * @brief Class that stores an experimental cumulative distribution.
 *
 * User simply provides all the values drawn and has access to jump values of the cumulative
 * and its value through () operator.
 */
template <typename T>
class ExperimentalCumulative
{
public:
  void add_pick (T value)
  {
    typename std::list<T>::iterator v_it = _drawn_values.begin();
    bool ok = false;
    while (ok == false)
      {
	if ((v_it != _drawn_values.end()) && (value >= *v_it))
	  {
	    ++v_it;
	  }
	else
	  {
	    ok = true;
	  }
      }
    _drawn_values.insert (v_it, value); 
  }

  double operator() (T x) const
  {
    int count = 0;
    typename std::list<T>::const_iterator v_it = _drawn_values.begin();
    bool ok = false;
    while (ok == false)
      {
	if ((x >= *v_it) && (v_it != _drawn_values.end()))
	  {
	    ++v_it;
	    ++count;
	  }
	else
	  {
	    ok = true;
	  }
      }
    return (static_cast<double> (count) / static_cast<double> (_drawn_values.size()));
  }

  std::set<T> jump_values (void) const
    {
      return std::set<T> (_drawn_values.begin(), _drawn_values.end());
    }

  friend std::ostream& operator<< (std::ostream& output, 
				   const ExperimentalCumulative <T>& ecf)
    {
      std::set <T> jumps = ecf.jump_values();
      for (typename std::set<T>::iterator jump_it = jumps.begin();
	   jump_it != jumps.end(); ++jump_it)
	{
	  output << *jump_it << " " << ecf(*jump_it) << "\n";
	}
    }
private:
  std::list<T> _drawn_values;
};

// Compute supremum distance from experimental cumulative to theoretical
// cumulative distribution for discrete distributions.
double distance_to_discrete_cumulative (ExperimentalCumulative<int>& exp_cum, double (*theor_cum)(int))
{
  // it suffices to compute the distance at each jump
  double max_distance = 0;
  double d = 0;

  std::set<int> jump_values = exp_cum.jump_values();
  for (std::set<int>::iterator k = jump_values.begin(); k != jump_values.end(); ++k)
    {
      // distance at jump
      d = fabs (exp_cum (*k) - (*theor_cum)(*k));
      if ( d > max_distance ) max_distance = d;
      // std::cout << exp_cum(*k) << " " << (*theor_cum)(*k) << " " << d << std::endl;
    }

  return max_distance;
}


// Compute supremum distance from experimental cumulative to theoretical
// cumulative distribution for continuous distributions.
double distance_to_continuous_cumulative (ExperimentalCumulative<double>& exp_cum, double (*theor_cum)(double))
{
  double max_distance = 0;
  double d = 0;

  // it suffices to compute the distance before and after each jump
  double prev = 0;  // the value of the experimental distribution before the first jump is 0
  std::set<double> jump_values = exp_cum.jump_values();
  for (std::set<double>::iterator x = jump_values.begin(); x != jump_values.end(); ++x)
    {
      double theoretical = (*theor_cum) (*x);

      // distance before jump
      d = fabs (prev - theoretical);
      if ( d > max_distance ) max_distance = d;
      // std::cout << prev << " " << theoretical << " " << d << " ";

      // distance after jump
      double after = exp_cum (*x);
      d = fabs (after - theoretical);
      if ( d > max_distance ) max_distance = d;
      // std::cout << after << " " << theoretical << " " << d << std::endl;

      // prepare value before jump for next iteration
      prev = after;
    }

  return max_distance;
}


#endif // EXPERIMENTAL_CUMULATIVE
