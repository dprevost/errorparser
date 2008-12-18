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

AC_INIT([Error Parser],
        [0.5.0],
        [Daniel Prevost <dprevost@users.sourceforge.net>],
        [errorParser])

AC_PREREQ(2.57)
AC_CONFIG_AUX_DIR(Config)
AC_CONFIG_LIBOBJ_DIR(parser)

AM_INIT_AUTOMAKE()
AM_CONFIG_HEADER(parser/config.h)

AC_AIX 
AC_ISC_POSIX 
AC_MINIX 

##################################
#                                #
# SECTION - checks for programs  #
#                                #
##################################

AC_PROG_LIBTOOL

# Tell our makefiles that we can use docbook2x to regenerate some 
# of the documentation, as needed.
# WARNING: On fedora, the program is called db2x_docbook2man and on
# ubuntu (and debian I guess) it is called docbook2x-man. There might
# be other variations out there...

AC_CHECK_PROG([DOCBOOK2XMAN_IS_PRESENT], [docbook2x-man], [yes], [no])
AM_CONDITIONAL([COND_USE_DOCBOOK2XMAN], [test "$DOCBOOK2XMAN_IS_PRESENT" = yes])
# On fedora, the exe has a different name
AC_CHECK_PROG([DOCBOOK2MAN_IS_PRESENT], [db2x_docbook2man], [yes], [no])
AM_CONDITIONAL([COND_USE_DOCBOOK2MAN], [test "$DOCBOOK2MAN_IS_PRESENT" = yes])

# Tell our makefiles if we can use db2pdf to regenerate some 
# of the documentation, as needed.
AC_CHECK_PROG([DB2PDF_IS_PRESENT], [db2pdf], [yes], [no])
AM_CONDITIONAL([COND_USE_DB2PDF], [test "$DB2PDF_IS_PRESENT" = yes])

##################################
#                                #
# SECTION - checks for libraries #
#                                #
##################################

PKG_CHECK_MODULES(ERR_PARSER, libxml-2.0,,
                  AC_MSG_ERROR([Error Parser requires libxml2]))
AC_SUBST(ERR_PARSER_CFLAGS)
AC_SUBST(ERR_PARSER_LIBS)

#####################################
#                                   #
# SECTION - checks for header files #
#                                   #
#####################################

AC_HEADER_STDC 

AC_CHECK_HEADERS(ctype.h)
AC_CHECK_HEADERS(time.h)

##############################
#                            #
# Section - checks for types #
#                            #
##############################

###################################
#                                 #
# Section - checks for structures #
# (do they miss data members?)    #
#                                 #
###################################

#################################################
#                                               #
# Section - checks for compiler characteristics #
#                                               #
#################################################

AC_C_VOLATILE 
AC_C_INLINE 

##################################
#                                #
# Section - checks for functions #
#                                #
##################################

AC_CHECK_FUNCS(ctime)

####################
#                  #
# Section - output #
#                  #
####################

AC_CONFIG_FILES([
   Makefile
   doc/Makefile
   installation/errorParser.spec
   parser/Makefile
   tests/Makefile
])

AC_OUTPUT

