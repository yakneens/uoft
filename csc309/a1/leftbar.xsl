<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html" indent="yes" />

<xsl:template match="/" xmlns:saxon="http://icl.com/saxon">

<xsl:for-each select="saxon:distinct(//menuItem)">
<xsl:variable name="mnuItem" select="." />
<xsl:variable name="file" select="concat(normalize-space($mnuItem),'.html')" />

<xsl:document href="{$file}">

<html>
<head>
<title>Grocery Shop</title>
<link rel="stylesheet" type="text/css" href="store.css" />
</head>
<body>



<table border="1" align="center" valign="center">
<tr height="100"><td colspan="3" align="center">LOGO</td></tr>
<tr>
<td>

<h2 class="menu"><xsl:value-of select="." /></h2>
<xsl:variable name="dummy" select="concat('saxon:distinct(ancestor::',normalize-space($mnuItem),')')" />
<xsl:text>{$dummy}</xsl:text>
<xsl:for-each select="ancestor::category">
	<xsl:variable name="group" select="." />
	<xsl:variable name="groupfile" select="concat(normalize-space($group),'.html')" />
	<xsl:sort select="." />	
	<ul>
		<a href="{$groupfile}"><xsl:value-of select="." /></a>
	</ul>
</xsl:for-each>	



<form name="search">
	<input type="text" name="query" size="15" />
	<input type="submit" value="SEARCH" />
</form>

</td>

<td width="500">PRODUCTS
	<table cellpadding="1">
		<tr>
			<td> HELLO </td>
		</tr>	
	</table>
</td>

<td width="100">CART</td>
</tr>
</table>



</body>
</html>
</xsl:document>
</xsl:for-each>
</xsl:template>
</xsl:stylesheet>