@REM
@REM build
@REM

@REM
@REM ����
@REM

@SET BUILD_NAME=%~n0
@SET BUILD_BODY_CMD=%~dp0subcmd\build.cmd

@REM
@REM �{�̏����Ăяo��
@REM

@CALL "%BUILD_BODY_CMD%" %~n0
