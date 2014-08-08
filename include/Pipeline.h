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

#ifndef PIPELINE_H_
#define PIPELINE_H_

#include <iostream>
#include <set>
#include "Filter.h"
#include "Bus.h"

using namespace std;

/*!
 * \enum PipelineStatus
 * Status of a filter.
 */
enum PipelineStatus {
	PIPELINE_RUNNING, /**< The pipeline is currently running. */
	PIPELINE_STOPPED, /**< The pipeline has stopped. */
	PIPELINE_PAUSED, /**< The pipeline is currently paused. */
	PIPELINE_FINISHED /**< The pipeline has finished processing its last item. */
};

/*!
 * \class Pipeline
 * A pipeline, consisting of a number of interconnected filters.
 * Filters have a many-to-many relation, with directed pipes. Cycles are not allowed.
 */
class Pipeline {
	string name; /**< The name of the pipeline. */
	PipelineStatus status; /**< The current status of the pipeline. */
	set<Filter *> filters; /**< The set of all filters in the pipeline. */
	Filter* start; /**< The initial element of the pipeline. Must be a data source (0 inputs). */
	Bus * bus;
public:
	/*!
	 * \param name
	 *   The name of the pipeline.
	 */
	Pipeline(const string& name);

	/*!
	 * Create a pipe between two filters in the pipeline.
	 * These filters should be in the pipeline.
	 *
	 * \param fi
	 *   The source filter for the pipe.
	 * \param fo
	 *   The target filter for the pipe.
	 */
	void connectFilters(Filter * fi, Filter * fo);

	/*!
	 * Set the initial filter for the pipeline.
	 *
	 * \param starter
	 *   The new initial filter for the pipeline. Should be a source (no input connections).
	 */
	//void setStarter(Filter *starter);

	/*!
	 * Initialize the pipeline.
	 *
	 * \return the current pipeline status.
	 */
	PipelineStatus init();

	/*!
	 * Run one iteration of the pipeline.
	 *
	 * \return the current pipeline status.
	 */
	PipelineStatus run();


	void setProp(const string & key, const string & val) {bus->setProp(key,val);}
	string getProp(const string & key) {return bus->getProp(key);}
	/*!
	 */
	virtual ~Pipeline();
};

#endif /* PIPELINE_H_ */
