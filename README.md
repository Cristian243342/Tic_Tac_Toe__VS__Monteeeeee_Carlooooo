**&copy;** Lazar Cristian-Stefan 314CA 2022-2023

**&copy;** Munteanu Eugen 315CA 2022-2023

# XOR 0 -- A simple Tic-Tac-Toe game, written in C

## Notice

On the first assignment regarding our Chess project, we had [the following link](https://github.com/EugenM03/simple--ai--chess) to access the source code and the initial implementation.

Unfortunately, the continuation of the project was not possible, due to limited time and homework/assignments for other classes. Thus, we have decided to create another simple game (with AI) to still create a project corresponding to the second PCLP3 task. We are sorry for any inconvenience caused.

The new link is [here](https://github.com/Cristian243342/XOR_0).

&nbsp;

## Introduction

**XOR 0** is a command-line based game written in C *(and 2% Makefile)* that implements the classic Tic Tac Toe (Noughts & Crosses) game. It is a simple yet fun game that you, *the user*, are playing against the opponent.

The game features a simple 3x3 board, with places that can be taken accordingly with X or O, according to the standard rules. The environment where the "drawing fight" is happening, *sadly enough*, is the console interface you are currently running the game. The application also features an AI whose"intelligence" is proportional to the difficulty chosen before starting the game.

---
&nbsp;

## Usage

Firstly, clone the repository using, for example, the following command in Bash:

```git clone https://github.com/Cristian243342/XOR_0```

Navigate to the folder you've cloned the repository. To start the program, first, use the command `make` (or any equivalent command for compiling the source code). Upon opening the executable, you can choose what side you want to play (either `X` or `O`) and what difficulty level you are comfortable with: ```1``` for easy (*requires little attention*), ```2``` for medium (*average, mostly*) or ```3``` for hard (*don't expect perfect moves though*).

After each move, a representation of the 3x3 board will be sent to ```STDOUT```. The program is done once we have a winner or if it's a draw.

---
&nbsp;

## General Implementation

The Tic Tac Toe board itself is dynamically allocated in memory and stored as a structure:

``` C
typedef struct mc_node_t {
    int64_t wins;  // Number of wins.
    
    int64_t sims;  // Number of simulations.

    
    double value;  // Value for selecting the best node for expansion.
    
    int8_t turn;  // Stores whose turn it is in the game states,
                  // stored by the node (1 for X; -1 for O).

    
    int8_t child_nr;  // The number of child game states of the node.

    
    int8_t max_children;  // // The maximum number of children the node can have,
                          // depending on the board state.

    
    list_t *child_list;  // // The list of expansions from the node.

    
    int8_t **image;  //  The state of the board stored in the node
                     //   (1 -> X; 0 -> empty; -1 -> O).
   
    struct mc_node_t *parent;  // The parrent node.

} mc_node_t;
```

We also use a doubly linked list for storing the expansions of the nodes.

``` C
typedef struct list_t {
    // Pointer to the data stored.
    void *data;
    // Pointer to the previous element of the list.
    struct list_t *prev;
    // Pointer to the next element of the list.
    struct list_t *next;
} list_t;
```

---
&nbsp;

## Tic-Tac-Toe AI Implementation

While Tic Tac Toe is not a necessarily complicated game like Chess, we still need to search for a great move in a given game state efficiently for better time and space complexity. In our implementation, we used the Monte Carlo algorithm; let's see how it works:

1. ***Selection***: From the current tree of game states (initially just the root node containing the current game state), the node with the best value is selected for expansion, based on the formula:

    $$value = \frac{no.\ wins}{no.\ simulations} + C\sqrt{\frac {log(current\ simulation\ no)}{no.\ simulations}}$$

    where $C$ is a constant chosen empirically as $\sqrt{2}$.

    This formula determines the value based on two concepts:

    * *Exploitation*: $\frac{no.\ wins}{no.\ simulations}$
    * *Exploration*: $C\sqrt{\frac {log(current\ simulation\ no)}{no.\ simulations}}$

2. ***Expansion***: From the selected node, a random expansion is made, which means finding an unexplored child game state of the chosen node and creating a child node for it.

3. ***Simulation***: From the expansion, a simulation is made from that game state, with moves being chosen based on  heuristics, but mostly random, and remembers the result of the simulation.

4. ***Backpropagation***: Backpropagates the result of the simulation from parent to parent, all the way to the root.

The previous 4-step algorithm is executed repeatedly, with more repetitions resulting in a better move chosen by the AI.

Once the simulations are executed, the algorithm chooses the direct child node of the root with the highest win rate: $\frac{no.\ wins}{no.\ simulations}$.

By simulating different game scenarios, the AI player can make strategic decisions and improve its chances of winning in the Tic Tac Toe game.

---
&nbsp;

## Team Composition & Tasks

This application was made by two students (*see **copyright** above*), each having multiple tasks regarding this project. The following list is related to each one's work:

* Lazar Cristian-Stefan:
  * Responsible for Tic-Tac-Toe AI Implementation (Monte-Carlo algorithm, data structures etc.);
  * Basically having a lot of "backend" work;
  * Fixer of memory leaks and debugging;
&nbsp;
* Munteanu Eugen:
  * Responsible for ```STDOUT``` (showing the board, printing correspondent messages etc.);
  * Basically having a lot of "frontend" work;
  * Game tester with multiple cases and bugs of having to improve the algorithm;
  &nbsp;
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

## Difficulties encountered

Whilst making our ideas practical, we had up and downs, happiness and anger depending on the situation of what we had in mind to do. Throughout our journey, we had some hard times, including:

* Having a hard time finding some memory leaks regarding not properly linking nodes in the list;

* It was not easy to free the whole memory at first;

* Related to front-end, we had to find an efficient way to ignore invalid input from STDIN;

---
&nbsp;

## Examples of project use

How could we use this project for something/support for another task? Let's see:

1. **Education & Learning** -- this project can be partially used as a teaching tool for introduction or another lesson to programming (in C); for instance, the use of some libraries, some basic commands, like: *printf()*, *scanf()* , use of pointers and arithetic operations and much more;

2. **Recreation & Entertainment** -- the Tic-Tac-Toe project can also be enjoyed as a recreational game. Despite its simplicity, it can be a good time killer;

3. **Open-Source contribution** -- this application can be (partially) (re)created, being open-source shared on GitHub. Other programmers can make changes and contribute by adding new features or improvements (a better Monte Carlo algorithm or use of another one, player vs. player mode, or creating an application related to this game, for instance, *Connect Four (4 in a row)*).

---
&nbsp;

## Feedback: TODO

---
&nbsp;

## Final words

Whether you want to have fun, or read the source code and still have some fun (*what a time to be alive!*), ***XOR 0*** is a wonderful way to get your attention, at least for a while (we think). So, why not give it a try today?
