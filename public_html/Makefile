
# Simple makefile to validate XML files and to generate HTML using SAXON. 

JAVA = java
JAVAFLAGS = -cp "/u/csc309h/lib:/u/csc309h/lib/saxon.jar:/u/csc309h/lib/xerces.jar"
XML = products.xml

build:
	build.sh

makedb:
	javac -classpath /u/csc309h/lib/pointbase/pbembedded41ev.jar:/ StoreCreate.java

rundb:
	java -cp /u/csc309h/lib/pointbase/pbembedded41ev.jar:. StoreCreate
sql:
	java -classpath .:/h/u2/csc309h/lib/pointbase/pbembedded41ev.jar:/h/u2/csc309h/lib/pointbase/pbtools41ev.jar com.pointbase.tools.toolsCommander

makeserver:
	javac Server.java
validatexml:
	$(JAVA) $(JAVAFLAGS) Validator -v products.xml

runserver:
	java Server 30917 &

clean:
	rm *.class
