<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<!--    <xsl:import href="/usr/share/sgml/docbook/xsl-stylesheets-1.70.1/htmlhelp/htmlhelp.xsl"/> -->
    <xsl:import href="http://docbook.sourceforge.net/release/xsl/current/htmlhelp/htmlhelp.xsl"/>
    <xsl:param name="admon.graphics" select="1"/>
    <xsl:param name="section.autolabel" select="1"/>
    <xsl:param name="htmlhelp.chm" select="'errorParser.chm'"></xsl:param>
    <xsl:param name="htmlhelp.hhp" select="'errorParser.hhp'"></xsl:param>
    <xsl:param name="htmlhelp.use.hhk" select="1"/>
</xsl:stylesheet>
