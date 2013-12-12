#include "cardinal.h"


float cardinal(float x, float v1,float v2,float v3,float v4) 
{
	float c1,c2,c3,c4;

	c1 = M12*v2;
                          c2 = M21*v1          + M23*v3;
                             c3 = M31*v1 + M32*v2 + M33*v3 + M34*v4;
                             c4 = M41*v1 + M42*v2 + M43*v3 + M44*v4;

                             return(((c4*x + c3)*x +c2)*x + c1);
                     }
