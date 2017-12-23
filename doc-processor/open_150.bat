:: call "%VS120COMNTOOLS%\%vsvars32.bat"
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
:: call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\vsdevcmd\ext\vcvars.bat" "x64"

set VS_SUFFIX=vc150
set POCO_BIN=%POCO_HOME%\bin\%VS_SUFFIX%
set PATH=%POCO_BIN%;%PATH%
set LIB=%POCO_HOME%\lib\%VS_SUFFIX%;%LIB%
set INCLUDE=^
%POCO_HOME%\Foundation\include;^
%POCO_HOME%\Net\include;^
%POCO_HOME%\Util\include;^
%POCO_HOME%\XML\include;^
%POCO_HOME%\Zip\include;^
%INCLUDE%


set INCLUDE=%BOOST_ROOT%;%INCLUDE%
set LIB=%BOOST_ROOT%\stage\lib\%VS_SUFFIX%;%LIB%


@start devenv /useenv C:\Users\Hayk\Code\repos\mygithub\doc-processor\vs\doc-processor.sln

:: %POCO_HOME%\NetSSL_OpenSSL\include;^
:: %POCO_HOME%\Crypto\include;^
:: %POCO_HOME%\SevenZip\include;^