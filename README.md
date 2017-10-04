# Quts

## Introduction
Quts is a programming language for several purposes, but Quts is not a general purpose programming language. Its design targets has been changed over the time and therefore its nature may be best reveal by its history (please see later <Road to Quts>). The language

## Features
Loosely stack based imperative language with some functional progeramming twists. The basic syntax is command + list of parameters. Command is case insensitive and parameters are separated y spaces. The core language consists of commands for basic constrol control flow and reserved words, all the other functionality is defined in subsystems. For example strings and math are in their own subsystems. 

## Install
Quts should be installed for every platfowm Qt 5.9 is available, and it is verified on Windows (tested windows 10), Linux (tested Ubuntu 16.04) and OSX (tested Sierra).

## Using
Open quts.pro in QtCreator and compile. That will generated two applications and a library interface.
Please Look .qts files at test/scripts folder.

### QutsConsole
QutsConsole is a IDE application for quts development, you can use built-in help system, set breakpoints and monitor stack and register content. 

### Command line
Command line interface to run quts files.

### Integrated (for testing)
For using Quts for automated testing you shall embed qutslib using in your application, the API is defined in quts.h header. When you do that in main function the QT subsystem will get access to all QWidgets and QML items in the application. 


## Example applications
Here are basic features explained by example.


#Hello world
Printing is done using with <b>!</b> command. 

```
! Hello World 
```
#Hello
Functions can be created with <b>fun<b> command and its block is closed using block closing notation <b>;;</b>. In printing ($<NAME>) refers to value in register, if NAME is omitted, the value printed is popped from the stack. Functions are called with their name and command line is pushed into stack. <b>$</b> is generic stack variable that is popped on every reference.

```
fun hi
    ! Hi ($) !
    ;;

hi "you"
```

# Graphcis
Subsystems are dynamically loaded using <b>use</b> command. 

```
use graphics
use time

graphics.line 50 50 150 50 150 200 50 200 50 50

graphics.line 20 240 620 240

graphics.show true

time.sleep 2000
```

# One kind of Fibonacci
There is also possible use named parameters instead of stack. Then variable passed via local variable context.

```
 # fibonacci 3         				
use INT ''					
fib n0:0 n1:1 count:20				

fun fib n0 n1 count
! ($n0)
push count
calc 1 push subt pop
pop count
if not count 0
    push n0
    push n1
    calc push push sum pop
    pop n
    call fib n0:n1 n1:n count:count
    ;;
;;
```

# Return values
Return values are stored from stack using with <b>=</b>, otherwise stack is cleaned after function call. 
Eval can create runnable functions from strings and execute them immediately.

```
eval 3 "hello again" =$ =dino
```

# Lambdas
<b>Lambda</b> create functions to stack. The <b>call</b> then can be used to call it. The other ways to use them are e.g. list subsystem iterators or time repeaters.

```
lambda
    ! nameless
    ;;

call $
```



# Road to Quts

Basically its story can be dated somewhere back to year 2003 and Doom. I implemented a copy protection system for S60 C2Doom as an interpreter to obfuscate the registration code. That language purpose was to be obscure “write only” language. It featured every command was a single character and program was written as single string. Program was loaded and executed on single 8-bit stack. Therefore program get changed on every step. Furthermore It had only single character hexadecimal numers as command, i.e. ‘9’ was a command that writes int nine into stack and e.g. something like “99*1+!” and writes “82” on console (assuming ‘!’ is a print command). However it was a complete programming language implementing conditional branching and label style jumping.

That engine year later ported Qt a small weeked project - A simple calculator language, the commands were still single characters, but as obfuscation was needed, it had real numbers, separated stack and instruction arrays.  Later I need need a simple test sequence automation, and I reused engine for driving simple command sequences. Quts (Qt User Test System) was there. Since that a lot of changes has been made, but the Qt testing functionality is availale in the QT subsystem. 

The initial Quts language was difficult to program, and to be useful for testing purposes it should be easy. Yet I considered the basic programming model would be very clear for non-programmers: imperative command and its parameters on each line, nothing else.  Initial idea was to keep language very low-level and clean and then write e.g. Python wrapper on top of that, but then I wanted a for-loop. For-loop was an abstraction that enforced me to reconsider many elements in the language. At first labels and goto had to go, as jumping in and out from for loop had confusing side effects. When there was no goto, I had to to implemement “subroutines”, that evolved to be functions. Subroutines and nested for-loops then were reason to have scoped variables.

After choosing that direction Quts have evolved to be a bit easier to program, at least small applications. Therefore I choose publish it if someone would find it fun tool of learn programming, or as a tool for testing engineers to write UI tests for Qt applications. 


