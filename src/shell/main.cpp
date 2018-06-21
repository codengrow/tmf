/*
 *   DF
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

#include "ui/modules.h"

#ifdef RDF_MODULE
    #include "rdf/rdfui.h"
#else
    #include "ui/ui.h"
#endif

int main(int argc, char * argv[]) {

    UI * ui;
    
#ifdef RDF_MODULE
	RDFParser * parser = new RDFParser();
	ui = new RDFUI(argc, argv, parser);
#else
	Parser * parser = new Parser();
	ui = new UI(argc, argv, parser);
#endif

	ui->loop();

	return 0;
}
