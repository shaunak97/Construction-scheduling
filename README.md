# Construction-scheduling
The project implements a way to keep track of all the buildings under construction
Data structures used here include Min Heap and Red Black Trees
Language of Implementation - C++


The source code has 3 classes which provide the required abstraction.

1) Class construct
  
  This is the top level class which set the order for construction functionality. Methods belonging to the class are
  -> void print(int buildingNum); Print the building triplet
  -> void print(int buildingNum1, int buildingNum2); Prints the building triplet information for given range of buildingNums
  ->void insert(int buildingNum, int time); Inserts buildingNums and the total time into the Min heap and Red Black tree
  ->construct(); Constructor that initializes the counter and creates objects of class heap and class rbt
  ->void syncTime(int time); Synchronizes time between global time and counter
  ->int dispatch(int); Send building for construction
  ->int ifbuild(); Checks if any building is left to build
  
  2) Class heap
  This class facilitates the Min Heap data structure
  -> heapnode*insert(int, int, int, rbtnode*); insert a new node into the heap
  -> struct heapnode* removeMin(); remove the item from the top of the heap ie minimum executed time
  -> void swapbuilding(struct heapnode* a, struct heapnode*b); swap two nodes of the heap
  ->void heapify(); regain heap property
  ->void updateMin(int exec_time); Updates the root node and re arranges the heap structure
  ->heap(int); constructor declaration
  -> void execute(int); function to execute building construction
  
  3) Class rbt
  This class is responsible for all features of a red black tree data structure
  ->rbtnode*insert(const int&n); insert new node into the tree
  ->rbtnode*findnode(int bnum); finds a node in the tree based on building number
  ->void inorder(int, int); find a range of building numbers
  ->void deletenode(rbtnode*); delete a node from tree
  ->void fixviolation(rbtnode*); fix violation caused due to delete
  ->void rightrotate(rbtnode*p); rotate right after delete
  ->void leftrotate(rbtnode*p); rotate left after delete
  
  
  Structure - 
  
  Min Heap and the Red Black Tree is connected to each other by pointers pointing in each direction on both ends of the same element
  Time complexity of search is O(logn) as Red Black Tree is a self balancing tree
  Time complexity of print command is executed in O(logn) and the print range command is executed in O(log(n)+S)
  
