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

CLEANFILES = *~

EXTRA_DIST =                             \
        configure                        \
        Config/config.guess              \
        Config/config.sub                \
        Config/install-sh                \
        Config/ltmain.sh                 \
        Config/missing                   \
        Config/mkinstalldirs             \
        COPYING                          \
        README                           \
        DTD/errorParser13.dtd            \
        DTD/errorParser12.dtd            \
        DTD/errorParserOptions20.dtd     \
        DTD/catalog.xml                  \
        examples/example_options.xml     \
        examples/example_photon.xml      \
        examples/simple_example.xml      \
        installation/errorParser.spec    \
        installation/install_catalog.sh  \
        installation/install_catalog.vbs 

SUBDIRS = parser tests doc

install-data-local:
	-mkdir -p $(DESTDIR)$(datadir)/doc/errorParser
	-mkdir -p $(DESTDIR)$(datadir)/doc/errorParser/examples
	-mkdir -p $(DESTDIR)$(datadir)/xml/errorParser/
	$(install_sh_DATA) $(top_srcdir)/README                          $(DESTDIR)$(datadir)/doc/errorParser/README
	$(install_sh_DATA) $(top_srcdir)/COPYING                         $(DESTDIR)$(datadir)/doc/errorParser/COPYING
	$(install_sh_DATA) $(top_srcdir)/examples/example_options.xml    $(DESTDIR)$(datadir)/doc/errorParser/examples/example_options.xml
	$(install_sh_DATA) $(top_srcdir)/examples/example_photon.xml     $(DESTDIR)$(datadir)/doc/errorParser/examples/example_photon.xml
	$(install_sh_DATA) $(top_srcdir)/examples/simple_example.xml     $(DESTDIR)$(datadir)/doc/errorParser/examples/simple_example.xml
	$(install_sh_DATA) $(top_srcdir)/DTD/errorParser13.dtd           $(DESTDIR)$(datadir)/xml/errorParser/errorParser13.dtd
	$(install_sh_DATA) $(top_srcdir)/DTD/errorParser12.dtd           $(DESTDIR)$(datadir)/xml/errorParser/errorParser12.dtd
	$(install_sh_DATA) $(top_srcdir)/DTD/errorParserOptions20.dtd    $(DESTDIR)$(datadir)/xml/errorParser/errorParserOptions20.dtd
	$(install_sh_DATA) $(top_srcdir)/DTD/catalog.xml                 $(DESTDIR)$(datadir)/xml/errorParser/catalog.xml
	$(install_sh_DATA) $(top_srcdir)/installation/install_catalog.sh $(DESTDIR)$(datadir)/xml/errorParser/install_catalog.sh

uninstall-local:
	-rm -f $(DESTDIR)$(datadir)/xml/errorParser/install_catalog.sh
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/examples/example_photon.xml
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/examples/example_options.xml
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/examples/simple_example.xml
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/README
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/COPYING
	-rm -f $(DESTDIR)$(datadir)/xml/errorParser/errorParser13.dtd
	-rm -f $(DESTDIR)$(datadir)/xml/errorParser/errorParser12.dtd
	-rm -f $(DESTDIR)$(datadir)/xml/errorParser/errorParserOptions20.dtd
	-rm -f $(DESTDIR)$(datadir)/xml/errorParser/catalog.xml
	-rmdir $(DESTDIR)$(datadir)/xml/errorParser
	-rmdir $(DESTDIR)$(datadir)/doc/errorParser/examples
	-rmdir $(DESTDIR)$(datadir)/doc/errorParser

