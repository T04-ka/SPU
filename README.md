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
3. Make your own assembly source and save it with .asm extension. Use only permitted [assembly commands](#assembler_commands).
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

## Assembler commands

Below are all possible assembler commands.

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

### Something else about assembler syntax
* Put a ';' symbol to make the source line after it non-readble for compiler (make a comment)
* You can set labels for some commands (that need a label, see the table above), just put symbol ':' in start of source line
* To put a label as a argument to command, put ':' symbol and then write label name
* You can add empty lines and spaces to make well-readble formatting
* To access a register cell, (for PUSH and POP), just write one of register names (see [section about registers](#registers))
* To access a memory cell (again for PUSH and POP), put argument in square brackets (whether it’s a constant number or a register)

## Assembler
Assembler compiles .asm source in 2 passes, in first pass it makes buffer with all labels, and with second pass, he writes <kbd>asmlistfl.txt</kbd> and <kbd>code.bin</kbd> files. 
<kbd>asmlistfl.txt</kbd> is an assembly listing for your program, usefull programm to debug. <kbd>code.bin</kbd> is an executable file for virtual machine. **DO NOT TRY TO RUN SPU WITH .bin FILE, MAKED NOT BY ASSEMBLER!!!**

## Jump
Jumps are very interesting commands. JUMP definitely jumps in code buffer, but actually it only changes PC value. Conditional jumps work by next way: it popes 2 last numbers from stack, compares them, and jump upon fulfillment of the condition. All of jumps compare nubers in dirrect order. 
FE, below JUMP jumpes if RAX greater than 10:
```text
PUSH RAX
PUSH 10
JMPG : RODIPAHSAY
```
After comparing, only second-to-last number will be pushed back to stack.

## Some fitches

* You can draw picture with only pushes, popes and conditional jumps.
* If you don't like to think every time about subbing order, just push -1, multiple, and add.
* One day I will add video in my assembler. But you can already do it by your own. But FPS will be limited only by your PC power.
* In foulder <kbd>asmsrc</kbd> you can find .asm soucres of factorial, sqaure equation solve, printing an ellipse and something else.
* Yasha is loh.
<p align="center">
  <img width="954" height="474" alt="image" src="https://github.com/user-attachments/assets/e69305ba-582a-44a9-bdbd-27cdb2260609" />
</p>

<h1 align="center">
  🆁🅰🅳🅸 🅽🅴🅴......
</h1>
