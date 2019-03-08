#pragma once

template< class ... Args >
void Log( Args&& ... args )
{
	int a[]{ (std::cout << args,0)... };
	static_cast<void>(a);
}

#define LOG_FN( ... ) Log( __FUNCSIG__, ">> ", __VA_ARGS__ )
#define LOG_FILELINE_FN( ... ) Log( __FILE__, "(", __LINE__ , ")::", __FUNCSIG__, ">> ", __VA_ARGS__ )