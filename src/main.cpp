//
// Created by Hugo JEANNIN on 3/10/17.
//

#include "../inc/Core.hpp"

int main(int argc, char **argv)
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{
		Core core;

		srand(static_cast<unsigned int>(time(nullptr)));

		core.init();
	}
	catch (std::exception &e)
	{
		std::cerr << "Caught " << e.what() << std::endl;
		std::cerr << "Type " << typeid(e).name() << std::endl;
	};
    return 0;
}