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
#include <iostream>
#include <exception>
#include <thread>
#include "data_generator.h"
#include "data_period_generator.h"
#include "event_data.h"
#include "basic_scheduler.h"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
using namespace anomaly;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
data_generator::data_generator(int time_start, int time_end, 
  int normal_lowerbound_duration, int normal_upperbound_duration,
  int anomaly_lowerbound_duration, int anomaly_upperbound_duration, 
  long probability_numerator, long probability_denominator, int time_step) 
  : 
  time_start_(time_start), 
  time_end_(time_end), 
  time_step_(time_step),
  respect_windows_(false),
  normal_duration_(normal_lowerbound_duration, normal_upperbound_duration),
  anomaly_duration_(anomaly_lowerbound_duration, anomaly_upperbound_duration),
  anomaly_probability_(probability_numerator, probability_denominator),
  anomaly_value_(kAnomalyDataType),
  normal_value_(kNormalDataType),
  scheduler_(0)
{
  anomaly_uniqueness_.set_name("anomaly_uniqueness");
  normal_uniqueness_.set_name("normal_uniqueness");
  scheduler_ = new basic_scheduler(*this);

  anomaly_value_fun_ = NULL;
  normal_value_fun_ = NULL;
}

//----------------------------------------------------------------------------
// generates a data entry for a range of duration of time all with the same
// label (e.g., anomalous, non-anomalous). will generate values for ALL
// variables inserted into the data generator.
//----------------------------------------------------------------------------
void data_generator::generate_data_entry(int i, int duration, int label,
  var_map &vmap, data_list &data, int time_step)
{
  // generate data entries for each time slice in the duration
  event_data event(duration, *this);
  
  for (int j = i; j < i + duration; j += time_step, 
    event.set_index(event.index() + time_step))
  {
    // for all the variables in the vmap, we need to generate one value
    data_entry d(label, j);
    event.set_timestamp(j);
    generate_single_data_entry(label, vmap, d, event, 0);
    data.insert(d);
  }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
double data_generator::generate_value_type(int data_type, variable &var, 
  event_data &event)
{
  switch (data_type)
  {
    case kAnomalyDataType:
      return var.generate_anomaly_value(event);
      break;
    case kNormalDataType:
      return var.generate_normal_value(event);
      break;
    default:
      throw std::string("unknown data_type for generate_data_entry");
  }
}

//----------------------------------------------------------------------------
// generates a single data entry for all the variables in the vmap
//----------------------------------------------------------------------------
void data_generator::generate_single_data_entry(int data_type, var_map &vmap, 
  data_entry &d, event_data &event, int number_of_failures)
{
  var_map::iterator it = vmap.begin();

  for (; it != vmap.end(); ++it)
  {
    double val = generate_value_type(data_type, *(it->second), event);
    d.insert_variable_value(it->first, val);    
  }
  
  // set the anomalous / non-anomalous label for the entire row
  d.set_label(get_label_value(data_type, event));
  
  perform_uniqueness_check(data_type, vmap, d, event, number_of_failures);
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void data_generator::perform_uniqueness_check(int data_type, var_map &vmap, 
  data_entry &d, event_data &event, int number_of_failures)
{
  // handle uniqueness check
  data_uniqueness const & unique_type = 
   get_uniqueness_based_on_data_type(data_type);

  if (unique_type.kind() == kNoUniqueness) return;
  
  if (is_unique_data_entry(d)) return;
  
  if (number_of_failures > unique_type.max_failures())
  {
    if (unique_type.kind() == kSoftUniqueness) return;
    else
    {
      throw std::runtime_error("\n\nData generator HARD uniqueness generation"
      " failed for '" + unique_type.name() + 
      "': halting execution. \n\nTry using soft uniqueness instead.\n\n");
    }
  }
  
  d.clear_variable_values();    
  // recurse
  generate_single_data_entry(data_type, vmap, d, event, ++number_of_failures);
}


