#pragma once

#include <functional>

namespace cw
{
	namespace util
	{
		//-------------------------------------------------------------------------
		// Implementation of std::reference_wrapper because Visual Studio 10 is crap with C++11
		// http://en.cppreference.com/w/cpp/utility/functional/reference_wrapper
		//-------------------------------------------------------------------------
		template< typename T >
		class ReferenceWrapper
		{
		public:
			ReferenceWrapper( T& x ) : m_object( &x ) {}
			ReferenceWrapper( T&& x ); // undefined, reference to temporary is not allowed
			ReferenceWrapper( const ReferenceWrapper< T >& other ) : m_object( &other.get() ) {}

			ReferenceWrapper& operator=( const ReferenceWrapper< T >& other ) { m_object = &other.get(); }

			T& get() { return *m_object; }
			const T& get() const { return *m_object; }

			operator T&() { return get(); }
			operator const T&() const { return get(); }

			operator std::reference_wrapper< T >() { return std::ref( *m_object ); }
			operator std::reference_wrapper< T >() const { return std::cref( *m_object ); }

		private:
			T* m_object;
		};
	}
}