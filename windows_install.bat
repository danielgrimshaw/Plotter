SET BUILD_DIR=build
SET SRC_DIR=src
SET BIN_DIR=bin
SET LIB_DIR=lib

IF NOT EXIST %BUILD_DIR% MKDIR %BUILD_DIR%

CD %BUILD_DIR%

WHERE mingw32-make
IF %ERRORLEVEL% NEQ 0 ECHO Unable to find mingw32-make, is MinGW installed?

mingw32-make

cd ..

IF NOT EXIST %BIN_DIR% MKDIR %BIN_DIR%

XCOPY /y %BUILD_DIR%\Plotter.exe %BIN_DIR%
XCOPY /y %SRC_DIR%\*.glsl %BIN_DIR%
XCOPY /y %SRC_DIR%\*.csv %BIN_DIR%
XCOPY /y %LIB_DIR%\* %BIN_DIR%

ECHO Compile finished!
EXIT 0