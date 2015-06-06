
#include "shape.h"

constantMaterial shape::defaultMaterial;

// The default constructor sets default colors and builds all the transforms
shape::shape() 
{ 
	surfaceColor = rgb::white;
	specularColor = rgb::white;
	shininess = 10.0;

	translation = point(0, 0, 0);
	scale = vector(1, 1, 1);
	rotX = rotY = rotZ = 0.0;

	totalTransform = matrix::Identity(4);
	inverseTransform = matrix::Identity(4);
	inverseTranspose = matrix::Identity(4); 
	canShadow = true;
	canBeShadowed = true;

	objectMat = &defaultMaterial;
}

// CalculateIntersection transforms the ray and then calls "Intersect" to 
// get the intersection with the untransformed object.  Then it must 
// transform the results.  Look in the derived classes for the definition
// of the "Intersect" method
bool shape::CalculateIntersection(const ray &R, intersection &I)
{
	// Hit the ray with inv(M)
	ray RTrans = TransformRay(R);
	
	// Calculate intersection with object, and if there is one, 
	// process it
	if (Intersect(RTrans, I))
	{
		// Transform the result to get the true intersection
		I.p = R.PointOnRay(I.t);
		I.n = TransformNormal(I.n);
		I.n.Normalize();
		I.v = -R.v;
		I.v.Normalize();
		return true;
	}
	else
		return false;	
}

vector shape::TransformNormal(const vector &n)
{
	//cout << "TransformNormal" << endl;
	// To Do
	//
	// Returned the normal transformed by this object's transform.
	// Hint: how do you transform a normal?
	return n;
}

ray shape::TransformRay(const ray &r)
{
	//cout << "TransformRay" << endl;
	// To Do
	//
	// Return the ray transformed by this object's transform.
	// Hint: how do you transform a ray?

	shape s = *this;
	ray result;

	result.p = r.p;
	result.v = r.v;

	return result;
}

void shape::SetTranslation(const point &p)
{
	translation = p;
	RecalculateMatrices();
}

void shape::SetXRotation(double angle)
{
	rotX = angle;
	RecalculateMatrices();
}

void shape::SetYRotation(double angle)
{
	rotY = angle;
	RecalculateMatrices();
}

void shape::SetZRotation(double angle)
{
	rotZ = angle;
	RecalculateMatrices();
}

void shape::SetRotation(double x, double y, double z)
{
	rotX = x;
	rotY = y;
	rotZ = z;
	RecalculateMatrices();
}

void shape::SetScale(const vector &s)
{
	scale = s;
	RecalculateMatrices();
}

void shape::RecalculateMatrices(void)
{
	// These are members of the shape class
	totalTransform = matrix(4, 4, 1.0);
	inverseTransform = matrix(4, 4, 1.0);
	inverseTranspose = matrix(4, 4, 1.0);

	matrix S, Rx, Ry, Rz, T;
	matrix S_Inv, Rx_Inv, Ry_Inv, Rz_Inv, T_Inv;
	cout << "RecalculateMatrices" << endl;

	shape s = *this;
	cout << s[0] << endl;

	// To Do
	//
	// Compute the totalTransform as the product of the scale, rotation
	// and translation.  Remember, we want the matrix to apply the 
	// Scale first then the x-Rotation, y-Rotation and z-Rotation and
	// then finally the Translation.  
	// 
	// Then compute the inverse by computing the inverse of the individual
	// matrices, and multiply them together in the proper order to 
	// compute the inverseTransform.
	//
	// Finally, set the inverseTranspose to the transpose of the 
	// inverseTransform
	//
}

rgb shape::ApplyMaterial(intersection &inter, LinkedList<light *> &lights, LinkedList<shape *> &shapes)
{
	return objectMat->ReflectLights(inter, lights, shapes);
}

