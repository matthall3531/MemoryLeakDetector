call "%VS140COMNTOOLS%\VsDevCmd.bat"

if exist .\build_release_win64_vs2015 rmdir /q /s .\build_release_win64_vs2015

mkdir .\build_release_win64_vs2015

cd .\build_release_win64_vs2015

cmake -G "Visual Studio 14 2015 Win64" -DMSVC_RUNTIME=static ..
