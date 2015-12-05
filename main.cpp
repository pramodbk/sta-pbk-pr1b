#include<string>
#include<vector>
#include<iostream>
#include<algorithm>
#include<iterator>
#include<fstream>
#include"NetlistGraph.hpp"
#include"ReadNetlist.hpp"

using namespace std;

int main(int argc, char *argv[]) {

if (argc < 3) { 
cerr << "Please provide in format > sta file.in file.out\n" ; 
return 0;
}
string FileInput = argv[1];
string Output = argv[2];

NetlistGraph* myNetlist = new NetlistGraph(); 
	
//Read netlist into a graph
read_netlist(myNetlist,FileInput);

// STA methods
myNetlist->levelize();
myNetlist->calculate_at();
myNetlist->calculate_rat();

//Print all required info from the graph  	
myNetlist->print_info(Output);

return 0;

}
 
