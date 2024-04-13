# MasywnyLabiryntC-Java
This is a project for Warsaw's University of technology.
===========
Topic: Make a Program that finds shortest path in the maze and is memory efficent (doesn't use more than 512kb)
===========
In our Approach we've developed an algorithm capable of solving the maze without using more than 512kb of memory for mazes no larger than 1024x1024.
We use BFS to find shortest path to the end, then we use DFS to delete paths from the end that dont lead to the beginning. 
In the end we print the result (the path from start to endd) to a new txt file.
How to use the program:
We've prepared the makefile that includes all instructions regarding the compilation of the program.
Then just start the program using the prepared ./program.exe(out) filename.txt/filename.bin
===========
Disclaimer:
Project is written mostly in English but in some files there might be variables or print statments in Polish (mostly used for debugging)
Program works for binary files but the result.txt file might be a little off (maybe improve in the future?)
===========
Time for solving the 1024x1024 maze - 1 min 8 secs
===========
Authors: Kacper Szczerba, Mikołaj Ciuba
