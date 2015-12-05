#include<map>
#include<vector>
#include<algorithm>
#include<string>
#include<iterator>
#include<ctype.h>
#include<sstream>
#include<iostream>
#include"node.hpp"

#ifndef NETLISTGRAPH_H
#define NETLISTGRAPH_H

using namespace std;
class NetlistGraph {
    public: 
	NetlistGraph() { gate_index=0; };

	// Create a map of gate names to gate nodes  
    	map<string,node*> node_map; 
    	map<int,node*> node_map_by_index;

	vector<int> level_order; 

 	int gate_index ;	

	// Max delay in the netlist
	int MaxDelay;	

	// Store the index of each input/output in the order that they are read
    	vector<int> input_map; 
    	vector<int> output_map; 
	
	void add_gate(string gate_name,string gate_type,string inputs);
	void add_gate(string gate_name);

	void print_info(string FileOutput);
	
	// STA methods
	void levelize();

	void calculate_at();
	
	void calculate_rat();

};

#endif
