#include<map>
#include<vector>
#include<algorithm>
#include<string>
#include<iterator>
#include<ctype.h>
#include<sstream>
#include<iostream>
#ifndef NODE_H
#define NODE_H

using namespace std;
class node{
 public:   
	node(string gate_name);				// constructor
	~node() {};                                 	// destructor

	// Node state variables
  	vector<node*> Inputs;			
  	vector<node*> Outputs;
   	string GateName;
	string GateType;
	int GateIndex;
	bool isPO;
 	bool isPI;	
	
	void add_input(node* input);
	
	// Timing variables	
	int StageDelay;
	int AT;
	int RAT;
	int slack;
	
	// Timing methods
	void set_AT();
	void set_RAT();
	void set_slack();

	// Traversal variables
	bool Visited;
	int VisitedInputs;

};

#endif
