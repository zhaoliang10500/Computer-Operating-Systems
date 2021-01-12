#!/bin/bash

gcc -c kernel.c cpu.c pcb.c ram.c interpreter.c shell.c shellmemory.c

gcc -o mykernel kernel.o cpu.o pcb.o ram.o interpreter.o shell.o shellmemory.o
