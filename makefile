G = gcc -g
O = -o
names = main customer cashier

files:
	$(G) main.c $(O) main
	$(G) customer.c $(O) customer
	$(G) cashiers.c $(O) cashier

clean:
	rm -f $(names)