#include "console.h"
#include "console_functions.h"

namespace cw
{
namespace console
{

using std::bind;
using std::make_pair;
using namespace std::placeholders;

/***************************************************/
//	COMMON COMMANDS

void clearConsole( const std::vector< std::string >& args )
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

void levelEditorCommands( Console& console )
{
}


/***************************************************/

} // namespace console

} // namespace cw