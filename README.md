**&copy;** Lazar Cristian-Stefan 314CA 2022-2023

**&copy;** Munteanu Eugen 315CA 2022-2023

# XOR 0 -- A simple Tic Tac Toe game, written in C~

## Notice

On the first assignment regarding to our Chess project, we had [the following link](https://github.com/EugenM03/simple--ai--chess) to access the source code and the initial implementation.

Unfortunately, the continuation of the project was not possible, due to limited time and homeworks/assignments to other classes. Thus, we have decided to create another simple game (with AI, duh!) to still create a project correspondent to PCLP3 task. We are sorry for any inconvenience caused.

The new link is [here](https://github.com/Cristian243342/Tic_Tac_Toe__VS__Monteeeeee_Carlooooo).

&nbsp;

## Introduction

**XOR 0** is a command-line based game written in C *(and 2% Makefile)* that implements the classic Tic Tac Toe (Noughts & Crosses) game. It is a simple yet fun game that you, *the user*, are playing against the opponent.

The game features a simple 3x3 board, with places that can be taken accordingly with X or O, according to the standard rules. The environment where the "drawing fight" is happening, *sadly enough*, is the console interface you are currently running the game. The application also features an AI which its "intelligence" is proportional to the difficulty choosed before starting the game.

---
&nbsp;

## Usage

Firstly, clone the repository using, for example, the following command in Bash:
**TODO: Please change the name of the repositooooooooooooooooory.**
```git clone git@github.com:Cristian243342/Tic_Tac_Toe__VS__Monteeeeee_Carlooooo.git```

Navigate to the folder you've cloned the repository. To start the program, firstly use the command `make` (or any equivalent command for compiling the source code). Upon opening the executable, you can choose what side do you want to play (either `X` or `O`) and what difficulty level you are comfortable with: 1 for easy (*requires little attention*), 2 for medium (*average person*) or hard (*don't expect perfect moves though*).

After each move, a representation of the 3x3 board will be sent to ```STDOUT```. The program is done once we have a winner or if it's a draw.

---
&nbsp;

## Implementation

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

We also use a simply linked list for  TODO TODO TODO TODO TODO

``` C
typedef struct list_t {
    void *data;
    struct list_t *prev;
    struct list_t *next;
} list_t;
```

## Chess AI Implementation

While Tic Tac Toe is not a necessarily complicated game like Chess, we still need to search for the best move in a given position efficiently for both time and space complexity. In our implementation, we used Monte Carlo algorithm; let's see how it works:

1. For each possible move, simulate multiple random games by making random moves until the game ends.

2. Keep track of the win rate for each move based on the simulated games.

3. Choose the move with the highest win rate as the AI's next move.

By simulating different game scenarios, the AI player can make strategic decisions and improve its chances of winning in the Tic Tac Toe game.

## Team Composition & Tasks

## Release

## Final words
