:: MUST set the following environment correctly:
:: 1. Make sure VS tool chain is set correctly (better use a VS prompt)
:: 2. Make sure Qt executables are in PATH
:: 3. Make sure QtInstallerFramework executables are in PATH

@echo off
SET CURRENT_DIR=%CD%
SET RELEASE_DIR=%CURRENT_DIR%\build
SET INSTALLER_CONFIG_DIR=%CURRENT_DIR%\Installer
SET BUILD_INSTALLER_DIR=%CURRENT_DIR%\build\installer\randotstudio
SET INSTALLER_DESTINATION=%INSTALLER_CONFIG_DIR%\packages\com.powersnail.randotstudio\data
SET INSTALLER_OUTPUT_DIR=%CURRENT_DIR%\build\installer

IF EXIST "%BUILD_INSTALLER_DIR%" (
    DEL /S /Q %BUILD_INSTALLER_DIR%/*
) else (
    MKDIR %BUILD_INSTALLER_DIR%
)

CD %BUILD_INSTALLER_DIR%

COPY /Y %RELEASE_DIR%\MinSizeRel\RandotStudio.exe .\
:: COPY /Y %RELEASE_DIR%\RandotStudio.exe.manifest .\
COPY /Y %RELEASE_DIR%\*.qm .\
if %ERRORLEVEL% NEQ 0 (
    echo "ERROR in copying: " %ERRORLEVEL%
    GOTO error
)

windeployqt.exe .\RandotStudio.exe
if %ERRORLEVEL% NEQ 0 (
    echo "ERROR in windeployqt.exe: " %ERRORLEVEL%
    GOTO error
)

IF EXIST %INSTALLER_DESTINATION%\randotstudio.7z DEL /Q %INSTALLER_DESTINATION%\randotstudio.7z

archivegen.exe %INSTALLER_DESTINATION%\randotstudio.7z .
if %ERRORLEVEL% NEQ 0 (
    echo "ERROR in archivegen.exe: " %ERRORLEVEL%
    GOTO error
)

CD %CURRENT_DIR%
binarycreator.exe  --offline-only -p %INSTALLER_CONFIG_DIR%\packages -c %INSTALLER_CONFIG_DIR%\config\config.xml %INSTALLER_OUTPUT_DIR%\RandotStudioInstaller.exe
if %ERRORLEVEL% NEQ 0 (
    echo "ERROR in binarycreator.exe: " %ERRORLEVEL%
    GOTO error
)

echo The installer has been created in %INSTALLER_OUTPUT_DIR%

:error

