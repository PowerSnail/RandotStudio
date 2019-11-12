:: MUST set the following environment correctly:
:: 1. Make sure VS tool chain is set correctly (better use a VS prompt)
:: 2. Make sure Qt executables are in PATH
:: 3. Make sure QtInstallerFramework executables are in PATH

@echo off
SET CURRENT_DIR=%CD%
SET RELEASE_DIR=%CURRENT_DIR%\build\release
SET BUILD_INSTALLER_DIR=%CURRENT_DIR%\build\installer\randotstudio
SET INSTALLER_DESTINATION=%CURRENT_DIR%\installer\packages\com.powersnail.randotstudio\data
SET INSTALLER_OUTPUT_DIR=%CURRENT_DIR%\build\installer

mkdir %BUILD_INSTALLER_DIR%
CD %BUILD_INSTALLER_DIR%

COPY /Y %RELEASE_DIR%\RandotStudio.exe .\
COPY /Y %RELEASE_DIR%\RandotStudio.exe.manifest .\
if %ERRORLEVEL% NEQ 0 (
    echo "ERROR in copying: " %ERRORLEVEL%
    GOTO error
)

windeployqt.exe .\RandotStudio.exe
if %ERRORLEVEL% NEQ 0 (
    echo "ERROR in windeployqt.exe: " %ERRORLEVEL%
    GOTO error
)

archivegen.exe %INSTALLER_DESTINATION%\randotstudio.7z .
if %ERRORLEVEL% NEQ 0 (
    echo "ERROR in archivegen.exe: " %ERRORLEVEL%
    GOTO error
)

CD %CURRENT_DIR%
binarycreator.exe  --offline-only -p .\installer\packages -c .\installer\config\config.xml %INSTALLER_OUTPUT_DIR%\installer.exe
if %ERRORLEVEL% NEQ 0 (
    echo "ERROR in binarycreator.exe: " %ERRORLEVEL%
    GOTO error
)

echo The installer has been created in %INSTALLER_OUTPUT_DIR%

:error

