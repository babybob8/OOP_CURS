
cd OOP_CURS\build\console

where qmake // Search for [ Qt\Qt5.12.12\5.12.12\mingw73_32\bin\qmake.exe ] -> path/to/qmake
where mingw32-make.exe // Search for [ Qt\Qt5.12.12\Tools\mingw730_32\bin\mingw32-make.exe ] -> path/to/make
where windeployqt // Search for [ Qt\Qt5.12.12\5.12.12\mingw73_32\bin\windeployqt.exe ] -> path/to/windeployqt

path/to/qmake ../../src/OOP_Curs.pro -spec win32-g++ "CONFIG+=debug" "CONFIG+=qml_debug" "CONFIG+=console"
path/to/make
path/to/windeployqt --debug debug/OOP_CURS_Console.exe
debug/OOP_CURS_Console.exe../../data/student.csv ../../data/work.csv ../../data/result.csv
