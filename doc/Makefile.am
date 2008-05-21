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

EXTRA_DIST =            \
        errorParser.chm \
        manual.html     \
        manual.pdf      \
        manual.xml      \
        xml2chm.xsl     \
        xml2html.xsl    

all-local: chm.zip manual.html manual.pdf

chm.zip: manual.xml
	-rm -rf chm
	-rm -f chm.zip
	mkdir chm
	cd chm; xsltproc ../$(top_srcdir)/doc/xml2chm.xsl ../$(top_srcdir)/doc/manual.xml; cd ..
	zip chm.zip $(top_builddir)/doc/chm/*

manual.html: manual.xml xml2html.xsl
	xsltproc xml2html.xsl manual.xml > manual.html

manual.pdf: manual.xml
	db2pdf manual.xml 

install-data-local:
	-mkdir -p $(DESTDIR)$(datadir)/doc/errorParser
	$(install_sh_DATA) $(top_srcdir)/doc/manual.html $(DESTDIR)$(datadir)/doc/errorParser/manual.html
	$(install_sh_DATA) $(top_srcdir)/doc/manual.pdf  $(DESTDIR)$(datadir)/doc/errorParser/manual.pdf

uninstall-local:
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/manual.html
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/manual.pdf
	-rmdir $(DESTDIR)$(datadir)/doc/errorParser
	-rmdir $(DESTDIR)$(datadir)/doc

clean-local:
	-rm -rf chm
   
distclean-local:
	-rm -f chm.zip

