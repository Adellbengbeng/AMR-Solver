#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include "boundary.h"
#include "octreegrid.h"
#include "adapt.h"
#include "direction.h"

namespace std {

myOctree::NodeBc string_to_NodeBc(string bc) {

	myOctree::NodeBc bcc;

	if(bc=="N")
		bcc = myOctree::NONE;

	if(bc=="B")
		bcc = myOctree::BOUNDARY;

	if(bc=="MPI_BOUNDARY")
		bcc = myOctree::MPI_BOUNDARY;
		
	return bcc;
}

myOctree::FieldBc string_to_FieldBc(string bc) {

	myOctree::FieldBc bcc;

	if(bc=="N")
		bcc = myOctree::none;

	if(bc=="D")
		bcc = myOctree::dirichlet;
	
	if(bc=="NE")
		bcc = myOctree::neumann;

	if(bc=="MB")
		bcc = myOctree::mpi_boundary;
		
	return bcc;
}

void read_scalar_fields(ifstream& file) {

	string line, str;

	//Skips if a line is empty		
	getline(file, line);
	while(line.empty()) {
		getline(file, line);
	}

	if(line=="scalar_fields") {

		file >> str;
                if(str != "{") {
                        cerr << "Expected an opening bracket" <<endl;
                        exit(1);
                }
		
		cerr << "\nUser defined scalar fields" << endl;

		while(file) { 
	
		
			if(file >> str && str == "}") break;
                        else    myOctree::scalar_fields.push_back(str);
			cerr << str << endl;

		}	
	}
}


void read_vector_fields(ifstream& file) {

	string line, str;
	
	//Skips if a line is empty		
	getline(file, line);
	while(line.empty()) {
		getline(file, line);
	}

	if(line=="vector_fields") {

		file >> str;
	        if(str != "{") {
	                cerr << "Expected an opening bracket" <<endl;
	                exit(1);
	        }
		
		cerr << "\nUser defined vector Fields" << endl;

		while(file) { 
	
		
			if(file >> str && str == "}") break;
	                else    myOctree::vector_fields.push_back(str);
			cerr << str << endl;	
		}	
	}

}

int read_blocks(ifstream& file) {
	
	string line, str;

	int blocknumber, level;	
	double xmin, xmax, ymin, ymax, zmin, zmax;
	string eastbc, westbc, northbc, southbc, topbc, bottombc;
	myOctree::NodeBc east_bc, west_bc, north_bc, south_bc, top_bc, bottom_bc;

	//skips line if line is empty
	getline(file,line);
	while(line.empty()) {
		getline(file, line);
	}


	if(line=="blocks") {


		file >> str;
		if(str != "{") {
			cerr << "Expected an opening bracket" <<endl;				
			exit(1);	
		}
		
		while(file) {
			if(file >> str && str == "}") break;
			else	blocknumber = atoi(str.c_str());

			//file >> blocknumber;
			file >> xmin >> xmax >> ymin >> ymax >> zmin >> zmax;
			file >> level;
			file >> eastbc >> westbc >> northbc >> southbc >> topbc >> bottombc;
						
			east_bc = string_to_NodeBc(eastbc);		
			west_bc = string_to_NodeBc(westbc);		
			north_bc = string_to_NodeBc(northbc);		
			south_bc = string_to_NodeBc(southbc);		
			top_bc = string_to_NodeBc(topbc);		
			bottom_bc = string_to_NodeBc(bottombc);		

			//cerr << blocknumber << xmin << xmax << ymin << ymax << zmin << zmax << endl;

			myOctree::create_node(blocknumber, xmin, xmax, ymin, ymax, zmin, zmax, level,\
				       	east_bc, west_bc, north_bc, south_bc, top_bc, bottom_bc);

		}

	}

	return blocknumber;
}

void read_scalar_field_Bc(ifstream& file, int number) {


	int blocknumber;	
	string line, str;
	string eastbc, westbc, northbc, southbc, topbc, bottombc;
	
	myOctree::FieldBc **bc;
	bc = new myOctree::FieldBc* [3];
	for(int i = 0; i < 3; i++)
     		bc[i] = new myOctree::FieldBc [2];	
	
	myOctree::FieldBc east_bc, west_bc, north_bc, south_bc, top_bc, bottom_bc;
	double eastbcval, westbcval, northbcval, southbcval, topbcval, bottombcval;

	//Skips if a line is empty		
	getline(file, line);
	while(line.empty()) {
		getline(file, line);
	}

	if(line=="scalar_field_boundary_conditions") {

		file >> str;
                if(str != "{") {
                        cerr << "Expected an opening bracket" <<endl;
                        exit(1);
                }
		
		cerr << "\nSetting scalar field boundary conditions" << endl;

		while(file) { 
	
		
			if(file >> str && str == "}") break;
                        //else    myOctree::scalar_fields.push_back(str);
			cerr << str << endl;

			for(int i = 0; i < number ; ++i) {
		
				file >> blocknumber; 
				file >> eastbc >> westbc >> northbc >> southbc >> topbc >> bottombc;
				file >> eastbcval >> westbcval >> northbcval >> southbcval >> topbcval >> bottombcval;
				
				bc[myOctree::X_DIR][myOctree::RIGHT] = string_to_FieldBc(eastbc);		
				bc[myOctree::X_DIR][myOctree::LEFT] = string_to_FieldBc(westbc);		
				bc[myOctree::Y_DIR][myOctree::RIGHT] = string_to_FieldBc(northbc);		
				bc[myOctree::Y_DIR][myOctree::LEFT] = string_to_FieldBc(southbc);		
				bc[myOctree::Z_DIR][myOctree::RIGHT] = string_to_FieldBc(topbc);		
				bc[myOctree::Z_DIR][myOctree::LEFT] = string_to_FieldBc(bottombc);		

				//cerr << blocknumber << eastbc << westbc << northbc << southbc << topbc << bottombc << endl;

				/*add a function here which sets boundary condition and boundary values*/
				myOctree::set_FieldBc_FieldBcVal(blocknumber, str, bc);
			}
		}	
	}

	delete [] bc;
}


void read_vector_field_Bc(ifstream& file, int number) {

	int blocknumber;	
	string line, str;
	string eastbc, westbc, northbc, southbc, topbc, bottombc;
	
	myOctree::FieldBc **xbc;
	myOctree::FieldBc **ybc;
	myOctree::FieldBc **zbc;
	xbc = new myOctree::FieldBc* [3];
	ybc = new myOctree::FieldBc* [3];
	zbc = new myOctree::FieldBc* [3];
	for(int i = 0; i < 3; i++) {
     		xbc[i] = new myOctree::FieldBc [2];	
     		ybc[i] = new myOctree::FieldBc [2];	
     		zbc[i] = new myOctree::FieldBc [2];	
	}

	myOctree::FieldBc east_bc, west_bc, north_bc, south_bc, top_bc, bottom_bc;
	double eastbcval, westbcval, northbcval, southbcval, topbcval, bottombcval;
	
	//Skips if a line is empty		
	getline(file, line);
	while(line.empty()) {
		getline(file, line);
	}

	if(line=="vector_field_boundary_conditions") {

		file >> str;
	        if(str != "{") {
	                cerr << "Expected an opening bracket" <<endl;
	                exit(1);
	        }
		
		cerr << "\nSetting vector field boundary conditions " << endl;

		while(file) { 
	
		
			if(file >> str && str == "}") break;
	                //else    myOctree::vector_fields.push_back(str);
			cerr << str << endl;	
			
			for(int i = 0; i < number ; ++i) {
		
				file >> blocknumber; 
			
			
				file >> eastbc >> westbc >> northbc >> southbc >> topbc >> bottombc;
				file >> eastbcval >> westbcval >> northbcval >> southbcval >> topbcval >> bottombcval;
				
				xbc[myOctree::X_DIR][myOctree::RIGHT] = string_to_FieldBc(eastbc);		
				xbc[myOctree::X_DIR][myOctree::LEFT] = string_to_FieldBc(westbc);		
				xbc[myOctree::Y_DIR][myOctree::RIGHT] = string_to_FieldBc(northbc);		
				xbc[myOctree::Y_DIR][myOctree::LEFT] = string_to_FieldBc(southbc);		
				xbc[myOctree::Z_DIR][myOctree::RIGHT] = string_to_FieldBc(topbc);		
				xbc[myOctree::Z_DIR][myOctree::LEFT] = string_to_FieldBc(bottombc);		

				//cerr << blocknumber << eastbc << westbc << northbc << southbc << topbc << bottombc << endl;

				file >> eastbc >> westbc >> northbc >> southbc >> topbc >> bottombc;
				file >> eastbcval >> westbcval >> northbcval >> southbcval >> topbcval >> bottombcval;
				
				ybc[myOctree::X_DIR][myOctree::RIGHT] = string_to_FieldBc(eastbc);		
				ybc[myOctree::X_DIR][myOctree::LEFT] = string_to_FieldBc(westbc);		
				ybc[myOctree::Y_DIR][myOctree::RIGHT] = string_to_FieldBc(northbc);		
				ybc[myOctree::Y_DIR][myOctree::LEFT] = string_to_FieldBc(southbc);		
				ybc[myOctree::Z_DIR][myOctree::RIGHT] = string_to_FieldBc(topbc);		
				ybc[myOctree::Z_DIR][myOctree::LEFT] = string_to_FieldBc(bottombc);		

				//cerr << blocknumber << eastbc << westbc << northbc << southbc << topbc << bottombc << endl;

				file >> eastbc >> westbc >> northbc >> southbc >> topbc >> bottombc;
				file >> eastbcval >> westbcval >> northbcval >> southbcval >> topbcval >> bottombcval;
				
				zbc[myOctree::X_DIR][myOctree::RIGHT] = string_to_FieldBc(eastbc);		
				zbc[myOctree::X_DIR][myOctree::LEFT] = string_to_FieldBc(westbc);		
				zbc[myOctree::Y_DIR][myOctree::RIGHT] = string_to_FieldBc(northbc);		
				zbc[myOctree::Y_DIR][myOctree::LEFT] = string_to_FieldBc(southbc);		
				zbc[myOctree::Z_DIR][myOctree::RIGHT] = string_to_FieldBc(topbc);		
				zbc[myOctree::Z_DIR][myOctree::LEFT] = string_to_FieldBc(bottombc);		

				//cerr << blocknumber << eastbc << westbc << northbc << southbc << topbc << bottombc << endl;


				/*add a function here which sets boundary condition and boundary values*/
				myOctree::set_VecFieldBc_VecFieldBcVal(blocknumber, str, xbc, ybc, zbc);
			}
		}	
	}

	delete 	[] xbc;
      	delete	[] ybc;
      	delete	[] zbc;

}

void read_max_level(ifstream& file) {

	string line, str;
	
	//Skips if a line is empty		
	getline(file, line);
	while(line.empty()) {
		getline(file, line);
	}

	if(line=="max_level") {

		file >> myOctree::max_level;
	
	}

}
		

void read_input_file() {

	int blocknumber;
	string line, str;

	ifstream file ("../input/input.pfs"); 
	if(file.fail()) {
		cerr << "Error opening input file\n";
		exit(1);
	}

	if(file.peek() == ifstream::traits_type::eof()) {
		cerr << "Nothing in the input file\n";
		exit(1);
	}

	else {

		cerr << "\nReading input files" << endl;

		read_scalar_fields(file);

		read_vector_fields(file);
	
		blocknumber = read_blocks(file);

		read_scalar_field_Bc(file, blocknumber);

		read_vector_field_Bc(file, blocknumber);

		read_max_level(file);

	}

	file.close();
}

}
