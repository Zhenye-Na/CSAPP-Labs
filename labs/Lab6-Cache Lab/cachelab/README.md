# Cache Lab

## Part A: Writing a Cache Simulator

### Hints

#### getopt()
`getopt()` automates parsing elements on the `UNIX` command line if function declaration is missing
- Typically called in a loop to retrieve arguments
- Its return value is stored in a local variable
- When `getopt()` returns `-1`, there are no more options

To use `getopt()`, your program must include the header file `#include <getopt.h>`

A switch statement is used on the local variable holding the return value from `getopt()`
- Each command line input case can be taken care of separately
- `optarg` is an important variable - it will point to the value of the option argument

Example:  
```c
int main(int argc, char** argv){
  int opt, x, y;
  /* looping over arguments */
  while(-1 != (opt = getopt(argc, argv, "x:y:"))){
    /* determine which argument it’s processing */
    switch(opt) {
      case 'x':
        x = atoi(optarg);
        break;
      case 'y':
        y = atoi(optarg);
        break;
      default:
        printf("wrong argument\n");
        break;
    }
  }
}
```

#### fscanf()
The `fscanf()` function is just like `scanf()` except it can specify a stream to read from (scanf always reads from stdin)

**parameters:**  
- A stream pointer
- format string with information on how to parse the file  
- the rest are pointers to variables to store the parsed data  
- You typically want to use this function in a loop. It returns `-1` when it hits `EOF` or if the data doesn't match the format string

**Example:**  

```c
FILE * pFile; //pointer to FILE object
pFile = fopen("tracefile.txt","r"); //open file for reading
char identifier;
unsigned address;
int size;
// Reading lines like " M 20,1" or "L 19,3"
while(fscanf(pFile,"%c %x,%d", &identifier, &address, &size) > 0)
{
  // Do stuff
}
fclose(pFile); //remember to close file when done
```

### Result

```sh
$ time ./test-csim
                        Your simulator     Reference simulator
Points (s,E,b)    Hits  Misses  Evicts    Hits  Misses  Evicts
     3 (1,1,1)       9       8       6       9       8       6  traces/yi2.trace
     3 (4,2,4)       4       5       2       4       5       2  traces/yi.trace
     3 (2,1,4)       2       3       1       2       3       1  traces/dave.trace
     3 (2,1,3)     167      71      67     167      71      67  traces/trans.trace
     3 (2,2,3)     201      37      29     201      37      29  traces/trans.trace
     3 (2,4,3)     212      26      10     212      26      10  traces/trans.trace
     3 (5,1,5)     231       7       0     231       7       0  traces/trans.trace
     6 (5,1,5)  265189   21775   21743  265189   21775   21743  traces/long.trace
    27

TEST_CSIM_RESULTS=27

real	0m0.426s
user	0m0.204s
sys	0m0.106s
```

## Part B: Optimizing Matrix Transpose
