call "%VS120COMNTOOLS%\VsDevCmd.bat"

if exist .\build_release_win64_vs2012 rmdir /q /s .\build_release_win64_vs2012

mkdir .\build_release_win64_vs2012

cd .\build_release_win64_vs2012

cmake -G "Visual Studio 12 2013 Win64" -DMSVC_RUNTIME=static ..
