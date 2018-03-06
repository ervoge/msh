# Silas - Mini Shell Terminal

Silas Mini Shell Terminal is a friendly chat assistant that mimics many features and commands found in normal terminal windows, while adding some more fun interactions.

## Features

#### Welcome Message
Silas outputs a friendly prompt when he is first booted, displaying a greeting, the time, and a suggested command.  

#### Continuous Prompts
When finished with a command, Silas outputs *What next:*  prompting the user for another command. He will continue to prompt until the user inputs *quit*.  

#### I/O Redirection
The user can redirect input or output to or from a file regardless of whether the file is already open or not.  
###### Output Redirection
`What next: echo Hello World > example.txt`  

In the above example, *example.txt* will contain *Hello World* and overwrite anything that was previously contained in the file. 

If *example.txt* does not exist, Silas will create and open a file using the inputted name. 

If the user would rather append the output from *echo Hello World*, two less than signs should be used. 

`What next: echo Hello World >> example.txt`

Now Silas will append *Hello World* to the end of the *example.txt* file. However, in order to append output, this file must exist.  If the file does not exist, Silas will return an error and ask the user to retry.

###### Input Redirection
`What next: ./countdown < parameters.txt`  

In the above example, the program countdown will execute using the contents of *parameters.txt*. In order to successfully execute input redirection, every file passed needs to alread exist.

#### Automatic Path Searching
`What next: /bin/echo Hello World`

vs. 

`What next: echo Hello World`

Silas understands any general terminal command if the user inputs */bin/* the command name.

If the user inputs only *echo*, Silas will look in three different locations for the command:
- */usr/local/bin/*
- */bin/*
- */usr/bin/*

#### Debugging Mode
`$ ./msh -x`

If the user passes the parameter -x to msh.c when it is first booted, Silas will initiate Debugging Mode. He will repeat every command passed to him and state *About to execute: * then the verbatum command.

## Commands
#### how are you
`What next: how are you`

Silas will answer with a hard-coded polite response.

#### tell me the time
`What next: tell me the time`

Gives the time in seconds and nanoseconds since the epoch.

#### tell me your name
`What next: tell me your name`

Silas will answer with his name, *Silas*.

#### tell me your id
`What next: tell me your id`

Outputs Silas' process ID number.

#### tell me your parent's id
`What next: tell me your parent's id`

Outputs Silas' parent process ID number.

#### tell me your age
`What next: tell me your age`

Outputs Silas' age in seconds since he was first launched.

#### who am i
`What next: who am i`

Grabs the user's Username from the syscall function `getenv()`

#### sleep
`What next: tell me your name`

Silas will sleep for the inputted amount of seconds.

#### list
`What next: list /bin`

Opens a directory, reads the directory, and outputs the files inside the directory.

#### open
`What next: open example.txt`

Opens the inputted file and outputs the resulting file descriptor number.

If the file does not exist,  a new file will be created using the inputted name.
#### read
`What next: read 3`

Silas will read the first line from the file that corresponds to the file descriptor inputted.  `read()` does not take a file name, but only the file descriptor of an already opened file.
#### close 
`What next: close 3`

Closes the file and clears it from the third slot in the process table.
#### help
`What next: help`

Lists all the possible commands and their syntax.
#### quit
`What next: quit`

Outputs a friendly goodbye message and quits the process.

Mini Shell Terminal was completed as an assignment for Holy Cross' CSCI 346 Operating Systems course.
