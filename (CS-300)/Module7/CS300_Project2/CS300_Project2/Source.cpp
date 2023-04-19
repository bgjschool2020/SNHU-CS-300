#include <algorithm>
#include <climits>
#include <iostream>
#include <string>
#include <time.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>  // string stream

#include "CSVparser.hpp"  // CREDIT: SNHU CS300 Assignments Modules 2 - 5, for more details please ask me.

// define a structure to hold course information
struct Course {
	std::string courseId; // unique identifier;
	std::string courseName;
	std::vector<std::string> prerequisites;
};

// structure for tree node
struct Node {
	Course course;
	Node* left;
	Node* right;

	Node() { //Default constructor
		left = nullptr;
		right = nullptr;
	}

	//initialize with a course
	Node(Course aCourse) : Node() {
		this->course = aCourse;
	}
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Defines a class containing data members and methods to implement a binary search tree
 */
class BinarySearchTree {

private:
	Node* root;

	void addNode(Node* node, Course course);
	void inOrder(Node* node); 

public:
	BinarySearchTree();
	void InOrder();
	void Insert(Course course);
	Course Search(std::string courseId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

/**
*  /private
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
	inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
	if (root == nullptr) {
		// root is equal to new node course
		root = new Node(course);
	}
	// else
	else {
		// addNode root and course
		this->addNode(root, course);
	}
}


/**
 * Search for a course
 * 
 * @param courseId the course ID to be searched
 */
Course BinarySearchTree::Search(std::string courseId) {
	Node* current = root;
	// keep looping downwards until bottom reached or matching courseId found
	while (current) {
		// if match found, return current course
		if (current->course.courseId.compare(courseId) == 0) {
			return current->course;
		}
		// if course is smaller than current node then traverse left
		if (courseId.compare(current->course.courseId) < 0) {
			current = current->left;
		}
		// else larger so traverse right
		else {
			current = current->right;
		}
	}
	Course course;
	return course;
}


/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
	// FIXME (8) Implement inserting a course into the tree

	// if node is larger then add to left subtree
	if (node->course.courseId.compare(course.courseId) > 0) {
		// if no left node
		if (node->left == nullptr) {
			// this node becomes left
			node->left = new Node(course);
		}
		// else recurse down the left node
		else {
			this->addNode(node->left, course);
		}
	}
	// else, add to right subtree
	else {
		// if no right node
		if (node->right == nullptr) {
			// this node becomes right
			node->right = new Node(course);
		}
		//else
		else {
			// recurse down the left node
			this->addNode(node->right, course);
		}
	}
}

void printVector(std::string separator, std::vector<std::string> toPrint) {
	for (int i = 0; i < toPrint.size(); i++) {
		if (i >= 1) {
			std::cout << separator;
		}
		std::cout << toPrint[i];
	}
	std::cout << std::endl;
}


void BinarySearchTree::inOrder(Node* node) {
	// FixMe (9): Pre order root
	//if node is not equal to null ptr
	if (node != nullptr) {
		//InOrder not left
		inOrder(node->left);
		std::cout << node->course.courseId << " | " << node->course.courseName << " | ";
		printVector(", ", node->course.prerequisites);
		//InOrder right
		inOrder(node->right);
	}
}

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourse(std::string csvPath, BinarySearchTree* bst) {
	std::cout << "Loading CSV file " << csvPath << std::endl;

	// initialize the CSV Parser using the given path
	csv::Parser file = csv::Parser(csvPath);

	// read and display header row - optional
	
	try {
		// loop to read rows of a CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++) {

			csv::Row& row = file[i];

			// Create a data structure and add to the collection of courses

			Course course;
			course.courseId = row[0];
			course.courseName = row[1];
			//course.prerequisites = file[i][2];
			
			for (unsigned int columnIndex = 2; columnIndex < row.size(); columnIndex++) {
				course.prerequisites.push_back(row[columnIndex]);
			}
			
			bst->Insert(course);
		}
	}
	catch (csv::Error& e) {
		std::cerr << e.what() << std::endl;
	}
}



void singlePrint(Course courseToPrint) {
	std::cout << courseToPrint.courseId << " | " << courseToPrint.courseName << " | ";
	printVector(", ", courseToPrint.prerequisites);
}



/**
* Prints the Menu
* 
* @return userInput
*/
int printMenu() {
	int userInput = 0;

	std::cout << "1: Load course data" << std::endl;
	std::cout << "2: Print course list" << std::endl;
	std::cout << "3: Print single course" << std::endl;
	std::cout << "9: Exit Program" << std::endl;
	std::cin >> userInput;
	
	return userInput;
}


int main(int argc, char* argv[]) {
	int choice = 0;
	clock_t ticks;
	std::string filename = "Courses.csv";

	BinarySearchTree* bst = new BinarySearchTree();

	while (choice != 9) {
		choice = printMenu();  // gets user input as well
		switch (choice) {
			case 1:  // load course data
				ticks = clock();
				loadCourse(filename, bst);
				ticks = clock() - ticks;  // current clock ticks minus starting clock ticks
				// outputs time in seconds
				std::cout << "time: " << ticks << " clock ticks" << std::endl;
				std::cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << std::endl;
				break;

			case 2:  // print entire course list
				bst->InOrder();
				break;

			case 3:  // print a single course
				std::string cChoice;
				Course courseToPrint;

				// loop for determining if the input was the correct length for a course ID
				do {
					std::cout << "Which course are you looking for?" << std::endl;
					std::cin >> cChoice;
					if (cChoice.length() != 7) {
						std::cout << "That is not a valid course. Please try again." << std::endl;
					}
				} while (cChoice.length() != 7);

				courseToPrint = bst->Search(cChoice);
				singlePrint(courseToPrint);
		}
	}
	return 0;
}