/*
 *   TMF
 *   Copyright (C) 2018
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

#include "parser.h"

Parser::Parser() {
}

int Parser::load_from_string(const std::string& app) {
	std::stringstream ss(app);
	return load_from_stream(ss);	
}

int Parser::load_from_file(const char * filename) {
	ifstream file(filename);
	if (!file)
       		return -1;

	std::stringstream ss;
	ss << file.rdbuf();
	file.close();

	return load_from_stream(ss);
}


int Parser::load_from_stream(stringstream& ss) {
	int ret;
	graph = new Graph();
	string gname;
        read_str(ss, "df");
        ss >> gname;
        graph->set_name(gname);
	ret = read_graph(ss, graph);
	if (ret < 0)
		return ret;
	if (ret == 0) {
		cout << "graph is loaded successfully.\n";
		return 0;
	} else
		return ret;
}

int Parser::read_str(stringstream & stream, string str) {
	string tmp;
	stream >> tmp;
	if (tmp == str)
		return 0;
	else if (stream.eof()) {
		return -2;
	}
	else {
		cout << "error: cannot read " << str << "\n";
		return -1;
	}
}

int Parser::trim_str(string & str) {

	auto end_pos = std::remove(str.begin(), str.end(), ' ');
	str.erase(end_pos, str.end());

	end_pos = std::remove(str.begin(), str.end(), '\n');
	str.erase(end_pos, str.end());

	end_pos = std::remove(str.begin(), str.end(), '\t');
	str.erase(end_pos, str.end());

	return 0;
}
int Parser::read_graph(stringstream & stream, Graph * g) {
	int ret;
	string tmp;
	read_str(stream, "{");
        read_str(stream, "topology");
	ret = read_topology(stream,g);
	if (ret < 0)
		return ret;
	while (true) {
		ret = 0;
		stream >> tmp;
		if (tmp == "}")
			break;
		else if (tmp == "parameter")
			ret = read_parameters(stream, g);
		else if (tmp == "actor")
			ret = read_props(stream, g);
		else if (tmp == "production")
			ret = read_productions(stream, g);
		else if (tmp == "consumption")
			ret = read_consumptions(stream, g);
		else {
			cout << "cannot parse the graph.\n";
			return -1;
		}
		if (ret < 0)
			return ret;
	}
	load_actor_types(g);
	return 0;
}

int Parser::load_actor_types(Graph * g) {

	string type;
	auto actors = g->get_actors();
	for (auto ac : actors) {
		type = g->get_actor_prop(ac, "computation");
		g->set_actor_type(ac,type);
	}

	return 0;
}


int Parser::read_productions(stringstream & stream, Graph * g) {

	int ret;
	string ratelist, edgename, rate;
	read_str(stream, "{");	
	getline(stream, ratelist, '}');

	trim_str(ratelist);

	istringstream ss(ratelist);
	while (getline(ss, rate, ';')) {
		ret = add_production_rate(rate,g);
		if (ret < 0)
			return ret;		
	}
	return 0;
}

int Parser::read_consumptions(stringstream & stream, Graph * g) {

	int ret;
	string ratelist, edgename, rate;
	read_str(stream, "{");	
	getline(stream, ratelist, '}');

	trim_str(ratelist);

	istringstream ss(ratelist);
	while (getline(ss, rate, ';')) {
		ret = add_consumption_rate(rate,g);
		if (ret < 0)
			return ret;		
	}
	return 0;
}

int Parser::read_parameters(stringstream & stream, Graph * g) {

	int ret;
	string params;
	read_str(stream, "{");	
	getline(stream, params, '}');

	trim_str(params);
	
	istringstream ss(params);
	while (getline(ss, params, ';')) {
		ret = add_graph_param(params,g);
		if (ret < 0)
			return ret;		
	}

	return 0;
}

int Parser::read_props(stringstream & stream, Graph * g) {

	int ret;
	string actname, props, prop;
	stream >> actname;
	read_str(stream, "{");	
	getline(stream, props, '}');
	
	trim_str(props);
	
	istringstream ss(props);
	while (getline(ss, prop, ';')) {
		ret = add_actor_prop(actname, prop,g);
		if (ret < 0)
			return ret;		
	}
	return 0;

}

int Parser::read_topology(stringstream & stream, Graph * g) {
        int ret;
	string actorlist, edgelist, actor, edge;
	read_str(stream, "{");
	read_str(stream, "nodes");
	read_str(stream, "=");
	getline(stream, actorlist, ';');

	read_str(stream, "edges");
	read_str(stream, "=");
	getline(stream, edgelist, ';');

	read_str(stream, "}");

	trim_str(actorlist);
	trim_str(edgelist);

	istringstream ssa(actorlist);
	while (getline(ssa, actor, ',')) {
		ret = add_actor(actor,g);
		if (ret < 0)
			return ret;		
	}

	istringstream sse(edgelist);
	while (getline(sse, edge, ')')) {
		if (edge[0]==',') 
			edge = edge.substr(1, edge.size());
		ret = add_edge(edge,g);
		if (ret < 0)
			return ret;		
	}

	return 0;
}

int Parser::add_edge(const string& edge, Graph * g) {
        string edge_name, edge_source, edge_sink;

	if (edge=="") {
		cout << "error: edge is not well formatted.\n";
		return -2;
	}
	std::istringstream ss(edge);
  	getline(ss, edge_name, '(');
  	getline(ss, edge_source, ',');
  	getline(ss, edge_sink);

	int ret = g->add_edge(edge_name, edge_source, edge_sink);
	if (ret == -2)
		cout << "error: edge already exists.\n";
	else if (ret == -1)
		cout << "error: edge source/sink actor does not exist.\n";
	return ret;
}

int Parser::add_actor(const string& actor, Graph * g) {
        if (actor=="") {
		cout << "error: actor is not well formatted.\n";
		return -2;
	}
	int ret = g->add_actor(actor, "");
	return ret;
}

int Parser::read_actors(stringstream & stream, Graph * g) {
        int ret;
	string actorlist, actor;
	read_str(stream, "{");
	getline(stream, actorlist, '}');

	trim_str(actorlist);

	istringstream ss(actorlist);
	while (getline(ss, actor, ';')) {
		ret = add_actortype(actor,g);
		if (ret < 0)
			return ret;		
	}
	return 0;
}

int Parser::read_edges(stringstream & stream, Graph * g) {
	int ret;	
	int source, sink;
	string edgelist, edgeline, source_str, sink_str;
	string edge, edge_source, edge_sink;
	read_str(stream, "{");

	getline(stream, edgelist, '}');

	trim_str(edgelist);
	
	istringstream ss(edgelist);

	while (getline(ss, edgeline, ';')) {
		istringstream edl(edgeline);
		getline(edl, edge_source, ',');
		getline(edl, edge_sink, ',');
		getline(edl, source_str, ',');
		getline(edl, sink_str);
		ret = add_edge(edge_source, edge_sink, g);
		if (ret < 0) {
			cout << "error: parsing stream failed.\n";
			return ret;
		}
		source = std::stoi(source_str);
		sink = std::stoi(sink_str);

		if (source < 1 || sink < 1) {
			cout << "error: source/sink rates must be valid positive integer.\n";
			return ret;
		}
		edge = edge_source + edge_sink;
		g->set_source_rate(edge, source);
		g->set_sink_rate(edge, sink);
	}
	return 0;
}

int Parser::add_production_rate(const string& rate, Graph * g) {
	int edr=1;
	string edgename="", edgerate="";
	std::istringstream ss(rate);
  	getline(ss, edgename, '=');
	getline(ss, edgerate);
        if (edgename=="" || edgerate=="") {
		cout << "error: edge production rate is not well formatted.\n";
		return -2;
	}
	try {
		edr = stoi(edgerate);
	}
	catch(...) {
		cout << "error: production rate of edge " << edgename << " is not correct.\n";
		return -2;
	}
	int ret = g->set_source_rate(edgename,edr);
	if (ret == -1)
		cout << "error: edge " << edgename << " is not found.\n";
	return ret;
}

int Parser::add_consumption_rate(const string& rate, Graph * g) {
	int edr=1;
	string edgename="", edgerate="";
	std::istringstream ss(rate);
  	getline(ss, edgename, '=');
	getline(ss, edgerate);
        if (edgename=="" || edgerate=="") {
		cout << "error: edge consumption rate is not well formatted.\n";
		return -2;
	}
	try {
		edr = stoi(edgerate);
	}
	catch(...) {
		cout << "error: consumption rate of edge " << edgename << " is not correct.\n";
		return -2;
	}
	int ret = g->set_sink_rate(edgename,edr);
	if (ret == -1)
		cout << "error: edge " << edgename << " is not found.\n";
	return ret;
}

int Parser::add_graph_param(const string& prop, Graph * g) {
	string key="", val="";
	std::istringstream ss(prop);
  	getline(ss, key, '=');
	getline(ss, val);
        if (key=="" || val=="") {
		cout << "error: parameter is not well formatted.\n";
		return -2;
	}
	int ret = g->add_graph_param(key, val);
	return ret;
}

int Parser::add_actor_prop(const string& actname, const string& prop, Graph * g) {
	string key="", val="";
	std::istringstream ss(prop);
  	getline(ss, key, '=');
	getline(ss, val);
        if (key=="" || val=="") {
		cout << "error: actor property is not well formatted.\n";
		return -2;
	}
	int ret = g->add_actor_prop(actname, key, val);
	return ret;
}

int Parser::add_actortype(const string& actortype, Graph * g) {
	string name="", type="";
	std::istringstream ss(actortype);
  	getline(ss, name, ':');
	getline(ss, type);
        if (name=="" || type=="") {
		cout << "error: actor is not well formatted.\n";
		return -2;
	}
	int ret = g->add_actor(name, type);
	return ret;
}

int Parser::add_edge(string edge_source, string edge_sink, Graph * g) {
	int ret = g->add_edge(edge_source, edge_sink);
	if (ret == -2)
		cout << "error: edge already exists.\n";
	else if (ret == -1)
		cout << "error: edge source/sink actor does not exist.\n";
	return ret;
}

Graph * Parser::get_graph() {
	return graph;
}