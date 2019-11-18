//////////////////////////////////////////////////////
// File named 'XompTree.h' is the header file for demo
// of binary tree graph, used to simulate user inputs.
//////////////////////////////////////////////////////

// helper function added to standard class
class fileStream: public ifstream
{
public:
	void rewind();
};

// abstract base class to compose objects into a tree structure
class node
{
public:
	static int theDepth;
	static int nodeCount;
	static int maxDepth;
	virtual ~node();	

	// pure virtual functions
	virtual void populate() = 0;
	virtual void express(ostream &) = 0;
};

// composite class, may have child nodes
class brnch: public node
{
private:
	char   pre[32];
	char inter[32];
	char  post[32];
	list<node *> the_list;

public:
	static fileStream brnchFile;
	~brnch();
	void populate();
	void express(ostream &);
};

// component class, no child nodes
class leaf: public node
{
private:
	char the_str[32];
	int myDepth;

public:
	static fileStream leafFile;
	leaf();
	~leaf();
	void populate();
	void express(ostream &);
};

// insertion opeartor for node class
ostream &operator<<(ostream &, node *);
