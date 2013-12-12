<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

<xsl:template match="product">
<html>
<head><title>booya</title></head>
<body>
<xsl:for-each select="@*">
<xsl:value-of select="."/><br/>
</xsl:for-each>
</body></html>
</xsl:template>
</xsl:stylesheet>
