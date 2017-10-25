websvsclient
============

A hack of libwsclient, a websocket client library for C [here](https://github.com/payden/libwsclient). The original notes are at the bottom...

The original is intended to be built using autogen.sh, etc. But I wanted to get into the guts of the
implementation, and rebuilding a library each time seemed complex. I therefore stripped out the
library parts so I could just build it as a standard app.

What follows are my notes...

// to execute the test program:
LD_LIBRARY_PATH=/usr/local/lib
export LD_LIBRARY_PATH
./test

Issues with the unix domain socket?
https://stackoverflow.com/questions/2149564/redirecting-tcp-traffic-to-a-unix-domain-socket-under-linux

// finally got this compiling. "errors" was defined as an array within the include file...
gcc -o test1 -pthread wsclient.c base64.c sha1.c test.c -lssl


Now have it compiling & can run. Using tcpdump to view traffic:
sudo tcpdump -i eth0 -A host 174.129.224.73
sudo tcpdump -i eth0 -A host 174.129.224.73 -w dump.pcap

ip.addr ==174.129.224.73 filter in wireshark


http://www.websocket.org/echo.html uses the same ws://echo.websocket.org end-point, So I now have wireshark traces to compare it against.

C code sends minimal http headers.

So the test app is now working in that it can send and receive a message.
I don't know why it started displaying the received msg tho, as I didn't make any changes to that.
I also added code so it would send a pong in response to a ping.

The code is really ugly though:
- processes i/p data ch by ch in a very heavy-handed way.
- It assumes Close (and now ping) can be split across multiple buffers, but this should never be the case
- Strange buffer management for received data - linked lists and such. really needed?
- Separate thread just for initial negotiation? Coalese into the background thread?
- why all the locking?

- what is the help socket all about? Okay, it's a thread that listens to "test.sock", and anything it receives 
on that socket it sends as text on the websocket. It's a bit fiddly in that it sends fragments, whereas the app 
interface sends whole messages; I'm not sure why it makes this disctinction - it adds a lot of code doing it this way

- how is the app meant to _not_ exit? The test app just doesn't work as is...

I added some code to test.c so it would periodically send websocket messages - it works fine.

I've now added linux-sock.c as a tiny app that creates a linux socket & periodically sends messages to the websocket client.
Works fine. Bit weird about how messages are displayed tho. They're all displayed at once though.

I wrote a new, much simpler helper-thread that reuses the existing websocket call - this works fine.










WebSocket client library for C (original notes; no longer relevant)
==============================

This library abstracts away WebSocket protocol framing for
client connections.  It aims to provide a *somewhat* similar
API to the implementation in your browser.  You create a new
client context and create callbacks to be triggered when
certain events occur (onopen, onmessage, onclose, onerror).

Your best bet for getting started is to look at test.c which shows
how to connect to an echo server using libwsclient calls.

Also, to install:

./autogen.sh

./configure && make && sudo make install

Then link your C program against wsclient: 'gcc -g -O2 -o test test.c -lwsclient'


