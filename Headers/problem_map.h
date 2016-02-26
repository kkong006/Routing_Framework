#ifndef _PROBLEM_MAP_H
#define _PROBLEM_MAP_H

#include "node.h"
#include "path.h"
#include "problem_object.h"
#include "claim.h"
#include <vector>

using std::cerr;
using std::endl;
using std::string;
using Utilities::Node;
using Utilities::Edge;
using Utilities::Path;
using Utilities::ProblemObject;

namespace Utilities {
	class ProblemMap {
	private:
		vector<vector<Node*> > map;
		int num_connections;
		vector<Path*> paths;
		vector<Connection> connections;
		vector<Blocker> blockers;

	public:
		vector<vector<bool> > blocker_map;
		/* Constructors/Destructors */
		ProblemMap();
		ProblemMap(ProblemObject* problem_object);
		~ProblemMap();

		/* Accessors */
		int get_width();
		int get_height();
		int get_num_connections();
		Node* get_node(int x, int y);
		Node* get_node(Point coord);
		vector<Path*> get_paths();
		Path* get_path(int i);

		/* Mutators */
		void replace_node(Node* replacement_node);
		void set_paths(vector<Path*> paths);
		void add_path(Path* path);
		void replace_path(int i, Path* path);
		void remove_path(int i);

		/* Algorithms */
		vector<Path*> test_algorithm();
		void assign_cost(Point source, Point sink);
		void assign_blockers();
		Path* backtrace(Point source, Point sink);
		void remove_cost();
		vector<Path*> post_process();
	};
}

#endif //_PROBLEM_MAP_H
