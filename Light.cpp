
#include "light.h"
#include "shape.h"
#include "scene.h"
#include "Shape.h"
#include "float.h" // for DBL_MAX

//////////////////////////////////////////////////////////////////////////////////////
// Methods for the light base class
//////////////////////////////////////////////////////////////////////////////////////

bool light::CalculateShadow(const intersection &inter, lightOutput &output)
{
	const LinkedList<shape *> &shapes = inter.w->shapes;
	intersection tmp;
	double tLight;

	if (CanShadow() && inter.s->CanBeShadowed())
	{
		ray r;

		// To Do
		// 
		// Calculate the ray going back from the surface to the light and
		// move the ray's starting point a little bit away from the surface

		tLight = Distance(inter.p, GetPosition()) / output.L.Length();

		// Now, we loop through the shapes and if the shape s can shadow
		// objects (see the CanShadow method) 		
		for (shapes.MoveFirst(); !shapes.AtEnd(); shapes.MoveNext())
		{
			tmp.w = inter.w;
			shape *s = shapes.GetCurrent();

			// To Do
			//
			// If s can cast a shadow, then see if r intersects the shape s.  
			// The intersection data should be stored in "tmp"
			//
			// If there is an intersection, compare the "t" parameter of the 
			// intersection with tLight to see if it is closer.  If it is,
			// multiply the diffuse part of the light output by the shadow color
			// Also, eliminate the specular light, and return true.
			// if not, keep searching.
		}
	}
	return false;	
}

//////////////////////////////////////////////////////////////////////////////////////
// Methods for the ambientLight class
//////////////////////////////////////////////////////////////////////////////////////

bool ambientLight::Illuminance(const intersection &inter, lightOutput &output) 
{ 
	output.L = vector(0, 0, 0);
	output.d = output.s = rgb(0, 0, 0);
	output.a = aC;
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////
// Methods for the distantLight class
//////////////////////////////////////////////////////////////////////////////////////

distantLight::distantLight(const vector &d, const rgb &dc, const rgb &sc, const rgb &ac, bool shad)
{ 
	SetDirection(d); 
	SetAmbientColor(ac); 
	SetDiffuseColor(dc); 
	SetSpecularColor(sc); 
	canShadow = shad; 
}

bool distantLight::Illuminance(const intersection &inter, lightOutput &output) 
{ 
	output.L = dir;
	output.a = aC;
	output.d = dC;
	output.s = sC; 
	CalculateShadow(inter, output);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////
// Methods for the pointLight class
//////////////////////////////////////////////////////////////////////////////////////

pointLight::pointLight(const point &p, const rgb &dc, const rgb &sc, const rgb &ac, 
					   double cAtten, double lAtten, double qAtten)
{ 
	hasPosition = true;
	SetPosition(p); 
	SetAmbientColor(ac); 
	SetDiffuseColor(dc); 
	SetSpecularColor(sc); 
	SetAttenuations(cAtten, lAtten, qAtten); 
}

bool pointLight::Illuminance(const intersection &inter, lightOutput &output) 
{ 
	double D, atten;

	output.L = pos - inter.p;				// point to the light
	D = output.L.Length();
	atten = 1.0 / (attenC + attenL * D + attenQ * D * D);

	output.a = aC;
	output.d = atten * dC; 
	output.s = atten * sC; 
	output.L /= D;							// Normalize the light vector

	CalculateShadow(inter, output);
	return true;
}

////////////////////////////////////////////////////////////////////////////////////
// Methods for the spotLight class
////////////////////////////////////////////////////////////////////////////////////

spotLight::spotLight(const point &p, const rgb &sc, const point &Target, 
					 double InnerThet, double OuterThet,
					 double cAtten, double lAtten, double qAtten)
{
	hasPosition = true;
	SetPosition(p);
	SetTarget(Target);
	SetSpecularColor(sc);
	SetAttenuations(cAtten, lAtten, qAtten);
	SetAngleLimits(InnerThet, OuterThet);
}

bool spotLight::Illuminance(const intersection &inter, lightOutput &output)
{
	// We calculate the output light vector, and attenuation just
	// like with a point light.
	double D, atten;

	output.L = pos - inter.p;				// point to the light
	D = output.L.Length();
	atten = 1.0 / (attenC + attenL * D + attenQ * D * D);

	output.a = aC;
	output.d = atten * dC; 
	output.s = atten * sC; 
	output.L /= D;							// Normalize the light vector

	// To Do
	//
	// Then calculate the angle from the spot direction to the 
	// light direction to the intersection point.  Then use 
	// the inner and outer theta angles to smooth-step down from
	// a fully bright light to a brightness of zero between these
	// two angles.  Simply multiply the poing light's colors by this 
	// "spot attenuation" value.

	CalculateShadow(inter, output);
	return true;
}

