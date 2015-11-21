#include "Game/Net/MasterCore.hpp"

int main(int argc, char* argv[])  
{  
	cge::MasterCore* s = new cge::MasterCore();
	s->run(8015);
	delete s;
	return 0;  
}  