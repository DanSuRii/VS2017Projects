#pragma once

#include <atomic>
#include <functional>

#define DISALLOW_COPY( TypeName ) \
	TypeName( const TypeName& ) = delete;

#define DISALLOW_ASSIGN( TypeName ) \
	TypeName& operator=( const TypeName& ) = delete;

#define DISALLOW_COPY_AND_ASSIGN( TypeName ) \
	DISALLOW_COPY( TypeName ) \
	DISALLOW_ASSIGN( TypeName )

template< typename T >
class AutoReset {
public:
	AutoReset(T* scoped_variable, T new_value)
		: scoped_vairable_(scoped_variable)
		, original_value_(std::move(*scoped_variable))
	{
		*scoped_vairable_ = std::move(new_value);
	}

	~AutoReset()
	{
		*scoped_vairable_ = std::move(original_value_);
	}

	DISALLOW_COPY_AND_ASSIGN(AutoReset);
		
private:
	T* scoped_vairable_;
	T original_value_;
};

template< class T, class ... Args >
std::shared_ptr<T> New(Args&& ... args)
{
	return std::make_shared< T >( std::forward<Args>(args) ... );
}

class RAII
{
public:
	using _CALLABLE = std::function< void(void) >;

	RAII(_CALLABLE fn)
		: _fn(fn)
	{

	}

	~RAII()
	{
		if (_fn) _fn();
	}
private:
	_CALLABLE _fn;
};

template< class Type >
class UniqueID
{
public:
	UniqueID()		
	{
		static std::atomic_int64_t generator;
		_id = ++generator;
	}	


	const auto GetID() { return _id; };
private:
	std::int64_t _id;
};

