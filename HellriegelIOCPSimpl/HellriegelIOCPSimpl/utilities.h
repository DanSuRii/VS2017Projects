#pragma once

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