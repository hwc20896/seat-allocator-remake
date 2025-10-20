rem Utility file.

@echo off

setlocal enabledelayedexpansion

for %%f in (*.ui) do (
    set "filename=%%~nf"
    echo Parsing file from %%f to ui_!filename!.py
    ..\.venv\Scripts\pyuic6 "%%f" -o "ui_!filename!.py"
)
echo All .ui files have been processed.
pause