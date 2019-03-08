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

	struct Out_T
	{
		KeyT idx = -1;
		std::shared_ptr< T > _data;

		Out_T() {};
		Out_T(const Out_T&) {};
		Out_T& operator=(const Out_T&) {};

		virtual ~Out_T() {};
	};

	static const int DEFAULT_LEN = 256;

	Pool(size_t contSize);
	Pool() : Pool(DEFAULT_LEN) {};
	virtual ~Pool() {};	

	Pool(const Pool&) = delete;
	Pool& operator =(const Pool&) = delete;

	//Out_T GetNew();
private:
	struct Data_T : public Out_T
	{ 		
		bool bUse = false;
		Pool* ptrBase;
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
