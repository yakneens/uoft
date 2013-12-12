#!/bin/bash

# Create the directory structure required by Tomcat.
CreateDirectoryStructure() {
  # create conf directory
  if test ! -d conf
  then
    echo making conf directory...
    mkdir conf
  fi

  # create webapps directory
  if test ! -d webapps
  then
    echo making webapps directory...
    mkdir webapps
  fi

  # create the HTTP root directory
  if test ! -d webapps/ROOT
  then
    echo making webapps/ROOT directory...
    mkdir webapps/ROOT
  fi

  # create the logs directory
  if test ! -d logs
  then
    echo making logs directory...
    mkdir logs
  fi
}

CreateServerXML() {
  echo making conf/server.xml...
  echo "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" > conf/server.xml
  echo >> conf/server.xml
  echo "<Server>" >> conf/server.xml
  echo "  <Logger name=\"tc_log\" verbosityLevel = \"INFORMATION\" path=\"logs/tomcat.log\" />" >> conf/server.xml
  echo "  <Logger name=\"servlet_log\" path=\"logs/servlet.log\" />" >> conf/server.xml
  echo "  <Logger name=\"JASPER_LOG\" path=\"logs/jasper.log\" verbosityLevel = \"INFORMATION\" />" >> conf/server.xml
  echo >> conf/server.xml
  echo "  <ContextManager debug=\"0\" workDir=\"work\" showDebugInfo=\"true\" >" >> conf/server.xml
  echo "    <ContextInterceptor className=\"org.apache.tomcat.context.AutoSetup\" />" >> conf/server.xml
  echo "    <ContextInterceptor className=\"org.apache.tomcat.context.WebXmlReader\" />" >> conf/server.xml
  echo "    <ContextInterceptor className=\"org.apache.tomcat.context.LoaderInterceptor\" />" >> conf/server.xml
  echo "    <ContextInterceptor className=\"org.apache.tomcat.context.DefaultCMSetter\" />" >> conf/server.xml
  echo "    <ContextInterceptor className=\"org.apache.tomcat.context.WorkDirInterceptor\" />" >> conf/server.xml
  echo >> conf/server.xml
  echo "    <RequestInterceptor className=\"org.apache.tomcat.request.SessionInterceptor\" noCookies=\"false\" />" >> conf/server.xml
  echo "    <RequestInterceptor className=\"org.apache.tomcat.request.SimpleMapper1\" debug=\"0\" />" >> conf/server.xml
  echo "    <RequestInterceptor className=\"org.apache.tomcat.request.InvokerInterceptor\" debug=\"0\" prefix=\"/servlet/\" />" >> conf/server.xml
  echo "    <RequestInterceptor className=\"org.apache.tomcat.request.StaticInterceptor\" debug=\"0\" suppress=\"false\" />" >> conf/server.xml
  echo "    <RequestInterceptor className=\"org.apache.tomcat.session.StandardSessionInterceptor\" />" >> conf/server.xml
  echo "    <RequestInterceptor className=\"org.apache.tomcat.request.AccessInterceptor\" debug=\"0\" />" >> conf/server.xml
  echo "    <RequestInterceptor className=\"org.apache.tomcat.request.SimpleRealm\" debug=\"0\" />" >> conf/server.xml
  echo >> conf/server.xml
  echo "    <ContextInterceptor className=\"org.apache.tomcat.context.LoadOnStartupInterceptor\" />" >> conf/server.xml
  echo >> conf/server.xml
  echo "    <Connector className=\"org.apache.tomcat.service.PoolTcpConnector\">" >>  conf/server.xml
  echo "      <Parameter name=\"handler\" value=\"org.apache.tomcat.service.http.HttpConnectionHandler\"/>" >> conf/server.xml
  echo "      <Parameter name=\"port\" value=\"$port\"/>" >> conf/server.xml
  echo "    </Connector>" >> conf/server.xml
  echo >> conf/server.xml
  echo "    </ContextManager>" >> conf/server.xml
  echo "</Server>" >> conf/server.xml
}

CreateUsersXML() {
  if test ! -f conf/tomcat-users.xml
  then
    echo making conf/tomcat-users.xml...
    echo "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" > conf/tomcat-users.xml
	echo >> conf/tomcat-users.xml
    echo "<tomcat-users>" >> conf/tomcat-users.xml
    echo "  <!-- TODO: what goes here? -->" >> conf/tomcat-users.xml
    echo "</tomcat-users>" >> conf/tomcat-users.xml
  fi
}

CreateIndexHTML() {
  if test ! -f webapps/ROOT/index.html
  then
    echo making webapps/ROOT/index.html...
    echo "<html>" > webapps/ROOT/index.html
    echo "<head>" >> webapps/ROOT/index.html
    echo "<title>Tomcat is running</title>" >> webapps/ROOT/index.html
    echo "</head>" >> webapps/ROOT/index.html
    echo "<body bgColor=#99ccff>" >> webapps/ROOT/index.html
    echo "<p><h2>Congratualtions! Tomcat is running</h2></p>" >> webapps/ROOT/index.html
    echo "</body>" >> webapps/ROOT/index.html
    echo "</html>" >> webapps/ROOT/index.html
  fi
}

CreatePointbaseINI() {
  if test ! -f pointbase.ini
  then
    echo making pointbase.ini...
    echo "database.home=." > pointbase.ini
    echo "transaction.isolationlevel=TRANSACTION_READ_COMMITTED" >> pointbase.ini
    echo "database.pagesize=4096" >> pointbase.ini
    echo "cache.size=2063" >> pointbase.ini
    echo "sort.size=1024" >> pointbase.ini
  fi
}

# Create the files required to run Tomcat and Pointbase
CreateRequiredFiles() {
  CreateServerXML
  CreateUsersXML
  CreateIndexHTML
  CreatePointbaseINI
}

TOMCAT_LIB="/u/csc309h/lib/tomcat"
POINTBASE_LIB="/u/csc309h/lib/pointbase"

TOMCAT_HOME=`pwd`
#JAVA_HOME=???

OLD_CLASSPATH=$CLASSPATH
unset CLASSPATH

# add tomcat stuff to CLASSPATH
for i in $TOMCAT_LIB/* ; do
  if [ "$CLASSPATH" != "" ]; then
    CLASSPATH=$CLASSPATH:$i
  	
	else
    CLASSPATH=$i
  fi
done
# add pointbase stuff to CLASSPATH
for i in $POINTBASE_LIB/* ; do
  if [ "$CLASSPATH" != "" ]; then
    CLASSPATH=$CLASSPATH:$i
  else
   CLASSPATH=$i
  fi
done
# stuff for JSP
CLASSPATH=$CLASSPATH:/local/lib/j2sdk1.4.1_01/lib/tools.jar:/local/lib/j2sdk1.4.1_01/bin/javac
if [ "$OLD_CLASSPATH" != "" ] ; then
 CLASSPATH=$CLASSPATH
fi
export CLASSPATH

# Check if the logs/tomcat.pid file exists. If it does, the user probably has
# an old instance of tomcat hanging around and they should be warned before
# starting a new one.
if test -f logs/tomcat.pid
then
  echo "Warning: logs/tomcat.pid file already exists!"
  echo "Another instance of Tomcat is probably still running and should be"
  echo "ended with stoptomcat.sh"
  echo
  echo -n "Continue anyway? [y/n]: "
  read answer
  if [ "$answer" != "y" ] ; then
    exit 1;
  fi
fi

# get the port number the server will listen on
if test -z "$1"
then
  echo -n "Enter your assigned port number: "
  read port
else
  port=$1;
fi

# Create the require files and directories needed by Tomcat
CreateDirectoryStructure
CreateRequiredFiles

# start tomcat
echo Starting Tomcat...

nohup java -Dtomcat.home=$TOMCAT_HOME org.apache.tomcat.startup.Tomcat &

# store the PID for the stoptomcat.sh script
echo $! > logs/tomcat.pid


#if [ "$OLD_CLASSPATH" != "" ] ; then
#  CLASSPATH=$OLD_CLASSPATH
#  export CLASSPATH
#else
#  unset OLD_CLASSPATH
#fi

unset port

