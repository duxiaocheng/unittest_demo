<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output omit-xml-declaration="yes" indent="yes"/>
<xsl:strip-space elements="*"/>

<xsl:template match="/">
<testExecutions version="1">
  <xsl:apply-templates/>
</testExecutions>
</xsl:template>

<xsl:template match="testsuites/testsuite/testcase">
  <file path="test/testStub.cpp">
    <testCase>
    <xsl:variable name="duration" select="./@time"/>
    <xsl:attribute name="duration"><xsl:value-of select="number($duration)*1000"/></xsl:attribute>
    <xsl:copy-of select="@*"/>
    <xsl:copy-of select="./*"/>
    </testCase>
  </file>
</xsl:template>

</xsl:stylesheet>
