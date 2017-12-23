mkdir "%~dp0bin\converter\"
mkdir "%~dp0bin\converter\input"
mkdir "%~dp0bin\converter\output"

copy "%~dp0bin\ascii-to-unicode.exe"                 "%~dp0\bin\converter\ascii-to-unicode.exe"
copy "%~dp0config\doc-processor.properties"          "%~dp0\bin\converter\ascii-to-unicode.properties"
copy "%~dp0config\launch-precise.bat"                "%~dp0\bin\converter\launch-precise.bat"
copy "%~dp0config\README.txt"                        "%~dp0\bin\converter\README.txt"

xcopy "%~dp0config\mapping\*.*" "%~dp0\bin\converter\mapping" /S /i /y

