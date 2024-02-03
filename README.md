## UID: 123456789

## Pipe Up

This program replicates teh behavior of the pipe operators in Unix-Like operating systems.

## Building

The Makefile has been set up. Simply using make will generate the executable for pipe.

## Running

Here's an example of the program running: ./pipe ls cat wc
This will execute ls, cat, wc in that order.
The expected output is that of 'ls | cat | wc'

## Cleaning up

To clean up the binary files, simply running make clean will remove the pipe executable and any other temporary files created. 
