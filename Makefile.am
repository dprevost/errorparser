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

CLEANFILES = *~

EXTRA_DIST =                 \
        configure            \
        Config/config.guess  \
        Config/config.sub    \
        Config/install-sh    \
        Config/ltmain.sh     \
        Config/missing       \
        Config/mkinstalldirs \
        COPYING              \
        Makefile.win32       \
        README               \
        DTD/errorParser.dtd  \
        DTD/example_vdsf.xml 

# The 'doc' subdir must be placed after 'src' since some of the 
# documentation and man pages are generated automatically when
# using "make dist" (otherwise, you may have missing files or
# outdated documentation).

SUBDIRS = m4 parser tests

# doc (commented for now until I take the time to create the documentation...)

ACLOCAL_AMFLAGS = -I m4

install-data-local:
	-mkdir -p $(DESTDIR)$(datadir)/doc/errorParser
	-mkdir -p $(DESTDIR)$(datadir)/doc/errorParser/DTD
	$(install_sh_DATA) $(top_srcdir)/README  $(DESTDIR)$(datadir)/doc/errorParser/README
	$(install_sh_DATA) $(top_srcdir)/DTD/errorParser.dtd  $(DESTDIR)$(datadir)/doc/errorParser/DTD/errorParser.dtd
	$(install_sh_DATA) $(top_srcdir)/DTD/example_vdsf.xml $(DESTDIR)$(datadir)/doc/errorParser/DTD/example_vdsf.xml

uninstall-local:
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/DTD/example_vdsf.xml
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/DTD/errorParser.dtd
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/README
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/COPYING
	-rmdir $(DESTDIR)$(datadir)/doc/errorParser/DTD
	-rmdir $(DESTDIR)$(datadir)/doc/errorParser
	-rmdir $(DESTDIR)$(datadir)/doc

