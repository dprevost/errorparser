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

EXTRA_DIST =             \
        admin.chm        \
        users.chm        \
        Makefile.win32   \
        admin.html       \
        admin.pdf        \
        admin.xml        \
        users.html       \
        users.pdf        \
        users.xml        \
        xml2chmAdmin.xsl \
        xml2chmUsers.xsl \
        xml2html.xsl     

if COND_USE_DB2PDF
 LOCAL_PDF=admin.pdf users.pdf
endif

all-local: admin.html users.html $(LOCAL_PDF)

admin.html: admin.xml xml2html.xsl
	xsltproc xml2html.xsl admin.xml > admin.html

users.html: users.xml xml2html.xsl
	xsltproc xml2html.xsl users.xml > users.html

if COND_USE_DB2PDF

admin.pdf: admin.xml
	db2pdf admin.xml 

users.pdf: users.xml
	db2pdf users.xml 

endif

install-data-local:
	-mkdir -p $(DESTDIR)$(datadir)/doc/errorParser
	$(install_sh_DATA) $(top_srcdir)/doc/admin.html $(DESTDIR)$(datadir)/doc/errorParser/admin.html
	$(install_sh_DATA) $(top_srcdir)/doc/admin.pdf  $(DESTDIR)$(datadir)/doc/errorParser/admin.pdf
	$(install_sh_DATA) $(top_srcdir)/doc/users.html $(DESTDIR)$(datadir)/doc/errorParser/users.html
	$(install_sh_DATA) $(top_srcdir)/doc/users.pdf  $(DESTDIR)$(datadir)/doc/errorParser/users.pdf

uninstall-local:
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/admin.html
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/admin.pdf
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/users.html
	-rm -f $(DESTDIR)$(datadir)/doc/errorParser/users.pdf
	-rmdir $(DESTDIR)$(datadir)/doc/errorParser
	-rmdir $(DESTDIR)$(datadir)/doc

