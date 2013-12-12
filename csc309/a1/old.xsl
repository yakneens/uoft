<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html" indent="yes" />

<xsl:template match="/" xmlns:saxon="http://icl.com/saxon">

<xsl:document href="index.html">

<html>
<head>
<title>bla</title>
<link rel="stylesheet" type="text/css" href="store.css" />
</head>
<body>



<table border="1" align="center" valign="center">
<tr height="100"><td colspan="3" align="center">LOGO</td></tr>
<tr>
<td>

<h2 class="menu">CATEGORY</h2>
<ul>
	<xsl:for-each select="saxon:distinct(//category)">
	<xsl:variable name="category" select="." />
	<xsl:variable name="file" select="concat(normalize-space($category),'.html')" />
	<xsl:sort select="." />
	<li>
		<a href="{$file}"><xsl:value-of select="." /></a>
	</li>

	</xsl:for-each>
</ul>

<h2 class="menu">MANUFACTURER</h2>
<ul>
	<xsl:for-each select="saxon:distinct(//manufacturer)">
	<xsl:variable name="mnfacture" select="." />
	<xsl:variable name="file" select="concat(normalize-space($mnfacture),'.html')" />
	<xsl:sort select="." />
	<li>
		<a href="{$file}"><xsl:value-of select="." /></a>
	</li>	
	</xsl:for-each>
</ul>

<form name="search">
	<input type="text" name="query" size="15" />
	<input type="submit" value="SEARCH" />
</form>

</td>

<td width="500">PRODUCTS
	<table cellpadding="1">
		<tr>
		
		</tr>	
	</table>
</td>

<td width="100">CART</td>
</tr>
</table>



</body>
</html>
</xsl:document>
</xsl:template>
</xsl:stylesheet>