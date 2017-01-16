# WARNING: do not run this directly, it should be run by the master Makefile

include ../../Makefile.defs
auto_gen=
NAME=rtpengine.so
LIBS=

DEFS+=-DKAMAILIO_MOD_INTERFACE

LIBS= -lcurl -lyajl -ljson-c

SERLIBPATH=../../lib
SER_LIBS+=$(SERLIBPATH)/kmi/kmi
SER_LIBS+=$(SERLIBPATH)/kcore/kcore
SER_LIBS+=$(SERLIBPATH)/srdb1/srdb1
include ../../Makefile.modules


undefined symbol: curl_easy_perform

