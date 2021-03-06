#include "includes.h"
#include <stddef.h>

using namespace std;

Node::Node() {
	pParent = pNext = NULL;
	name = cost = huristic = explored = expanded = 0;
}

Node::Node(char name, float huristic = 0) :
	name(name),
	huristic(huristic) {
	pParent = pNext = NULL;
	cost = explored = expanded = 0;
}

Node::~Node() {
	//pParent->pNext = NULL;
	//delete pNext;
}

Node& Node::operator=(const Node& right) {
	// Check for self-assignment!
	if (this == &right) // Same object?
		return *this; // Yes, so skip assignment, and just return *this.
	// Deallocate, allocate new space, copy values...
	this->name = right.name;
	this->pParent = right.pParent;
	this->pNext = right.pNext;
	this->cost = right.cost;
	this->explored = right.explored;
	this->huristic = right.huristic;
	this->expanded = right.expanded;
	
	return *this;
}

ostream& operator<<(std::ostream& os, const Node& obj) {
	os	<< obj.name << "\t"
		<< (obj.pParent != NULL ? obj.pParent->name : ' ') << "\t"
		<< obj.cost << "\t"
		<< obj.huristic << "\t"
		<< obj.expanded << "\t"
		<< (obj.explored ? "+ True" : "- False");
	return os;
}

bool Node::isGoal(char goalCity) { return this->name == goalCity; }

char Node::getName() const { return this->name; }
Node* Node::getParent() const { return this->pParent; }
Node* Node::getNext() const { return this->pNext; }
float Node::getCost() const { return this->cost; }
float Node::getHuristic() const { return this->huristic; }
bool Node::isExplored() const { return this->explored; }
int Node::getExpanded() const { return this->expanded; }

float Node::calcCost(int algo) {
	switch(algo) {
		case ASTAR    : return this->huristic + this->cost;
		case UCS      : return this->cost;
		case HURISTIC : return this->huristic;
		default       : return 0;
	}
}

/**
 * finds potential sibling nodes of the calling node 
 * pushes potential nodes to list pFrontier (by calling pushNodetoList(pFrontier)).
 */
void Node::expand(vector<Link> &links, std::map<char,float>& huristicMap, List* pFrontier) {
	this->explored = true;

	for (int i = 0; i < links.size(); i++) {
		if (links[i].getC1() == this->name || links[i].getC2() == this->name) {

			Node* tmpNode = new Node();
			tmpNode->name =	(links[i].getC1() == this->name) ? links[i].getC2() : links[i].getC1();
			tmpNode->pParent = this;
			tmpNode->cost = this->cost + links[i].getLinkCost();
			tmpNode->huristic = huristicMap[tmpNode->name];

			if (tmpNode->isQualified(pFrontier)) {
				tmpNode->pushNodeToList(pFrontier);
				this->expanded++;
			} else {
				delete tmpNode;
			}
		}
	}

}

void Node::pushNodeToList(List* pFrontier) {
	pFrontier->getLastNode()->pNext = this;
	pFrontier->setLastNode(this);
}

void Node::rmNodeFromList(List* pFrontier) {
	Node* current = pFrontier->getFirstNode();

	while (current != NULL) {
		if (current->pParent == this) {
			//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< fix recurgent
			current->rmNodeFromList(pFrontier);
			//delete current; //<<<<<<<<<<<<<<<<<<<<< possible mem leak
		} 
		if (current->pNext == this) {
			if (this == pFrontier->getLastNode()) {
				pFrontier->setLastNode(current);
			}
			current->pNext = current->pNext->pNext;
		}
		current = current->pNext;
	}
}

/**
 * determines if the calling node (which is a potential node) is qualified to be added to the input argument list.
 * The function should also discard from the list any node once a cheaper replacement for it is found.
 * @param pFrontier
 * @return 
 */
bool Node::isQualified(List* pFrontier) {

	Node* current = pFrontier->getFirstNode();

	while (current != NULL) {
		if(current->name == this->name) {
			//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			if (current->getCost() > this->getCost()) {
				// found one with larger cost, time to replace
				//<<<<<<<<<<<<<<<<<<<< should remove it siblings here
				current->rmNodeFromList(pFrontier);
				delete current;
				return true;
			} else {
				return false; // found one with a cheaper or similar cost
			}
		}
		current = current->getNext();
	}
	return true; // didn't find any repetition
}