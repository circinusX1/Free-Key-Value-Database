
### TEST: LIVEIMAGE + CRONTAB + MEEIOT

```
    3  sudo apt-get install lighttpd  41  sudo apt-get install wireless-tools
    2  sudo apt-get install wpa-suplicant curl
    4  sudo apt-get install git lighttpd php5-gd php5-cgi php5-gd
    5  sudo apt-get install git lighttpd php-gd php-cgi php-gd

git clone https://github.com/comarius/liveimage
git clone https://github.com/comarius/wifiphp-one

# sudo update-rc.d liveimage defaults


#/etc/init.d/liveimage


#!/bin/sh
# kFreeBSD do not accept scripts as interpreters, using #!/bin/sh and sourcing.
if [ true != "$INIT_D_SCRIPT_SOURCED" ] ; then
    set "$0" "$@"; INIT_D_SCRIPT_SOURCED=true . /lib/init/init-d-script
fi
### BEGIN INIT INFO
# Provides:          liveimage
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Example initscript
# Description:       This file should be used to construct scripts to be
#                    placed in /etc/init.d.  This example start a
#                    single forking daemon capable of writing a pid
#                    file.  To get other behavoirs, implemend
#                    do_start(), do_stop() or other functions to
#                    override the defaults in /lib/init/init-d-script.
### END INIT INFO

# Author: Foo Bar <foobar@baz.org>
#
# Please remove the "Author" lines above and replace them
# with your own name if you copy and modify this script.

DESC="Liveimage"
BINF=liveimage
SHH=meeiot.sh
GPIO=gpioinit.sh

CRON=/home/$USER/liveimage/cron/$SHH
GPOS=/home/$USER/$GPIO

export PATH=$PATH:/usr/local/bin
stop() {
    echo "stopping"
    kill -9 $(pidof liveimage)
    touch /tmp/cronx
    echo 1 > /tmp/cronx
    chmod 777 /tmp/cronx
    chown $USER:$USER /tmp/cronx
    sleep 2
}

start() {
     rm /tmp/cronx
#     $GPOS > /dev/null
     $CRON > /tmp/liveimage.log &
}

case "$1" in
start)
    start
    ;;
stop)
    stop
    ;;
retart)
    stop
    start
    ;;
*)
    echo "Usage: $0 {start|stop|restart}"
esac



# sudo crontab -e

# Edit this file to introduce tasks to be run by cron.
#
# Each task to run has to be defined through a single line
# indicating with different fields when the task will be run
# and what command to run for the task
#
# To define the time you can provide concrete values for
# minute (m), hour (h), day of month (dom), month (mon),
# and day of week (dow) or use '*' in these fields (for 'any').#
# Notice that tasks will be started based on the cron's system
# daemon's notion of time and timezones.
#
# Output of the crontab jobs (including errors) is sent through
# email to the user the crontab file belongs to (unless redirected).
#
# For example, you can run a backup of all your user accounts
# at 5 a.m every week with:
# 0 5 * * 1 tar -zcf /var/backups/home.tgz /home/
#
# For more information see the manual pages of crontab(5) and cron(8)
#
# m h  dom mon dow   command
0 1 * * * /data/removeold.sh



sudo mkdir -p /data/snaps

chown -R $USER:USER /data/


# /etc/fstab
/dev/mmcblk0p1    /boot vfat defaults 0 0
/dev/sda1               /data/snaps     ext4    defaults        0 0


#$HOME/liveimage/cron/meeiot.sh
#!/bin/bash

killall liveimage
kill -9 $(pidof liveimage)
cd /home/$USER/liveimage/
./liveimage > /dev/null &
sleep 1
oimage="/tmp/liveimage.jpg"
cd /data/snaps
rm /tmp/cronx
TOKEN="VALID_UID_MEEIOT.ORG"
error=0
while [[ 1 ]];do
        sleep 1
        [[ -f /tmp/cronx ]] && exit

        p=`ping -c 1 192.168.1.1 2>&1 | grep "1 received"`

        if [[ -z ${p} ]];then
                error=$((error+1))
                if (( a > 8 )); then # 8 seconds check
                        break;
                fi
        fi
        error=0
        image=$(ls -rt | tail --lines=1)
        [[ -z $image ]] && continue
        fsz=$(stat -c%s $image)
	echo "chec king $image"
        if [[ -f $image && fsz > 1000 && $oimage != $image ]];then
                sleep .2
                oimage=$image
                echo "saving $oimage"
                curl -F frontcam=@$image -F hdr="Content-Type: image/jpeg" https://meeiot.org/push/$TOKEN
        fi

	li=$(ps ax | grep liveimage | grep -v meeiot | grep -v grep) 
	if [[ -z $li ]];then
		/home/$USER/liveimage/cron/resetusb.sh
		break;
	fi

done
cd /home/$USER
echo "rebooting"
reboot


#$HOME/liveimage/cron/resetusb.sh#!/bin/bash

set -euo pipefail
IFS=$'\n\t'
#1908:2311
VENDOR="1908"
PRODUCT="2311"

service udev stop
for DIR in $(find /sys/bus/usb/devices/ -maxdepth 1 -type l); do
  if [[ -f $DIR/idVendor && -f $DIR/idProduct &&
        $(cat $DIR/idVendor) == $VENDOR && $(cat $DIR/idProduct) == $PRODUCT ]]; then
    echo 0 > $DIR/authorized
    sleep 1
    echo 1 > $DIR/authorized
  fi
done
sleep 1
service udev start




####  optional meeiot.sh startup if wlan does not get IP (encounterred on nanopi+ wifi)
 
#!/bin/bash

sleep 5
pkill liveimage
systemctl stop wpa_supplicant
pkill wpa_supplicant
ifconfig eth0 down
ifconfig wlan0 down
ifconfig wlan0 up
sudo wpa_supplicant -B -i wlan0 -Dnl80211 -c /etc/wpa_supplicant/wpa_supplicant$

sleep 1
dhclient wlan0

#  rest of the file


```
