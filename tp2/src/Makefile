
# This is a template Makefile generated by rpcgen

# Parameters

CLIENT = ../obj/timesync_client
SERVER = ../obj/timesync_server

SOURCES_CLNT.c = 
SOURCES_CLNT.h = 
SOURCES_SVC.c = 
SOURCES_SVC.h = 
SOURCES.x = timesync.x

TARGETS_SVC.c = timesync_svc.c timesync_server.c   
TARGETS_CLNT.c = timesync_clnt.c timesync_client.c   
TARGETS = timesync.h   timesync_clnt.c timesync_svc.c

OBJECTS_CLNT = $(SOURCES_CLNT.c:%.c=%.o) $(TARGETS_CLNT.c:%.c=%.o)
OBJECTS_SVC = $(SOURCES_SVC.c:%.c=%.o) $(TARGETS_SVC.c:%.c=%.o)
# Compiler flags 

CFLAGS += -g 
LDLIBS += -lnsl
RPCGENFLAGS = 

# Targets 

all : $(CLIENT) $(SERVER)

$(TARGETS) : $(SOURCES.x) 
	rpcgen $(RPCGENFLAGS) $(SOURCES.x)

$(OBJECTS_CLNT) : $(SOURCES_CLNT.c) $(SOURCES_CLNT.h) $(TARGETS_CLNT.c) 

$(OBJECTS_SVC) : $(SOURCES_SVC.c) $(SOURCES_SVC.h) $(TARGETS_SVC.c) 

$(CLIENT) : $(OBJECTS_CLNT) 
	$(LINK.c) -o $(CLIENT) $(OBJECTS_CLNT) $(LDLIBS) 

$(SERVER) : $(OBJECTS_SVC) 
	$(LINK.c) -o $(SERVER) $(OBJECTS_SVC) $(LDLIBS)

clean:
	 $(RM) $(TARGETS) core $(OBJECTS_CLNT) $(OBJECTS_SVC) $(CLIENT) $(SERVER)

copy:
	mkdir -p /home/soa/2007/tp2/UNO/
	cp -r ../* /home/soa/2007/tp2/UNO/

