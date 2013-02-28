/******************|********************|*******************|******************|
 *  																		   *
 *								ROUTER ALGORITHM							   *
 *																			   *
 *		PREPARED BY: 	Bennett D. Clarke									   *
 *						University of Illinois ( Chicago )					   *
 *						Chicago, Il.  60610									   *
 *		Date:			February 22, 2013									   *
 *																			   *
 *******************************************************************************
 *																			   *
 *	OBJECTIVE:	   	To use a data set to create an adjacency matrix of a graph *
 *					of n vertices and m edges. The user will then choose a     *
 *					source vertex and the probram will produce output that 	   *
 *					identifies the shortest path from the source to each of    *
 * 				   	the other vertices. 							   		   *
 *																			   *
 *	METHODOLOGY:	Use Dykstra's algorithm with uniform cost of 1 per router  *
 *					hop ( 1 per edge ).										   *
 *******************************************************************************/ 
 
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
// DECLARATIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef unsigned int vertex;

// VARIABLES

struct DataLineIn{

	vertex vertexANo;
	vertex vertexBNo;
	double weight;
	
	DataLineIn( vertex initVertexANo, vertex initVertexBNo, double initWeight )
		: vertexANo( initVertexANo ), vertexBNo( initVertexBNo ), weight( initWeight ) 
	{}	


};

struct Node{

	DataLineIn 	data;
	Node *		next;
	
	Node( DataLineIn initData )
		: data ( initData )
	{
		next = NULL;
	}
	
	Node( DataLineIn initData, Node * initNext )
		: data ( initData ), next ( initNext )
	{}	
};

struct EdgeQueue{
	
		Node * 		head;
		Node * 		tail;
		
		EdgeQueue( )
		{
			head = NULL;
			tail = NULL;
		}

};

struct TreeNode{
	
	vertex 									vertexNo;
	double									distance;
	std::map < vertex, TreeNode * > 		childMapPtr;
	TreeNode * 								parent;
	
	TreeNode( vertex initVertexNo, double initDistance )
		: vertexNo ( initVertexNo ), distance ( initDistance )
	{
		parent = NULL;
	}	

};

struct Queue{

	TreeNode *				head;
	TreeNode * 			tail;
	
	Queue( )
	{
		head = NULL;
		tail = NULL;
	}
	
};

std::vector < DataLineIn * > DataIn;

static void show_usage(std::string fileName)
{
    std::cerr << "Usage: " << fileName << " <option(s)> DATA INPUT FILE  " << "Options:\n"
            << "\t-h,--help\t\tShow this help message\n"
            << "\t-d,--data_file_path \tSpecify the data inputs path"
            << std::endl;
}




int main ( int argc, char ** argv ){

	if (argc < 3) {
        show_usage(argv[0]);
        return 1;
    }
    std::string dataIn_Name = argv[ 1 ];
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            return 0;
        } else if ((arg == "-d") || (arg == "--data_file_path")) {
            if (i + 1 < argc) { 
                dataIn_Name = argv[++i];
            } else {
                std::cerr << "--data_file_path option requires one argument." 
                			<< std::endl;
                return 1;
            }  
        }    
	}
	std::cout << "datafilename is: " << dataIn_Name << std::endl;
	int mVertices ( 0 );
	int nEdges ( 0 );
	int a, b;
	double c;
	std::string line;
	std::ifstream dataIn;
	dataIn.open ( dataIn_Name.c_str() );
	if ( dataIn.is_open() )
  	{
    	dataIn >> mVertices >> nEdges;
    	std::cout << "mVertices: " << mVertices << ", nEdges: " << nEdges << std::endl;
    	unsigned int dataCount ( 0 );
    	while ( dataCount < nEdges )
    	{
      		dataIn >> a >> b >> c;
      		std::cout << "a: " << a << ", b: " << b << ", c: " << c << std::endl;
      		DataLineIn *ptr = new DataLineIn( a, b, c );
      		// Enqueue new data element ptr to vector
      		DataIn.push_back( ptr );
      		++dataCount;
    	}
    	dataIn.close();
    	std::cout << "DataIn contains " << DataIn.size() << " records." << std::endl;
  	}
	else std::cout << "Unable to open file";
	
	// INITIALIZE TO AN INFINITE VALUE
	double adjacency [ mVertices ][ mVertices ];
	for ( int iCount = 0; iCount < mVertices; ++iCount ){
		for ( int jCount = 0; jCount < mVertices; ++jCount ){
			adjacency[ iCount ][ jCount ] = 1000000.0;
		}	
	}
	
	// CREATE EDGE TABLE AND INITIALIZE
	Node * EdgeTable [ mVertices ];
	for ( int iCount = 0; iCount < mVertices; ++iCount ){
		EdgeTable [ iCount ] = NULL;
	}	
	
	// LOAD GRAPH DATA INTO ADJACENCY MATRIX 
	int kCount = 0;
	while( kCount < DataIn.size() ){
		adjacency[ DataIn[ kCount ]->vertexANo ][ DataIn[ kCount ]->vertexBNo ] = 
														DataIn[ kCount ]->weight;											
 		++kCount;
	} // END WHILE
	
	// PRINT ADJACENCY TABLE
	std::cout << "Adjacency Matrix:" << std::endl;
	std::cout << "  ";
	for( int iCount = 0; iCount < mVertices; ++iCount ){
		std::cout << "   v" << iCount;
	}
	std::cout << std::endl;
	for ( int iCount = 0; iCount < mVertices; ++iCount ){
		std::cout << "v" << iCount;
		for( int jCount = 0; jCount < mVertices; ++jCount ){
			if( adjacency[ iCount ][ jCount ] > 999999.9 )
				std::cout << "    " << "-" ;
			else
				std::cout << "    " << adjacency[ iCount ][ jCount ];
		}
		std::cout << std::endl;
	}
	
	kCount = 0;
	while ( kCount < DataIn.size() ){
			
		// EDGE TABLE
		DataLineIn dat = *DataIn[ kCount ];
		Node * nPtr = new Node( dat );
		Node * traveller;
		if ( !EdgeTable[ DataIn[ kCount ]->vertexANo ] ){
			EdgeTable[ DataIn[ kCount ]->vertexANo ]  = nPtr;
		} else
		{
			traveller = EdgeTable[ DataIn[ kCount ]->vertexANo ];
			while ( traveller->next ){
				traveller = traveller->next;
			}
			traveller->next = nPtr;	
		}		// END IF-ELSE BLOCKS	
		++kCount;
	
	} // END WHILE
	
	std::cout << "Edge Table:" << std::endl;
	for ( int iCount = 0; iCount < mVertices; ++iCount ){
		std::cout << iCount << ":  " ;
		Node * traveller;
		traveller = EdgeTable[ iCount ];
		while  ( traveller ){
			std::cout << iCount << "->" << traveller->data.vertexBNo << "   " ;
			traveller = traveller->next;
		}
		std::cout << std::endl;
	}
	
	// TRANSFORM TO ARRAY OF VERTICES AT HEAD OF LINKED LISTS
	std::cout << "Enter integer identifier of single source node: ";
	int singleSourceId ( -1 );
	std::cin >> singleSourceId;
	
	// CREATE A SINGLE SOURCE DATALINE NODE 
	DataLineIn * initDataLine = new DataLineIn( singleSourceId, singleSourceId, 0.0 );
	Node * initNode = new Node( *initDataLine );
	
	// CREATE A ROOT NODE FOR USE IN THE EXPLORED QUEUE
	TreeNode * root = new TreeNode( singleSourceId, 0.0 );
	root->parent = NULL;
	std::map< vertex, TreeNode* > * mapPtr = new std::map<vertex, TreeNode* >();
	root->childMapPtr = *mapPtr;
	
	
	// SET EXPLORED NODE QUEUE
	Queue *exploredPtr = new Queue();
	
	// INITIALIZE ARRAY TO HOLD DISTANCE BY NODE ID
	double distanceToS [ mVertices ];
	for ( int iCount = 0; iCount < mVertices; ++iCount ){
		distanceToS[ iCount ] = -1.0;
	}
	
	// ENTER 0 DISTANCE FOR THE SOURCE NODE
	distanceToS[ singleSourceId ] = 0.0;	
	
	// SET ARRAY TO HOLD PTR TO PARENT'S TREENODE
	TreeNode * parentNodeArray[ mVertices ];
	for ( int iCount = 0; iCount < mVertices; ++iCount ){
		parentNodeArray[ iCount ] = NULL;
	}
	
	//INITIALIZE THE EXPLORING QUEUE WITH THE SINGLE SOURCE
	EdgeQueue *exploringPtr = new EdgeQueue();
	exploringPtr->head = initNode;
	std::cout << "adding to exploring queue: " << exploringPtr->head->data.vertexANo
				<< std::endl;			
	
int xCount = 0;
while( exploringPtr->head ){
	// FIND CLOSEST NODE
	Node * trav;
	trav = exploringPtr->head;
	Node * followNode;
	Node * preMinNode;
	followNode = exploringPtr->head;
	double minDist ( 1000000.0 );
	Node * minNode; 
	while ( trav ){
		std::cout << "Checking for min with: " << trav->data.vertexBNo << std::endl;
		if( distanceToS[ trav->data.vertexANo ] < 0.0 ){
			std::cout << "ERROR - Negative distance" << std::endl;
		} else {
			 if ( trav->data.weight + distanceToS[trav->data.vertexANo ] < minDist ) {
				minDist = trav->data.weight + distanceToS[ trav->data.vertexANo ];
				std::cout << "New min is Node : " << trav->data.vertexBNo << std::endl;
				minNode = trav;
				preMinNode = followNode;
			 }
		 }
		 // INCREMENT ITERATORS SO THAT FOLLOWNODE IS IMMEDIATELY BEHIND trav
		 if ( trav != exploringPtr->head )	
		 	followNode = trav;
		trav = trav->next; 
	}
	std::cout << "closest node: " << minNode->data.vertexBNo << ", distance: "
				<< minDist << std::endl;	
	
	// REMOVE CLOSEST NODE FROM EXPLORING QUEUE
	if ( minNode == exploringPtr->head ){
		exploringPtr->head = exploringPtr->head->next;
	} else {
		preMinNode->next = minNode->next;
	}
	if ( minNode == exploringPtr->tail ){
		exploringPtr->tail = preMinNode;
	}	
	std::cout << "Deleted from exploring queue node: " <<  minNode->data.vertexBNo 
				<< std::endl;
	
	// PURGE ALL INSTANCES OF REMOVED NODE (THEY ARE GREATER DISTANCE )
	trav = exploringPtr->head;
	followNode = exploringPtr->head;
	while( trav ){
		if ( trav->data.vertexBNo == minNode->data.vertexBNo ){
			if( trav == exploringPtr->head ){
				exploringPtr->head = exploringPtr->head->next;
			} else {
				followNode->next = trav->next;			
			}
			if ( trav == exploringPtr->tail ){
				exploringPtr->tail = followNode;
			}		
		}
		// INCREMENT ITERATORS SO THAT FOLLOWNODE IS IMMEDIATELY BEHIND trav
		 if ( trav != exploringPtr->head )	
		 	followNode = trav;
		trav = trav->next;
	}				

	// ADD REMOVED NODE TO THE EXPLORED QUEUE
		// IF EXPLORED NODE IS EMPTY MAKE THE FIRST NODE (SINGLE SOURCE ) ROOT
	if ( !exploredPtr->head ){
		exploredPtr->head = root;
		exploredPtr->tail = root;
		root->childMapPtr[ singleSourceId ] = root;
		std::cout << "Adding to explored queue root: " << exploredPtr->head->vertexNo
				<< std::endl;
				
		// SET ( LEAVE ) PARENTNODE QUEUE TO NULL
		parentNodeArray[ exploredPtr->head->vertexNo ] = root;
	}
		// IF EXPLORED NODE IS NOT EMPTY ADD A NEW NODE
		// ADD NEWEST NODE TO TREE
	else {
		TreeNode * nwNodePtr = 
						new TreeNode( minNode->data.vertexBNo, minNode->data.weight );
		
		// UPDATE PARENTNODEARRAY FOR NEW EXPLORED NODE
		parentNodeArray[ nwNodePtr->vertexNo ] = nwNodePtr;

		// FIND NWNODEPTR'S PARENT NODE FROM PARENTNODEARRAY AND 
		// ADD PARENT PTR TO NEW NODE, THEN ADD CHILD PTR TO PARENT
		// -node of parent
		nwNodePtr->parent = parentNodeArray[ minNode->data.vertexANo ];
		std::cout << "Parent Node is: " << nwNodePtr->parent->vertexNo << std::endl;
		// Parent adds ptr to child (new addition)
		nwNodePtr->parent->childMapPtr[ nwNodePtr->vertexNo ] = nwNodePtr;
		std::cout << "parent: " << nwNodePtr->parent->vertexNo << " has child in node: "
					<< nwNodePtr->parent->childMapPtr[ nwNodePtr->vertexNo ]->vertexNo
					<< std::endl;
		TreeNode * tnTrav = nwNodePtr->parent;			
		while( tnTrav->parent  ){
			tnTrav->parent->childMapPtr[ nwNodePtr->vertexNo ] = tnTrav;
			std::cout << "parent: " << tnTrav->parent->vertexNo 
					<< "has child on route to " << nwNodePtr->vertexNo << " in node: " 
					<< tnTrav->vertexNo << std::endl;
			tnTrav = tnTrav->parent;
		}
		
		// minNode->data.vertexANo as treenode.vertexNo
	} // END IF ELSE BLOCK
	
	// PRINT TREE
	std::cout << "root: " << root->vertexNo << std::endl;
	int iCount = 0;
	
	// SET DISTANCE OF NODE ADDED TO EXPLORED QUEUE
	distanceToS[ minNode->data.vertexBNo ] = minDist;
	std::cout << "Setting total distance for node: " << minNode->data.vertexBNo
				<< " at " << minDist << std::endl;
				
	// PRINT EXPLORING QUEUE
	std::cout << "Exploring Queue after purge before expansion: ";
	trav = exploringPtr->head;
	while ( trav ){
		std::cout << trav->data.vertexBNo << ", " ;
		trav = trav->next;
	}
	std::cout << std::endl;				
	
	// EXPAND CHOSEN NODE IN EXPLORING QUEUE AND ADD TO EXPLORING NODE QUEUE	
	// ADD ALL NODES REACHABLE FROM MINNODE (BUT NOT IN EXPLORED QUEUE )
	// TO THE EXPLORING QUEUE
	// minNode->data

	trav = EdgeTable[ minNode->data.vertexBNo ];
	while( trav ){
		if ( distanceToS[ trav->data.vertexBNo ] < 0.0 ){
			std::cout << "have entry to add" << std::endl;
			if ( !exploringPtr->head ){
				exploringPtr->head = trav;
				exploringPtr->tail = trav;
				std::cout << "Adding at head of queue" << std::endl;
			} else {
				exploringPtr->tail->next = trav;
				exploringPtr->tail = trav;
			}
			std::cout << "edge from: " << trav->data.vertexANo << " to: " 
						<< trav->data.vertexBNo << std::endl;			
		}			
		trav = trav->next;		
	}
	// PRINT EXPLORING QUEUE
	std::cout << "Exploring Queue: ";
	trav = exploringPtr->head;
	while ( trav ){
		std::cout << trav->data.vertexBNo << ", " ;
		trav = trav->next;
	}
	std::cout << std::endl;	
	
	// PRINT DISTANCE ARRAY
	for( int iCount = 0; iCount < mVertices; ++iCount ){
		std::cout << iCount << ": " << distanceToS[ iCount ] << std::endl;
	}	
	
	// PURGE ALL INSTANCES OF REMOVED NODE (THEY ARE GREATER DISTANCE )
	trav = exploringPtr->head;
	followNode = exploringPtr->head;
	while( trav ){
		if ( distanceToS[ trav->data.vertexBNo ] >= 0.0 ){
			if( trav == exploringPtr->head ){
				exploringPtr->head = exploringPtr->head->next;
			} else {
				followNode->next = trav->next;			
			}
			if ( trav == exploringPtr->tail ){
				exploringPtr->tail = followNode;
			}		
		}
		// INCREMENT ITERATORS SO THAT FOLLOWNODE IS IMMEDIATELY BEHIND trav
		 if ( trav != exploringPtr->head )	
		 	followNode = trav;
		trav = trav->next;
	}
++xCount;
} // END WHILE	

	std::cout << "First hop router list: \n" 
				<< "Destination Node	First Hop Node" << std::endl;
	for ( unsigned int iCount = 0; iCount < mVertices; ++iCount ){
		std::cout << "        " << iCount << "                     " 
				<< root->childMapPtr[ iCount ]->vertexNo << std::endl; 	
	
	}

		
	return 0;

}
