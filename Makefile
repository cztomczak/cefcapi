all:
	@echo "----------------------------------------------------------------------"
	@rm -f Release/cefcapi.out
	@clear
	gcc -Wall -Werror -o Release/cefcapi.out -I. -I.. -Wl,-rpath,. -L./Release examples/main_linux.c -lX11 -lcef `pkg-config --libs --cflags gtk+-2.0`
	cd Release/ && ./cefcapi.out && cd ../
