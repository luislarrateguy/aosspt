all:
	rpcgen timesync.x -a
	make -f Makefile.timesync

clean:
	rm timesync_*
	rm Makefile.timesync
	rm timesync.h

rpc:
	make -f Makefile.timesync
	
