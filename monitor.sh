#!/bin/bash
LOGFILE="./conn.log"
WEBHOOK="https://discord.com/api/webhooks/1236745992615760033/VWvVRlCWZ6JUZBKjlnBz4ORslhtath9X_mRWYRXA8erz6Mv1nXGUVXTEc9UsS1fAXX1z"

touch $LOGFILE

readlog() {
  tr '\n' ' ' < $LOGFILE
}

while :;
do
  MONITOR=$(netstat -tn 2>/dev/null | grep ESTABLISHED | grep 8080 | \
    grep -o '[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}')
  if [[ ! -z "${MONITOR// }" ]]; then
    while IFS= read -r line; do
      if ! grep -q "$line" "$LOGFILE"; then 
        echo $line >> $LOGFILE
        echo "$(date)" >> $LOGFILE
      fi
    done <<< $MONITOR
    curl -H "Content-Type: application/json" \
      -d "{\"content\": \"new connection: $(readlog)\"}" \
      $WEBHOOK
  fi
  sleep 5
done

