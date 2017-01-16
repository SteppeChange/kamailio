#!/bin/bash

LOCAL_IP=$(curl -s --connect-timeout 0.5 http://169.254.169.254/latest/meta-data/local-ipv4)
ret=$?
if [[ $ret -ne 0 ]]
then
   (>&2 echo Amazon API is unavailible)
   LOCAL_IP="127.0.0.1"
fi

PUBLIC_IP=$(curl -s --connect-timeout 0.5  http://169.254.169.254/latest/meta-data/public-ipv4)
ret=$?
if [[ $ret -ne 0 ]]
then
   (>&2 echo Amazon API is unavailible)
   PUBLIC_IP="127.0.0.1"
fi

LOCAL_IP+=":5060"
PUBLIC_IP+=":5060"

CONFIG=/home/ogo/src/kamailio/gate_kamailio.cfg
UP_CONFIG=/home/ogo/src/kamailio/_gate_kamailio.cfg


cp -f $CONFIG $UP_CONFIG
sed 's/LOCAL_IP_PORT/'"$LOCAL_IP"'/g' -i $UP_CONFIG
sed 's/PUBLIC_IP_PORT/'"$PUBLIC_IP"'/g' -i $UP_CONFIG

/usr/local/sbin/kamailio -f $UP_CONFIG -P /var/run/kamailio/kamailio.pid -m 64 -M 8

