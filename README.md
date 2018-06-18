# 15-213/18-213/15-513: Intro to Computer Systems (ICS), Fall 2015

* * *

<div align="center">
    <a href="http://csapp.cs.cmu.edu/3e/labs.html"><strong>Computer Systems: A Programmer's Perspective, 3/E (CS:APP3e)</strong></a>
</div>
<br>
> **Lab Assignments**
> This page contains a complete set of turnkey labs for the CS:APP3e text. The labs all share some common features. Each lab is distributed in a self-contained tar file. You will need a CS:APP account to download the code. To untar foo.tar, type "tar xvf foo.tar" to the Unix shell. This will create a directory called "foo" that contains all of the material for the lab.
> 
> Handout directories for each lab (without solutions) are available to students who are using the book for self-study and who want to work on the labs. Solutions are provided only to instructors.

* * *

## Course Description

The ICS course provides a programmer's view of how computer systems execute programs, store information, and communicate. It enables students to become more effective programmers, especially in dealing with issues of performance, portability and robustness. It also serves as a foundation for courses on compilers, networks, operating systems, and computer architecture, where a deeper understanding of systems-level issues is required. Topics covered include: machine-level code and its generation by optimizing compilers, performance evaluation and optimization, computer arithmetic, memory organization and management, networking technology and protocols, and supporting concurrent computation.


## Lab Assignments

### Lab1: Datalab

```
$ ./btest 
Score	Rating	Errors	Function
 1	1	0	bitAnd
 2	2	0	getByte
 3	3	0	logicalShift
 4	4	0	bitCount
 4	4	0	bang
 1	1	0	tmin
ERROR: Test fitsBits(-2147483648[0x80000000],32[0x20]) failed...
...Gives 1[0x1]. Should be 0[0x0]
 2	2	0	divpwr2
 2	2	0	negate
 3	3	0	isPositive
 3	3	0	isLessOrEqual
 4	4	0	ilog2
 2	2	0	float_neg
 4	4	0	float_i2f
 4	4	0	float_twice
Total points: 39/41
```

