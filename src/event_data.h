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

Justin Gottschlich, Intel Labs (justin.gottschlich@intel.com)

*/

//----------------------------------------------------------------------------
#ifndef EVENT_DATA_H_
#define EVENT_DATA_H_

//----------------------------------------------------------------------------
// within the anomaly namespace
//----------------------------------------------------------------------------
namespace anomaly
{

class data_generator; 

class event_data
{
public:
  event_data(long const duration, data_generator &dg) 
  : duration_(duration), index_(0), timestamp_(0), data_gen_(&dg) {}

  //--------------------------------------------------------------------------
  // duration methods
  //--------------------------------------------------------------------------
  void set_duration(long const duration) { duration_ = duration; }
  long const & duration() const { return duration_; }

  //--------------------------------------------------------------------------
  // index methods
  //--------------------------------------------------------------------------
  void set_index(long const index) { index_ = index; }
  void increment_index() { ++index_; }
  long const & index() const { return index_; }

  //--------------------------------------------------------------------------
  // timestamp methods
  //--------------------------------------------------------------------------
  void set_timestamp(long const timestamp) { timestamp_ = timestamp; }
  void increment_timestamp() { ++timestamp_; }
  long const & timestamp() const { return timestamp_; }
  
  data_generator& data_gen() const;
    
private:
  long duration_;
  long index_;
  long timestamp_;
  
  mutable data_generator *data_gen_;

};

} // namespace anomaly


#endif //ANOMALY_DATA_H_
