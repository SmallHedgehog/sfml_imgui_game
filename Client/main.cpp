// tutorial_02.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "Application.h"


Application app;

int main()
{
	app.appInit("127.0.0.1", 6666);

	sf::RenderWindow window(sf::VideoMode(1200, 900), "");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	app.setWindow(&window);
	
	sf::Color bgColor;

	float color[3] = { 1.0f, 1.0f, 1.0f };

	// let's use char array as buffer, see next part
	// for instructions on using std::string with ImGui
	char windowTitle[255] = "ImGui-SFML GAME";
	window.setTitle(windowTitle);
	window.resetGLStates();		// call it if you only draw ImGui. Otherwise not needed.
	sf::Clock deltaClock;
	while (window.isOpen()) 
	{
		sf::Event event;
		while (window.pollEvent(event)) 
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
				window.close();
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		app.display();

		window.clear(bgColor);	// fill background with color
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
	
	return 0;
}
