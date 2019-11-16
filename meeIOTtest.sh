#!/bin/bash

#
#  all the calls http://PREDICATE.meeiot.org   should be replaced with https://meeiot.org/PREDICATE
#  http://PREDICATE.meeiot.org  are deprecated
#



token="5ba8e5788973226b3255844b37221c2a0f8a3921784b8cf1c93b9f"
delay=4
echo "Test with free user token. Delay between put's/gets $delay seconds. Adding some values"
curl -k  http://put.meeiot.org/${token}/value=xxxxxxxx
sleep $delay
curl  -k http://put.meeiot.org/${token}/value1=somevalue
sleep $delay
curl  -k http://put.meeiot.org/${token}/value2=zzzzzzzzzzzzz
sleep $delay
echo "getting all keys"
sleep $delay

curl -k http://get.meeiot.org/${token}
echo ""
echo "getting all values"
sleep $delay
curl -k http://get.meeiot.org/${token}/value
echo ""
sleep $delay
curl -k http://get.meeiot.org/${token}/value1
echo ""
sleep $delay
curl -k http://get.meeiot.org/${token}/value2
echo " deleting all keys"
sleep $delay
curl -k http://del.meeiot.org/${token}
echo "getting all keys"
sleep $delay
curl -k http://get.meeiot.org/${token}

sleep $delay

echo "sending a text file as multipart"
curl  -k -F file1=@testfile http://put.meeiot.org/${token}
sleep $delay
echo "getting all keys"
echo ""
sleep $delay
curl -k http://get.meeiot.org/${token}
echo ""
echo "getting file"
sleep $delay
curl  -k http://get.meeiot.org/${token}/file1
echo ""

echo "put an image as raw then with headers"
sleep $delay
curl  -F imageweb=@./meeiot.jpg -F hdr="Content-Type: image/jpeg"  http://put.meeiot.org/${token}
sleep $delay
curl   -F image=@./meeiot.jpg http://put.meeiot.org/${token}
echo "get  image as raw"
echo "open the link in the browser:. See headers for content type are OK"
echo " http://get.meeiot.org/${token}/image"
read dummy
echo "get  image as web with http headers. open thislink in browser. then press a key to continue"
sleep $delay
echo  http://get.meeiot.org/${token}/imageweb
read dummy

echo "push an image as raw then with headers"
sleep $delay
curl  -F imageweb=@./meeiot.jpg -F hdr="Content-Type: image/jpeg"  http://push.meeiot.org/${token}
sleep $delay
curl   -F image=@./meeiot.jpg http://push.meeiot.org/${token}
echo "get  image as raw. open link in browser"
echo "http://pop.meeiot.org/${token}/image"
read dummy

echo "get  image as web with http headers. open thislink in browser. then press a key to continue"
sleep $delay
echo  http://pop.meeiot.org/${token}/imageweb
read dummy

echo "put a jsonraw to [jsonraw] = {...} and get it back raw "
sleep $delay
curl -H "Content-Type: application/json" -X POST -d '{"value":"xyz"}' http://put.meeiot.org/${token}/jsonraw
echo "keys for user"
sleep $delay
curl  http://get.meeiot.org/${token}
echo "getting back json"
curl  http://get.meeiot.org/${token}/jsonraw

echo "put a json and get it back with HTTP headers application/json "
sleep $delay
curl  -F jsonweb=@./json.txt -F hdr="Content-Type: application/json"  http://put.meeiot.org/${token}
echo "open the link in the browser:. See headers for content type are OK"
echo https://get.meeiot.org/${token}/jsonweb
read dummy;

