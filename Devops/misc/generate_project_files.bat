@echo off

call "%~dp0\..\config.bat"

rem UBT flags for build from sources: -game -engine
"%VersionSelector%" -switchversionsilent "%ProjectPath%" "%EnginePath%"
"%EnginePath%\%UBTRelativePath%" -projectfiles -progress -project="%ProjectPath%"

