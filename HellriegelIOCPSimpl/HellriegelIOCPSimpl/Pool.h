#pragma once

#include <vector>
#include <list>
#include <mutex>
#include <shared_mutex>
#include <memory>

//Key Type, Object ID

template< class T >
class Pool
{
public:
	using KeyT = std::int64_t;
	static const int DEFAULT_LEN = 256;

	Pool(size_t contSize);
	Pool() : Pool(DEFAULT_LEN) {};
	virtual ~Pool() {};	

	Pool(const Pool&) = delete;
	Pool& operator =(const Pool&) = delete;
private:
	struct Data_T
	{ 
		KeyT nIdx = -1;
		bool bUse = false;
		std::shared_ptr< T > _data;
	};

	std::shared_mutex _mtxCont;
	std::vector<Data_T> _container;

	std::mutex _mtxAvail;
	std::list< KeyT > _ObjAvailable;
};

template<class T>
Pool<T>::Pool(size_t contSize)
	:_container( contSize )
{

}
