#pragma once

namespace cw
{
	namespace serial
	{
		class InputDatastream;
		class OutputDatastream;

		class Serializable
		{
		public:
			virtual ~Serializable() {}
			virtual void read( InputDatastream& ) = 0;
			virtual void write( OutputDatastream& ) const = 0;
		};
	}
}