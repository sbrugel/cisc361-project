# S23 CISC361 Term Project
by Simon Brugel, Brendan Lewis, Evan Watson.

## Design Approach
We used C++ for this project since it contains a definition for a List ADT, so we used that to implement Queue data structures for our job queues. 

We used a struct to hold all the data for the System, including current time, total/available memory, etc, since it would make organization easier and make our code more readable. There is also a Job struct used for a similar purpose.

All of the queue types - hold, ready, wait, and complete - are bundled into one class, JobQueue. Each queue is given a sort type and a name (for printing). Jobs are stored in this class in a Queue, which has peek, push, clear, and pop functions. 
There is also a sort function which sorts the jobs in the queue, based on the sorting type:

`FIFO` - Sort jobs by when they arrived

`SJF` - Shorter jobs go first

`COMPLETE` - (For printing purposes) Lower job IDs go first.

*(All functions in JobQueue.cpp are documented)*

As for running the program, we open up the input file (located in ROOT/inputs) and read the lines one by one. Every time we read a line, we get its command type, and its arrival time. 
Then we simulate the system up to that point, managing the queues as needed. Then we actually read in the command.

`C` - Wipes the system data and initializes a new system using the given fields

`A` - Creates a new job with given fields, and puts it in the ready or hold queues. If requires too much memory/devices (in regard to the limits of the system), it doesn't get added anywhere.

`Q` - Creates a new device request with the given properties.

`L` - Creates a new device release request with the given properties.

`D` - Displays all system data, including available resources, jobs in every queue, and turnaround times.
 
 Finally we have a Commands source and header file used for parsing commands and their properties easier. This uses a custom series of String utilities to make parsing a lot easier.

## Output
### i0/o0
// TODO

### i1/o1
// TODO

### i2/o2
// TODO

## Reflection
I feel like we made a good choice using C++ since we didn't have to implement many basic yet time-consuming things on our own such as Lists. Merging all the queues into one class was also advantageous since it
allows for more modularity and readable, less, non-repeated code.
