#include <exception>
#include <iostream>

#include "Controller.hpp"

int main()
{
	Controller controller;

	try
	{
		controller.run();
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
