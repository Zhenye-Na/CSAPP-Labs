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
  int opt,x,y;
  /* looping over arguments */
  while(-1 != (opt = getopt(argc, argv, “x:y:"))){
    /* determine which argument it’s processing */
    switch(opt) {
      case 'x':
        x = atoi(optarg);
        break;
      case ‘y':
        y = atoi(optarg);
        break;
      default:
        printf(“wrong argument\n");
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
pFile = fopen ("tracefile.txt",“r"); //open file for reading
char identifier;
unsigned address;
int size;
// Reading lines like " M 20,1" or "L 19,3"
while(fscanf(pFile,“ %c %x,%d”, &identifier, &address, &size)>0)
{
  // Do stuff
}
fclose(pFile); //remember to close file when done
```

### Result

```

```

## Part B: Optimizing Matrix Transpose
