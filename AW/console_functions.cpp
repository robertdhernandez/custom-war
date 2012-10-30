#include "console.h"
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

void clearConsole( Arguments args )
{
	Console::getSingleton().clearHistory();
}

void defaultCommands( Console& console )
{
	static Console::Cmds cmds;
	if ( cmds.empty() )
	{
		cmds.insert( make_pair( "clear", bind( clearConsole, _1 ) ) );
	}

	console.addCommands( cmds );
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

void le_create( Arguments args )
{
	if ( args.size() >= 2 )
		getLevelEditor().createMap( std::stoi( args[ 0 ] ), std::stoi( args[ 1 ] ) );
}

void levelEditorCommands( Console& console )
{
	static Console::Cmds cmds;
	if ( cmds.empty() )
	{
		cmds.insert( make_pair( "le_create", bind( le_create, _1 ) ) );
	}

	console.addCommands( cmds );
}


/***************************************************/

} // namespace console

} // namespace cw