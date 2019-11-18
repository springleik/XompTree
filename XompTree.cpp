//////////////////////////////////////////////////////
// File named 'XompTree.cpp' implements demonstration of
// binary tree graph, used to simulate user inputs.
//////////////////////////////////////////////////////

// library header files
#include <iostream>
#include <fstream>
#include <cassert>
#include <list>

// library declarations
using std::cout;
using std::cerr;
using std::endl;
using std::ostream;
using std::ifstream;
using std::list;

// project header file
#include "XompTree.h"

// static data members
int node::nodeCount;
int node::theDepth;
int node::maxDepth;
fileStream brnch::brnchFile;
fileStream leaf::leafFile;

// ----------------------------------------------------------
// main entry point
int main(int argc, char *argv[], char *envp[])
{
	// check number of command line arguments
	if (argc < 3)
	{
		cerr << "Usage: XompTree brnch.txt leaf.txt" << '\n'
			<< "Built: " << __DATE__ << '.' << endl;
		return EXIT_FAILURE;
	}

	// open input files
	brnch::brnchFile.open(argv[1]);
	leaf::leafFile.open(argv[2]);
	
	// test for success
	if(!brnch::brnchFile || !leaf::leafFile)
	{
		cerr << "Failed to open input files." << endl;
		return EXIT_FAILURE;
	}

	// generate 25 random outputs on console
	cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<root>" << endl;
	for (int i = 0; i < 25; i++)
	{
		node::nodeCount = 0;
		node::theDepth = 0;
		node::maxDepth = 0;
		
		// construct & randomly populate tree
		node *listHead = new brnch;
		listHead->populate();

		// express tree recursively as text on console
		cout << listHead  << "<!-- " << node::nodeCount << ", " << node::maxDepth << " -->" << endl;
		
		// check that all memory has been freed
		delete listHead;
		assert(!node::nodeCount);
	}

	cout << "</root>" << endl;
	return EXIT_SUCCESS;
}

// required default destructor for base class
node::~node()
{
}

// ----------------------------------------------------------
// randomly populate composite node
void brnch::populate()
{
	int    low = 0;
	int    high = 0;
	theDepth += 1;

	// read parameters from disk file
	while(brnchFile.good())
	{
		brnchFile >> pre >> inter >> post >> low >> high;

		// reset file pointer if EOF reached
		if (brnchFile.eof())
			{brnchFile.rewind();}
		else
			{break;}
	}
	
	// randomly decide whether to add branches or leafs, and how many
	// NOTE: there are 3 different tunable parameters in the if test
	int numChild = (rand() % (high - low + 1)) + low;
	node *newNode = NULL;
	if ((rand() < (RAND_MAX / 3 * 2)) && (nodeCount < 10) && (theDepth < 7))
	{
		while(numChild--)
		{
			// append new composite node
			newNode = new brnch;
			the_list.push_back(newNode);
			newNode->populate();
		}
	}
	else
	{
		while(numChild--)
		{
			// append new leaf node
			newNode = new leaf;
			the_list.push_back(newNode);
			newNode->populate();
		}
	}
	theDepth -= 1;
}

// express composite node and all child nodes on console
void brnch::express(ostream &the_stream)
{
	using namespace std;
	
	// show preamble, but skip '.' chars
	if (*pre != '.'){the_stream << pre;}

	// express child nodes
	list<node *>::iterator i;
	for (i = the_list.begin(); i != the_list.end(); i++)
	{
		// show separator text if more than one child
		if ((i != the_list.begin()) && (*inter != '.'))
			{the_stream << inter;}

		// express child node on console
		the_stream << *i;
	}

	// show postamble, but skip '.' chars
	if (*post != '.'){the_stream << post;}
}

// virtual destructor for derived class
brnch::~brnch()
{
	using namespace std;
	
	// delete child nodes in list
	list<node *>::iterator i;
	for (i = the_list.begin(); i != the_list.end(); i++) {delete *i;}
}

// ----------------------------------------------------------
// construct new leaf node from disk
void leaf::populate()
{
	// fill data member of this
	while(leafFile.good())
	{
		leafFile >> the_str;

		// reset file pointer if EOF reached
		if (leafFile.eof())
			{leafFile.rewind();}
		else
			{break;}
	} 
	
	// update maximum branching depth
	if(myDepth > maxDepth) {maxDepth = myDepth;}
}

// express leaf name on console
void leaf::express(ostream &the_stream)
{
	the_stream << ' ' << the_str << ' ';
}

// count leafs, to be sure they all get deleted
leaf::leaf()
{
	nodeCount += 1;
	myDepth = theDepth;
}

// virtual destructor for leaf class
leaf::~leaf()
{
	nodeCount -= 1;
}

// ----------------------------------------------------------
// insertion operator for node class
ostream &operator<<(ostream &the_stream, node *the_node)
{
	// call virtual method to express node on output stream
	the_node->express(the_stream);
	return the_stream;
}

// helper method added to ifstream class
void fileStream::rewind()
{
	// must clear first for this to work
	clear();
	seekg(0);
}

