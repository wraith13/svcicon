@REM
@REM build
@REM


@REM
@REM �R���p�C��
@REM

@REM �R���p�C�����O�o�͐�
@SET RC_COMPILE_LOG_FILE=.\rc_compile.log
@SET COMPILE_LOG_FILE=.\compile.log

@REM �R���p�C�����ʂ̃R�s�[��f�B���N�g���̍쐬
@SET RESULT_DIR=%SOLOMON_SNAPSHOT_DIR%\result\%BUILD_NAME%
@IF NOT EXIST "%RESULT_DIR%" CALL "%SOLOMON_LIBCMD_DIR%\mkdir.ex.cmd" "%RESULT_DIR%"

@REM �R���p�C�����ʃf�B���N�g���Ƀo�[�W�������t�@�C�������݂��Ă�����폜
@IF EXIST "%RESULT_DIR%\VERSION.cmd" DEL "%RESULT_DIR%\VERSION.cmd"

@REM ���\�[�X�R���p�C��
@SETLOCAL
@CALL "%CALL_VCVARSALL_CMD%" %VCVARSALL_ARG% >NUL
@SET COMPILE_EXECUTE_CMD=rc %SVCICON_MACRO% .\svcicon.rc
@ECHO ��%COMPILE_EXECUTE_CMD%
@%COMPILE_EXECUTE_CMD%>>"%RC_COMPILE_LOG_FILE%" 2>&1
@ENDLOCAL

@REM ���\�[�X�R���p�C�����ʔ���
@IF NOT EXIST ".\svcicon.res" CALL "%SOLOMON_COMPILE_FAILED_CMD%" "%RC_COMPILE_LOG_FILE%" & GOTO :EOF

@REM ���\�[�X�R���p�C���x������
@TYPE "%RC_COMPILE_LOG_FILE%" | FIND /I " warning " >NUL
@IF /I "1" NEQ "%ERRORLEVEL%" CALL "%SOLOMON_COMPILE_WARNED_CMD%" "%RC_COMPILE_LOG_FILE%"

@REM �ʏ�R���p�C��
@SETLOCAL
@CALL "%CALL_VCVARSALL_CMD%" %VCVARSALL_ARG% >NUL
@SET COMPILE_EXECUTE_CMD=cl ".\svcicon.cpp" %VC_CL_ARG% %SVCICON_MACRO% /EHsc /MP /W4 /Fesvcicon.exe /link %VCLINKER_ARG% svcicon.res
@ECHO ��%COMPILE_EXECUTE_CMD%
@%COMPILE_EXECUTE_CMD%>>"%COMPILE_LOG_FILE%" 2>&1
@ENDLOCAL

@REM �ʏ�R���p�C�����ʔ���
@IF NOT EXIST ".\svcicon.exe" CALL "%SOLOMON_COMPILE_FAILED_CMD%" "%COMPILE_LOG_FILE%" & GOTO :EOF

@REM �ʏ�R���p�C���x������
@TYPE "%COMPILE_LOG_FILE%" | FIND /I " warning " >NUL
@IF /I "1" NEQ "%ERRORLEVEL%" CALL "%SOLOMON_COMPILE_WARNED_CMD%" "%COMPILE_LOG_FILE%"

@REM �R���p�C�����ʂ��R�s�[
@COPY /Y ".\*.exe" "%RESULT_DIR%" >NUL
@COPY /Y ".\VERSION.cmd" "%RESULT_DIR%" >NUL

@REM �R���p�C���������������Ƃ̒ʒm
CALL "%SOLOMON_COMPILE_SUCCESS_CMD%"