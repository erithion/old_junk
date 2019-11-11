
@SET PATH=%~1

@PUSHD %~dp0Hardware

@SET info=Hardware.a done 

@dcc -tMCOREEN:windiss -c -o Hardware.o Hardware.c
@IF NOT EXIST Hardware.o (
  @SET info=Terminated due to the compile error
  @GOTO term
)

@dcc -tMCOREEN:windiss -c -o Int.o Int.c
@IF NOT EXIST Int.o (
  @SET info=Terminated due to the compile error
  @GOTO term
)

@dcc -I .\..\..\Inc -tMCOREEN:windiss -c -o Ctl.o Ctl.c
@IF NOT EXIST Ctl.o (
  @SET info=Terminated due to the compile error
  @GOTO term
)

@dcc -I .\..\..\Inc -tMCOREEN:windiss -c -o MemMove.o MemMove.c
@IF NOT EXIST MemMove.o (
  @SET info=Terminated due to the compile error
  @GOTO term
)

@dcc -I .\..\..\Inc -tMCOREEN:windiss -c -o USB.o USB.c
@IF NOT EXIST USB.o (
  @SET info=Terminated due to the compile error
  @GOTO term
)


@dar -r .\..\Hardware.a Hardware.o USB.o MemMove.o Ctl.o Int.o
@IF NOT EXIST .\..\Hardware.a (
  @SET info=Terminated due to the librarian error
)


@:term
@ECHO %info%

@IF EXIST Hardware.o del Hardware.o
@IF EXIST USB.o del USB.o
@IF EXIST MemMove.o del MemMove.o
@IF EXIST Ctl.o del Ctl.o
@IF EXIST Int.o del Int.o

@POPD
