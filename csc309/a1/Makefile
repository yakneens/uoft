
# Simple makefile to validate XML files and to generate HTML using SAXON. 

JAVA = java
JAVAFLAGS = -cp "/u/csc309h/lib:/u/csc309h/lib/saxon.jar:/u/csc309h/lib/xerces.jar"
XML = products.xml

processxml:
	$(JAVA) $(JAVAFLAGS) com.icl.saxon.StyleSheet $(XML) index.xsl
	$(JAVA) $(JAVAFLAGS) com.icl.saxon.StyleSheet $(XML) category.xsl
	$(JAVA) $(JAVAFLAGS) com.icl.saxon.StyleSheet $(XML) manufacture.xsl
	$(JAVA) $(JAVAFLAGS) com.icl.saxon.StyleSheet $(XML) sale.xsl
	$(JAVA) $(JAVAFLAGS) com.icl.saxon.StyleSheet $(XML) populate.xsl
	$(JAVA) $(JAVAFLAGS) com.icl.saxon.StyleSheet $(XML) checkout.xsl
	chmod og+rx *.html
	chmod og+rx *.js

validatexml:
	$(JAVA) $(JAVAFLAGS) Validator -v products.xml

clean:
	rm index.html
