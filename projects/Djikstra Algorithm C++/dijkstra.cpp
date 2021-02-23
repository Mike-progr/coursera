
#include<iostream>
#include<cstdlib>
#include<ctime>
#include<string>
#include<sstream>
#include<queue>
#include<bits/stdc++.h>
#include<limits>

using namespace std;

typedef pair<int, int> iPair; 

class Graph {
	
	public:
		
		Graph(int max_d, float den): max_distance(max_d),density(den) {
			
			srand(time(0));
			
			vertices = 5 + rand()%6;   //at least 5 vertices
			graph = new int*[vertices];
			
			edges = 0;
			
			for(int i = 0; i < vertices; i++) {
				
				graph[i] = new int[vertices];
			}
			
			for(int i = 0; i < vertices; i++) {
				
				for(int j = i; j < vertices; j++) {
					
					if((i!=j) && (((float) rand() /(RAND_MAX)) < den))  {
						
						graph[i][j] = graph[j][i] = rand() % max_d + 1;  
                        edges++;
					}
					else {
						
						graph[i][j] = graph[j][i] = 0;    // edges not connected
					}
				}
			}
			
			for(int i = 0; i < vertices; i++) {
				
				cout << i << "->{";
				
				for(int j = 0; j < vertices; j++) {
					
					cout << graph[i][j] << ",";
				}
				
				cout << "}\n";
			}
		}
		
		int V() {
			
			return(vertices);
		}
		
		int E() {
			
			return(edges);
		}
		
		bool adjacent (int x, int y) {
			
			if(graph[x][y] > 0) {
				
				return(true);
			}
			
			return(false);
		}
		
		std::string toString(auto &i){
   		
			std::stringstream ss;
   			ss << i;
 
   			return ss.str();
		}
		
		string neighbors(int x) {
			
			string str="Neighbor vertices of " + toString(x) + ":";
			
			for(int i = 0; i < vertices; i++) {
				
				if(graph[x][i] > 0) {
					
					str = str + toString(i) + ",";
				}
			}
			
			return(str.substr(0, str.size()-1));
		}
	
		void add_edge(int x, int y) {
			
			graph[x][y] = graph[y][x] = rand() % max_distance + 1;
			edges++;
		}
		
		void delete_edge(int x, int y) {
			
			graph[x][y] = graph[y][x] = 0;
			edges--;
		}
		
		int get_edge_value(int x,int y) {
			
			return(graph[x][y]);
		}
		
		int set_edge_value(int x,int y, int v) {
			
			if(graph[x][y] > 0) {
				
				graph[x][y] = graph[y][x] = v;
			}
			else {
				
				graph[x][y] = graph[y][x] = v;
				edges++;
			}
		}
		
		int get_node_value (int x) {
			
			int value = 0;
			
			for(int i = 0; i < vertices; i++) {
				
				value = value + graph[i][x];
			}
		}
		
		int **ret_graph() {
			
			return(graph);
		}
		
	private:
		
		int vertices; //vertices name 0,1,2,3,.......
		int edges;
		int **graph;
		int max_distance;
		float density;
};

class PriorityQueue {
	
	public:
		
		void Insert(iPair pair1) {
			
			pri_que.push(pair1);
		}
		
		int return_size() {
				
			return(pri_que.size());
		}
		
		int return_TopandPop() {
			
			int node = pri_que.top().second;
			pri_que.pop();
			
			return(node);
		}
	
	private:
		
		priority_queue <iPair, vector <iPair>, greater<iPair> > pri_que;
};

class ShortestPath {
	
	public:
	
		ShortestPath(Graph g, int source,int destination) {
			
			int vertices = g.V();
			int **graph = g.ret_graph();
			
			src = source;
			dest = destination;
			
			adj_list = new list<iPair> [vertices];
			complete = false;
			path = new int[vertices];
			distance = new int[vertices];
			
			for(int i = 0; i < vertices; i++) {
				
				for(int j = i; j < vertices; j++) {
			
					if(i == j || graph[i][j] == 0) {
						
						continue;
					}
					
					adj_list[i].push_back(make_pair(j,graph[i][j]));
					adj_list[j].push_back(make_pair(i,graph[i][j]));		
				}
				
				distance[i] = INT_MAX;
			}
	
			distance[src] = 0;
			q.Insert(make_pair(0,src));   //distance,node
			path[src] = -1;
		}
		
		void path_size() {
			
			int node;
			
			if(!complete) {
				
				while(q.return_size() > 0) {
					
					node = q.return_TopandPop();
					
					list<iPair>::iterator i;
					
					for(i = adj_list[node].begin(); i != adj_list[node].end(); i++) {
						
						if(distance[(*i).first] > distance[node] + (*i).second) {
							
							distance[(*i).first] = distance[node] + (*i).second;
							path[(*i).first] = node;
							
							q.Insert(make_pair(distance[(*i).first],(*i).first));
						}
					}
				}
			}
			
			cout << "The path size from " << src << " to " << dest << " is " << distance[dest] << "\n";
		}
		
		std::string toString(auto &i){
   		
			std::stringstream ss;
   			ss << i;
 
   			return ss.str();
		}
		
		void print_str(string str) {
		
	
   			for (int i=str.length()-1; i>=0; i--) {
      
	  			cout << str[i];
			}
		}		 
		
		void path_nodes() {
			
			int pntr = dest;
			string str = toString(dest);
			
			cout << "The path from " << src << " to " << dest << " is" << ":";
			
			while(true) {
				
				if(path[pntr] == -1) {
					
					break;
				}
				
				str = str + toString(path[pntr]);
				
				pntr = path[pntr];
			}
			
			print_str(str);
		}
	
	private:
		
		int *path;
		int src,dest;
		list<iPair> *adj_list;
		PriorityQueue q;
		int *distance;
		bool complete;
		
		
};

int main() {
	
	int source,destination;
	
	Graph G(10,0.65); //max distance between two edges and density respectively
	
	cout << "Enter source node:";
	
	cin >> source;
	
	cout << "Enter destination node:";
	
	cin >> destination;
	
	ShortestPath sp(G,source,destination);
	
	sp.path_size();
	
	sp.path_nodes();
	
	cout << "\n\n   The following algorithm is an implementation of Dijkstra's Shortest Path \n";
	cout << "At first we build the graph via the Graph constructor. We make sure that it has \n";
	cout << "at least five (5) vertices and the edges are provided randomly. The number of \n";
	cout << "both varies through different executions of the program. Class Graph has some methods \n";
	cout << "like add_edge (adds edge to the Graph) V() (return the number of vertices) etc. \n";
	cout << "Then the user is asked to provide the source and destination node which are given as \n";
	cout << "parameters along with the Graph to the constructor of the ShortestPath Class. \n";
	cout << "This Class through its methods creates the nessecary data structures along with the \n";
	cout << "PriorityQueue Class (adjecent list, priority queue etc) that store distance and node values \n";
	cout << "in oreder to find the shortest path and its distance.";
	        
	return(0);
}
