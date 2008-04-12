#############################################################################
#                                                                           #
# Copyright (C) 2008 Daniel Prevost <dprevost@users.sourceforge.net>        #
#                                                                           #
# This file may be distributed and/or modified under the terms of the       #
# MIT License as described by the Open Source Initiative                    #
# (http://opensource.org/licenses/mit-license.php) and appearing in         #
# the file License.txt included in the packaging of this library.           #
#                                                                           #
# This program is distributed in the hope that it will be useful, but       #
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the    #
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  #
#                                                                           #
#############################################################################

AC_INIT([Error Parser and Generator],
        [0.1.0],
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

# Tell our makefiles if we can use docbook2x to regenerate some 
# of the documentation, as needed.
AC_CHECK_PROG([DOCBOOK2MAN_IS_PRESENT], [docbook2x-man], [yes], [no])
AM_CONDITIONAL([COND_USE_DOCBOOK2MAN], [test "$DOCBOOK2MAN_IS_PRESENT" = yes])
AC_CHECK_PROG([DOCBOOK2TEXI_IS_PRESENT], [docbook2x-texi], [yes], [no])
AM_CONDITIONAL([COND_USE_DOCBOOK2TEXI], [test "$DOCBOOK2TEXI_IS_PRESENT" = yes])

##################################
#                                #
# SECTION - checks for libraries #
#                                #
##################################

AC_CHECK_LIB([xml2],[xmlParseFile],,AC_MSG_ERROR(could not find libxml2))

#####################################
#                                   #
# SECTION - checks for header files #
#                                   #
#####################################

AC_HEADER_STDC 
AC_HEADER_TIME 

AC_LANG_PUSH([C++])
AC_CHECK_HEADERS(new.h)
AC_CHECK_HEADERS(new)
AC_LANG_POP([C++])

AC_CHECK_HEADERS(assert.h ctype.h limits.h sys/param.h errno.h fcntl.h)
AC_CHECK_HEADERS(sys/time.h sys/mman.h signal.h sys/wait.h)
AC_CHECK_HEADERS(semaphore.h sys/semaphore.h sys/ipc.h sys/sem.h)
AC_HEADER_DIRENT 
AC_HEADER_STDBOOL

##############################
#                            #
# Section - checks for types #
#                            #
##############################

AC_TYPE_SIZE_T
AC_TYPE_OFF_T 
AC_TYPE_PID_T 
AC_CXX_BOOL
AC_TYPE_SIGNAL 

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

AC_CHECK_FUNCS(memcpy access vsnprintf)

AC_FUNC_MKDIR
AC_CHECK_FUNCS(strcpy bcopy stat)
AC_FUNC_STRERROR_R

AC_MSG_CHECKING(whether gettimeofday is broken)
gettimeofday_is_broken=no
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM(
     [[#if TIME_WITH_SYS_TIME
       # include <sys/time.h>
       # include <time.h>
       #else
       # if HAVE_SYS_TIME_H
       #  include <sys/time.h>
       # else
       #  include <time.h>
       # endif
       #endif]],
     [[struct timeval tv;
       int ok;
       ok = gettimeofday( &tv, NULL );]])],,
    [gettimeofday_is_broken=yes])
if test "gettimeofday_is_broken" = "yes"; then
  AC_MSG_RESULT(yes)
  AC_MSG_ERROR([This version of gettimeofday is old... please contact us for support])
else
  AC_MSG_RESULT(no)
fi

AC_REPLACE_FUNCS(localtime_r)
# There are 2 versions of asctime_r, with 2 or 3 arguments.
AC_MSG_CHECKING(for asctime_r with 3 args)
replace_asctime_r=no
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM([[#include <time.h>]],
                   [[  struct tm tm;
                       char buff[26], *res;
                       res = asctime_r(&tm, buff, 26);]])],,
  [replace_asctime_r=yes])
if test "$replace_asctime_r" = "yes"; then
  AC_MSG_RESULT(no)
  AC_LIBOBJ(asctime_r)
else
  AC_MSG_RESULT(yes) 
  AC_DEFINE(HAVE_ASCTIME_R_OK, 1, asctime_r is ok)
fi

# There are 2 versions of ctime_r, with 2 or 3 arguments.
AC_MSG_CHECKING(for ctime_r with 3 args)
replace_ctime_r=no
AC_COMPILE_IFELSE(
  [AC_LANG_PROGRAM([[#include <time.h>]],
                   [[  time_t t;
                       char buff[26], *res;
                       res = ctime_r(&t, buff, 26);]])],,
  [replace_ctime_r=yes] )
if test "$replace_ctime_r" = "yes"; then
  AC_MSG_RESULT(no)
  AC_LIBOBJ(ctime_r)
else
  AC_MSG_RESULT(yes) 
  AC_DEFINE(HAVE_CTIME_R_OK, 1, ctime_r is ok)
fi

########################################
#                                      #
# Section - checks for system services #
#                                      #
########################################

AC_C_BIGENDIAN

###################################
#                                 #
# Section - automake conditionals #
#                                 #
###################################

AC_ARG_ENABLE([dbc],
              AC_HELP_STRING([--enable-dbc],
                             [add contract-validation checks to the code. You might want to disable this on production systems for performance reasons @<:@default=yes@:>@]),
	           [use_dbc=$enableval],
	           [use_dbc=yes])

AM_CONDITIONAL([COND_USE_DBC], [test "$use_dbc" = yes])

####################
#                  #
# Section - output #
#                  #
####################

AC_CONFIG_FILES([
   Makefile
   doc/Makefile
   m4/Makefile
   parser/Makefile
])

AC_OUTPUT

