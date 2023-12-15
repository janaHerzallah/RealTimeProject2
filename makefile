G = gcc -g
O = -o
names = main customer cashier  generater

files:
	$(G) main.c $(O) main
	$(G) customer.c $(O) customer
	$(G) cashiers.c $(O) cashier
	$(G) generater.c $(O) generater


clean:
	rm -f $(names)