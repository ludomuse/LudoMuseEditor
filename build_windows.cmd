echo on

SET project_dir="%cd%"

echo Configuring cocos
call configure-cocos-win.cmd %project_dir%

echo Set up environment...
set PATH=%QT%\bin\;C:\Qt\Tools\QtCreator\bin\;C:\Qt\QtIFW2.0.1\bin\;%PATH%
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" %PLATFORM%


rem echo Building LudoMuseEditor...
rem curl -s -O http://ihmtek-services.com/files/LudoMuse/deps.zip
rem unzip -n deps.zip -d .

cd %APPVEYOR_BUILD_FOLDER%
mkdir LudoMuseEditorWin
mkdir ..\BUILD
rem cd LudoMuseEditorWin
echo "trying to copy files from LudoMuse build"
rem dir "..\..\LudoMuse\proj.win32\Release.win32"
xcopy "..\LudoMuse\proj.win32\Release.win32" "..\BUILD\" /D /E /I /F /Y
xcopy "deploy\win" "..\BUILD\" /D /E /I /F /Y
rem xcopy "..\LudoMuse\proj.win32\Release.win32\*.dll" ".\"
rem xcopy "..\LudoMuse\proj.win32\Release.win32\*.lib" ".\"
rem xcopy ..\..\LudoMuse\Resources\ .\ /D /E /I /F /Y
cd ..\BUILD
qmake -spec win32-msvc2015 CONFIG+=x86_64 CONFIG-=debug CONFIG+=release -config release ../LudoMuseEditor LUDOMUSE_PATH=../LudoMuse
nmake release
rem nmake INSTALL_ROOT=../LudoMuseEditor/LudoMuseEditorWin install


cd ..\

rem xcopy deploy\common LudoMuseEditorWin\ /D /E /I /F /Y
for /R BUILD %%a in (*.dll) do xcopy "%%a" LudoMuseEditor\LudoMuseEditorWin
for /R BUILD %%a in (*.exe) do xcopy "%%a" LudoMuseEditor\LudoMuseEditorWin
REM for /R BUILD %%a in (*.dll) do xcopy "%%a" LudoMuseEditor\LudoMuseEditorWin\debug
REM for /R BUILD %%a in (*.dll) do xcopy "%%a" LudoMuseEditor\LudoMuseEditorWin\release
xcopy LudoMuseEditor\deploy\common LudoMuseEditor\LudoMuseEditorWin /D /E /I /F /Y
xcopy LudoMuse\Resources LudoMuseEditor\LudoMuseEditorWin /D /E /I /F /Y


curl -s "https://ihmtek-services.com/files/LudoMuse/video.mp4" -o LudoMuseEditor\LudoMuseEditorWin\default\cache\video.mp4

cd LudoMuseEditor\LudoMuseEditorWin

windeployqt LudoMuseEditor.exe

cd ..\

rem zip -r LudoMuseEditor-win32.zip LudoMuseEditorWin


rem echo Running tests...

REM echo Packaging...
REM cd %project_dir%\build\windows\msvc\x86_64\release\
REM windeployqt LudoMuseEditor.exe

rem rd /s /q LudoMuseEditorWin\moc\
rem rd /s /q LudoMuseEditorWin\obj\
rem rd /s /q LudoMuseEditorWin\qrc\

echo "creating archive ..."
7z a LudoMuseEditor-win-%APPVEYOR_BUILD_NUMBER%-portable.zip LudoMuseEditorWin


rem echo Copying project files for archival...
rem copy "%project_dir%\README.md" "LudoMuseEditor\README.md"
rem copy "%project_dir%\LICENSE" "LudoMuseEditor\LICENSE.txt"
rem copy "%project_dir%\Qt License" "LudoMuseEditor\Qt License.txt"

rem echo Copying files for installer...
rem mkdir "%project_dir%\installer\windows\x86_64\packages\com.yourappproject.yourapp\data\"
rem robocopy LudoMuseEditor\ "%project_dir%\installer\windows\x86_64\packages\com.yourappproject.yourapp\data" /E

rem echo Packaging portable archive...
rem 7z a LudoMuseEditor_%TAG_NAME%_windows_x86_64_portable.zip LudoMuseEditor

rem echo Creating installer...
rem cd %project_dir%\installer\windows\x86_64\
rem binarycreator.exe --offline-only -c config\config.xml -p packages LudoMuseEditor_%TAG_NAME%_windows_x86_64_installer.exe
