#!/bin/sh


gcc -g -o main main.c
gcc -g customer.c -o customer
gcc -g drawer.c -o drawer -lglut -lGLU -lGL -lm -lpthread -lrt
gcc -g timer.c -o timer
gcc -g shelving_team.c -o shelving_team -lpthread -lrt



./main