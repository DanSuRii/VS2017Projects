#ifndef __INTERFACES_H__
#define __INTERFACES_H__

#include <memory>
#include <vector>

class ITest
{
public:
	virtual void Do() = 0;
};
using PITest = std::shared_ptr<ITest>;

class TestRepo
{
public:
private:
	std::vector< PITest > _cont;
};

#endif // !__INTERFACES_H__
