/*
 *   RDF
 *   Copyright (C) INRIA - Orange Labs
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

#ifndef RDFGRAPH_H
#define RDFGRAPH_H

#include <iostream>
#include "graph/graph.h"
#include "rule.h"

class Condition {
public:
	string var;

	string actor;
	string metric;

	char sign;
	int val;
	string rule;

	void set(string var, string ac, string mt, char sn, int vl, string rl);
};

/*!
 * \class RDFGraph
 * The class for RDF graph.
 *
 */
class RDFGraph {
public:
	Graph * graph;
	map<string, Rule *> rules; /**< RDF transformation rules. */
	map<string, vector<Condition>> prog; /**< RDF program containing the values of variables for which the rules are applied. */

	RDFGraph();
	int add_rule(Rule * r);
	int add_condition(string var, string actor, string metric, char sign, int val, string rule);
};

#endif
