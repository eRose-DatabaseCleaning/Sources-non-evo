@rem =====================================================
@rem Build a batch file
@rem 
@rem 2004-01-27 zho
@rem =====================================================


@rem if "%VS71COMNTOOLS%"=="" goto error

@rem Visual studio environment variable settings
@rem call %VS71COMNTOOLS%\vsvars32.bat

@if "%VSINSTALLDIR%"=="" goto Usage
@SET DEVENV=%VSINSTALLDIR%\devenv.exe

@rem =====================================================
@rem znzin release
@echo building znzin release...
%DEVENV% msvc71\engine.sln /build Release /project znzin
@rem =====================================================

@goto end

:error
@echo Error: The environment variable is not set correctly.
@goto end

:end
pause