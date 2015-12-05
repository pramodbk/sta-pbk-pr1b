#include<map>
#include<vector>
#include<queue>
#include<algorithm>
#include<string>
#include<iterator>
#include<ctype.h>
#include<sstream>
#include<iostream>
#include<fstream>
#include"node.hpp"
#include"NetlistGraph.hpp"

using namespace std;

// function to create placeholder nodes
void NetlistGraph::add_gate(string gate_name) {
	node *new_gate = new node(gate_name);
	node_map[gate_name] = new_gate;
	new_gate->GateIndex = -1;
}
// function to add a gate to the graph along with input/output connections	
void NetlistGraph::add_gate(string gate_name,string gate_type,string inputs){
	    // Assign index to nodes
	    if ( gate_type == "OutputDefault") { 
		// Output node has only 1 input
	        output_map.push_back(gate_index);
	    	node_map[gate_name]->add_input(node_map[inputs]);	
	        node_map[gate_name]->GateType=gate_type;
	    	node_map[gate_name]->GateIndex = gate_index;
		node_map[gate_name]->isPO = true;
	    	node_map_by_index[gate_index] = node_map[gate_name];
	    	gate_index++;
		return;
	    }
	    // No inputs for input node 
	    if ( gate_type == "InputDefault") { 
	        input_map.push_back(gate_index);
		node_map[gate_name]->isPI = true;
	    }
	    if (node_map[gate_name]->GateIndex == -1) { 
	    	node_map[gate_name]->GateIndex = gate_index;
	    	node_map_by_index[gate_index] = node_map[gate_name];
	    	gate_index++;
	    }	
	    stringstream gate_s(inputs);
	    string node_s;
	    // Create/assign inputs to the node
	    while(getline(gate_s,node_s,',')) {
	    	if (node_map[node_s]->GateIndex == -1) { 
	    	node_map[node_s]->GateIndex = gate_index; 
	    	node_map_by_index[gate_index] = node_map[node_s];
	    	gate_index++;
	    	}
	    	node_map[gate_name]->add_input(node_map[node_s]);	
	    }
	    // Track gate types
	    node_map[gate_name]->GateType=gate_type;
}


void NetlistGraph::levelize() {
		
	queue<int> to_levelize;

	// Start with all primary inputs
	for(size_t i = 0; i < input_map.size(); i++){
	    to_levelize.push(input_map[i]);
	}
	while ( !to_levelize.empty() ) { 
		int node_x = to_levelize.front();
		to_levelize.pop();
		// If all inputs are already visited
		if ( node_map_by_index[node_x]->Visited == false) {
		if ( node_map_by_index[node_x]->VisitedInputs == node_map_by_index[node_x]->Inputs.size() ) {
			level_order.push_back(node_x);
			
			// Add all outputs of the node to queue
			vector<node*> nodes_to_add = node_map_by_index[node_x]->Outputs;
			for(size_t i = 0; i < nodes_to_add.size(); i++){
			   //cout << node_x << ": "<< nodes_to_add[i]->GateIndex << " " << "\n";
		 	   to_levelize.push(nodes_to_add[i]->GateIndex);
			   // Increment number of visited inputs for output nodes
			   nodes_to_add[i]->VisitedInputs++;
			}
			node_map_by_index[node_x]->Visited = true;
		} else {
			to_levelize.push(node_x);
		}
		}
	}
}

void NetlistGraph::calculate_at() {
	// Arrival time calculation starting from inputs 
	for(size_t i = 0; i < level_order.size(); i++){
		// Calculate stage delay
		node* node_x = node_map_by_index[level_order[i]];
		node_x->set_AT();		
	}
}

void NetlistGraph::calculate_rat() {
	
	// MaxDelay = Worst arrival time among output nodes	
	MaxDelay = 0;	
	for(size_t i = 0; i < output_map.size(); i++){
		node* node_x = node_map_by_index[output_map[i]];
		if (node_x->AT > MaxDelay) { 
			MaxDelay = node_x->AT;
		}
	}

	// Set RAT for outputs as MaxDelay	
	for(size_t i = level_order.size(); i > 0 ; i--){
		// Calculate RAT
		node* node_x = node_map_by_index[level_order[i-1]];
		if (node_x->isPO) {
		node_x->RAT = MaxDelay;
		} else {
		node_x->set_RAT();
		}		
		node_x->set_slack();
	}
}


void NetlistGraph::print_info(string FileOutput) {
    ofstream OFile (FileOutput.c_str());
    if (OFile.is_open()) { 

	// Print MaxDelay
	OFile << MaxDelay << "\r\n"; 	

	// Print PI
	int numInputs=input_map.size();
	OFile << numInputs << " " ;
	for (int i=0; i<input_map.size(); i++) {
	OFile << input_map[i] << " ";
	}
	OFile << "\r\n" ;


	// Print PO
	int numOutputs=output_map.size();
	OFile << numOutputs << " ";
	for (int i=0; i<output_map.size(); i++) {
	OFile << output_map[i] << " " ;
	}
	OFile << "\r\n" ;
	
	// Print timing info
	for (int i=0; i<node_map_by_index.size(); i++) { 
		node* node_x = node_map_by_index[i];
		OFile << i << " " << node_x->AT << " " << node_x->slack << "\r\n";
	}	
	OFile << "\r\n" ;
   	 
	} else { 
	cerr << "Unable to open file for writing\n" ;
    }
}

