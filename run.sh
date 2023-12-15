#!/bin/sh


gcc -g -o main main.c
gcc -g customer.c -o customer
gcc  -g cashiers.c -o cashier
gcc -g drawer.c -o drawer -lglut -lGLU -lGL -lm -lpthread -lrt
gcc -g  generater.c -o generater



./main