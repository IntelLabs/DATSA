/*

MIT License

Copyright 2018, Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
of the Software, and to permit persons to whom the Software is furnished to do 
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.

*/

//----------------------------------------------------------------------------
#ifndef DATA_ENTRY_H_
#define DATA_ENTRY_H_

#include <vector>
#include <list>
#include <map>
#include <string>
#include <iostream>
#include <set>

namespace anomaly
{
  
//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------
class data_entry
{
public:

  data_entry(double label, int time_stamp)
    : label_(label), time_stamp_(time_stamp)
  {}

  void insert_variable_value(std::string name, double value)
  {
    variable_values_[name] = value;
  }
 
  void set_label(double const rhs) { label_ = rhs; } 
  double label() const { return label_; }
  int time_stamp() const { return time_stamp_; }

  std::map<std::string, double> & variable_values() const { return variable_values_; }
  
  bool operator<(data_entry const &rhs) const
  {
    if (time_stamp_ < rhs.time_stamp_) return true;
    else return false;
  }

  // complex equality check across all variables and their values
  bool operator==(data_entry const &rhs) const
  {
    // if the variable sets aren't equal, these data entries can't be equal
    if (variable_values_.size() != rhs.variable_values_.size()) return false;
    
    std::map<std::string, double>::const_iterator i = rhs.variable_values_.begin(),
      j = variable_values_.begin();
    
    for ( ; i != rhs.variable_values_.end(); ++i, ++j)
    {
      if (i->first != j->first) return false;
      if (i->second != j->second) return false;
    }
    
    return true;
  }

  void clear_variable_values()
  {
    variable_values_.clear();
  }
  
private:
  
  double label_;
  int time_stamp_;
  mutable std::map<std::string, double> variable_values_;  
};

typedef std::vector<data_entry> data_vector;
typedef std::set<data_entry> data_list;
typedef std::map<std::string, variable*> var_map;

}

#endif // DATA_ENTRY_H_


