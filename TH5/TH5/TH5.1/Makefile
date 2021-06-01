all:
	gcc -c -Wall -Werror -fpic listnetinfo.c	# compile pic
	gcc -shared -o liblistnetinfo.so listnetinfo.o	# compile shared
	sudo cp liblistnetinfo.so /usr/lib		# copy .so to /usr/lib
	sudo chmod 0755 /usr/lib/ liblistnetinfo.so	# change permision
	sudo ldconfig					# update ld.so config file and cache
	@sed -i '3 i // author: Le Thanh Vinh' main.c
	gcc -Wall -o test main.c -llistnetinfo		# compile test program

lib:
	gcc -c -Wall -Werror -fpic listnetinfo.c
	gcc -shared -o liblistnetinfo.so listnetinfo.o
	sudo cp liblistnetinfo.so /usr/lib
	sudo chmod 0755 /usr/lib/ liblistnetinfo.so
	sudo ldconfig
test:
	gcc -Wall -o test main.c -llistnetinfo
clean:
	rm -rf *.so *.o test
	@sed -i '9 a // author: Le Thanh Vinh' main.c