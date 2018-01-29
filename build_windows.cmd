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


cd ../
mkdir LudoMuseEditorWin
cd LudoMuseEditorWin
copy ../LudoMuse/proj.win32/Release.win32/LudoMuse.exe ./
copy ../LudoMuse/proj.win32/Release.win32/*.dll ./
copy ../LudoMuse/proj.win32/Release.win32/*.lib ./
qmake -spec win32-msvc2015 CONFIG+=x86_64 CONFIG-=debug CONFIG+=release ../LudoMuseEditor
nmake

rem echo Running tests...

REM echo Packaging...
REM cd %project_dir%\build\windows\msvc\x86_64\release\
REM windeployqt LudoMuseEditor.exe

rem rd /s /q LudoMuseEditor\moc\
rem rd /s /q LudoMuseEditor\obj\
rem rd /s /q LudoMuseEditor\qrc\

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
