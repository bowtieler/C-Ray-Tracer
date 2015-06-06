
#include "time.h"
#include "scene.h"
#include "ray.h"
#include "win.h"

void scene::DrawScene(char *outputFileName)
{
	int w = win->Width(); // num of pixels disp on screen
	int h = win->Height();
	double pixelW = windowW / w; //in the world
	double pixelH = windowH / h;
	

	cout << "w/h = " << w << "/" << h << endl;
	cout << "windowW/windowH = " << windowW << "/" << windowH << endl;
	cout << "pixelW/pixelH = " << pixelW << "/" << pixelH << endl;
	cout << "Camera = " << camera << endl;
	cout << "Target = " << target << endl;

	if (bit != NULL)
		delete bit;
	bit = new Bitmap(*win, w, h);

	vector view; // from camera to target
	// = (T-C)/|T-C|, where T = target and C = Camera location
	
	vector TmC = target - camera;	
	view = TmC / TmC.Length();
	cout << "View = " << view << endl;

	double d = Distance(point(camera[0],camera[1], camera[2]), point(windowD));
	cout << "d = " << d << endl;

	// Calculate the center of the window
	point center = camera + d*view;
	// = C+d*v, where d = distance to the window and v = vector
	cout << "Center = " << center << endl;

	// Before we can use the up vector, we need to modify up
	// so that it is perpendicular to the view vector, and
	// normalize it.

	up.Perp(view);
	up.Normalize();
	cout << "Up = " << up << endl;

	// Now calculate the left vector.
	vector left = Cross(up, view);
	//left += view;
	cout << "Left = " << left << endl;

	// Calculate the coordinates of upperLeft = pixel(0, 0)
	point upperLeft = center + up*(windowH/2) + (windowW/2)*left;
	cout << "upperLeft = " << upperLeft << endl;

	int t1 = clock();
	int c = -1;
	// Now, loop through the pixels on the screen
	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			c++;
			// Calculate the position of pixel(i, j) in space

			point screenP = upperLeft - (i*pixelW*left) - (j*pixelH*up);
			point ijSpace = screenP;
			ijSpace[0] = 0;

			
			// Then build the ray from the camera to the pixel
			ray r;
			r.p = camera;
			r.v = (screenP - camera);
			r.v.Normalize();
			r.v[0] = 0;

			// Finally, we trace the ray to get the color we see,
			// clamp the color and write the result to the screen
			rgb result = TraceRay(r, i, j, c);
			result.Clamp();
			bit->Point(i, j, result.R(), result.G(), result.B());							
		}
		CopySceneToWindow();
		Window::HandleEvents();
	}
	int t2 = clock();
	cout << "Frame drawn in " << double(t2-t1) / CLOCKS_PER_SEC << endl;
	if (outputFileName != NULL)
		bit->Save(outputFileName);
}

bool scene::CastRay(const ray &r, intersection &inter)
{
	bool found = false;
	intersection tmp;

	inter.t = DBL_MAX;	 // Make an impossibly large t
	for (shapes.MoveFirst(); !shapes.AtEnd(); shapes.MoveNext())
	{
		shape *s = shapes.GetCurrent();
		shape curShape = *s;
		found = curShape.Intersect(r, inter);
		
	}
	return found;
}

rgb scene::TraceRay(const ray &r, int i, int j, int c)
{
	intersection inter;	
	if (CastRay(r, inter) == true)
	{
		return inter.s->ApplyMaterial(inter, lights, shapes);
	}
	else
		return backgroundColor;			
}

void scene::CopySceneToWindow(void)
{
	if (bit)
		bit->Copy(*win, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////////////
// The following are advanced antialiased versions of the DrawScene method
/////////////////////////////////////////////////////////////////////////////////////

void scene::DrawSceneAntialias(int xSamples, int ySamples, char *outputFileName)
{
	// To Do
	// 
	// Do the same as in DrawScene, but sample each pixel on a rectangular
	// grid evenly inside a pixel with xSamples * ySamples rays
	// and average the results
}

void scene::DrawSceneMonteCarlo(int samples, char *outputFileName)
{
	// To Do
	// 
	// Do the same as in DrawScene, but sample each pixel randomly
	// samples times and average the results
}

