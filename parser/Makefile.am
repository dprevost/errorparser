#############################################################################
#                                                                           #
# Copyright (C) 2008 Daniel Prevost <dprevost@users.sourceforge.net>        #
#                                                                           #
# This file may be distributed and/or modified under the terms of the       #
# MIT License as described by the Open Source Initiative                    #
# (http://opensource.org/licenses/mit-license.php) and appearing in         #
# the file COPYING included in the packaging of this software.              #
#                                                                           #
# This program is distributed in the hope that it will be useful, but       #
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the    #
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  #
#                                                                           #
#############################################################################

AM_CPPFLAGS = -Wall
AM_CPPFLAGS += @ERR_PARSER_CFLAGS@

CLEANFILES = *~ 

bin_PROGRAMS = errorParser

man_MANS = errorParser.1

if COND_USE_DOCBOOK2XMAN
errorParser.1: errorParser.xml
	docbook2x-man --encoding=iso-8859-1 errorParser.xml
endif
if COND_USE_DOCBOOK2MAN
errorParser.1: errorParser.xml
	db2x_docbook2man --encoding=iso-8859-1 errorParser.xml
endif

EXTRA_DIST =                        \
        config-win32.h              \
        errorParser.1               \
        errorParser.xml             \
        icon2.ico                   \
        Makefile.win32              \
        parser.rc                   \
        parser.sln                  \
        Project_Files/parser.vcproj \
        resource.h

errorParser_SOURCES = \
        group.c   \
        message.c \
        options.c \
        parser.c  \
        parser.h  \
        text.c    

errorParser_LDFLAGS = @ERR_PARSER_LIBS@

