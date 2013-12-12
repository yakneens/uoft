#!/bin/sh

echo "boo"

CP=$TOMCAT_HOME/ant.jar:$TOMCAT_HOME/servlet.jar
CP=$CP:$TOMCAT_HOME/jaxp.jar:$TOMCAT_HOME/parser.jar
CP=$CP:$JAVA_HOME/lib/tools.jar
touch ~/public_html/web/index.jsp
# Execute ANT to perform the requested build target
java -classpath $CP:$CLASSPATH org.apache.tools.ant.Main \
  -Dtomcat.home=$TOMCAT_HOME  -Ddist.home=~/public_html/webapps/store "$@"
