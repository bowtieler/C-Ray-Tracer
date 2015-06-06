
#include "jmisc.h"
#include "sphere.h"

// Returns a boolean indicating whether the ray intersects
// The sphere, if it does then it fills t with the closest
// intersection "time", t > 0 and fills p and n with the 
// data for the intersection.  Also fills the r, g and b
// values of the intersection with the shape's r, g, and b.
bool sphere::Intersect(const ray &R, intersection &inter)
{
	double t1, t2;
	double A, B, C;

	vector Q = (R.p - center);
	
	A = Dot(R.v, R.v); 
	B = 2.0 * Dot(R.v, Q);
	C = Dot(Q, Q) - sqr(radius);

	double discriminant = sqr(B) - 4*A*C;
	if (discriminant >= 0)
	{
		double q = -.5 * (B + sgn(B)*sqrt(discriminant));
		t1 = q / A;
		t2 = C / q;

		if ((t1 < 0) || (t2 < 0))
		{
			if ((t1 < 0) && (t2 < 0))
				return false;	
			else if (t1 > 0)
				inter.t = t1;
			else
				inter.t = t2;
		}
		else
			inter.t = min(t1, t2);

		// Point and normal of intersection
		inter.p = R.PointOnRay(inter.t);
		inter.n = inter.p - center;

		// Other data needed by the tracer
		inter.s = this;
		inter.diff = surfaceColor;
		inter.spec = specularColor;
		inter.shiny = shininess;

		// Advanced, for texturing later
		inter.tex = point(atan2(inter.p[1], inter.p[0]) / (2*PI), acos(-inter.p[2] / Distance(inter.p, center)) / PI);

		return true;
	}
	else
		return false;
}

