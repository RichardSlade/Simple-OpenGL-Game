#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "OGLRenderer.hpp"
#include "World.hpp"

class World;
//class OGLRenderer;

class Controller
{
public:

private:
	static const sf::Time	mFPS;

	OGLRenderer::WindowUPtr	mWindow;
	OGLRenderer					mOGLRenderer;
	World::WorldUPtr			mWorld;

public:
							Controller();

	void 					run();
};

#endif // CONTROLLER_HPP
