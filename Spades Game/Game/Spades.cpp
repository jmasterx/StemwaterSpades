#include <stdlib.h>
#include <iostream>
#include "Game/Engine/Core.hpp"
int main(int argc, char *argv[])
{
	try
	{
		cge::Core* core = new cge::Core(argc,argv);
		core->run();
		delete core;
	}
	catch (...)
	{

	}


	return 0;
}