/*
 *   TMF
 *   Copyright (C) TMF Team
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DF_BOXFILTER_H_
#define DF_BOXFILTER_H_

#include "core/df.h"
#include "tokens/opencv/mat.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>

using namespace std;

class BoxFilter: public df::Actor {
  
private:

  cv::Mat frame;
  cv::Size ksize;
  int kernel_size, ddepth;
  cv::Point anchor;
  df::InputPort<df::Mat> * input;
  df::OutputPort<df::Mat> * output;
 
  static  df::ActorRegister<BoxFilter> reg;
public:
  
  BoxFilter(const string& name);
  
  virtual void init();
  
  virtual void run();
  
  virtual ~BoxFilter();
  
};

#endif /* DF_BOXFILTER_H_ */
