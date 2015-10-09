/*
 * 
 *  Tiny Multimedia Framework
 *  Copyright (C) 2014 Arash Shafiei
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "filters/ehealth/PulsioximeterSensor.h"

using namespace tmf;
using namespace std;

FilterRegister<PulsioximeterSensor> PulsioximeterSensor::reg("pulsioximeter");

PulsioximeterSensor::PulsioximeterSensor(const string & name) :
Filter(name) {
  outputPulse = createOutputPort<PulseData>("pulse output");
  outputOxi = createOutputPort<OxiData>("oxi output");
}

void PulsioximeterSensor::init() {
  
  this->period = stoi(getProp("period"));

}

void PulsioximeterSensor::run() {
  
  std::this_thread::sleep_for(std::chrono::seconds(period));
  int pulse = 80;
  int oxi = 90;
  
  outputPulse->lock();
  PulseData * outputPulseData =  outputPulse->get();
  (*outputPulseData).pulse = pulse;
  outputPulse->unlock();
  
  outputOxi->lock();
  OxiData * outputOxiData =  outputOxi->get();
  (*outputOxiData).oxi = oxi;
  outputOxi->unlock();
  
}

PulsioximeterSensor::~PulsioximeterSensor() {
  destroyPort(outputPulse);
  destroyPort(outputOxi);
}