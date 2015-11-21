#include "Game/Net/ServerCore.hpp"
#include "Game/Net/ClientCore.hpp"
#include "Game/Net/NetMessageParser.hpp"
#include "Game/Net/NetMessageEncoder.hpp"
#include "Game/Utility/StringUtil.hpp"
#include <sstream>
#include "vld.h"
int main(int argc, char* argv[])  
{  
	cge::ServerCore* s = NULL;
	if(argc < 10)
	{
		s = new cge::ServerCore("tcp://url:3306","user","password","127.0.0.1",8015,"Josh-PC",-1);
		std::cout << "Please enter the port number" << std::endl;
		int port = 0;
		std::cin >> port;
		std::cin.ignore();
		std::cout << "Please enter the name of the server" << std::endl;
		std::string name;

		std::getline(std::cin,name);
		s->setName(name);

		s->run(port);
	}
	else
	{
		int port = 0;
		std::stringstream ss;
		ss << argv[1];
		ss >> port;
		std::string name = std::string(argv[2]);
		StringUtil::replace(name,"\"","");
		std::string dbIP = std::string(argv[3]);
		StringUtil::replace(dbIP,"\"","");
		std::string dbUser = std::string(argv[4]);
		StringUtil::replace(dbUser,"\"","");
		std::string dbPass = std::string(argv[5]);
		StringUtil::replace(dbPass,"\"","");
		std::string masterIP = std::string(argv[6]);
		StringUtil::replace(masterIP,"\"","");
		int masterPort = 0;
		ss.str("");
		ss.clear();
		ss << argv[7];
		ss >> masterPort;
		std::string externIP = std::string(argv[8]);
		StringUtil::replace(externIP,"\"","");
		int leagueID = -1;
		ss.str("");
		ss.clear();
		ss << argv[9];
		ss >> leagueID;

		s = new cge::ServerCore(dbIP,dbUser,dbPass,masterIP,masterPort,externIP,leagueID);
		s->setName(name);
		s->run(port);
	}
	delete s;
	return 0;  
}  