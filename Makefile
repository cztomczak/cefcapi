all:
	rm -f Release/cefcapi.out
	clear
	gcc -Wall -o Release/cefcapi.out -I. -Wl,-rpath,. -L./Release main_linux.c -lcef `pkg-config --libs --cflags gtk+-2.0`
	cd Release/ && ./cefcapi.out && cd ../
