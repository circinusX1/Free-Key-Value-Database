#/bin/bash
#
# cron script with liveimage timelapse or motion detection image sharing trough meeiot.
#
p=`ping -c 1 meeiot.org 2>&1 | grep "0% packet loss"`
[[ -z ${p} ]] && exit
TOKEN="5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f" ## need a paid account to allow over 16k POST's
pushd /data/snaps
image=$(ls -rt | tail --lines=1)
if [[ ! -z $lastfile ]];then
        curl -k -F img=@$image https://meeiot.org/push/5aafe8c0cebc1ca6b729a84b2d3f364925942e720e62d9509f87fd
fi
popd

