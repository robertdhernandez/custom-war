#include "console.h"
#include "console_command.h"
#include "console_functions.h"

#include "state_leveleditor.h"

namespace cw
{
namespace con
{

using std::bind;
using std::make_pair;
using namespace std::placeholders;

typedef const std::vector< std::string >& Arguments;

/***************************************************/
//	COMMON COMMANDS

static class : public Command
{
	std::string getName() const
	{
		return "clear";
	}

	unsigned getMinArgs() const 
	{
		return 0;
	}

	void help( Console& c )
	{
		c << "Clears the console" << endl;
	}

	void execute( const Arguments& args )
	{
		Console::getSingleton().clearHistory();
	}
} CLEAR;

void defaultCommands( Console& console )
{
	console.addCommand( CLEAR );
}

/***************************************************/
// LEVEL EDITOR COMMANDS

using state::LevelEditor;

LevelEditor& getLevelEditor()
{
	LevelEditor* ret = dynamic_cast< LevelEditor* >( &StateBase::getGlobal() );
	if ( ret == nullptr )
		throw std::runtime_error( "must be in level editor" );
	return *ret;
}

static class : public con::Command
{
	std::string getName() const
	{
		return "le_create";
	}

	unsigned getMinArgs() const
	{
		return 2;
	}

	void help( Console& c )
	{
	}

	void execute( const Arguments& args )
	{
		getLevelEditor().createMap( std::stoi( args[ 0 ] ), std::stoi( args[ 1 ] ) );
	}
} LE_CREATE;

static class : public con::Command
{
	std::string getName() const
	{
		return "le_resize";
	}

	unsigned getMinArgs() const
	{
		return 2;
	}

	void help( Console& c )
	{
	}

	void execute( const Arguments& args )
	{
		getLevelEditor().resizeMap( std::stoi( args[ 0 ] ), std::stoi( args[ 1 ] ) );
	}
} LE_RESIZE;

static class : public con::Command
{
	std::string getName() const
	{
		return "le_settile";
	}

	unsigned getMinArgs() const
	{
		return 1;
	}

	void help( Console& c )
	{
	}

	void execute( const Arguments& args )
	{
		getLevelEditor().setCurrentTile( args[ 0 ] );
	}
} LE_SETTILE;

void levelEditorCommands( Console& console )
{
	console.addCommand( LE_CREATE );
	console.addCommand( LE_RESIZE );
	console.addCommand( LE_SETTILE );
}


/***************************************************/

} // namespace console

} // namespace cw