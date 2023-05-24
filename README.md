**&copy;** Lazar Cristian-Stefan 314CA 2022-2023

**&copy;** Munteanu Eugen 315CA 2022-2023

# XOR 0 -- A simple Tic-Tac-Toe game, written in C

## Notice

On the first assignment regarding our Chess project, we had [the following link](https://github.com/EugenM03/simple--ai--chess) to access the source code and the initial implementation.

Unfortunately, the continuation of the project was not possible, due to limited time and homework/assignments for other classes. Thus, we have decided to create another simple game (with AI) to still create a project corresponding to the second PCLP3 task. We are sorry for any inconvenience caused.

The new link is [here](https://github.com/Cristian243342/Tic_Tac_Toe__VS__Monteeeeee_Carlooooo).

&nbsp;

## Introduction

**XOR 0** is a command-line based game written in C *(and 2% Makefile)* that implements the classic Tic Tac Toe (Noughts & Crosses) game. It is a simple yet fun game that you, *the user*, are playing against the opponent.

The game features a simple 3x3 board, with places that can be taken accordingly with X or O, according to the standard rules. The environment where the "drawing fight" is happening, *sadly enough*, is the console interface you are currently running the game. The application also features an AI whose"intelligence" is proportional to the difficulty chosen before starting the game.

---
&nbsp;

## Usage

Firstly, clone the repository using, for example, the following command in Bash:

**TODO: Please change the name of the repositooooooooooooooooory.**

```git clone git@github.com:Cristian243342/Tic_Tac_Toe__VS__Monteeeeee_Carlooooo.git```

Navigate to the folder you've cloned the repository. To start the program, first, use the command `make` (or any equivalent command for compiling the source code). Upon opening the executable, you can choose what side you want to play (either `X` or `O`) and what difficulty level you are comfortable with: ```1``` for easy (*requires little attention*), ```2``` for medium (*average, mostly*) or ```3``` for hard (*don't expect perfect moves though*).

After each move, a representation of the 3x3 board will be sent to ```STDOUT```. The program is done once we have a winner or if it's a draw.

---
&nbsp;

## General Implementation

The Tic Tac Toe board itself is dynamically allocated in memory and stored as a structure:

``` C
// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
typedef struct mc_node_t {
    int64_t wins, sims;  // wins = no. of wins, sims = no. of simulations

    double value;  // maximum selection value of the node in the tree

    //  -1 for O and 1 for X;
    int8_t turn, child_nr, max_children; // (???)

    list_t *child_list;  // list of children used for ????

    int8_t **image;  // current state of the board

    struct mc_node_t *parent;  // parent node
} mc_node_t;
```

We also use a simply linked list for TODO TODO TODO TODO TODO

``` C
typedef struct list_t {
    void *data;
    struct list_t *prev;
    struct list_t *next;
} list_t;
```

---
&nbsp;

## Tic-Tac-Toe AI Implementation

While Tic Tac Toe is not a necessarily complicated game like Chess, we still need to search for the best move in a given position efficiently for better time and space complexity. In our implementation, we used the Monte Carlo algorithm; let's see how it works:

1. For each possible move, simulate multiple random games by making random moves until the game ends.

2. Keep track of the win rate for each move based on the simulated games.

3. Choose the move with the highest win rate as the AI's next move.

By simulating different game scenarios, the AI player can make strategic decisions and improve its chances of winning in the Tic Tac Toe game.

---
&nbsp;

## Team Composition & Tasks

This application was made by two students (*see **copyright** above*), each having multiple tasks regarding this project. The following list is related to each one's work:

* Lazar Cristian-Stefan:
  * Responsible for Tic-Tac-Toe AI Implementation (Monte-Carlo algorithm, data structures, associating them etc.;
  * Basically having a lot of "backend" work;
  * Fixer of memory leaks and debugging;
&nbsp;
* Munteanu Eugen:
  * Responsible for ```STDOUT``` (showing the board, printing correspondent messages etc.);
  * Basically having a lot of "frontend" work;
  * Game tester with multiple cases of having to improve the algorithm;
* Both:
  * Responsible for documentation, *README.md*;
  * Coding Style, Indentation & Spacing;

---
&nbsp;

## Concepts covered from the courses: TODO

From PCLP3 course, we consider we covered the following topics:

* 1st Course - use of ```#define```, ```#ifdef``` and corresponding fields; implementation of Makefile; many memory leaks were found and further corrected using *Valgrind* and *Gdb*; 
* 2nd Course - (maybe bits operations ?) **TODO**
* 3rd Course - many operations on pointers; using both stack and heap;
* 4th Course - dynamically allocated memory throughout the executable (for instance the 3x3 game board or the tree used for Monte Carlo algorithm; we used *malloc()*, *calloc()* and *free()*);
* 5th Course - use of a generic doubly linked list; *memcpy()* also used in some cases;
* 6th Course - (optimisations? time/space complexities?) **TODO**

---
&nbsp;

## Difficulties encountered: TODO

Whilst making our ideas practical, we had up and downs, happiness and anger depending on the situation of what we had in mind to do. Throughout our journey, we had some hard times, including:

* Having a hard time finding some memory leaks regarding not properly linking nodes in the list;

* It was not easy to free the whole memory at first;

* Related to front-end, we had to find an efficient way to ignore invalid input from STDIN;


---
&nbsp;

## Examples of project use: TODO

How could we use this project for something/support for another task? Let's see:

1. Education & Learning -- this project can be partially used as a teaching tool for introduction to programming (in C); for instance, the use of some libraries, some basic commands, like: *printf()*, *scanf()* , use of pointers and arithetic operations and much more;

2. Recreation & Entertainment -- the Tic-Tac-Toe project can also be enjoyed as a recreational game. Despite its simplicity, it can be a good time killer

3. Open-Source contribution -- this application can be (partially) (re)created, being open-source shared on GitHub. Other programmers can make changes and contribute by adding new features or improvements (a better Monte Carlo algorithm or use of another one, player vs. player mode, or creating an application related to this game, for instance, *Connect Four (4 in a row)*).

---
&nbsp;

## Feedback: TODO

---
&nbsp;

## Final words: REDO

Whether you want to have fun, or read the source code and still have some fun (*what a source code!*), ```XOR 0``` is a wonderful way to get your attention, at least for a while (we think). So, why not give it a try today?
