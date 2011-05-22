all: subfetch

subfetch: main.o
	gcc `xmlrpc-c-config client --libs` main.o -o subfetch

main.o: main.c
	gcc -c `xmlrpc-c-config --cflags` main.c

clean:
	rm -rf *o subfetch

	
	
	
	
#client	: xmlrpc_sample_client.cpp
#	g++ `xmlrpc-c-config --cflags` -g -O2 -W -Wall -Wmissing-prototypes -Wstrict-prototypes -DPROTOTYPES -o client xmlrpc_sample_client.cpp `xmlrpc-c-config --libs` -lxmlrpc_client -pthread

#covit : send_rtp.cpp recv_rtp.cpp unicapVidCap.cpp videoenc.cpp capture.cpp xmlrpc_server.cpp
#	g++ `xmlrpc-c-config --cflags` -g -O2 -W -Wall -Wmissing-prototypes -Wstrict-prototypes -DPROTOTYPES -o covit.o send_rtp.cpp recv_rtp.cpp unicapVidCap.cpp videoenc.cpp capture.cpp xmlrpc_server.cpp librtpunix.a librtp.a -lm -lnsl -lSDL -lavformat -lavcodec -lz -lavutil `xmlrpc-c-config --libs` -lxmlrpc_server_abyss -pthread `pkg-config --cflags --libs libunicap`



#all: subfetch
#subfetch : main.cpp
#	gcc `xmlrpc-c-config --cflags` -o subfetch `xmlrpc-c-config client c++2 --libs` main.cpp
#clean:	
#	rm -f subfetch *.o
