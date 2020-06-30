<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output omit-xml-declaration="yes" indent="yes"/>
<xsl:strip-space elements="*"/>

<xsl:template match="/">
<testExecutions version="1">
  <file path="test/testStub.cpp">
    <xsl:apply-templates/>
  </file>
</testExecutions>
</xsl:template>

<xsl:template match="testsuites/testsuite/testcase">
  <testCase>
  <xsl:copy-of select = "@*"/>
  </testCase>
</xsl:template>

</xsl:stylesheet>
