@REM
@REM build
@REM


@REM
@REM ����
@REM

@CALL VERSION.cmd
@SET SVCICON_MACRO=
@SET VC_CL_ARG=-D_USING_V110_SDK71_
@IF /I "" NEQ "%~1" CALL "%~dp0%1.cmd"
@IF /I "" NEQ "%~2" CALL "%~dp0%2.cmd"
@IF /I "" NEQ "%~3" CALL "%~dp0%3.cmd"
@IF /I "" NEQ "%~4" CALL "%~dp0%4.cmd"
@SET CALL_VCVARSALL_CMD=%~dp0call.vcvarsall.cmd


@REM
@REM BUILD.h �̍쐬
@REM

@ECHO // ���̃t�@�C���� solomon\build\subcmd\build.cmd �Ŏ����������ꂽ���̂ł��B >BUILD.h
@ECHO #define BUILD %* >>BUILD.h


@REM
@REM VERSION.h �̍쐬
@REM

@ECHO // ���̃t�@�C���� VERSION.cmd �̓��e������ solomon\build\subcmd\build.cmd �Ŏ����������ꂽ���̂ł��B >VERSION.h
@ECHO #define VERSION_MAJOR %VERSION_MAJOR% >>VERSION.h
@ECHO #define VERSION_MINOR %VERSION_MINOR% >>VERSION.h
@ECHO #define VERSION_BUILD %VERSION_BUILD% >>VERSION.h


@REM
@REM �{�̏����Ăяo��
@REM

@CALL "%~dp0vc.build.cmd"
