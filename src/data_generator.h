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
#ifndef DATA_GENERATOR_H_
#define DATA_GENERATOR_H_

//----------------------------------------------------------------------------
#include <vector>
#include <string>
#include <map>
#include <stdlib.h>
#include <iostream>
#include <iomanip>

#include "variable.h"
#include "data_entry.h"
#include "stochastic_duration.h"
#include "data_uniqueness.h"
#include "integer_probability.h"
#include "data_generation_scheduler.h"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
namespace anomaly
{
  
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
enum e_data_types
{
  kMinDataType = 0,
  kNormalDataType = kMinDataType,
  kAnomalyDataType,
  kMaxDataType
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
class data_generator
{
public:

  //--------------------------------------------------------------------------
  data_generator(int time_start, int time_end, int normal_lowerbound_duration,
    int normal_upperbound_duration, int anomaly_lowerbound_duration, 
    int anomaly_upperbound_duration, long probability_numerator,
    long probability_denominator, int time_step = 1);

  //--------------------------------------------------------------------------
  // TODO: also delete all the items in the variables_ list - their ownership
  //       is claimed by the data_generator
  //--------------------------------------------------------------------------
  ~data_generator() { delete scheduler_; }

  //--------------------------------------------------------------------------
  bool is_anomaly_based_on_probability() const
  {
    return anomaly_probability_.roll();
  }

  //--------------------------------------------------------------------------
  // adds a new variable to generate data for in the data_generator
  //--------------------------------------------------------------------------
  void insert_variable(var_map &vmap, variable *v)
  {
    var_map::iterator i = vmap.find(v->name());

    // this variable isn't found, insert it
    if (vmap.end() == i)
    {
      vmap[v->name()] = v;
    }
    // this varibale was already found, delete prior one and add new one
    else
    {
      variable *temp = vmap[v->name()];
      vmap[v->name()] = v;
      delete temp;
    }
  }
  
  //--------------------------------------------------------------------------
  void insert_variable(variable *v)
  {
    insert_variable(variables_, v);
  }
  
  data_list& generated_data() { return generated_data_; }

  //--------------------------------------------------------------------------
  void generate_data()
  {
    generated_data_.clear();
    scheduler_->execute();
  }

  //--------------------------------------------------------------------------
  void set_scheduler(data_generation_scheduler *s) 
  {
    delete scheduler_; 
    scheduler_ = s; 
  }
  
  void generate_periods();

  //--------------------------------------------------------------------------
  // set the kind of uniqueness for normal data
  // max_failures -1 does not change prior failures value, otherwise it does 
  //--------------------------------------------------------------------------
  void set_uniqueness_for_normal_data(e_uniqueness kind, int failures = -1)
  {
    normal_uniqueness_.set_kind(kind);
    if (failures != -1) normal_uniqueness_.set_max_failures(failures);
  }

  void set_uniqueness_for_anomaly_data(e_uniqueness kind, int failures = -1)
  {
    anomaly_uniqueness_.set_kind(kind);
    if (failures != -1) anomaly_uniqueness_.set_max_failures(failures);
  }
  
  //--------------------------------------------------------------------------
  int time_start() const { return time_start_; }
  int time_end() const { return time_end_; }
  int time_step() const { return time_step_; }
  void time_step(int const &rhs) { time_step_ = rhs; }
  
  bool respect_windows() const { return respect_windows_; }
  void respect_windows(bool const &rhs) { respect_windows_ = rhs; }

  //--------------------------------------------------------------------------
  int generate_duration_based_on_type(anomaly::e_data_types type) const
  {
    if (type == kNormalDataType) return generate_normal_duration();
    else return generate_anomaly_duration();
  }

  //--------------------------------------------------------------------------
  int generate_normal_duration() const
  {
    return align_for_time_step
      (normal_duration_.generate_stochastic_duration());
  }
  
  int generate_anomaly_duration() const
  {
    return align_for_time_step
      (anomaly_duration_.generate_stochastic_duration());
  }

  //--------------------------------------------------------------------------
  int align_for_time_step(int dur) const
  {
    //------------------------------------------------------------------------
    // assuming the duration is not balanced for the time_step and respect
    // windows is off, realign to make it fit
    //------------------------------------------------------------------------
    if (dur % time_step() != 0 && !respect_windows())
    {
      dur += time_step() - (dur % time_step());
    }
    
    return dur;
  }

  //--------------------------------------------------------------------------
  void nary_anomaly_function(double (*fun)(event_data const &))
  {
    anomaly_value_fun_ = fun;
  }

  void nary_normal_function(double (*fun)(event_data const &))
  {
    normal_value_fun_ = fun;
  }
  
  void anomaly_label(double rhs) { anomaly_value_ = rhs; }
  void normal_label(double rhs) { normal_value_ = rhs; }
  
  //--------------------------------------------------------------------------
  double get_anomaly_label(event_data &event) const 
  {
    if (!anomaly_value_fun_) return kAnomalyDataType;
    return anomaly_value_fun_(event);
  }
    
  double get_normal_label(event_data &event) const 
  {
    if (!normal_value_fun_) return kNormalDataType;
    return normal_value_fun_(event);
  }

  //----------------------------------------------------------------------------
  // This function generates an anomaly or non-anomalous label provided by
  // the user or kAnomalyDataType or kNonAnomalyDataType if the user has not 
  // provided a value.
  //----------------------------------------------------------------------------
  double get_label_value(int data_type, event_data &event) const
  {
    return data_type == kAnomalyDataType ? 
      get_anomaly_label(event) : get_normal_label(event);
  }

  //----------------------------------------------------------------------------
  // generates a data entry for a range of duration of time all with the same
  // label (e.g., anomalous, non-anomalous). will generate values for ALL
  // variables inserted into the data generator.
  //----------------------------------------------------------------------------
  void generate_data_entry(int i, int duration, int label,
    var_map &vmap, data_list &data, int time_step);

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  double generate_value_type(int data_type, variable &var, event_data &event);
  
  //----------------------------------------------------------------------------
  // generates a single data entry for all the variables in the vmap
  //----------------------------------------------------------------------------
  void generate_single_data_entry(int data_type, var_map &vmap, data_entry &d, 
    event_data &event, int number_of_failures);
  
  //--------------------------------------------------------------------------
  data_uniqueness const & get_uniqueness_based_on_data_type(int const data_type) const
  {
    if (data_type == kAnomalyDataType) return anomaly_uniqueness();
    return normal_uniqueness();
  }
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  void perform_uniqueness_check(int data_type, var_map &vmap, data_entry &d, 
    event_data &event, int number_of_failures);

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  bool is_unique_data_entry(data_entry const &d)
  {
    data_list::iterator i = generated_data().begin();

    for (; i != generated_data().end(); ++i)
    {
      // if d is equal to *i then this is not a unique data entry
      if (d == *i) return false;
    }

    // otherwise, it is
    return true;
  }

  var_map& variables() { return variables_; }
  
  data_uniqueness& normal_uniqueness() { return normal_uniqueness_; }
  data_uniqueness& anomaly_uniqueness() { return anomaly_uniqueness_; }

  data_uniqueness const & normal_uniqueness() const { return normal_uniqueness_; }
  data_uniqueness const & anomaly_uniqueness() const { return anomaly_uniqueness_; }
  
private:  

  var_map variables_;
  
  int time_start_;
  int time_end_;
  int time_step_;

  //--------------------------------------------------------------------------
  // respect_windows_ 
  //
  // used to override time_step if the time_step increment exceeds the window
  // range. For example, say a range is 11, but the time_step = 5. The steps
  // would be 0, 5, 10, 15, however this would break the range of 11. respect_
  // windows_ ensures that the window is respected and would instead use the
  // time_steps of 0, 5, 10, 11.
  //
  // respect_windows_ is false by default because windows are usually 
  // stochastically generated and when a time_step() is set, the user usually
  // prefers time_steps to be respected over DATSA's stochastically generated 
  // window sizes for anomalous or non-anomalous events.
  //--------------------------------------------------------------------------  
  bool respect_windows_;

  stochastic_duration normal_duration_;
  stochastic_duration anomaly_duration_;

  data_list generated_data_;

  data_uniqueness normal_uniqueness_;
  data_uniqueness anomaly_uniqueness_;
  integer_probability anomaly_probability_;
  
  double (*normal_value_fun_)(event_data const &);
  double (*anomaly_value_fun_)(event_data const &);
  
  double anomaly_value_;
  double normal_value_;
  
  data_generation_scheduler* scheduler_;
};

} // namespace anomaly

#endif // DATA_GENERATOR_H_











