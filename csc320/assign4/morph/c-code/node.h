#ifndef NODE_H
#define NODE_H

#include <math.h>
#include <vector>

#define MAX_NEIGHBOURS 8
class Node{

	public:
		Node();
		Node(int x, int y);
		double* getLinkCosts();
		void setLinkCosts(double linkCosts[MAX_NEIGHBOURS]);
		void setLinkCost(double cost, int index);
		void setX(int x);
		void setY(int y);
		int getX() const;
		int getY() const;
		double getCost() const;
		void setCost(double cost);
		bool getProcessed() const;
		void setProcessed(bool val);
		Node **getNeighbours();
		void setNeighbour(Node *neighbour, int index);
		void setActive(bool val);
		bool getActive() const;
		void setAncestor(Node *ancest);
		Node *getAncestor() const;
		void setValidQueueElem(void *queueElem);
		void *getValidQueueElem();
	private:
		int x,y;
		int predX, predY;
		bool processed;
		bool active;
		double cost;
		double linkCosts[MAX_NEIGHBOURS];
		Node *ancest;
		Node *neighbours[MAX_NEIGHBOURS];
		void *validQueueElem;

};
#endif