#  Makefile for template command
#
#  $RCSfile: Makefile,v $	$Revision: 0.11 $
#
#  $Author: scs $	$Date: 1992/06/06 16:07:42 $
#
#  $State: Exp $	$Locker:  $
#
#  $Log: Makefile,v $
#  Revision 0.11  1992/06/06 16:07:42  scs
#  Added installation of templates directory.
#
#  Revision 0.10  1990/10/30  17:04:59  scs
#  Changed template directory created by install.

TARGET	=	template

# For the support library
#
LIBSRCS	= newstr.c newnstr.c errors.c optparse.c
LIBOBJS	= newstr.o newnstr.o errors.o optparse.o
#  If you have stdc.h in your /usr/include, you don't need it
#  as one of the LIBCLUDES.
LIBCLUDES	= stdc.h

#  Feel free to change these to match your local definitions

BIN	= /usr/local/bin
LIB	= /usr/local/lib
MAN	= /usr/man/man
SHELL	= /bin/sh

CC	= cc
# General definitions here which apply to all -- form -DDEF
#  If you have stdc.h in your /usr/include, you
#  don't need the -I./ switch
CLUDDIR	= -I./
# Link libraries -- form -lLIB
LIBS	=
# Lint definitions -- same as DEFS, but form -D DEF
LDEFS	= -I ./
# Lint libraries -- same as LIBS, but form -l LIB
LLIBS	=
# Set to -g for debugging, -O for optimise, or both if compiler handles it
DEBUG	= -g
# Define BSD for BSD 4.X or derivatives thereof.
OS	= -DBSD
# Define this if you have POSIX compatibility
POSIX	= -DPOSIX

DEFS	= $(OS) $(CLUDDIR) $(POSIX)

CFLAGS	= $(DEBUG) $(DEFS)

SRCS	= copyfile.c list.c main.c mktempl.c switches.c templist.c $(LIBSRCS)
OBJS	= copyfile.o list.o main.o mktempl.o switches.o templist.o $(LIBOBJS)
CLUDES	= stdc.h template.h patchlevel.h

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

$(OBJS):	$(CLUDES)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(OBJS):	$(CLUDES)

all:	man analysis $(TARGET)

man:	$(TARGET).1
	nroff -man $(TARGET).1 > $(TARGET).man

optparse.man:	optparse.3
	nroff -man optparse.3 > optparse.man

Manifest:	$(SRCS) Makefile
	touch Manifest
	ls -ls $(SRCS) Makefile > Manifest

calls:	$(SRCS) $(HDRS)
	calls $(DEFS) $(SRCS) > calls

analysis:	tags TAGS cflow calls lint # cxref

clean:
	rm -f $(OBJS) core lint tags TAGS $(TARGET).man *~ #*#

clobber:	clean
	rm -f $(TARGET) Make.Log $(SHARS)

lint:	$(SRCS)
	lint -D lint $(LLIBES) $(LDEFS) $(SRCS) > lint

tags:	$(SRCS)
	ctags $(SRCS)

TAGS:	$(SRCS) $(CLUDES)
	etags $(SRCS) $(CLUDES)

install:	$(TARGET) $(TARGET).1 optparse.3
	install -o root -g bin -m 511 -s -c $(TARGET) $(BIN)
	install -o root -g bin -m 644 -c $(TARGET).1 $(MAN)1
	install -o root -g bin -m 644 -c optparse.3 $(MAN)3
	if [ ! -d ${LIB}/Templates ] ; then	\
		mkdir -f ${LIB}/Templates ;	\
		chmod 755 ${LIB}/Templates ;	\
		chown root ${LIB}/Templates ;	\
		chgrp bin ${LIB}/Templates ;	\
		cp Templatelib/* ${LIB}/Templates ; \
		chmod 444 ${LIB}/Templates/* ;	\
		chown root ${LIB}/Templates/* ;	\
		chgrp bin ${LIB}/Templates/* ;	\
	fi
