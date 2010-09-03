#  Makefile for template command
#
#  $RCSfile: Makefile,v $	$Revision: 0.23 $
#
#  $Author: scs $	$Date: 2010/09/03 21:01:23 $
#
#  $State: Exp $	$Locker:  $
#
#  $Log: Makefile,v $
#  Revision 0.23  2010/09/03 21:01:23  scs
#  Added cscope build.
#
#  Revision 0.22  2008/08/11 21:40:28  scs
#  Move to stock name.
#
#  Revision 0.21  2008/04/10 14:42:41  scs
#  Clean up last remaining bits of my old posix/os macros.
#
#  Revision 0.20  2006/01/27 15:19:22  scs
#  Removed my locally developed stdc.h.
#
#  Revision 0.19  2006/01/27 13:46:02  scs
#  Removed double definition of optparse.test.
#
#  Revision 0.18  2003/04/14 15:01:37  scs
#  Added build of tests.
#
#  Revision 0.17  2002/04/25 03:44:27  scs
#  Reset a few things for FreeBSD use and Inland Sea.
#
#  Revision 0.16  2001/02/08 19:51:37  scs
#  Lightened permissions on binary so ordinary users can copy it.
#
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
LIBCLUDES	=

#  Feel free to change these to match your local definitions

PREFIX  = /usr
BIN	= $(PREFIX)/bin
LIB	= $(PREFIX)/lib
MANTOP	= $(PREFIX)/man
MAN1	= $(MANTOP)/man1
MAN3	= $(MANTOP)/man3
PKGDIRS = $(PREFIX) $(BIN) $(LIB) $(MANTOP) $(MAN1) $(MAN3)
SHELL	= /bin/sh

CC	= gcc
# General definitions here which apply to all -- form -DDEF
CLUDDIR	= -I.
# Link libraries -- form -lLIB
LIBS	=
# Lint definitions -- same as DEFS, but form -D DEF
LDEFS	= -I .
# Lint libraries -- same as LIBS, but form -l LIB
LLIBS	=
# Set to -g for debugging, -O for optimise, or both if compiler handles it
DEBUG	= -g

DEFS	= $(CLUDDIR)

CFLAGS	= $(DEBUG) $(DEFS)

SRCS	= copyfile.c list.c main.c mktempl.c switches.c templist.c $(LIBSRCS)
OBJS	= copyfile.o list.o main.o mktempl.o switches.o templist.o $(LIBOBJS)
CLUDES	= template.h patchlevel.h

# Units which can be self-tested.

TESTS	= copyfile.test list.test mktempl.test optparse.test \
	switches.test templist.test

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

clean.tests:
	rm -f $(TESTS)

clobber:	clean clean.tests
	rm -f $(TARGET) Make.Log $(SHARS)

lint:	$(SRCS)
	lint -D lint $(LLIBES) $(LDEFS) $(SRCS) > lint

tags:	$(SRCS)
	ctags $(SRCS)

cscope:	$(SRCS)
	cscope -R -b

TAGS:	$(SRCS) $(CLUDES)
	etags $(SRCS) $(CLUDES)

shar:	ReadMe $(TARGET).1 optparse.3 Makefile $(SRCS) $(CLUDES)
	shar ReadMe Templatelib Templatelib/* Makefile $(TARGET).1 optparse.3 $(SRCS) $(CLUDES) > template.shar

install:	$(TARGET) $(TARGET).1 optparse.3 installdirs
	install -o root -g bin -m 755 -s -c $(TARGET) $(BIN)/$(TARGET)
	install -o root -g bin -m 644 -c $(TARGET).1 $(MAN1)/$(TARGET).1
	install -o root -g bin -m 644 -c optparse.3 $(MAN3)/optparse.3

# Only install templatelib if you want *my* templates as *your* defaults!

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
	#echo mkdir $(PKGDIRS)
	for D in $(PKGDIRS) ; do \
		if [ ! -d $$D ] ; then \
			mkdir $$D ; \
			chmod 755 $$D ; \
		fi \
	done

copyfile.test:	copyfile.c errors.o
	$(CC) $(CFLAGS) -DTEST copyfile.c errors.o -o copyfile.test

list.test:	list.c errors.o
	$(CC) $(CFLAGS) -DTEST list.c errors.o -o list.test

mktempl.test:	mktempl.c errors.o newnstr.o newstr.o
	$(CC) $(CFLAGS) -DTEST mktempl.c errors.o newnstr.o newstr.o -o mktempl.test

switches.test:	switches.c errors.o newnstr.o newstr.o optparse.o
	$(CC) $(CFLAGS) -DTEST switches.c errors.o newnstr.o newstr.o optparse.o -o switches.test

optparse.test:	optparse.c errors.o
	$(CC) $(CFLAGS) -DTEST optparse.c errors.o -o optparse.test

templist.test:	optparse.c errors.o newnstr.o newstr.o
	$(CC) $(CFLAGS) -DTEST templist.c errors.o newnstr.o newstr.o -o templist.test

tests:	$(TESTS)
