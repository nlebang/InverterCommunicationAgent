VPATH = src
OBJS = invCtrl.o
CC = $(CROSS_COMPILE)gcc
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -fPIC
LFLAGS = -Wall $(DEBUG)

# INCLUDES = -I/usr/local/opt/openssl/include -I./src/host/include
# LIBRARIES = -L/usr/local/opt/openssl/lib -L./src/host
INCLUDES = -I../../output/host/headers
LIBRARIES = -L../../output/host/ -L$(TEEC_EXPORT)/lib -L../../output/libinvctrl
LIBS = -lteec 
STATIC_LIBS = -linv_ctrl_host

all: libinvctrl

libinvctrl: $(OBJS)
	@ echo "Linking object files to create .so file..."
	@ $(CC) $(INCLUDES) -Wl,--no-undefined -shared -o libinvctrl.so $(OBJS) $(LIBRARIES) $(STATIC_LIBS) $(LIBS)

invCtrl.o: invCtrl.c invCtrl.h
	@ echo "Compiling invCtrl.c..."
	@ $(CC)  $(CFLAGS) $(INCLUDES) $(LIBRARIES) ./invCtrl.c $(STATIC_LIBS) $(LIBS)

libinvctrl.dylib:
	@ $(CC) -dynamiclib ./invCtrl.c -o libinvctrl.dylib $(CFLAGS) $(INCLUDES) $(LIBRARIES) $(LIBS)


other:
	@ $(CC) $(INCLUDES) -fPIC \
	reseed.c -o reseed \
	-linvctrl $(LIBRARIES) $(LIBS)

install: create-dir install-headers install-so

create-dir:
	@ echo "Creating output directory..."
	@ mkdir -p output/headers

install-headers:
	@ echo "Copying headers..."
	@ cp -rf ./invCtrl.h ../../output/libinvctrl/headers/

install-so:
	@ echo "Copying library..."
	@ cp -rf libinvctrl.so ../../output/libinvctrl/

install-dylib:
	@ echo "Copying library..."
	@ cp -rf libinvctrl.dylib ../../output/libinvctrl/

clean:
	rm -f *.o *~ libinvctrl.so libinvctrl.dylib reseed