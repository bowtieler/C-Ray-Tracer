
#include "win.h"
#include "sphere.h"
#include "scene.h"

class Application : public EventListener
{
public:

	Application();

private:

	AppWindow	Win;

	sphere				globe;
	scene				s;

	void UpdateWindow(intRect UpdateRect);
	void DrawScene(void);
};

Application::Application()
{
	Win.Setup(0, 0, 500, 500, false);
	Win.SetText("Implementing Constant Material");

	Win.AddMainMenu();
	Win.AddFileMenu();

	Win.SetPaintEvent(this, &Application::UpdateWindow);
	Win.Show();

	globe.SetCenter(point(0.0, 0.0, 0.0));
	globe.SetRadius(1.0);
	globe.SetColor(1.0, 0.0, 0.0);

	// Have to append the shape to the scene's shape list
	s.shapes.Append(&globe);

	s.camera = point(4.0, 0.0, 0.0);
	s.target = point(0.0, 0.0, 0.0);
	s.up = vector(0, 0, 1);
	s.win = &Win;
	s.windowD = 2;
	s.windowW = 1.5;
	s.windowH = 1.5;

	s.DrawScene();
}

void Application::UpdateWindow(intRect UpdateRect)
{
	DrawScene();
}

void Application::DrawScene(void)
{
	s.CopySceneToWindow();
}

void main()
{
	Application app;
	EventLoop(); //loops nothing until user closed
}

