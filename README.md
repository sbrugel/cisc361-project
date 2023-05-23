# S23 CISC361 Term Project
by Simon Brugel, Brendan Lewis, Evan Watson.

For Dr. Mauriello's CISC 361 class. A operating system simulation of job scheduling, resource management, and deadlock avoidance.

## Building & Running

#### Requirements:
You need CMake installed, as well as a C++17 compiler.

If you have a working C++ IDE, feel free to use that (we used CLion, so it's pretty much guaranteed to work on that). If not, run these commands in this directory:

`cmake -B build`

`cmake --build .`

`cd build`

`./CISC361-Term-Project <options>` (use whatever this folder is named)

#### Program Options:

`-h` - Print a help message and exit.

`-f <path to file>` - Specify the full absolute path to an input file. (e.g. `/home/user/.../input.txt`) Can also be relative to the executable. (e.g. `../inputs/input.txt`)

`-f <input file number>` - Specify what input file to run. (0, 1, or 2)

`-i` - Interactive mode. Enter commands manually into the terminal. Quits after a display command at time 9999 is ran. If this option is specified, the provided file path or number is ignored if it exists.

## Design Approach
We used C++ for this project since it contains a definition for a List ADT, so we used that to implement Queue data structures for our job queues. 

We used a struct to hold all the data for the System, including current time, total/available memory, etc, since it would make organization easier and make our code more readable. There is also a Job struct used for a similar purpose.

All the queue types - hold, ready, wait, and complete - are bundled into one class, JobQueue. Each queue is given a sort type and a name (for printing). Jobs are stored in this class in a Queue, which has peek, push, clear, and pop functions. 
There is also a sort function which sorts the jobs in the queue, based on the sorting type:

`FIFO` - Sort jobs by when they arrived

`SJF` - Shorter jobs go first

`COMPLETE` - (For printing purposes) Lower job IDs go first.

*(All functions in JobQueue.cpp are documented)*

As for running the program, we open up the input file supplied in the command and read the lines one by one. Every time we read a line, we get its command type, and its arrival time. 
Firstly we read in the first `C` command, then we simulate the system up to the time of each command, managing the queues as needed (order is wait->ready, hold->ready, ready->running, running->running, running->ready, running->wait (i.e. for Bankers), running->complete). 
Then we read in and process other types of commands that aren't system related.

`C` - Wipes the system data and initializes a new system using the given fields

`A` - Creates a new job with given fields, and puts it in the ready or hold queues. If requires too much memory/devices (in regard to the limits of the system), it doesn't get added anywhere.

`Q` - Creates a new device request with the given properties.

`L` - Creates a new device release request with the given properties.

`D` - Displays all system data, including available resources, jobs in every queue, and turnaround times.
 
We have a Commands source and header file used for parsing commands and their properties easier. This uses a custom series of String utilities to make parsing a lot easier.

## Output
The following images are our console output for each sample input file.

### i0/o0
![img](https://i.imgur.com/q7Nzupi.png)

### i1/o1
![img](https://i.imgur.com/J4ejt9f.png)
![img](https://i.imgur.com/xyMAn55.png)
![img](https://i.imgur.com/O6zdT4Q.png)
![img](https://i.imgur.com/W75ENuR.png)
![img](https://i.imgur.com/3DKRWna.png)

### i2/o2
![img](https://i.imgur.com/PFOrdYY.png)

## Reflection
I feel like we made a good choice using C++ since we didn't have to implement many basic yet time-consuming things on our own such as Lists. Merging all the queues into one class was also advantageous since it allows for more modularity and readable, less, non-repeated code. We learned a lot about job scheduling in a larger environment, as well as the order of which jobs should be moved around and avoiding deadlocks. To improve the program further we could test it on various other types of inputs, such as ones where two jobs arrive at the same time and ones where some jobs are super long and some are super short.
