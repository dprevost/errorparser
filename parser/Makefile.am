#############################################################################
#                                                                           #
# Copyright (C) 2008-2009 Daniel Prevost <dprevost@users.sourceforge.net>   #
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

EXTRA_DIST =                               \
        config-win32.h                     \
        errorParser.1                      \
        errorParser.xml                    \
        icon2.ico                          \
        Makefile.vs2005                    \
        Makefile.vs2008                    \
        parser.rc                          \
        parser.vs2005.sln                  \
        parser.vs2008.sln                  \
        Project_Files/parser.vs2005.vcproj \
        Project_Files/parser.vs2008.vcproj \
        resource.h                         

errorParser_SOURCES =        \
        options.cpp          \
        parser.cpp           \
        parser.h             \
        text.cpp             \
        AbstractHandler.h    \
        CfamilyHandler.cpp   \
        CfamilyHandler.h     \
        Copyright.cpp        \
        Copyright.h          \
        CSharp.cpp           \
        CSharp.h             \
        ErrMessage.cpp       \
        ErrMessage.h         \
        ErrMessageHeader.cpp \
        ErrMessageHeader.h   \
        ErrorHeader.cpp      \
        ErrorHeader.h        \
        ErrorXML.cpp         \
        ErrorXML.h           \
        ExtPython.cpp        \
        ExtPython.h          \
        GroupIdent.cpp       \
        GroupIdent.h         \
        HeaderHandler.cpp    \
        HeaderHandler.h      \
        Java.cpp             \
        Java.h               \
        MessageXML.cpp       \
        MessageXML.h         \
        PurePython.cpp       \
        PurePython.h         

errorParser_LDFLAGS = @ERR_PARSER_LIBS@

