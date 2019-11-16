# ONLINE KEY VALUE DATABASE & RELAYING DATA
# MQTT up and running in testing mode
## All SSL secured


#### meeiot server is located in Frankfurd Germany, so for North America is bit slow. Make a donation to expand myself.

##### Comming soon. UDP relaying peer-to-peer node, UDP streaming from behind any proxies and routers device to device.
    * https://github.com/comarius/perlink/tree/master/doc 
    * https://github.com/comarius/perlink

```
# data sharing,
devices(1) :   https://meeiot.org/put/TOKEN/temperature=33.3
devices(2) :   https://meeiot.org/get/TOKEN/temperature      
	       33.3	

// or site 1:
https://meeiot.org/push/TOKEN/pressure=1000045
https://meeiot.org/push/TOKEN/pressure=999046
https://meeiot.org/push/TOKEN/pressure=900047

// site 2
https://meeiot.org/pop/TOKEN/pressure
1000045
https://meeiot.org/pop/TOKEN/pressure
999046
https://meeiot.org/pop/TOKEN/pressure
900047

	       
	       
```

### API's

* https://meeiot.org/put/TOKEN/key<=value> <HTTP_POST/JSON/IMAGE/BLOB/FILE>   store a value/POST
* https://meeiot.org/get/TOKEN/<key>                                          reads the value/POST
* https://meeiot.org/push/TOKEN/key<=value> <HTTP_POST/JSON/IMAGE/BLOB/FILE>  enqueue  a vlue/POST
* https://meeiot.org/pop/TOKEN                                                dequeue the value       
* https://meeiot.org/lst/TOKEN/key                                            list (check queue witout poping)
* https://meeiot.org/act/TOKEN                                                checks TOKE activity
* https://meeiot.org/del/TOKEN/<key>                                          deletes all keys for TOKEN or a key
* https://meeiot.org/qdel/TOKEN/<key>                                         same as above 
* https://meeiot.org/mail/TOKEN/key  [HTTP_POST MAIL-JSON ]                   much complex API :)
* https://meeiot.org/mqtt/<NEW!!!!>



## Ad : R-PI Preloaded SD Cards at https://www.redypis.org  

## 

```console
https://meeiot.org/put</get/push/pop/del/qdel>/user/<key>=<value> <HTTP_POST <hdr='headers,...'> <multipart> <json> >
```

    PUT / GET to key value storage
    PUSH / POP  to queue value storage
    

### API's
    put - stores a value under a key
    get - get's a value from a key, updates last access time to ttl
    push - push a value to the head of the key queue. (keep adding)
    pop - pops (removes) a value from the tail of the key queue. updates last access time for ttl
    del - deletes the key / or a key      
        https://meeiot.org/del/TOKEN         all the keys and values for token
        https://meeiot.org/del/TOKEN/key     just the key and it's val
    qdel - deletes all the queue(s) or one queue
        https://meeiot.org/qdel/TOKEN        all the queues
        https://meeiot.org/qdel/TOKEN/que    just the queue
        
    ttl 24 hours for keys and queues since last put/push
    access log 4 days ttl

    https://meeiot.org/put/<userid>/<key><=value>  POST (multipart-data)(json)
    
### SAMPLES
    Test UID: 5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f
        Operations using demo user id has a delay of 8 secoonds.
    see the meeIOTtest.sh file
        see the extra hdr="headers,..."  option
        If the POST has a multipart field hdr="HTTP_HEADERS,COMMA,SEPARATED" when the 
        data is being pulled back, the headers are send. This allows to store a media / mime
        type and get it back right into the browser without any post processing.
   
#### All samples are in curl so they can be easy ported to any language
### *** If you see in the sample VERB.meeiot.org, I ment  meeiot.org/VERB/ ***
The variants put/get.....meeiot.org are not longer valids due the SSL certificate issuer.



```console
#!/bin/bash
token="5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f"
delay=4
echo "Test with free user token. Delay between put's/gets $delay seconds. Adding some values"
curl -k  http://meeiot.org/put/${token}/value=xxxxxxxx
sleep $delay
curl  -k http://meeiot.org/put/${token}/value1=somevalue
sleep $delay
curl  -k http://meeiot.org/put/${token}/value2=zzzzzzzzzzzzz
sleep $delay
echo "getting all keys"
sleep $delay

curl -k http://get.meeiot.org/${token}
echo ""
echo "getting all values"
sleep $delay
curl -k http://meeiot.org/get/${token}/value
echo ""
sleep $delay
curl -k http://meeiot.org/get/${token}/value1
echo ""
sleep $delay
curl -k http://meeiot.org/get/${token}/value2
echo " deleting all keys"
sleep $delay
curl -k http://meeiot.org/del/${token}
echo "getting all keys"
sleep $delay
curl -k http://meeiot.org/get/${token}

sleep $delay

echo "sending a text file as multipart"
curl  -k -F file1=@testfile http://meeiot.org/put/${token}
sleep $delay
echo "getting all keys"
echo ""
sleep $delay
curl -k http://meeiot.org/get/${token}
echo ""
echo "getting file"
sleep $delay
curl  -k http://meeiot.org/get/${token}/file1
echo ""

echo "put an image as raw then with headers"
sleep $delay
curl  -F imageweb=@./meeiot.jpg -F hdr="Content-Type: image/jpeg"  http://meeiot.org/put/${token}
sleep $delay
curl   -F image=@./meeiot.jpg http://meeiot.org/put/${token}
echo "get  image as raw"
echo "open the link in the browser:. See headers for content type are OK"
echo " http://meeiot.org/get/${token}/image"
read dummy
echo "get  image as web with http headers. open thislink in browser. then press a key to continue"
sleep $delay
echo  http://meeiot.org/get/${token}/imageweb
read dummy

echo "push an image as raw then with headers"
sleep $delay
curl  -F imageweb=@./meeiot.jpg -F hdr="Content-Type: image/jpeg"  http://meeiot.org/push/${token}
sleep $delay
curl   -F image=@./meeiot.jpg http://meeiot.org/push/${token}
echo "get  image as raw. open link in browser"
echo "http://meeiot.org/pop/${token}/image"
read dummy

echo "get  image as web with http headers. open thislink in browser. then press a key to continue"
sleep $delay
echo  http://meeiot.org/pop/${token}/imageweb
read dummy

echo "put a jsonraw to [jsonraw] = {...} and get it back raw "
sleep $delay
curl -H "Content-Type: application/json" -X POST -d '{"value":"xyz"}' http://meeiot.org/put/${token}/jsonraw
echo "keys for user"
sleep $delay
curl  http://meeiot.org/get/${token}
echo "getting back json"
curl  http://meeiot.org/get/${token}/jsonraw

echo "put a json and get it back with HTTP headers application/json "
sleep $delay
curl  -F jsonweb=@./json.txt -F hdr="Content-Type: application/json"  http://meeiot.org/put/${token}
echo "open the link in the browser:. See headers for content type are OK"
echo https://meeiot.org/get/${token}/jsonweb
read dummy;
```


```console
# GET variation 
# unsecure
curl  http://meeiot.org/put/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/mykey=whatever # stores whatever
curl  http://meeiot.org/get/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/whatever   # get's whatever
curl  http://meeiot.org/get/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f            # gets all keys, mykey,


curl  http://meeiot.org/push/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/myq=1  # puts 1 in q
# secure
curl -k https://meeiot.org/push/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/myq=1  # puts 1 in q
curl -k https://meeiot.org/push/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/myq=3  # puts 3 in q
curl -k https://meeiot.org/pop/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/myq     # returns 1
curl -k https://meeiot.org/pop/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/myq     # returns 1
curl -k https://meeiot.org/pop/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/myq     # returns 3

#REST with HTTP_POST
curl -k -H "Content-Type: application/json" -X POST -d '{"value":"xyz"}' https://meeiot.org/put/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/myjson

curl -k -v -H "Content-Type: application/json" -X POST -d @./json.txt https://meeiot.org/put/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/myjson

# FILE POSTING is forbidden with this demo ID
curl -k -F mykey=@testfile https://meeiot.org/put/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f
```
        
#### Error Codes

#### !!! On: Dec 1 2019 Be prepared to handle returned error codes as JSON
```
# no error
{
	"error":0 
}

# in case of error
{
	"error":code
	"description":"description "
}

```

### current error codes

```console
0:X     success:scheme used
1:*     user id missing
2:*     value missing
3:*     
4:*     empty json post
5:*     key  too big or to small
6:*     value too big for the account, or empty
7:*     json too big for the account, or empty
8:*     multipart attachment FILE to big for this type of account
9:*     user id mismatch
10:*    user key mismatch. Error++, on 3 errors IP is banned.
11:*    access time to early since last access
12:*    access time error. could not determine last access time.
13:*    delete operation not allowed from other then put/push ip (demo id only)
14:*    client ip retrieval fail
20:*    system error
30:*    database err
31:*    database err
60:*    database err
61:*    database err
99:*    invalid request
111:*   no more data for the key

```

### PHP

```javascript
$ch = curl_init();
curl_setopt($ch, CURLOPT_URL,"https://meeiot.org/put/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/mydata");
curl_setopt($ch, CURLOPT_POST, 1);
$payload = json_encode( array( "jsonkey"=> "json value" ) );
curl_setopt( $ch, CURLOPT_POSTFIELDS, $payload );
curl_setopt( $ch, CURLOPT_HTTPHEADER, array('Content-Type:application/json'));
curl_setopt( $ch, CURLOPT_RETURNTRANSFER, true );
$result = curl_exec($ch);
curl_close($ch);
echo "$result";

// or just inline using GET

file_get_contents("https://meeiot.org/put/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/mydata={$MYDATA}");
file_get_contents("https://meeiot.org/get/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/mydata");

````

### ARDUINO
Using https://www.arduino.cc/en/Tutorial/WebClient

```javascript
//
// share data
//
EthernetClient client;
if (client.connect(server, 80)) 
{
    char req[200];
    sprintf(req,"GET /put/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/mysensor=$d HTTP/1.1", sensorvalue);
    client.println(req);
    client.println("Host: www.meeiot.org");
    client.println("Connection: close");
    client.println();
    while (!client.available()) {delay(1000);}
    char c = client.read();
    Serial.print(c);
    client.close();    
  }
}
```

### LINUX BASH SCRIPT, running form cron. 


```console
p=`ping -c 1 meeiot.org 2>&1 | grep "0% packet loss"`
[[ -z ${p} ]] && exit
JSON_TO_SHARE+/tmp/appjson
[[ ! -f ${JSON_TO_SHARE} ]] && exit
MYID="5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f"
curl -vX POST https://meeiot.org/put/${NYID}/mydata -d @${JSON_TO_SHARE}  --header "Content-Type: application/json"
```

### LINUX CRON script sharing live image motion camera into meeIOT queue. 
```
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
```



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

Write me in case of any issues.

Make a donation to keep the service alive at https://www.meeiot.org/


# SSH mqtt broker server (opened for testing Nov 6).

* Get a user token using community sigin (or contact me). 
    * The mqtt registration does not work with freen token.
    
* For messing around you can use
    * TOKEN:  5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f
    * PASSWORD: mariusxxx
    
* To setup up a mqtt password for your username(TOKEN)   use 'mqt/TOK/register/' API

```bash
	
	https://meeiot.org/mqt/YOURTOKEN/register/mqtt_password
aka:
	https://meeiot.org/mqt/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/register/mypassword
	
```
* After above querry is executed an account on mosquitto is created for 
    * user:     5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f
    * password: mypassword   
    
a screen like this would popup    
    
```bash
0:92, mqtt: user=5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f password=mypassword was changed/updated successfuly,link:client_keys

```
* The 'client_keys' is a link to a tar file having the ssh mqtt security certificates as:
    * ca.crt           credit authority self signe certificate used to sign/generate server ceritificates
    * meeiot_cli.crt   your client certificate for mqtt subscriber publisher calls
    * meeiot_cli.key   your client key for mqtt subscriber publisher calls
     * get them and untar them in a folder
*     Now you can use the mqtt with the user name = TOKEN and the password.mypassword and the certificates
*  

#### Web interface: (added just for testing your token and password)

```bash
	# browser 1 (subscriber to data set point test, as user 5ba8e57889... identified with password mariusxxx):
	# the data point can have a maximum depth of 4  like /test/subtest/subsubtest/subsubsubtest
	https://meeiot.org/mqt/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/mariusxxx/test
	
	# browser 2 (pushing data to mqtt to channel test from user 5ba8e57889... identified with password mariusxxx):
	https://meeiot.org/mqt/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/mariusxxx/test=some_data
		
```
Click to see the live links (open 2 separate browsers)

[publisher](https://meeiot.org/mqt/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/mariusxxx/test=some_data_dude){:target="_blank" rel="noopener"}

[subscriber](https://meeiot.org/mqt/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/mariusxxx/test){:target="_blank" rel="noopener"}


#### Test from web pusher to shell console

```bash
export MQTARGS="-p 8883 -h mqt.meeiot.org -u 5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f -P mariusxxx --cafile <path_to>/ca.crt --cert <path_to>/meeiot_cli.crt --key <path_to>/meeiot_cli.key -t"

# browser 1 (pushing data to mqtt to channel test from user 5ba8e57889... identified with password mariusxxx):
	https://meeiot.org/mqt/5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f/mariusxxx/test=frombrowser
	
# shell getting data
       mosquitto_sub $MQTARGS test
frombrowser

	
```

#### Test publisher and subscriber form 2 shells

```
#shell 1
export MQTARGS="-p 8883 -h mqt.meeiot.org -u 5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f -P mariusxxx --cafile <path_to>/ca.crt --cert <path_to>/meeiot_cli.crt --key <path_to>/meeiot_cli.key -t"

mosquitto_sub  te/st

#shell 2
export MQTARGS="-p 8883 -h mqt.meeiot.org -u 5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f -P mariusxxx --cafile <path_to>/ca.crt --cert <path_to>/meeiot_cli.crt --key <path_to>/meeiot_cli.key -t"
mosquitto_pub  "te/st" -m "hello world"
```



* Please make a donation to keep the service alive at https://www.meeiot.org/
    * This is one man business
* Check also and buy some sdcards https://www.redypis.org
    * This is one man business


