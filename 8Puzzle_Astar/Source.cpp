#include <iostream>
#include "Puzzle.h"

#define PUZZLE_SIZE 3

int main()
{
	TileType<PUZZLE_SIZE> initState =
	{
	8, 6, 7,
	2, 5, 4,
	3, 0, 1
	};

	Puzzle<PUZZLE_SIZE> puzzle(initState);
	Puzzle<PUZZLE_SIZE>::Node* result = puzzle.solveProblem();
	traceBack<PUZZLE_SIZE>(result);
}