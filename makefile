G = gcc -g
O = -o
names = main customer

files:
	$(G) main.c $(O) main
	$(G) customer.c $(O) customer

clean:
	rm -f $(names)