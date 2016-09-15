call "%VS120COMNTOOLS%\%vsvars32.bat"


set VS_SUFFIX=vs120
set POCO_BIN=%POCO_DIR%\bin\%VS_SUFFIX%
set PATH=%POCO_BIN%;%PATH%
set LIB=%POCO_DIR%\lib\%VS_SUFFIX%;%LIB%
set INCLUDE=^
%POCO_DIR%\Foundation\include;^
%POCO_DIR%\Net\include;^
%POCO_DIR%\Util\include;^
%POCO_DIR%\XML\include;^
%POCO_DIR%\Zip\include;^
%INCLUDE%


set INCLUDE=%BOOST_DIR%;%INCLUDE%
set LIB=%BOOST_DIR%\stage\lib\%VS_SUFFIX%;%LIB%


@start devenv /useenv vs/doc-processor.sln

:: %POCO_DIR%\NetSSL_OpenSSL\include;^
:: %POCO_DIR%\Crypto\include;^
:: %POCO_DIR%\SevenZip\include;^