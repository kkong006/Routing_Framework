
#include "../Headers/problem_map.h"
#include "../Headers/problem_object.h"
#include <time.h>
#include <cstdlib>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

int main(int argc,char* argv[]) {

	// DO NOT CHANGE THIS SECTION OF CODE
	if(argc < 2) {
		cout << "Usage: ./grid_router <test_file>" << endl;
		exit(1);
	}
	Utilities::ProblemObject* first_problem = new Utilities::ProblemObject(std::string(argv[1]));
	// EDIT FROM HERE DOWN

	//Create your problem map object (in our example, we use a simple grid, you should create your own)

	Utilities::ProblemMap p(first_problem);
	vector<Path*> paths = p.lees();
	for(unsigned i = 0; i<paths.size(); ++i) {
		cout<<"Path "<<i<<": ";
		paths.at(i)->print();
	}

	vector<Path*> post_processed_paths = p.post_process();
	cout<<endl;
	for(unsigned i = 0; i<post_processed_paths.size(); ++i) {
		cout<<"Post-Processed Path "<<i<<": ";
		post_processed_paths.at(i)->print();
	}
	cout<<endl;
	delete first_problem;
	//testing code
	// for(int j = 0; j<p.get_width(); ++j) {
	// 	cout<<' '<<j;
	// }
	// cout<<endl;
	// for(int i = 0; i<p.get_height();++i) {
	// 	cout<<i<<' ';
	// 	for(int j = 0; j<p.get_width();++j) {
	// 		cout<<p.get_node(j,i)->get_cost()<<' ';
	// 	}
	// 	cout<<endl;
	// }
	// vector<Path*> post_paths = p.post_process();
	// for(unsigned i = 0; i<post_paths.size(); ++i) {
	// 	post_paths.at(i)->print();
	// }
	// for(int i = 0; i<p.get_height();++i) {
	// 	for(int j = 0; j<p.get_width();++j) {
	// 		cout<<p.blocker_map.at(i).at(j)<<' ';
	// 	}
	// 	cout<<endl;
	// }
	//cout<<p.get_node(250,0)->get_cost()<<endl;
	/*
	Note: we do not take into account the connections or blockers that exist in the Project Object
	You should be accouting for these in your problem map objects (you should not be using Grid). You
	should be using both the Node and Edge classes as the background for whatever problem map object
	you create.
	*/

	/*
	Run your algorithm after creating your problem map object. You should be returning from your algorithm
	either a Path or a Netlist

	Path: a series of straight line segments, with a single source and a single sink
	Netlist: a series of stright line segments, with a single source and more than one sink
	*/
	// vector<Path*> paths = g.test_algorithm();

	//Print the paths/netlists that you return from your algorithm
	// for(unsigned i = 0; i < paths.size(); i++) {
	// 	cout << "Path " << i << ": ";
	// 	paths.at(i)->print();
	// 	Path* temp = paths.at(i);
	// 	delete temp;
	// }

	// paths.clear();


	// delete first_problem;

	return 0;
}
