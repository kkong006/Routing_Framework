#include "../Headers/problem_map.h"
#include "../Headers/edge.h"
#include "../Headers/claim.h"

Utilities::ProblemMap::ProblemMap() { };
/*
ProblemMap(ProblemObject* problem_object)

Parameter problem_object(ProblemObject*): problem_object is compiled into a
ProblemMap. Connections, blockers, edges, and nodes are constructed to fill the
ProblemMap.
*/
Utilities::ProblemMap::ProblemMap(ProblemObject* problem_object) {
	this->num_connections = problem_object->get_connections().size();
	this->blockers = problem_object->get_blockers();
	this->connections = problem_object->get_connections();
	int height = problem_object->get_height();
	int width = problem_object->get_width();
	for (int y = 0; y < height; y++) {
		vector<Node*> temp_row;
		vector<bool> temp_block;
		for (int x = 0; x < width; x++)
		{
			Node* new_node = new Node(x, y);
			new_node->set_cost(0);
			if (x > 0) {
				Edge* west = new Edge(new_node, temp_row.at(temp_row.size() - 1));
				new_node->add_connection(west);
				Edge* east = new Edge(temp_row.at(temp_row.size()-1), new_node);
				temp_row.at(temp_row.size()-1)->add_connection(east);
			}
			if (y > 0) {
				Edge* north = new Edge(new_node, map.at(y - 1).at(x));
				new_node->add_connection(north);
				Edge *south = new Edge(map.at(y-1).at(x), new_node);
				map.at(y-1).at(x)->add_connection(south);
			}
			temp_block.push_back(false);
			temp_row.push_back(new_node);
		}
		this->blocker_map.push_back(temp_block);
		this->map.push_back(temp_row);
	}
}

Utilities::ProblemMap::~ProblemMap() {
	int width = this->get_width();
	int height = this->get_height();
	for(int x = 0; x < width; x++) {
		for(int y = 0; y < width; y++) {
			delete map.at(y).at(x);
		}
	}
}

int Utilities::ProblemMap::get_width() {
	return map.empty()?0:map.at(0).size();
}

int Utilities::ProblemMap::get_height() {
	return this->map.size();
}

int Utilities::ProblemMap::get_num_connections() {
	return this->num_connections;
}

Node* Utilities::ProblemMap::get_node(int x, int y) {
	if(y >= this->map.size()) {
		claim("Attemping to access a node outside of the grid's range (y-value out of range)", kError);
		return NULL;
	}
	if(x >= this->map.at(y).size()) {
		claim("Attemping to access a node outside of the grid's range (x-value out of range)", kError);
		return NULL;
	}
	return this->map.at(y).at(x);      //at(...) will throw an out of range exception
}

Node* Utilities::ProblemMap::get_node(Point coord) {
	return this->get_node(coord.x, coord.y);
}

vector<Path*> Utilities::ProblemMap::get_paths() {
	return this->paths;
}

Path* Utilities::ProblemMap::get_path(int i) {
	if(i >= paths.size()) {
		claim("Attempting to access a path outside of the path list's range", kError);
	}
	return this->paths.at(i);           //at(...) will throw an out of range exception
}

void Utilities::ProblemMap::replace_node(Node* replacement_node) {
	delete this->map.at(replacement_node->get_y()).at(replacement_node->get_x());
	this->map.at(replacement_node->get_y()).at(replacement_node->get_x()) = replacement_node;
}

void Utilities::ProblemMap::set_paths(vector<Path*> paths) {
	this->paths = paths;
}

void Utilities::ProblemMap::add_path(Path* path) {
	paths.push_back(path);
}

void Utilities::ProblemMap::replace_path(int i, Path* path) {
	if(i >= this->paths.size()) {
		claim("Attemping to replace path outside of the path list's range", kError);
	}
	paths.at(i) = path;
}

void Utilities::ProblemMap::remove_path(int i) {
	if(i >= paths.size()) {
		claim("Attempting to remove a path outside of the path list's range", kError);
	}
	vector<Path*>::iterator it = this->paths.begin();
	it += i;
	paths.erase(it);
}
/*
vector<Path*> test_algorithm()

return paths(vector<Path*>): Returns vector of Path* from specified sources to sinks.
*/
vector<Path*> Utilities::ProblemMap::test_algorithm() {
	remove_cost();
	assign_blockers();
	for(unsigned i = 0; i<num_connections; ++i) {
		assign_cost(connections.at(i).source, connections.at(i).sink);
		add_path(backtrace(connections.at(i).source, connections.at(i).sink));
		//remove_cost();
	}
	return get_paths();

}
/*
void assign_cost(Point source, Point sink)

Parameter source(Point): source Point that is used as a starting point for
propagating assignment of costs to nodes.
Parameter sink(Point): sink Point that is used as an ending point to test if
the sink has been reached.
*/
void Utilities::ProblemMap::assign_cost(Point source, Point sink) {
	if(!(source == sink)) {
		vector<Node*> curr_ring;
		curr_ring.push_back(get_node(source));
		get_node(source)->set_cost(-1);
		int cost = 1;
		bool done = false;
		while(!done) {
			vector<Node*> next_ring;
			for(unsigned i = 0; i<curr_ring.size(); ++i) {
				for(unsigned j = 0; j<curr_ring.at(i)->connections_size(); ++j) {
					Node* next = curr_ring.at(i)->connections_at(j)->get_tail();
					if(!blocker_map.at(next->get_x()).at(next->get_y())
					&& next->get_cost() == 0) {
						next->set_cost(cost);
						next_ring.push_back(next);
						if(next == get_node(sink)) {
							done = true;
						}
					}
				}
			}
			curr_ring = next_ring;
			cost++;
		}
	}
}
/*
void assign_blockers()
	Assigns blockers to blocker map.
*/
void Utilities::ProblemMap::assign_blockers() {
	for(unsigned i = 0; i<blockers.size(); ++i) {
		int x = blockers.at(i).location.x;
		int y = blockers.at(i).location.y;
		for(unsigned row = 0; row<blockers.at(i).width; ++row) {
			for(unsigned col = 0; col<blockers.at(i).height; ++col) {
				blocker_map.at(row+x).at(col+y) = true;
			}
		}
	}
}
/*
Path * backtrace(Point source, Point sink)

Parameter source(Point): source Point that is used as an ending point for
tracing backwards through Node costs.
Parameter sink(Point): sink Point that is used as a starting point of
backtracing.
*/
Path* Utilities::ProblemMap::backtrace(Point source, Point sink) {
	Path* path = new Path();
	path->set_source(source);
	path->set_sink(sink);
	if(source == sink) {
		path->add_segment(sink, source);
	}
	else {
		Node* source_node = get_node(source);
		Node* curr = get_node(sink);
		bool added_segment = false;
		int curr_cost = curr->get_cost();
		for(int i = get_node(sink)->get_cost(); i>0; --i) {
			added_segment = false;
			if(i==1) {
				path->add_segment(curr->get_coord(), source);
				added_segment = true;
			}
			else {
				for(unsigned j = 0; j<curr->connections_size(); ++j) {
					Node* next = curr->connections_at(j)->get_tail();
					if(next->get_cost() == (curr_cost-1) && !added_segment) {
						path->add_segment(curr->get_coord(), next->get_coord());
						curr = next;
						added_segment = true;
						--curr_cost;
					}
				}
			}
		}
	}
	return path;
}
/*
void remove_cost()
	clears Node* costs after each test case.
*/
void Utilities::ProblemMap::remove_cost() {
	for(unsigned row = 0; row<get_width(); ++row) {
		for(unsigned col = 0; col<get_height(); ++col) {
			get_node(row, col)->set_cost(0);
		}
	}
}
/*
vector<Path*> post_process()

return ret_paths(vector<Path*>): Returns vector of Path* with post-processed
pathsegments.
*/
// vector<Path*> Utilities::ProblemMap::post_process() {
// 	vector<Path*> ret_paths;
// 	for(unsigned i = 0; i<paths.size(); ++i) {
// 		Path* post = new Path();
// 		Point start = paths.at(i)->get_source();
// 		Point end = start;
// 		bool up_down = false;
// 		bool left_right = false;
// 		for(unsigned j = 0; j<paths.at(i)->size(); ++j) {
// 			if(up_down == left_right) {
// 				if(paths.at(i)->at(j)->get_source().x == paths.at(i)->at(j)->get_sink().x) {
// 					end = paths.at(i)->at(j)->get_sink();
// 					up_down = true;
// 				}
// 				else {
// 					end = paths.at(i)->at(j)->get_sink();
// 					left_right = true;
// 				}
// 			}
// 			else if(up_down) {
// 				if(paths.at(i)->at(j)->get_source().x == paths.at(i)->at(j)->get_sink().x) {
// 					end = paths.at(i)->at(j)->get_sink();
// 				}
// 				else {
// 					post->add_segment(start, end);
// 					start = end;
// 					end = paths.at(i)->at(j)->get_sink();
// 					up_down = false;
// 					left_right = true;
// 				}
// 			}
// 			else if(left_right) {
// 				if(paths.at(i)->at(j)->get_source().y == paths.at(i)->at(j)->get_sink().y) {
// 					end = paths.at(i)->at(j)->get_sink();
// 				}
// 				else {
// 					post->add_segment(start, end);
// 					start = end;
// 					end = paths.at(i)->at(j)->get_sink();
// 					up_down = true;
// 					left_right = false;
// 				}
// 			}
// 		}
// 		ret_paths.push_back(post);
// 	}
// 	return ret_paths;
// }
