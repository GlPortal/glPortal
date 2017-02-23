del %1
set VSTRING=#define GAME_VERSION "
FOR /F "tokens=* USEBACKQ" %%F IN (`git describe`) DO (
SET VSTRING=%VSTRING%%%F
)
set VSTRING=%VSTRING%"
echo %VSTRING% >> %1
