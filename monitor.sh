#!/bin/sh
LOGFILE="./conn.log"
touch $LOGFILE
while :;
do
  MONITOR=$(netstat -tn 2>/dev/null | grep ESTABLISHED | grep 8080 | \
    grep -o '[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}')
  while IFS= read -r line; do
    if ! grep -q "$line" "$LOGFILE"; then
      if [[ ! -z "${line// }" ]]; then
        echo $line >> $LOGFILE
        echo "$(date)" >> $LOGFILE
      fi
    fi
  done <<< $MONITOR
  sleep 5
done

