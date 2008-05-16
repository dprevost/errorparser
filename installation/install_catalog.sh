#!/bin/sh

#############################################################################
#                                                                           #
# Copyright (C) 2008 Daniel Prevost <dprevost@users.sourceforge.net>        #
#                                                                           #
# This file may be distributed and/or modified under the terms of the       #
# MIT License as described by the Open Source Initiative                    #
# (http://opensource.org/licenses/mit-license.php) and appearing in         #
# the file COPYING included in the packaging of this software.               #
#                                                                           #
# This program is distributed in the hope that it will be useful, but       #
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the    #
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  #
#                                                                           #
#############################################################################

# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--
#
# Please fix/set these variables to fit your installation.
#
# The Error Parser catalog and the DTD must be install in the same 
# directory.
#
# --+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--

THE_PROGRAM=/usr/bin/xmlcatalog
THE_TOP_CATALOG=/etc/xml/catalog
THE_ERRP_CATALOG=/usr/local/share/xml/errorParser/catalog12.xml

$THE_PROGRAM --noout --add delegatePublic "-//Error Parser project//DTD Error Parser XML V1.2" $THE_ERRP_CATALOG $THE_TOP_CATALOG
$THE_PROGRAM --noout --add delegateSystem "http://errorparser.sourceforge.net/xml/1.2/" $THE_ERRP_CATALOG $THE_TOP_CATALOG
