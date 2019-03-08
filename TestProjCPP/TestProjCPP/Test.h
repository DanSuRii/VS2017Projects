#pragma once

enum ETests
{
	HELLOWORLD,
	THHELLOWORLD,


	ETESTS_CNT,
};

class ITest
{
public:
	virtual void Do() = 0;
	virtual const std::string& GetName() = 0;
	virtual const int GetMyIdx() = 0;
};
using PITest = std::shared_ptr< ITest >;

template< ETests eTest >
class TestImpl : public ITest
{
public:
	TestImpl(char const* strName)
		: _strName(strName)
	{

	}
	TestImpl(std::string&& strName)
		: _strName(strName)
	{

	}
	// Inherited via ITest
	virtual void Do() override;
	virtual const std::string & GetName() override
	{
		// TODO: insert return statement here
		return _strName;
	}

	// Inherited via ITest
	virtual const int GetMyIdx() override
	{
		return static_cast<int>(eTest);
	}

private:
	std::string _strName;
	// Inherited via ITest
};