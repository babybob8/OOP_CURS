# How to build (follow carefully)
## for WINDOWS
### Console App

- cd OOP_CURS\build\console
- where qmake // Search for [ Qt\Qt5.12.12\5.12.12\mingw73_32\bin\qmake.exe ] -> path/to/qmake
- where mingw32-make.exe // Search for [ Qt\Qt5.12.12\Tools\mingw730_32\bin\mingw32-make.exe ] -> path/to/make
- where windeployqt // Search for [ Qt\Qt5.12.12\5.12.12\mingw73_32\bin\windeployqt.exe ] -> path/to/windeployqt

> #### ! THIS PATH YOU HAVE TO SPECIFY YOURSELF !
> - set PATH=C:\Qt\Qt5.12.12\Tools\mingw730_32\bin;%PATH%

- path/to/qmake ../../src/OOP_Curs.pro -spec win32-g++ "CONFIG+=debug" "CONFIG+=qml_debug" "CONFIG+=console"
- path/to/make
- path/to/windeployqt --debug debug/OOP_CURS_Console.exe

Now you can:<br>
debug\OOP_CURS_Console.exe ../../data/student.csv ../../data/work.csv ../../data/result.csv

### GUI App

- cd OOP_CURS\build\debug
- where qmake // Search for [ Qt\Qt5.12.12\5.12.12\mingw73_32\bin\qmake.exe ] -> path/to/qmake
- where mingw32-make.exe // Search for [ Qt\Qt5.12.12\Tools\mingw730_32\bin\mingw32-make.exe ] -> path/to/make
- where windeployqt // Search for [ Qt\Qt5.12.12\5.12.12\mingw73_32\bin\windeployqt.exe ] -> path/to/windeployqt

> #### THIS PATH YOU MIGHT HAVE TO SPECIFY YOURSELF
> - set PATH=C:\Qt\Qt5.12.12\Tools\mingw730_32\bin;%PATH%

- path/to/qmake ../../src/OOP_Curs.pro -spec win32-g++ "CONFIG+=debug" "CONFIG+=qml_debug"
- path/to/make
- path/to/windeployqt --debug debug/OOP_CURS_GUI.exe

Now you can:<br>
debug\OOP_CURS_GUI.exe

## for LINUX (tested only on mint)
### Console app

> #### You might have to get OpenGL
> - sudo apt update
> - sudo apt install libgl1-mesa-dev
> #### and i kinda guess that you already have make somewhere there:
> /bin/make

- cd OOP_CURS/build/console
- find ~/Qt -name qmake // Search for [ Qt/6.10.1/gcc_64/bin/qmake ] -> path/to/qmake
- path/to/qmake ../../src/OOP_Curs.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug CONFIG+=console
- /bin/make

Now you can:<br>
./OOP_CURS_Console ../../data/student.csv ../../data/work.csv ../../data/result.csv

### GUI App

> #### You might have to get OpenGL
> - sudo apt update
> - sudo apt install libgl1-mesa-dev
> #### and i kinda guess that you already have make somewhere there:
> /bin/make

- cd OOP_CURS/build/debug
- find ~/Qt -name qmake // Search for [ Qt/6.10.1/gcc_64/bin/qmake ] -> path/to/qmake
- path/to/qmake ../../src/OOP_Curs.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug
- /bin/make

Now you can:<br>
./OOP_CURS_GUI
