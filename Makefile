#  Makefile for template command
#
#  $RCSfile: Makefile,v $	$Revision: 0.15 $
#
#  $Author: scs $	$Date: 2001/02/08 19:48:19 $
#
#  $State: Exp $	$Locker:  $
#
#  $Log: Makefile,v $
#  Revision 0.15  2001/02/08 19:48:19  scs
#  Added ANS-format install, creation of subdirs.
#
#  Revision 0.14  1996/10/31 19:37:51  scs
#  Moved to GCC.
#
#  Revision 0.13  1993/06/28 02:34:39  scs
#  Added template.shar.

TARGET	=	template

# For the support library
#
LIBSRCS	= newstr.c newnstr.c errors.c optparse.c
LIBOBJS	= newstr.o newnstr.o errors.o optparse.o
#  If you have stdc.h in your /usr/include, you don't need it
#  as one of the LIBCLUDES.
LIBCLUDES	= stdc.h

#  Feel free to change these to match your local definitions

#PREFIX  = /usr/local
# ANSland is peculiar - PKG should exist already
PKG	= /usr/local/pkg
PREFIX  = $(PKG)/scsutils-0.1
BIN	= $(PREFIX)/bin
LIB	= $(PREFIX)/lib
MANTOP	= $(PREFIX)/man
MAN1	= $(MANTOP)/man1
MAN3	= $(MANTOP)/man3
PKGDIRS = $(PREFIX) $(BIN) $(LIB) $(MANTOP) $(MAN1) $(MAN3)
SHELL	= /bin/sh

CC	= gcc
# General definitions here which apply to all -- form -DDEF
#  If you have stdc.h in your /usr/include, you
#  don't need the -I. switch
CLUDDIR	= -I.
# Link libraries -- form -lLIB
LIBS	=
# Lint definitions -- same as DEFS, but form -D DEF
LDEFS	= -I .
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

shar:	ReadMe $(TARGET).1 optparse.3 Makefile $(SRCS) $(CLUDES)
	shar ReadMe Templatelib Templatelib/* Makefile $(TARGET).1 optparse.3 $(SRCS) $(CLUDES) > template.shar

install:	$(TARGET) $(TARGET).1 optparse.3 installdirs
	install -o root -g bin -m 511 -s -c $(TARGET) $(BIN)/is$(TARGET)
	install -o root -g bin -m 644 -c $(TARGET).1 $(MAN1)/is$(TARGET).1
	install -o root -g bin -m 644 -c optparse.3 $(MAN3)/optparse.3

install.templatelib:
	if [ ! -d ${LIB}/Templates ] ; then	\
		mkdir ${LIB}/Templates ;	\
		chmod 755 ${LIB}/Templates ;	\
		chown root ${LIB}/Templates ;	\
		chgrp bin ${LIB}/Templates ;	\
		if [ -d ./Templatelib ] ; then	\
			cp ./Templatelib/* ${LIB}/Templates ; \
			chmod 444 ${LIB}/Templates/* ;	\
			chown root ${LIB}/Templates/* ;	\
			chgrp bin ${LIB}/Templates/* ;	\
		fi \
	fi

installdirs:
	echo mkdir $(PKGDIRS)
	for D in $(PKGDIRS) ; do \
		if [ ! -d $$D ] ; then \
			mkdir $$D ; \
			chmod 755 $$D ; \
		fi \
	done
