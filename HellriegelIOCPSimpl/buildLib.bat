REM pushd "%VS140COMNTOOLS%..\..\VC\" 
REM vcvarsall.bat x86_amd64
CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsamd64_x86.bat"

msbuild C:\GitRepos\VS2017Projects\HellriegelIOCPSimpl\HellriegelIOCPSimpl\HellriegelIOCPSimpl.vcxproj /t:Rebuild /p:ConfigurationType=StaticLibrary;OutDir=C:\GitRepos\VS2017Projects\HellriegelIOCPSimpl\lib\;Configuration=Debug;PlatformTarget=x86;TargetName=HellIOCPSimpl_Debug_x86

popd