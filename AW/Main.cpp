#include "Game.h"

#ifdef _WIN32
#	include <Windows.h>
#else
#	include <iostream>
#endif

int main( int argc, char** argv )
{
	try
	{
		aw::Game game;
		game.ParseCmdArgs( argc, argv );
		game.Run();
		return EXIT_SUCCESS;
	}
	catch ( std::exception& err )
	{
#ifdef _WIN32
		MessageBox( NULL, err.what(), NULL, MB_OK | MB_ICONERROR );
#else
		std::cout << err.what() << std::endl;
#endif
		return EXIT_FAILURE;
	}
}