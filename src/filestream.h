#pragma once

#include "datastream.h"
#include <fstream>

namespace cw
{
	namespace serial
	{
		class InputFilestream : public InputDatastream
		{
		public:
			InputFilestream( const std::string& );

		private:
			void read( char*, std::size_t );
			std::ifstream m_file;
		};

		class OutputFilestream : public OutputDatastream
		{
		public:
			OutputFilestream( const std::string& );

		private:
			void write( const char*, std::size_t );
			std::ofstream m_file;
		};
	}
}