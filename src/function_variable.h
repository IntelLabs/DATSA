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

#ifndef FUNCTION_VARIABLE_H_
#define FUNCTION_VARIABLE_H_

#include "variable.h"
 
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
namespace anomaly
{
  
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
inline double no_function_supplied_function(event_data const &)
{
  throw std::runtime_error("no normal function supplied.");
  return 0.0;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
class function_variable : public variable
{
public:
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  function_variable(std::string name) : variable(name) 
  {
    normal_function_ = anomaly::no_function_supplied_function;
    anomaly_function_ = anomaly::no_function_supplied_function;
    
    anomaly_error_string_ = "no ANOMALY data range available for variable: '";
    anomaly_error_string_ += this->name();
    anomaly_error_string_ += "'";

    normal_error_string_ = "no NORMAL data range available for variable: '";
    normal_error_string_ += this->name();
    normal_error_string_ += "'";
  }

  //--------------------------------------------------------------------------
  void normal_function(double (*fun)(event_data const &))
  { normal_function_ = fun; }

  //--------------------------------------------------------------------------
  void anomaly_function(double (*fun)(event_data const &))
  { anomaly_function_ = fun; }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  double generate_anomaly_value(event_data const &event) 
  { return anomaly_function_(event); }
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  double generate_normal_value(event_data const &event)
  { return normal_function_(event); }
  
private:

  double (*normal_function_)(event_data const &);
  double (*anomaly_function_)(event_data const &);

  std::string anomaly_error_string_;
  std::string normal_error_string_;
};

}

#endif // FUNCTION_VARIABLE_H_










