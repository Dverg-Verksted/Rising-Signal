@echo off

rem Engine params
set EnginePath=C:\Program Files\Epic Games\UE_4.27

set UBTRelativePath=Engine\Binaries\DotNET\UnrealBuildTool.exe
set VersionSelector=C:\Program Files (x86)\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe

rem !! Engine version for packaging !!
set RunUATPath=%EnginePath%\Engine\Build\BatchFiles\RunUAT.bat

rem Project params
set ProjectRoot=C:\Users\Mark\Desktop\Rising-Signal
set ProjectPureName=RisingSignal
set ProjectName=%ProjectPureName%.uproject
set ProjectPath=%ProjectRoot%\%ProjectName%

rem Build params
set Platform=Win64
set Configuration=Shipping
set ArchivePath=%ProjectRoot%\Build

rem Other params
set SourceCodePath=%ProjectRoot%\Source
set dirsToRemove=Intermediate DerivedDataCache Saved Binaries .vs .idea Script
set filesToRemove=*.sln

rem Target params
set COPYRIGHT_LINE=// My game copyright