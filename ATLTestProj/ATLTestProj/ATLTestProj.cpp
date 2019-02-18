// ATLTestProj.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "ATLTestProj_i.h"


using namespace ATL;


class CATLTestProjModule : public ATL::CAtlExeModuleT< CATLTestProjModule >
{
public :
	DECLARE_LIBID(LIBID_ATLTestProjLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLTESTPROJ, "{3b45d432-470b-4a28-b9b8-ecda49340186}")
};

CATLTestProjModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

