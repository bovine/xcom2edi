# makefile MAKE V2.0 or higher
.autodepend

# to enable debugging information, set "DEBUG" to "yes"
DEBUG = no

# if this is a beta test version, set "BETAVER" to "yes"
BETAVER = no

# to create a distribution zip, set to "BUILDZIP" to "yes"
BUILDZIP = yes


# set these to the version number
VERMAJ = 1
VERMIN = 5

##############################################################################

# define all object files that make up the executable
OBJS = editor1.obj editor2.obj editor3.obj editor4.obj editor5.obj \
       editor6.obj editor7.obj editor8.obj \
       sharewar.obj farewell.obj errormsg.obj \
       facil.obj product.obj research.obj up.obj

##############################################################################

BCC_ARGS = /v /ml /c /2 /Os /DVERSION="$(VERMAJ).$(VERMIN)"
TLINK_ARGS = /x


!if ($(BETAVER) == yes)
BCC_ARGS = $(BCC_ARGS) /DDEBUG
!endif

!if ($(BUILDZIP) != yes)
build: xcom2edi.exe
!else
build: xcom2e$(VERMAJ)$(VERMIN).zip
!endif

!if ($(DEBUG) == yes)
TLINK_ARGS = $(TLINK_ARGS) /v
!endif

##############################################################################

.cpp.obj:
    bcc $(BCC_ARGS) $*.cpp

.c.obj:
    bcc $(BCC_ARGS) $*.c

##############################################################################

xcom2e$(VERMAJ)$(VERMIN).zip:   xcom2edi.exe file_id.diz xcom2edi.txt
                @if exist $< del $<
!if ($(DEBUG) != yes)
                    fd file_id.diz /d /t$(VERMAJ):$(VERMIN)0
                    fd xcom2edi.txt /d /t$(VERMAJ):$(VERMIN)0
                    fd xcom2edi.exe /d /t$(VERMAJ):$(VERMIN)0
                    pkzip $< xcom2edi.exe xcom2edi.txt file_id.diz /ex
                    @if exist ~banner~.* del ~banner~.*
                    @echo.>~banner~.000
                    @copy ~banner~.000+file_id.diz ~banner~.001
                    pkzip /z $< < ~banner~.001
                    @if exist ~banner~.* del ~banner~.*
                    fd $< /d /t$(VERMAJ):$(VERMIN)0
!endif

##############################################################################

xcom2edi.exe:    $(OBJS)
        tlink $(TLINK_ARGS) @&&|
c0l.obj $(OBJS)
$<
$(<B).map
noehl.lib cl.lib tui.lib spawnl.lib
|
!if ($(DEBUG) != yes)
            wwpack $(<F)
            crypt $<
            @if exist $(<B).old del $(<B).old 
            unp l -h $<
            tinyprog /F /Mb "Load Checksum Error" /Ut _STEALTH /K $<
            unp l -h $<
            crcset $<
!endif
        @if exist c:\sierra\xcom2\$< del c:\sierra\xcom2\$<
        copy $< c:\sierra\xcom2

##############################################################################

farewell.obj:   farewell.bin
                2obj b farewell.bin .large:_Farewell

facil.obj:      facil.dat
                2obj b facil.dat .large:_FacilDat

product.obj:    product.dat
                2obj b product.dat .large:_ProductDat

research.obj:   research.dat
                2obj b research.dat .large:_ResearchDat

up.obj:         up.dat
                2obj b up.dat .large:_UpDat

##############################################################################
