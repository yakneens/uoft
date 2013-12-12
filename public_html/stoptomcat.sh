#!/bin/sh

# make sure the logs/tomcat.pid exists
if test ! -f logs/tomcat.pid
then
  "Can't find the tomcat.pid file in the logs directory, aborting"
  exit 1
fi

# send the process a kill signal
pid=`cat logs/tomcat.pid`
echo sending PID:$pid a kill signal...

rc=`kill $pid`

# did it work?
if test $rc
then
  echo Error terminating PID:$pid, kill exit code $rc
  exit 1
else
  echo PID:$pid successfully sent kill signal.

  # remove the tomcat.pid file
  /bin/rm -f logs/tomcat.pid
fi

unset pid rc
