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
#ifndef MULTITHREADED_SCHEDULER_H_
#define MULTITHREADED_SCHEDULER_H_

#include <iostream>
#include <exception>
#include <thread>

#include "data_generation_scheduler.h"
#include "data_period_generator.h"
#include "event_data.h"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
namespace anomaly
{

  //---------------------------------------------------------------------------
  // This is a thread-entry point for generating data for the 
  // multithreaded data scheduler and must be declared non-static, outside the
  // scope of a class definition -- that's why it's here.
  //---------------------------------------------------------------------------
  static void generate_data_period(int starting_time, int ending_time, 
    void *dg_p, void *dl_p, anomaly::e_data_types type)
  {
    data_generator &dg = *(data_generator*)dg_p;
    data_list &dl = *(data_list*)dl_p;
    event_data event(ending_time - starting_time, dg);

    for (int i = starting_time; i < ending_time; i += dg.time_step(), 
      event.set_index(event.index() + dg.time_step() ) )
    {
      // for all the variables in the vmap, we need to generate one value
      data_entry d(type, i);
      event.set_timestamp(i);
      dg.generate_single_data_entry(type, dg.variables(), d, event, 0);
      dl.insert(d);
    }
  }
  
//----------------------------------------------------------------------------
// Multi-threaded data generation scheduler - zero embedded synchronization
//----------------------------------------------------------------------------
class multithreaded_scheduler : public data_generation_scheduler
{
public:

  multithreaded_scheduler(data_generator &dg) : data_generation_scheduler(dg) {}

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  virtual void execute()
  {
    data_period_generator periods(this->dg_);
    
    auto &normal_period = periods.normal_periods();
    auto &anomaly_period = periods.anomaly_periods();
    
    std::list<std::thread*> normal_threads, anomaly_threads;
    std::list<data_list*> normal_data, anomaly_data;

    //periods.output_periods();

    generate_data(normal_period, normal_data, normal_threads, kNormalDataType);
    generate_data(anomaly_period, anomaly_data, anomaly_threads, kAnomalyDataType);

    join_threads(normal_threads);
    join_threads(anomaly_threads);

    insert_generated_data(normal_data);
    insert_generated_data(anomaly_data);
  }
  
private:

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  void insert_generated_data(std::list<data_list*> data)
  {
    // walk through the normal data and output it
    for (auto i = data.begin(); i != data.end(); ++i)
      for (auto j = (*i)->begin(); j != (*i)->end(); ++j)
        this->dg_.generated_data().insert(*j);
  }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  void join_threads(std::list<std::thread*> threads)
  {
    // joining and delete threads
    for (auto i = threads.begin(); i != threads.end(); ++i)
    {
      (*i)->join();
      delete *i;
    }
  }
  
  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  void generate_data(std::map<int, int> &period, 
    std::list<data_list*> &data, std::list<std::thread*> &threads,
    anomaly::e_data_types type)
  {
    for (auto i = period.begin(); i != period.end(); ++i)
    {
      data_list* list = new data_list;
      data.push_back(list);
      
      auto n = new std::thread(generate_data_period, i->first, 
        i->first + i->second, (void*)&(this->dg_), (void*)list, type);
        
      threads.push_back(n);
    }
  }
  
  // protectedly inherit: 
  // data_generator &dg_;

};

} // namespace anomaly


#endif //MULTITHREADED_SCHEDULER_H_


