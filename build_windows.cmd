echo on

SET project_dir="%cd%"

echo Set up environment...
set PATH=%QT%\bin\;C:\Qt\Tools\QtCreator\bin\;C:\Qt\QtIFW2.0.1\bin\;%PATH%
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" %PLATFORM%

echo Building LudoMuseEditor...
qmake -spec win32-msvc2015 CONFIG+=x86_64 CONFIG-=debug CONFIG+=release
nmake

echo Running tests...

echo Packaging...
cd %project_dir%\build\windows\msvc\x86_64\release\
windeployqt --qmldir ..\..\..\..\..\src\ LudoMuseEditor\LudoMuseEditor.exe

rd /s /q LudoMuseEditor\moc\
rd /s /q LudoMuseEditor\obj\
rd /s /q LudoMuseEditor\qrc\

echo Copying project files for archival...
copy "%project_dir%\README.md" "LudoMuseEditor\README.md"
copy "%project_dir%\LICENSE" "LudoMuseEditor\LICENSE.txt"
copy "%project_dir%\Qt License" "LudoMuseEditor\Qt License.txt"

echo Copying files for installer...
mkdir "%project_dir%\installer\windows\x86_64\packages\com.yourappproject.yourapp\data\"
robocopy LudoMuseEditor\ "%project_dir%\installer\windows\x86_64\packages\com.yourappproject.yourapp\data" /E

echo Packaging portable archive...
7z a LudoMuseEditor_%TAG_NAME%_windows_x86_64_portable.zip LudoMuseEditor

echo Creating installer...
cd %project_dir%\installer\windows\x86_64\
binarycreator.exe --offline-only -c config\config.xml -p packages LudoMuseEditor_%TAG_NAME%_windows_x86_64_installer.exe
