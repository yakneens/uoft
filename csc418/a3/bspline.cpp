#include <stdio.h>
#include <iostream.h>

#define M  4 // A degree 3 cubic

// A generic point class to hold the value of any point in three
// dimensional space.
class Point {
	public:
		double x,y,z;
		Point(double x, double y, double z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}
};

double bSpline(int k, int m, double t, double knot[])
{
  double denom1, denom2, sum = 0.0;
  if(m == 1)
	 return (t >= knot[k] && t < knot[k+1]); // 1 or 0
  // m exceeds 1.. recurse
  denom1 = knot[k + m -1] - knot[k];
  if(denom1 != 0.0)
	 sum = (t - knot[k]) * bSpline(k,m-1,t, knot) / denom1;
  denom2 = knot[k + m] - knot[k+1];
  if(denom2 != 0.0)
	 sum += (knot[k+m] - t) * bSpline(k+1,m-1,t,knot) / denom2;
  
  //		  for (int i= 0; i < 5+M;i++) {
  //cout << knot[i] << endl;
  //}
  
  return sum;
}


void buildKnots(int m, int L, double knot[])
{
  // Build the standard knot vector for L+1 control points
  // and B-splines of order m
  int i;
  if(L < (m - 1)) return;         // too few control points
  for(i = 0; i <= L + m; i++)
	 if (i < m) knot[i] = 0.0;
	 else if (i <= L) knot[i] = i - m + 1;    // i is at least m here 
	 else knot[i] = L - m + 2;       // i exceeds L here
}

// Returns a spline point based on the given time and control points.
Point *getSplinePoint(double time, Point *cp[], int l) {
        double knots[l+M];
  	  	  buildKnots(M,l,knots); // This is inefficient.

		  double sx = 0, sy = 0, sz = 0;

		  //cout << cp[4]->x << " " << cp[4]->y << " " << cp[4]->z << endl;
		  int i;
		  for(i = 0; i <= l; i++) {
			 double spline = bSpline(i, M, time, knots);
			 cout << "SPLINEY " << spline << endl;
			 sx += cp[i]->x * spline;
			 sy += cp[i]->y * spline;
			 sz += cp[i]->z * spline;
		  }

		  cout << "i equals " << i << endl;
		  return new Point(sx,sy,sz);
}

#ifdef TEST

int main(void)
{
	double x;
	int l = 5; 
	
	double t = 4;

	cout << "For t equals " << t << endl;
	
	Point *cp[6];

	cp[0] = new Point(1,5,3);
	cp[1] = new Point(8,3,2);
	cp[2] = new Point(1,3,9);
	cp[3] = new Point(4,5,1);
	cp[4] = new Point(3,6,2);
	cp[5] = new Point(15,3,3);

	Point *point = getSplinePoint(t, cp, l);
	
	cout << point->x << " " << point->y << " " <<
	point->z << endl;

	return 0;
}

#endif
