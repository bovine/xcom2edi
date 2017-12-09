# makefile MAKE V2.0 or higher
.autodepend

##############################################################################

# define the name of the library
LIBNAME = tui.lib
.precious: $(LIBNAME)

# define all object files that make up the executable
OBJS = screen.obj mouse.obj button.obj menubox.obj textbox.obj txtinput.obj \
       checkbox.obj dropdown.obj pulldown.obj

##############################################################################

BCC_ARGS = /ml /c /2 /Os

##############################################################################

.cpp.obj:
    bcc $(BCC_ARGS) $*.cpp
    tlib $(LIBNAME) +-$*.obj

.c.obj:
    bcc $(BCC_ARGS) $*.c
    tlib $(LIBNAME) +-$*.obj

.asm.obj:
    bcc $(BCC_ARGS) $*.asm
    tlib $(LIBNAME) +-$*.obj

##############################################################################

$(LIBNAME): $(OBJS)
            if exist $*.bak del $*.bak
            if exist $*.map del $*.map

##############################################################################
