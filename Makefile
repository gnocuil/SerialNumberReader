#CFLAGS := -O2 -lpthread -lX11 -Wno-unused-result
CFLAGS := -O2 -lpthread -Dcimg_display=0 -Wno-unused-result

all: image
image: main.cpp
	g++ -o image main.cpp $(CFLAGS)
clean:
	rm -f image

