
#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <list>

template <typename T>
inline
bool compare_lists (const std::list <T>& list1, const std::list <T>& list2)
{
  std::list <T> my_list2 (list2);
  if (list1.size() != list2.size()) return false;
  typename std::list<T>::const_iterator list1_it = list1.begin();
  while (list1_it != list1.end())
    {
      typename std::list<T>::iterator list2_it = my_list2.begin();
      while ((list2_it != my_list2.end()) 
	     && (*list1_it != *list2_it)) { ++list2_it; }
      if (list2_it == my_list2.end()) return false;
      my_list2.erase (list2_it); ++list1_it;
    }
  return true;
}

template <typename T>
inline void display_list (const std::list <T>& list)
{
  std::cout << "List content:";
  for (typename std::list<T>::const_iterator list_it = list.begin();  
       list_it != list.end(); ++list_it) { std::cout << " " << *list_it; }
  std::cout << "\n";
}

#endif // UTILITY_H
