:: MUST set the following environment correctly:
:: 1. Make sure VS tool chain is set correctly (better use a VS prompt)
:: 2. Make sure Qt executables are in PATH
:: 3. Make sure QtInstallerFramework executables are in PATH

@echo off
SET RELEASE_DIR=build\release
SET BUILD_INSTALLER_DIR=build\installer\randotstudio
SET INSTALLER_DESTINATION=installer\packages\com.powersnail.randotstudio\data
SET INSTALLER_OUTPUT_DIR=build\installer

mkdir %BUILD_INSTALLER_DIR%
if %ERRORLEVEL% NEQ 0 exit %ERRORLEVEL%
copy %RELEASE_DIR%\RandotStudio.exe %BUILD_INSTALLER_DIR%
if %ERRORLEVEL% NEQ 0 exit %ERRORLEVEL%
windeployqt.exe %BUILD_INSTALLER_DIR%\RandotStudio.exe
if %ERRORLEVEL% NEQ 0 exit %ERRORLEVEL%
archivegen.exe %INSTALLER_DESTINATION%\randotstudio.7z %BUILD_INSTALLER_DIR%
if %ERRORLEVEL% NEQ 0 exit %ERRORLEVEL%
binarycreator.exe -p .\installer\packages -c .\installer\config\config.xml %INSTALLER_OUTPUT_DIR%\installer.exe
if %ERRORLEVEL% NEQ 0 exit %ERRORLEVEL%

echo The installer has been created in %INSTALLER_OUTPUT_DIR%
