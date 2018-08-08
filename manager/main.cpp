#include <iostream>

#include "slotmanager.h"

using namespace std;

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
	  std::cout << "usage: " << argv[0] << "configFile slotDir" << std::endl;
	  return -1;
	}

    SlotManager m(argv[1], argv[2]);
    m.start();
    std::string cmd;
    while(true)
    {
        std::cout << "input exit to exit:" << std::endl;
        std::cin >> cmd;
        if(cmd == "exit")
            break;
    }
    m.stop();
    return 0;
}
