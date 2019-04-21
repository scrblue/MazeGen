#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>

constexpr int cellLength = 40;
constexpr int lineWidth = 1;
constexpr int windowX = 800;
constexpr int windowY = 800;
constexpr int numColumns = windowX / cellLength;
constexpr int numRows    = windowY / cellLength;

struct position
{
	int x, y;
};

class cell
{
	private:
		//Cell Position
		position p;
		//Toggle for whether to draw the line along each direction
		bool drawUp = true, drawLeft = true, drawDown = true, drawRight = true;
		//Array containing the upper, left, lower, and right lines
		sf::RectangleShape lines[4];
		//Variables controlling highlight status
		sf::Color highlightColor = sf::Color::Blue;
		bool isHighlighted = false;
		//Whether or not the cell has been visited by the navigator already
		bool hasBeenVisited = false;
	public:
		cell(int setX, int setY);
		//Configures SFML shapes
		void configLine(int d);
		//Returns a copy of the cell created
		sf::RectangleShape returnLine (int d);
		sf::RectangleShape returnHighlight();
		//Returns/sets whether the specified direction is to be drawn
		bool returnDrawStatus (int d);
		void setDrawStatus (int d, bool status);
		
		void deleteLine (int d);

		void setHighlighted(bool status);
		void setHighlightColor(sf::Color color);
		bool returnHighlightStatus();

		bool returnVisited();
		void setVisited(bool status);
};

bool cell::returnVisited()
{
	return hasBeenVisited;
}

void cell::setVisited(bool status)
{
	hasBeenVisited = status;
}

bool cell::returnHighlightStatus()
{
	return isHighlighted;
}

void cell::setHighlighted(bool status)
{
	isHighlighted = status;
}

void cell::setHighlightColor(sf::Color color)
{
	highlightColor = color;
}

void cell::deleteLine (int d)
{
	setDrawStatus(d, false);
}

void cell::setDrawStatus (int d, bool status)
{
	if (d == 0)
	{
		drawUp = status;
	}
	else if (d == 1)
	{
		drawLeft = status;
	}
	else if (d == 2)
	{
		drawDown = status;
	}
	else if (d == 3)
	{
		drawRight = status;
	}
}

bool cell::returnDrawStatus (int d)
{
	if (d == 0)
	{
		return drawUp;
	}
	else if (d == 1)
	{
		return drawLeft;
	}
	else if (d == 2)
	{
		return drawDown;
	}
	else if (d == 3)
	{
		return drawRight;
	}
}

cell::cell (int setX, int setY)
{
	p.x = setX;
	p.y = setY;
	configLine(0);
	configLine(1);
	configLine(2);
	configLine(3);
}

sf::RectangleShape cell::returnLine (int d)
{
	if (d == 0)
	{
		return lines[0];
	}
	else if (d == 1)
	{
		return lines[1];
	}
	else if (d == 2)
	{
		return lines[2];
	}
	else if (d == 3)
	{
		return lines[3];
	}
}

sf::RectangleShape cell::returnHighlight()
{
	sf::RectangleShape highlight(sf::Vector2f(cellLength, cellLength));
	highlight.setPosition(p.x*cellLength, p.y*cellLength);
	highlight.setFillColor(highlightColor);
	return highlight;
}

void cell::configLine(int d)
{
	if (d == 0)
	{
		lines[0].setSize(sf::Vector2f(cellLength, lineWidth));
		lines[0].setPosition(p.x*cellLength, p.y*cellLength);
		lines[0].setFillColor(sf::Color::White);
	}
	else if (d == 1)
	{
		
		lines[1].setSize(sf::Vector2f(cellLength, lineWidth));
		lines[1].setPosition(p.x*cellLength, p.y*cellLength);
		lines[1].setRotation(90.f);
		lines[1].setFillColor(sf::Color::White);
	}

	else if (d == 2)
	{
		
		lines[2].setSize(sf::Vector2f(cellLength, lineWidth));
		lines[2].setPosition(p.x*cellLength, (p.y+1)*cellLength);
		lines[2].setFillColor(sf::Color::White);
	}


	else if (d == 3)
	{
		
		lines[3].setSize(sf::Vector2f(cellLength, lineWidth));
		lines[3].setPosition((p.x+1)*cellLength, p.y*cellLength);
		lines[3].setRotation(90.f);
		lines[3].setFillColor(sf::Color::White);
	}
}

class navigator
{
	position p;
	std::vector<position> history;
	public:
		navigator(int setX, int setY);
		position returnPos();
		void setPosition(position p);
		void move(int d);
		void pushHistory(position setP);
		position popHistory();
		int returnHistorySize();
};

int navigator::returnHistorySize()
{
	return history.size();
}

void navigator::pushHistory(position p)
{
	history.push_back(p);
}

position navigator::popHistory()
{
	position lastPosition = history.back();
	history.pop_back();
	return lastPosition;
}

void navigator::setPosition(position setP)
{
	p.x = setP.x;
	p.y = setP.y;
}

void navigator::move(int d)
{
	if (d == 0)
	{
		p.y--;
	}
	else if (d == 1)
	{
		p.x--;
	}
	else if (d == 2)
	{
		p.y++;
	}
	else if (d == 3)
	{
		p.x++;
	}
}

navigator::navigator(int setX, int setY)
{
	p.x = setX;
	p.y = setY;
}

position navigator::returnPos()
{
	return p;
}

void checkValidMoves(std::vector<std::vector<cell>>cellMap, position currentPosition,  bool validMoves[4])
{
	validMoves[0] = false;
	validMoves[1] = false;
	validMoves[2] = false;
	validMoves[3] = false;
	if (currentPosition.y > 0 && !cellMap[currentPosition.x][currentPosition.y-1].returnVisited())
	{
		validMoves[0] = true;
	}
	if (currentPosition.x > 0 && !cellMap[currentPosition.x-1][currentPosition.y].returnVisited())
	{
		validMoves[1] = true;
	}
	if (currentPosition.y < numRows-1 && !cellMap[currentPosition.x][currentPosition.y+1].returnVisited())
	{
		validMoves[2] = true;
	}
	if (currentPosition.x < numColumns-1 && !cellMap[currentPosition.x+1][currentPosition.y].returnVisited())
	{
		validMoves[3] = true;
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(windowX, windowY), "Maze Generator");
	std::vector<std::vector<cell> >  cellMap;
	bool allVisited = false;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, 3);

	for (int i = 0; i < numColumns; i++)
	{
		cellMap.push_back( std::vector<cell>() );
		for (int j = 0; j < numRows; j++)
		{
			cell newCell(i, j);
			cellMap[i].push_back(newCell);
		}
	}

	navigator mapNav(0,0);
	//cellMap[mapNav.returnPos().x][mapNav.returnPos().y].setHighlighted(true);
	cellMap[mapNav.returnPos().x][mapNav.returnPos().y].setVisited(true);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		
		//char debug;
		//std::cin >> debug;

		//Maze-gen algorithm
		if (!allVisited)
		{
			//Check validity of moves
			bool validMoves[4];
			checkValidMoves(cellMap, mapNav.returnPos(), validMoves);
			std::cout << validMoves[0] << " " << validMoves[1] << " " << validMoves[2] << " " << validMoves[3] << std::endl;
			//If there are valid moves
			if (validMoves[0] || validMoves[1] || validMoves[2] || validMoves[3])
			{
				//Choose random cell
				bool isMoveValid = false;
				int directionChose;
				while (!isMoveValid)
				{
					directionChose = distribution(generator);
					
					std::cout << "Boop: " << directionChose << std::endl;

					if (validMoves[directionChose])
					{
						isMoveValid = true;
					}
				}
				//Push current celll to stack
				mapNav.pushHistory(mapNav.returnPos());
				//Remove walls
				cellMap[mapNav.returnPos().x][mapNav.returnPos().y].deleteLine(directionChose);
				if(directionChose == 0)
				{
					cellMap[mapNav.returnPos().x][mapNav.returnPos().y-1].deleteLine(2);
				}
				else if (directionChose == 1)
				{
					cellMap[mapNav.returnPos().x-1][mapNav.returnPos().y].deleteLine(3);
				}
				else if (directionChose == 2)
				{
					cellMap[mapNav.returnPos().x][mapNav.returnPos().y+1].deleteLine(0);
				}
				else if (directionChose == 3)
				{
					cellMap[mapNav.returnPos().x+1][mapNav.returnPos().y].deleteLine(1);
				}
				//Make chosen cell the current cell and mark it as visited
				mapNav.move(directionChose);
				//cellMap[mapNav.returnPos().x][mapNav.returnPos().y].setHighlighted(true);
				cellMap[mapNav.returnPos().x][mapNav.returnPos().y].setVisited(true);
			}
			//Else if the history stack isn't empty
			else if(mapNav.returnHistorySize() > 0)
			{
				mapNav.setPosition(mapNav.popHistory());
				//Pop cell and make it curret
			}
			//Checks if all cells have been visited
			allVisited = true;
			for (int i = 0; i < numColumns; i++)
			{
				for (int j = 0; j < numRows; j++)
				{
					if (!cellMap[i][j].returnVisited())
					{
						allVisited = false;
					}
				}
			}
		}

		//Draws highlights and lines for each cell
		for (int i = 0; i < numColumns; i++)
		{
			for (int j = 0; j < numRows; j++)
			{
				if (cellMap[i][j].returnHighlightStatus())
				{
					window.draw(cellMap[i][j].returnHighlight());
				}
				for (int k = 0; k < 4; k++)
				{
					if (cellMap[i][j].returnDrawStatus(k))
					{
						window.draw(cellMap[i][j].returnLine(k));
					}
				}
			}
		}
		window.display();
	}

	return 0;
}
