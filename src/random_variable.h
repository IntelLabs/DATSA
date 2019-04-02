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
#ifndef RANDOM_VARIABLE_H_
#define RANDOM_VARIABLE_H_

#include "variable.h"
#include "event_data.h"

namespace anomaly
{
  
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
class random_variable : public variable
{
public:
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  random_variable(std::string name) : variable(name) 
  {
    anomaly_error_string_ = "no ANOMALY data range available for variable: '";
    anomaly_error_string_ += this->name();
    anomaly_error_string_ += "'";

    normal_error_string_ = "no NORMAL data range available for variable: '";
    normal_error_string_ += this->name();
    normal_error_string_ += "'";
  }
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  void insert_normal_range(double lower, double upper)
  {
    normal_lower_.push_back(lower);
    normal_upper_.push_back(upper);
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  void insert_anomaly_range(double lower, double upper)
  {
    anomaly_lower_.push_back(lower);
    anomaly_upper_.push_back(upper);
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  double generate_value(std::vector<double> &lower, std::vector<double> &upper,
    int timestamp, std::string const &error_string)
  {
    // if we have no ranges for this, throw an exception
    if (lower.size() == 0) throw error_string;
    
    int loc = rand() % lower.size();
    
    // randomly generate a number from the upperbound - lowerbound
    // then add the lowerbound back in so it's within range
    double val = (double)(rand() % (abs(int(upper[loc] - lower[loc]))));
    val += lower[loc];
    
    // randomly generate a double between 0.0 - 1.0
    double r = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
    
    val -= r;
    
    if (val < lower[loc]) val = lower[loc];
    if (val > upper[loc]) val = upper[loc];

    return val;
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  double generate_anomaly_value(event_data const & event) 
  { 
    return generate_value(anomaly_lower_, anomaly_upper_, event.timestamp(),
      anomaly_error_string_); 
  }
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  double generate_normal_value(event_data const &event)
  { 
    return generate_value(normal_lower_, normal_upper_, event.timestamp(),
      normal_error_string_); 
  }
  
private:

  std::vector<double> normal_lower_;
  std::vector<double> normal_upper_;

  std::vector<double> anomaly_lower_;
  std::vector<double> anomaly_upper_;

  std::string anomaly_error_string_;
  std::string normal_error_string_;
};

}

#endif // RANDOM_VARIABLE_H_


