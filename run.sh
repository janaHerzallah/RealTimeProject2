#!/bin/sh


gcc -g -o main main.c
gcc -g customer.c -o customer
gcc  -g cashiers.c -o cashier


./main