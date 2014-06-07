<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="html" encoding="utf-8" indent="yes"/>
  <xsl:template match="/">
    <xsl:text disable-output-escaping='yes'>&lt;!DOCTYPE html></xsl:text>
    <html>
      <head>
        <title>GlPortal - static C/C++ code analysis</title>
      </head>
      <body>
        <h1>Static C/C++ code analysis of GlPortal</h1>
        <p>Based on the output of cppcheck.</p>
        <ul>
          <xsl:for-each select="results/error">
            <li>
              <xsl:value-of select="@file"/>(<xsl:value-of select="@line"/>): (<xsl:value-of select="@severity"/>) <xsl:value-of select="@msg"/>
            </li>
          </xsl:for-each>
        </ul>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>
