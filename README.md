<h1 align="center">
  PROCESSOR
</h1>

## Lore
This is my university project, 
completed as part of an educational semester course, prodused under direction of Ilya Rudolfovich Dedinski (aka DED, aka SARODIP).

Again a special, big thank you to my mentor, Yasha [Bolda] (also SARODIP and CHMO(d +x)).

## Synopsis
Here presented my virtual processor project, that globaly includes 2 programms: <kbd>assembler</kbd> and <kbd>spu</kbd>. As you have noticed, <kbd>assembler</kbd> is the assemby code compiler, and <kbd>spu</kbd> is a script, that launches a virtual machine.
My assembly language includes commands, which you can see in section [Assembly commands](#assembly_commands).

## Usage
1. Download the archive from [the last page of the release](https://github.com/T04-ka/SPU/releases/tag/yasha_pidor).
2. Extract the contents of the archive to a path that does not contain Russian characters or special symbols.
3. Make your own assembly source and save it with .asm extension. Use only permitted [assembly commands](#assembly_commands).
4. Compile your .asm source with command
```text
./assembler asmsrc.asm
```
<kbd>asmlistfl.txt</kbd> and <kbd>code.bin</kbd> will be created after compilation (see more in section [assembler](#assembler)).

5. Run virtual machine with command 
```text
./spu code.bin
```
6. Here you are.

## Assembler


| Command name | Bytecode | Hex code | Need an argument | Need a label | Description                                                                |
|--------------|----------|----------|------------------|--------------|----------------------------------------------------------------------------|
| HALT         | 0        | 0X0000   | NO               | NO           | End SPU programm                                                           |
| PUSH         | 1        | 0X0001   | YES              | NO           | Pushes number to a stack (see [chapter about PUSH and POP](#push-and-pop)) |
| ADD          | 2        | 0X0002   | NO               | NO           | Adds the last two numbers                                                  |
| SUB          | 3        | 0X0003   | NO               | NO           | Substracts the last number from the second-to-last one                     |
| DIV          | 4        | 0X0004   | NO               | NO           | Divides the second‑to‑last number by the last one                          |
| MUL          | 5        | 0X0005   | NO               | NO           | Multiplies the last two numbers                                            |
| OUT          | 6        | 0X0006   | NO               | NO           | Put out the last number                                                    |
| JUMP         | 7        | 0X0007   | NO               | YES          | Jumps in code buffer (see [chapter about JUMP](#jump))                     |
| JMPG         | 8        | 0X0008   | NO               | YES          | Eq. to JUMP command, except that compares last two numbers                 |
| JMPL         | 9        | 0X0009   | NO               | YES          | Eq. to JUMP command, except that compares last two numbers                 |
| JMPE         | 10       | 0X000A   | NO               | YES          | Eq. to JUMP command, except that compares last two numbers                 |
| JMPLE        | 11       | 0X000B   | NO               | YES          | Eq. to JUMP command, except that compares last two numbers                 |
| JMPGE        | 12       | 0X000C   | NO               | YES          | Eq. to JUMP command, except that compares last two numbers                 |
| JMPNE        | 13       | 0X000D   | NO               | YES          | Eq. to JUMP command, except that compares last two numbers                 |
| CALL         | 14       | 0X000E   | NO               | YES          | Eq. to JUMP command, except that needs RET command at the end              |
| RET          | 15       | 0X000F   | NO               | NO           | Returns to place, where from CALL was called                               |
| POP          | 20       | 0X0014   | YES              | NO           | Popes number from stack (see [chapter about PUSH and POP](#push-and-pop))  |
| IN           | 23       | 0X0017   | NO               | NO           | Pushes number from input stream to stack                                   |
| SQRT         | 24       | 0X0018   | NO               | NO           | Takes the square root of the last number                                   |
| DRAW         | 25       | 0X0019   | NO               | NO           | Displays memory in graphical form                                          |
