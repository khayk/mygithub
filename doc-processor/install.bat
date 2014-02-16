mkdir "%~dp0\release\"

copy "%~dp0\bin\ascii-to-unicode.exe"                 "%~dp0\release\ascii-to-unicode.exe"
copy "%~dp0\config\doc-processor.properties"          "%~dp0\release\ascii-to-unicode.properties"
copy "%~dp0\config\launch.bat"                        "%~dp0\release\launch.bat"
copy "%~dp0\config\README.txt"                        "%~dp0\release\README.txt"

xcopy "%~dp0\config\mapping\*.*" "%~dp0\release\mapping" /S /i /y

