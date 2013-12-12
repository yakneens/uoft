#include <math.h>
#include <limits.h>
#include <float.h>
#include <stdlib.h>

#include "node.h"


Node::Node(){
	this->x = 0;
	this->y = 0;
	this->predX = 0;
	this->predY = 0;
	this->processed = false;
	this->active = false;
	this->cost = DBL_MAX;
	this->ancest = NULL;

	for(int i = 0; i < MAX_NEIGHBOURS; i++){
		this->neighbours[i] = NULL;
	}
}

Node::Node(int x, int y){
	this->x = x;
	this->y = y;
	this->predX = 0;
	this->predY = 0;
	this->processed = false;
	this->active = false;
	this->cost = DBL_MAX;
	this->ancest = NULL;

	for(int i = 0; i < MAX_NEIGHBOURS; i++){
		this->linkCosts[i] = DBL_MAX;
	}

	for(i = 0; i < MAX_NEIGHBOURS; i++){
		this->neighbours[i] = NULL;
	}
}

double* Node::getLinkCosts(){
	return this->linkCosts;
}

void Node::setLinkCosts(double linkCosts[MAX_NEIGHBOURS]){
	*(this->linkCosts) = *linkCosts;
}

/********************************************************************************
 * function getX() - returns the x-coordinate of this point.
 *******************************************************************************/
int Node::getX() const{
	return this->x;
}

/********************************************************************************
 * function setX() - sets the x-coordinate of this point to x.
 *******************************************************************************/
void Node::setX(int x){
	this->x = x;
}

/********************************************************************************
 * function getY() - returns the y-coordinate of this point.
 *******************************************************************************/
int Node::getY() const{
	return this->y;
}

/********************************************************************************
 * function setY() - sets the y-coordinate of this point to y.
 *******************************************************************************/
void Node::setY(int y){
	this->y = y;
}

void Node::setLinkCost(double cost, int index){
	this->linkCosts[index] = cost;
}

double Node::getCost() const{
	return this->cost;
}

void Node::setCost(double cost){
	this->cost = cost;
}

bool Node::getProcessed() const{
	return this->processed;
}

void Node::setProcessed(bool val){
	this->processed = val;
}

Node **Node::getNeighbours(){
	return this->neighbours;
}

void Node::setActive(bool val){
	this->active = val;
}

bool Node::getActive() const{
	return this->active;
}

void Node::setAncestor(Node *ancest){
	this->ancest = ancest;
}

Node *Node::getAncestor() const{
	return this->ancest;
}

void Node::setNeighbour(Node *neighbour, int index){
	if(index <= MAX_NEIGHBOURS){
		neighbours[index] = neighbour;
	}
}

void Node::setValidQueueElem(void *queueElem){
	this->validQueueElem = queueElem;
}

void *Node::getValidQueueElem(){
	return this->validQueueElem;
}