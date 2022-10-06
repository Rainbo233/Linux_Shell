Project 4: File system manager

CST-315

Professor Ricardo Citro

April 10, 2022

Allen Hall, Ethan Gladen, Gabriel T L Aracena

----------------------------------------------------

This program is to show that our shell has a file manager that 
accomplishes the commands that are required for this assignment
based on our Implementation there may be extra commands but will specify on what they do and should meet the requirements given by the assignment


----------------------------------------------------

To run the Shell, please use the following commands:

$ gcc Linux_Shell.c -o Shell
$ ./Shell


To show that our Shell runs great with our file manager there a couple of text files like lol.text, 1.txt, and 2.txt and about directory which can be used to test our Shell.

----------------------------------------------------

And here are all of the custom commands and what they do:

crf (filename): Creates file

dltf (filename): Deletes file

rename (filename/directory): renames a file or directory

crdir (directoryname): creates directory

copyf (sourcefile) (newfilename): copies a file to an existing one or creates a new file

copydir (sourcedirectory)(targetdirectory): copies source directory and puts that copy into the target directory

dltdir (directoryname): deletes a directory

sif (filename): displays size of file

movef (filename) (directory): moves file to specified directory

tree (directoryname)/, finds the directory and lists contents inside
example "tree directoryname"

list: lists each file and directory within the current directory

list -d: the same as list, but displays metadata about each file as well

chgdir (directory): changes the current directory and is similar to cd

editf (filename): allows user to add text to file

exit: to exit shell

basicf: (filename): allows user to see basic info of file which is the content inside file
