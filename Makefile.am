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

EXTRA_DIST =                  \
        configure             \
        Config/config.guess   \
        Config/config.sub     \
        Config/install-sh     \
        Config/ltmain.sh      \
        Config/missing        \
        Config/mkinstalldirs  \
        COPYING               \
        Makefile.win32        \
        README                \
        DTD/errorParser11.dtd \
        DTD/example_vdsf.xml  \
        DTD/catalog11.xml     

# The 'doc' subdir must be placed after 'src' since some of the 
# documentation and man pages are generated automatically when
# using "make dist" (otherwise, you may have missing files or
# outdated documentation).

SUBDIRS = m4 parser tests

# doc (commented for now until I take the time to create the documentation...)

ACLOCAL_AMFLAGS = -I m4

install-data-local:
	-mkdir -p $(DESTDIR)$(datadir)/doc/errorParser
	-mkdir -p $(DESTDIR)$(datadir)/doc/errorParser/examples
	-mkdir -p $(DESTDIR)$(datadir)/xml/errorParser/
	$(install_sh_DATA) $(top_srcdir)/README  $(DESTDIR)$(datadir)/doc/errorParser/README
	$(install_sh_DATA) $(top_srcdir)/examples/example_options.xml $(DESTDIR)$(datadir)/doc/errorParser/examples/example_options.xml
	$(install_sh_DATA) $(top_srcdir)/examples/example_vdsf.xml $(DESTDIR)$(datadir)/doc/errorParser/examples/example_vdsf.xml
	$(install_sh_DATA) $(top_srcdir)/DTD/errorParser11.dtd $(DESTDIR)$(datadir)/xml/errorParser/errorParser11.dtd
	$(install_sh_DATA) $(top_srcdir)/DTD/catalog11.xml $(DESTDIR)$(datadir)/xml/errorParser/catalog11.xml

#if COND_USE_XMLCATALOG
#	xmlcatalog --noout --add delegatePublic "-//Error Parser project//DTD Error Parser XML V1.1" $(DESTDIR)$(datadir)/xml/errorParser/catalog11.xml /etc/xml/catalog
#	xmlcatalog --noout --add delegateSystem "http://errorparser.sourceforge.net/xml/1.1/" $(DESTDIR)$(datadir)/xml/errorParser/catalog11.xml /etc/xml/catalog
#endif

uninstall-local:
#if COND_USE_XMLCATALOG
#	xmlcatalog --noout --del "-//Error Parser project//DTD Error Parser XML V1.1" 
#	xmlcatalog --noout --del "http://errorparser.sourceforge.net/xml/1.1/" 
#endif
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/examples/example_vdsf.xml
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/examples/example_options.xml
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/README
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/COPYING
	-rm -f $(DESTDIR)$(datadir)/xml/errorParser/errorParser11.dtd
	-rm -f $(DESTDIR)$(datadir)/xml/errorParser/catalog11.xml
	-rmdir $(DESTDIR)$(datadir)/xml/errorParser
	-rmdir $(DESTDIR)$(datadir)/doc/errorParser/examples
	-rmdir $(DESTDIR)$(datadir)/doc/errorParser

