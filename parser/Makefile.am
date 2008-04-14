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

if COND_USE_DBC
   AM_CPPFLAGS += -DUSE_DBC
endif
AM_CPPFLAGS += @ERR_PARSER_CFLAGS@

CLEANFILES        = *~

bin_PROGRAMS = errorParser

EXTRA_DIST =            \
        Makefile.win32  \
        errorParser.dsp \
        errorParser.dsw 

errorParser_SOURCES = \
        group.c  \
        parser.c \
        parser.h 

errorParser_LDFLAGS = @ERR_PARSER_LIBS@

