@REM
@REM �����[�X�p�b�P�[�W�̍쐬
@REM

@REM
@REM ���ϐ��̏�����
@REM

@SET SVCICON_RELEASE_DIR=%~dp0release
@SET SVCICON_BUILD=release x86 unicode
@SET SOLOMON_7ZIP_EXE=C:\Program Files\7-Zip\7z.exe

@SET SOLOMON_CONFIG_DIR=%~dp0source\solomon\conf
@SET SOLOMON_CONFIG_CMD=%SOLOMON_CONFIG_DIR%\config.cmd
@SET SOLOMON_CONFIG_LOCALHOST_CMD=%SOLOMON_CONFIG_DIR%\config.%COMPUTERNAME%.cmd
@SET SOLOMON_CONFIG_LOCALHOST_USER_CMD=%SOLOMON_CONFIG_DIR%\config.%COMPUTERNAME%.%USERNAME%.cmd

@REM
@REM SOLOMON���ʐݒ�t�@�C���̓ǂݍ���
@REM

@REM solomon\conf\config.cmd �̓ǂݍ���
@IF NOT EXIST "%SOLOMON_CONFIG_CMD%" COLOR 0C&ECHO CONFIG-ERROR: �ݒ�t�@�C�� "%SOLOMON_CONFIG_CMD%" �����݂��܂���B&GOTO :EOF
@CALL "%SOLOMON_CONFIG_CMD%"

@REM solomon\conf\config.%COMPUTERNAME%.cmd ������ꍇ�͂�������ǂݍ���
@IF EXIST "%SOLOMON_CONFIG_LOCALHOST_CMD%" CALL "%SOLOMON_CONFIG_LOCALHOST_CMD%"

@REM solomon\conf\config.%COMPUTERNAME%.%USERNAME%.cmd ������ꍇ�͂�������ǂݍ���
@IF EXIST "%SOLOMON_CONFIG_LOCALHOST_USER_CMD%" CALL "%SOLOMON_CONFIG_LOCALHOST_USER_CMD%"


@REM
@REM 7zip �̑��݊m�F
@REM

:ZIP_TOOL_CHECK
@IF NOT EXIST "%SOLOMON_7ZIP_EXE%" COLOR 0C&ECHO %SOLOMON_MESSAGE_PREFIX%CONFIG-ERROR: "%SOLOMON_7ZIP_EXE%"��������܂���B7zip���C���X�g�[������Ă��Ȃ���ZIP�t�@�C���쐬�@�\�͗��p�ł��܂���B�ݒ���m�F���Ă��������B&GOTO :EOF
:ZIP_TOOL_CHECK_END


@REM
@REM �o�͐�f�B���N�g���̍쐬
@REM

@IF NOT EXIST "%SVCICON_RELEASE_DIR%" MKDIR "%SVCICON_RELEASE_DIR%"


@REM
@REM �o�͐�p�X�̍쐬
@REM

@CALL "%~dp0snapshot\result\%SVCICON_BUILD%\VERSION.cmd"
@SET SVCICON_RELEASE_ZIP=%SVCICON_RELEASE_DIR%\svcicon-%VERSION_MAJOR%.%VERSION_MINOR%.%VERSION_BUILD%.zip


@REM
@REM �p�b�P�[�W�쐬
@REM

@CALL "%SOLOMON_7ZIP_EXE%" a -tzip "%SVCICON_RELEASE_ZIP%" "%~dp0snapshot\result\%SVCICON_BUILD%\svcicon.exe" >NUL
@CALL "%SOLOMON_7ZIP_EXE%" a -tzip "%SVCICON_RELEASE_ZIP%" "%~dp0LICENSE_1_0.txt" >NUL
@REM CALL "%SOLOMON_7ZIP_EXE%" a -tzip "%SVCICON_RELEASE_ZIP%" "%~dp0readme.txt" >NUL
@REM CALL "%SOLOMON_7ZIP_EXE%" a -tzip "%SVCICON_RELEASE_ZIP%" "%~dp0history.txt" >NUL

@ECHO �����[�X�p�p�b�P�[�W %SVCICON_RELEASE_ZIP% ���쐬���܂����B
@ECHO �܂܂��ׂ��t�@�C�����܂܂�Ă��邱�ƂƊ܂܂��ׂ��łȂ��t�@�C�����܂܂�Ă��Ȃ����Ƃ��f�B���N�g��������r�c�[���Ń`�F�b�N���Ă��������B
@ECHO history.txt ��ҏW���Y��Ă���ꍇ�͐������ҏW������ł��̃R�}���h���ēx���s���ă����[�X�p�p�b�P�[�W���쐬�������Ă��������B
