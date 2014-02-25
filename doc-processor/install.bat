mkdir "%~dp0\bin\converter\"
mkdir "%~dp0\bin\converter\input"
mkdir "%~dp0\bin\converter\output"

copy "%~dp0\bin\ascii-to-unicode.exe"                 "%~dp0\bin\converter\ascii-to-unicode.exe"
copy "%~dp0\config\doc-processor.properties"          "%~dp0\bin\converter\ascii-to-unicode.properties"
copy "%~dp0\config\launch-precise.bat"                "%~dp0\bin\converter\launch-precise.bat"
copy "%~dp0\config\launch-quick.bat"                  "%~dp0\bin\converter\launch-quick.bat"
copy "%~dp0\config\README.txt"                        "%~dp0\bin\converter\README.txt"

xcopy "%~dp0\config\mapping\*.*" "%~dp0\bin\converter\mapping" /S /i /y

