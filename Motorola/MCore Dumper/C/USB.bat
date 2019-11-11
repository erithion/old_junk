
@PUSHD .\Diab\5.2.1.0\WIN32\bin\

@SET DiabPath=%CD%

@POPD

@CALL .\Lib\Hardware.bat "%DiabPath%"

@SET info=%1.s19 done 

@SET PATH=%DiabPath%

@das .\Lib\crt0.s
@IF NOT EXIST .\Lib\crt0.s (
  @SET info=Terminated due to the assembly error
  @GOTO term
)


@rem @dcc -tMCOREEN:windiss -c -o .\Lib\Vector.o .\Lib\Vector.c
@rem @IF NOT EXIST .\Lib\Vector.o (
@rem   @SET info=Terminated due to the compile error
@rem   @GOTO term
@rem )

@dcc -tMCOREEN:windiss -c -o Crc.o Crc.c
@IF NOT EXIST Crc.o (
  @SET info=Terminated due to the compile error
  @GOTO term
)

@dcc -I .\Inc -tMCOREEN:windiss -c -o Main.o Main.c
@IF NOT EXIST Main.o (
  @SET info=Terminated due to the compile error
  @GOTO term
)

@dcc -I .\Inc -tMCOREEN:windiss -D USB_MODE -c -o Comm.o Comm.c
@IF NOT EXIST Comm.o (
  @SET info=Terminated due to the compile error
  @GOTO term
)

@SET LibPath="%CD%\Lib"

@dld -L %LibPath% -tMCOREEN:windiss -m6 -o %1.bin .\Lib\crt0.o .\main.o .\crc.o .\comm.o -l:Hardware.a usb.dld > %1.map
@IF NOT EXIST %1.bin (
  @SET info=Terminated due to the linker error
  @GOTO term
)


@ddump -R %1.bin -o %1.s19
@IF NOT EXIST %1.s19 (
  @SET info=Terminated due to the S3 Utility error
  @GOTO term
)


@:term
@ECHO %info%

@IF EXIST .\Lib\crt0.o del .\Lib\crt0.o
@rem @IF EXIST .\Lib\Vector.o del .\Lib\Vector.o
@IF EXIST .\Lib\Hardware.a del .\Lib\Hardware.a
@IF EXIST %1.bin del %1.bin
@IF EXIST main.o del main.o
@IF EXIST %1.map del %1.map
@IF EXIST crc.o del crc.o
@IF EXIST comm.o del comm.o

