#ifndef PQELEMENT_H
#define PQELEMENT_H


// Define a class that defines the elements of the priority queue
class PQElement {
    
	public: 
		PQElement(int pixelIndex, double priority);
		double getPriority() const;
		void setPriority(double priority);
		int getPixelIndex() const;
		void setPixelIndex(int pixelIndex);
		bool less(PQElement &n1, PQElement &n2)
				   { return(n1.getPriority() > n2.getPriority()); };
		bool operator<(const PQElement &n1) const 
				   { return this->priority > n1.getPriority(); };
		


	private:
		double priority;
		int pixelIndex;
};

#endif
