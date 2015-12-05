#include<stdio.h>
#include<string>
#include<iostream>
#include<fstream>
#include<algorithm>
#include<iterator>
#include<ctype.h>
#include"NetlistGraph.hpp"

int read_netlist(NetlistGraph* myNetlist, string FileInput) {

// Read netlist

string NetlistFileLine;
vector<string> ValidNetlistLines;
vector<string> gates;

// Read file and store list of all gate names
ifstream NetlistFile (FileInput.c_str());
if (NetlistFile.is_open())
  {
    while ( getline (NetlistFile,NetlistFileLine) )
    {
    // Parse netlist information to C++ internals
        NetlistFileLine = NetlistFileLine.c_str(); 
	// Ignore comments
	string CommentCharacter = "#";
	size_t CommentCharacterPos = NetlistFileLine.find(CommentCharacter);
	if (CommentCharacterPos!=string::npos) { 
	   NetlistFileLine.replace(NetlistFileLine.begin()+CommentCharacterPos,NetlistFileLine.end(),"");
	}
			
	// Trim
	NetlistFileLine.erase(remove(NetlistFileLine.begin(), NetlistFileLine.end(), ' '), NetlistFileLine.end());
	NetlistFileLine.erase(remove(NetlistFileLine.begin(), NetlistFileLine.end(), '\n'), NetlistFileLine.end());
	NetlistFileLine.erase(remove(NetlistFileLine.begin(), NetlistFileLine.end(), '\r'), NetlistFileLine.end());
	NetlistFileLine.erase(remove(NetlistFileLine.begin(), NetlistFileLine.end(), '\t'), NetlistFileLine.end());

	// Ignore null lines
	if ( NetlistFileLine.empty() ) { continue; }

	// Primary Inputs
	if (NetlistFileLine.compare(0,6,"INPUT\(") == 0 ) {
		ValidNetlistLines.push_back(NetlistFileLine);
		string InputGate = NetlistFileLine.substr(6,NetlistFileLine.size());
		InputGate.erase(remove(InputGate.begin(), InputGate.end(), ')'), InputGate.end());
		gates.push_back(InputGate);
		continue;
	}

	// Primary Outputs
	if (NetlistFileLine.compare(0,7,"OUTPUT\(") == 0 ) {
		ValidNetlistLines.push_back(NetlistFileLine);
		string OutputGate = NetlistFileLine.substr(7,NetlistFileLine.size());
		OutputGate.erase(remove(OutputGate.begin(), OutputGate.end(), ')'), OutputGate.end());
		gates.push_back(OutputGate);
		continue;
	}
	
	// Connections
	if (NetlistFileLine.find("=") != string::npos) {
		ValidNetlistLines.push_back(NetlistFileLine);
		stringstream gate_s(NetlistFileLine);
		string gate_n,gate_t,gate_i;
		getline(gate_s,gate_n,'=');
		getline(gate_s,gate_t,'(');
		getline(gate_s,gate_i,')');
		
		stringstream gate_str(gate_i);
		string gate_input;
		gates.push_back(gate_n);
		while(getline(gate_str,gate_input,',')) {
			gates.push_back(gate_input);
		}
		continue;
	}
		
	cout << NetlistFileLine << "  : not a valid input line syntax, Please check \n" ;
	return 0;	
    }
    NetlistFile.close();
}

//Unique sort gates
sort( gates.begin(), gates.end() );
gates.erase( unique( gates.begin(), gates.end() ), gates.end() );    

// Add placeholder nodes for all gates encountered
for(size_t i = 0; i < gates.size(); i++){
myNetlist->add_gate(gates[i]);
}

for(size_t i = 0; i < ValidNetlistLines.size(); i++){
        // Parse netlist information to C++ internals
	string ValidNetlistFileLine = ValidNetlistLines[i];
	// Primary Inputs
	if (ValidNetlistFileLine.compare(0,6,"INPUT\(") == 0 ) {
		string InputGate = ValidNetlistFileLine.substr(6,ValidNetlistFileLine.size());
		InputGate.erase(remove(InputGate.begin(), InputGate.end(), ')'), InputGate.end());
		//cout << InputGate << '\n';
		myNetlist->add_gate(InputGate,"InputDefault","");
		continue;
	}

	// Primary Outputs
	if (ValidNetlistFileLine.compare(0,7,"OUTPUT\(") == 0 ) {
		string OutputGate = ValidNetlistFileLine.substr(7,ValidNetlistFileLine.size());
		OutputGate.erase(remove(OutputGate.begin(), OutputGate.end(), ')'), OutputGate.end());
		//cout << OutputGate << '\n';
		myNetlist->add_gate(OutputGate+"_output");
		myNetlist->add_gate(OutputGate+"_output","OutputDefault",OutputGate);
		continue;
	}
	
	// Connections
	if (ValidNetlistFileLine.find("=") != string::npos) {
		stringstream gate_s(ValidNetlistFileLine);
		string gate_n,gate_t,gate_i;
		getline(gate_s,gate_n,'=');
		getline(gate_s,gate_t,'(');
		getline(gate_s,gate_i,')');
	  	
		myNetlist->add_gate(gate_n,gate_t,gate_i);
		continue;
	}

	cout << ValidNetlistFileLine << "  : not a valid input line syntax, Please check \n" ;
	return 0;	
        //cout << ValidNetlistFileLine << ":" << gate_index << '\n';
}

// End read netlist 
}
