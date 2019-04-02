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
#ifndef DATA_UNIQUENESS_H_
#define DATA_UNIQUENESS_H_

#include <string>

namespace anomaly
{

//----------------------------------------------------------------------------
// types of uniqueness:
//
// hard -> program will terminate if uniqueness is not guaranteed after N tries
// soft -> program will continue if uniqueness is not guaranteed after N tries
//----------------------------------------------------------------------------
enum e_uniqueness
{
  kMinUniqueness = 0,
  kNoUniqueness = kMinUniqueness,
  kHardUniqueness,
  kSoftUniqueness,
  kMaxUniqueness
};

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
class data_uniqueness
{
public:
  
  data_uniqueness() { kind_ = kNoUniqueness; max_failures_ = -1; name_ = "no name"; }

  e_uniqueness kind() const { return kind_; }
  void set_kind(e_uniqueness const rhs) { kind_ = rhs; }

  unsigned int max_failures() const { return max_failures_; }
  void set_max_failures(unsigned int const rhs) { max_failures_ = rhs; }
  
  std::string const & name() const { return name_; }
  void set_name(std::string const rhs) { name_ = rhs; }
 
private:  

  e_uniqueness kind_;
  unsigned int max_failures_;
  std::string name_;
  
};

} // namespace anomaly

#endif // DATA_UNIQUENESS_H_











