#include <iostream>

#include <SFML/System.hpp>

#include "Controller.hpp"
#include "ComputeMatricesFromInput.hpp"

const sf::Time Controller::mFPS = sf::seconds(1.f / 60.f);

Controller::Controller()
: mWindow(nullptr)
, mWorld(nullptr)
{
	mWindow = mOGLRenderer.createWindow();

	if(!mOGLRenderer.init(mWindow.get()))
	{
		std::cerr << "Error initliaising OGLRenderer" << std::endl;
		exit(1);
	}

	mWorld = World::WorldUPtr(new World(mOGLRenderer));
}

void Controller::run()
{
	sf::Clock clock;
	sf::Time countDown = mFPS;
	bool closeWindow = false;

	// Start game loop
	do
	{
		// Process events
		sf::Event event;

		while(mWindow->pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				closeWindow = true;

			// Escape key: exit
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				closeWindow = true;

			// Adjust the viewport when the window is resized
			if (event.type == sf::Event::Resized)
				glViewport(0, 0, event.size.width, event.size.height);
		}

		countDown -= clock.restart();

		if(countDown < sf::Time::Zero)
		{
			countDown = mFPS;
			mWorld->handleInput(mFPS);
		}

		mWindow->clear();
		mWorld->drawWorld();

		// Finally, display the rendered frame on screen
		mWindow->display();

	} while(!closeWindow);

	mWindow->close();
}
