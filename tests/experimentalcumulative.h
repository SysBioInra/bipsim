

#ifndef EXPERIMENTAL_CUMULATIVE
#define EXPERIMENTAL_CUMULATIVE

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

  double operator() (T x)
  {
    int count = 0;
    typename std::list<T>::iterator v_it = _drawn_values.begin();
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

  std::set<T> jump_values (void)
  {
    return std::set<T> (_drawn_values.begin(), _drawn_values.end());
  }
private:
  std::list<T> _drawn_values;
};

#endif // EXPERIMENTAL_CUMULATIVE
