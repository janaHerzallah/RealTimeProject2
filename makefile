G = gcc -g
O = -o
names = main customer shelving_team
thread_args = -lpthread -lrt

files:
	$(G) main.c $(O) main
	$(G) customer.c $(O) customer
	$(G) shelving_team.c $(O) shelving_team $(thread_args)


clean:
	rm -f $(names)