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

static class HELP : public con::Command
{
	std::string getName() const
	{
		return "help";
	}

	unsigned getMinArgs() const
	{
		return 0;
	}

	void help( Console& c )
	{
		c << setcinfo << "Displays the list of console commands" << endl;
	}

	void execute( Console& c, const Arguments& args )
	{
		const auto& cmds = c.getCommands();

		c << setcinfo << "Commands:" << endl;
		for ( auto it = cmds.begin(); it != cmds.end(); ++it )
			c << setcinfo << "\t" << (*it)->getName() << endl;
	}
} HELP;

static class CLEAR : public Command
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

	void execute( Console& c, const Arguments& args )
	{
		c.clearHistory();
	}
} CLEAR;

void defaultCommands( Console& console )
{
	console.addCommand( HELP );
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

static class LE_CREATE : public con::Command
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
		c << setcinfo << "[Level Editor only]" << endl;
		c << setcinfo << "Generates an empty map" << endl;
		c << setcinfo << "le_create width height" << endl;
		c << setcinfo << "\twidth: width (in tiles) of the new map" << endl;
		c << setcinfo << "\theight: height (in tiles) of the new map" << endl;
	}

	void execute( Console& c, const Arguments& args )
	{
		getLevelEditor().createMap( std::stoi( args[ 0 ] ), std::stoi( args[ 1 ] ) );
	}
} LE_CREATE;

static class LE_RESIZE : public con::Command
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

	void execute( Console& c, const Arguments& args )
	{
		getLevelEditor().resizeMap( std::stoi( args[ 0 ] ), std::stoi( args[ 1 ] ) );
	}
} LE_RESIZE;

static class LE_SETTILE : public con::Command
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

	void execute( Console& c, const Arguments& args )
	{
		getLevelEditor().setCurrentTile( args[ 0 ] );
	}
} LE_SETTILE;

static class LE_SAVE : public con::Command
{
	std::string getName() const
	{
		return "le_save";
	}

	unsigned getMinArgs() const
	{
		return 1;
	}

	void help( Console& c )
	{
	}

	void execute( Console& c, const Arguments& args )
	{
		std::string name = args[ 0 ] + ".cwmap";
		getLevelEditor().save( args[ 0 ] + ".cwmap" );
		c << con::setcinfo << "Saved to " << name << con::endl;
	}
} LE_SAVE;

static class LE_LOAD : public con::Command
{
	std::string getName() const
	{
		return "le_load";
	}

	unsigned getMinArgs() const
	{
		return 1;
	}

	void help( Console& c )
	{
	}

	void execute( Console& c, const Arguments& args )
	{
		std::string name = args[ 0 ] + ".cwmap";
		getLevelEditor().load( name );
		c << con::setcinfo << "Saved to " << name << con::endl;
	}
} LE_LOAD;

void levelEditorCommands( Console& console )
{
	console.addCommand( LE_CREATE );
	console.addCommand( LE_RESIZE );
	console.addCommand( LE_SETTILE );
	console.addCommand( LE_SAVE );
	console.addCommand( LE_LOAD );
}


/***************************************************/

} // namespace console

} // namespace cw