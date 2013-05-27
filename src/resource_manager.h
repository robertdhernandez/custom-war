#pragma once

#include <string>
#include <memory>
#include <unordered_map>

namespace cw
{
	template< typename T >
	class BaseResourceManager
	{
	public:
		virtual ~BaseResourceManager() {}

		std::shared_ptr< T > load( const std::string& str )
		{
			auto find = m_data.find( str );
			if ( find != m_data.end() )
			{
				if ( !find->second.expired() )
					return find->second.lock();
				else
				{
					std::shared_ptr< T > val = generate( str );
					find->second = val;
					return val;
				}
			}
			else
			{
				std::shared_ptr< T > val = generate( str );
				m_data.insert( std::make_pair( str, std::weak_ptr< T >( val ) ) );
				return val;
			}
		}

	private:
		virtual std::shared_ptr< T > generate( const std::string& str ) const = 0;

	private:
		std::unordered_map< std::string, std::weak_ptr< T > > m_data;
	};
}