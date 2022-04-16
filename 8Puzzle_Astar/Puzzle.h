#ifndef PUZZLE_H
#define PUZZLE_H

#include <iostream>
#include <array>
#include <queue>
#include <vector>
#include <unordered_set>

template <std::size_t N>
using TileType = std::array<std::array<uint16_t, N>, N>;

template <std::size_t N>
TileType<N> GOAL = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };

template <std::size_t N>
std::pair<size_t, size_t> findBlank(std::array<std::array<uint16_t, N>, N> state);

enum MV_OPTION { MV_UP, MV_DOWN, MV_LEFT, MV_RIGHT, FAKE };

template <std::size_t N>
void printState(TileType<N> state);

template <std::size_t N>
class Compare;

template <std::size_t N>
class Puzzle
{
public:
	struct Node;
	Puzzle(TileType<N> initState);

	void printFrontier();


	std::pair<Node*, bool> moveTile(typename Puzzle<N>::Node* currentNode, MV_OPTION moveList);

	void pushIntoFrontier(typename Puzzle<N>::Node* currentNode);

	Node* solveProblem();
private:
	Node* m_rootNode;
	std::priority_queue<Node*, std::vector<Node*>, Compare<N>> frontier;
};


template <std::size_t N>
int calculateHeuristic(typename Puzzle<N>::Node* node)
{
	int distanta_Manhattan = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			if (node->getState()[i][j] != 0)
			{
				int tinta_verticala = (node->getState()[i][j] - 1) / N;
				int tinta_horizontala = (node->getState()[i][j] - 1) % N;
				int dist_verticala = i - tinta_verticala;
				int dist_horizontala = j - tinta_horizontala;
				distanta_Manhattan += std::abs(dist_verticala) + std::abs(dist_horizontala);
			}
		}
	return distanta_Manhattan + node->m_pathCost;
}

template<size_t N>
struct HashArr
{
	auto operator() (const TileType<N>& key) const {
		std::hash<uint16_t> hasher;
		size_t result = 0;
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < N; ++j)
				result = result * 31 + hasher(key[i][j]); // ??
		}
		return result;
	}
};

template <std::size_t N>
class Compare
{
public:
	bool operator()(typename Puzzle<N>::Node* node1, typename Puzzle<N>::Node* node2)
	{
		return node1->heuristicValue > node2->heuristicValue;
	}
};

template<std::size_t N>
inline void printState(TileType<N> state)
{
	for (const auto& row : state)
	{
		for (const auto& element : row)
			std::cout << element << ' ';
		std::cout << '\n';
	}
}

template<std::size_t N>
inline std::pair<size_t, size_t> findBlank(std::array<std::array<uint16_t, N>, N> state)
{
	for (size_t i = 0; i < N; i++)
		for (size_t j = 0; j < N; j++)
			if (state[i][j] == 0)
				return { i, j };
}

template<std::size_t N>
struct Puzzle<N>::Node
{
	TileType<N> m_state;
	Node* m_parent;
	std::pair<size_t, size_t> m_blankPos;
	int m_pathCost;
	int heuristicValue;

public:
	Node(TileType<N> state)
	{
		m_state = state;
		m_parent = nullptr;
		m_blankPos = findBlank(state);
		m_pathCost = 0;
		heuristicValue = calculateHeuristic<N>(this);
	}

	Node(Node* parentNode)
	{
		m_state = parentNode->m_state;
		m_parent = parentNode;
		m_pathCost = parentNode->m_pathCost + 1;
		m_blankPos = parentNode->m_blankPos;
		heuristicValue;
	}

	TileType<N> getState()
	{
		return m_state;
	}

};

template<std::size_t N>
inline Puzzle<N>::Puzzle(TileType<N> initState)
{
	m_rootNode = new Node(initState);
}

template<std::size_t N>
std::pair<typename Puzzle<N>::Node*, bool> Puzzle<N>::moveTile(typename Puzzle<N>::Node* currentNode, MV_OPTION moveList)
{
	using Node = typename Puzzle<N>::Node;

	size_t xPos = currentNode->m_blankPos.first;
	size_t yPos = currentNode->m_blankPos.second;
	switch (moveList) {
	case MV_UP:
	{
		if (xPos != 0)
		{
			Node* newNode = new Node(currentNode);
			std::swap(newNode->m_state[xPos][yPos], newNode->m_state[xPos - 1][yPos]);
			newNode->m_blankPos.first -= 1;
			newNode->heuristicValue = calculateHeuristic<N>(newNode);
			newNode->m_parent = currentNode;
			/*m_curNode = newNode;*/
			return { newNode, true };
		}
		else
			return { currentNode, false };
		break;
	}
	case MV_DOWN:
	{
		if (xPos != N - 1)
		{
			Node* newNode = new Node(currentNode);
			std::swap(newNode->m_state[xPos][yPos], newNode->m_state[xPos + 1][yPos]);
			newNode->m_blankPos.first += 1;
			newNode->heuristicValue = calculateHeuristic<N>(newNode);
			newNode->m_parent = currentNode;
			/*m_curNode = newNode;*/
			return { newNode, true };
		}
		else
			return { currentNode, false };
		break;
	}
	case MV_LEFT:
	{
		if (yPos != 0)
		{
			Node* newNode = new Node(currentNode);
			std::swap(newNode->m_state[xPos][yPos], newNode->m_state[xPos][yPos - 1]);
			newNode->m_blankPos.second -= 1;
			newNode->heuristicValue = calculateHeuristic<N>(newNode);
			newNode->m_parent = currentNode;
			/*m_curNode = newNode;*/
			return { newNode, true };
		}
		else
			return { currentNode, false };
		break;
	}
	case MV_RIGHT:
	{
		if (yPos != N - 1)
		{
			Node* newNode = new Node(currentNode);
			std::swap(newNode->m_state[xPos][yPos], newNode->m_state[xPos][yPos + 1]);
			newNode->m_blankPos.second += 1;
			newNode->heuristicValue = calculateHeuristic<N>(newNode);
			newNode->m_parent = currentNode;
			/*m_curNode = newNode;*/
			return { newNode, true };
		}
		else
			return { currentNode, false };
		break;
	}
	}
}

template <std::size_t N>
void Puzzle<N>::pushIntoFrontier(typename Puzzle<N>::Node* currentNode)
{
	for (int moveInt = MV_UP; moveInt != FAKE; moveInt++)
	{
		MV_OPTION move = static_cast<MV_OPTION>(moveInt);
		std::pair<typename Puzzle<N>::Node*, bool> movedNode = moveTile(currentNode, move);
		if (movedNode.second)
			frontier.push(movedNode.first);
	}
}

template <std::size_t N>
bool isGoal(typename Puzzle<N>::Node* currentNode)
{
	return currentNode->m_state == GOAL<N> ? true : false;
}

template<std::size_t N>
inline void Puzzle<N>::printFrontier()
{
	while (!frontier.empty())
	{
		printState(frontier.top()->m_state);
		std::cout << '\n';
		frontier.pop();
	}
}

template<std::size_t N>
inline typename Puzzle<N>::Node* Puzzle<N>::solveProblem()
{
	size_t reps = 0;
	std::unordered_set<TileType<N>, HashArr<N>> exploredSet;
	frontier.push(m_rootNode);
	while (!frontier.empty())
	{
		if (reps % 1000 == 0)
			std::cout << "In progress...\n";
		reps++;
		if (frontier.top()->m_state == GOAL<N>)
		{
			std::cout << "Number of iterations made: " << reps << '\n';
			std::cout << "Depth: " << frontier.top()->m_pathCost << '\n';
			return frontier.top();
		}
		Node* chosenNode = frontier.top();
		if (exploredSet.find(chosenNode->m_state) == exploredSet.end())
			pushIntoFrontier(chosenNode);
		exploredSet.insert(chosenNode->m_state);
		frontier.pop();
	}
	return m_rootNode;

}

template<std::size_t N>
void traceBack(typename Puzzle<N>::Node* result)
{

	if (result == nullptr)
		return;
	traceBack<N>(result->m_parent);
	printState(result->m_state);
	std::cout << '\n';
}

#endif