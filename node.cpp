#include<map>
#include<vector>
#include<algorithm>
#include<string>
#include<iterator>
#include<ctype.h>
#include<sstream>
#include<iostream>
#include"node.hpp"

using namespace std;

node::node(string gate_name) 
{
	GateName = gate_name;  
	isPO = false;
	isPI = false;
	VisitedInputs = 0;
	Visited = false;
	AT = -1;
};   						// constructor
void node::add_input(node* input)
{
	Inputs.push_back(input);
	input->Outputs.push_back(this);
}						// add input

void node::set_AT() { 
	
	// Output delay is the fanout
	StageDelay = Outputs.size();
	int max_ip_AT = 0;
	// AT is the max of input AT + stage delay
	for (int i=0; i<Inputs.size(); i++) {
 		if (Inputs[i]->AT > max_ip_AT) { 
			max_ip_AT = Inputs[i]->AT;
		}
	}
	AT = StageDelay + max_ip_AT;
}

void node::set_RAT() { 
	int min_ip_RAT = 2147483647;
	//cout << "RAT CALC for node" << GateIndex << "\n" ;
	// RAT is min of output (RAT - stage delay)
	for (int i=0; i<Outputs.size(); i++) {
		//cout << "\t OP:  node" << Outputs[i]->GateIndex <<" : " <<  Outputs[i]->RAT << " - " << Outputs[i]->StageDelay << "\n";
 		int new_RAT = Outputs[i]->RAT - Outputs[i]->StageDelay;
		if (new_RAT < min_ip_RAT) { 
			min_ip_RAT = new_RAT;
		}
	}
	RAT = min_ip_RAT;
	//cout << "RAT = " << min_ip_RAT << "\n\n" ;
}

void node::set_slack() { 
	slack = RAT - AT;
}

