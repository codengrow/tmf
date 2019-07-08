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

#include "dummy_load.h"

using namespace df;
using namespace std;

ActorRegister<DummyLoad> DummyLoad::reg("DummyLoad");

DummyLoad::DummyLoad(const std::string & name): df::Actor(name) {
  input = createInputPort<df::Mat>("input");
  output = createOutputPort<df::Mat>("output");
}

void DummyLoad::init() {
  sleep_time = 1;
  increase = true;
}

void DummyLoad::run() {
  auto in = consume(input);
  auto out = produce(output);
  out->set(*in->get());
  log("dummy load "+to_string(stepno));
  timer.sleep(sleep_time/5);

  if (sleep_time == 500)
	 increase = false;
  if (sleep_time == 0)
	 increase = true;

  increase?sleep_time++:sleep_time--;

  release(input);
  release(output);
}

DummyLoad::~DummyLoad() {
  destroyPort(input);
  destroyPort(output);
}

