#include<iostream>
#include<vector>
#include<algorithm>
#include<ctime>

using namespace std;

class SeaFight
{
	enum Direction
	{
		Left, Right, Up, Down
	};
	struct Field
	{
		bool hidden = true;
		bool ship = false;
		bool DestroyedShip = false;
	};
	struct Ship
	{
		int size;
		vector<vector<Field>> *map;
		vector<Field*> shipFields;
		bool destroyed = false;
		Ship(int s, vector<vector<Field>> *m)
		{
			size = s;
			map = m;
		}
		void setShip(int vert, int hor, Direction dir)
		{
			if (dir == Direction::Left)
			{
				for (int i = 0; i < size; ++i)
				{
					(*map)[hor][vert - i].ship = true;
					shipFields.push_back(&(*map)[hor][vert - i]);
				}
			}
			else if (dir == Direction::Right)
			{
				for (int i = 0; i < size; ++i)
				{
					(*map)[hor][vert + i].ship = true;
					shipFields.push_back(&(*map)[hor][vert + i]);
				}
			}
			else if (dir == Direction::Up)
			{
				for (int i = 0; i < size; ++i)
				{
					(*map)[hor + i][vert].ship = true;
					shipFields.push_back(&(*map)[hor + i][vert]);
				}
			}
			else if (dir == Direction::Down)
			{
				for (int i = 0; i < size; ++i)
				{
					(*map)[hor - i][vert].ship = true;
					shipFields.push_back(&(*map)[hor - i][vert]);
				}
			}
		}
	};
	vector<vector<Field>> map1;
	vector<vector<Field>> map2;

	vector<Ship> navy1;
	vector<Ship> navy2;

	bool turn = false;

public:
	SeaFight():map1(10,vector<Field>(10)), map2(10, vector<Field>(10))
	{
		navy1.push_back(Ship(4, &map1));
		navy1.push_back(Ship(3, &map1));
		navy1.push_back(Ship(3, &map1));
		navy1.push_back(Ship(2, &map1));
		navy1.push_back(Ship(2, &map1));
		navy1.push_back(Ship(2, &map1));
		navy1.push_back(Ship(1, &map1));
		navy1.push_back(Ship(1, &map1));
		navy1.push_back(Ship(1, &map1));
		navy1.push_back(Ship(1, &map1));

		navy2.push_back(Ship(4, &map2));
		navy2.push_back(Ship(3, &map2));
		navy2.push_back(Ship(3, &map2));
		navy2.push_back(Ship(2, &map2));
		navy2.push_back(Ship(2, &map2));
		navy2.push_back(Ship(2, &map2));
		navy2.push_back(Ship(1, &map2));
		navy2.push_back(Ship(1, &map2));
		navy2.push_back(Ship(1, &map2));
		navy2.push_back(Ship(1, &map2));
	}
	void switchTurn()
	{
		turn = !turn;
	}
	vector<Field*> neighbours(vector<vector<Field>>& map, int vert, int hor)
	{
		vector<Field*> result;
		if (vert > 0 && vert < 9 && hor > 0 && hor < 9)
		{
			result.push_back(&map[vert - 1][hor]);
			result.push_back(&map[vert + 1][hor]);
			result.push_back(&map[vert][hor - 1]);
			result.push_back(&map[vert][hor + 1]);
		}
		else if (vert == 0)
		{
			if (hor == 0)
			{
				result.push_back(&map[vert + 1][hor]);
				result.push_back(&map[vert][hor + 1]);
			}
			else if (hor == 9)
			{
				result.push_back(&map[vert + 1][hor]);
				result.push_back(&map[vert][hor-1]);
			}
			else
			{
				result.push_back(&map[vert + 1][hor]);
				result.push_back(&map[vert][hor + 1]);
				result.push_back(&map[vert][hor - 1]);
			}
		}
		else if (vert == 9)
		{
			if (hor == 0)
			{
				result.push_back(&map[vert - 1][hor]);
				result.push_back(&map[vert][hor + 1]);
			}
			else if (hor == 9)
			{
				result.push_back(&map[vert - 1][hor]);
				result.push_back(&map[vert][hor - 1]);
			}
			else
			{
				result.push_back(&map[vert - 1][hor]);
				result.push_back(&map[vert][hor + 1]);
				result.push_back(&map[vert][hor - 1]);
			}
		}
		else if (hor == 0)
		{
			result.push_back(&map[vert - 1][hor]);
			result.push_back(&map[vert + 1][hor]);
			result.push_back(&map[vert][hor + 1]);
		}
		else if (hor == 9)
		{
			result.push_back(&map[vert - 1][hor]);
			result.push_back(&map[vert + 1][hor]);
			result.push_back(&map[vert][hor - 1]);
		}
		return result;
	}
	int victoryCheck()
	{
		if (count_if(navy2.begin(), navy2.end(), [](Ship ship) {return ship.destroyed; }) == navy2.size())
			return 1;
		if (count_if(navy1.begin(), navy1.end(), [](Ship ship) {return ship.destroyed; }) == navy1.size())
			return 2;
		return 0;
	}
	void updateDestroyed()
	{
		for (int i = 0; i < navy1.size(); ++i)
		{
			if (count_if(navy1[i].shipFields.begin(), navy1[i].shipFields.end(), [](Field* field) {return (field->hidden == false && field->ship == true); }) == navy1[i].shipFields.size())
			{
				navy1[i].destroyed = true;			
				for (Field* field : navy1[i].shipFields)
					field->DestroyedShip = true;
			}
		}
		for (int i = 0; i < navy2.size(); ++i)
		{
			if (count_if(navy2[i].shipFields.begin(), navy2[i].shipFields.end(), [](Field* field) {return (field->hidden == false && field->ship == true); }) == navy2[i].shipFields.size())
			{
				navy2[i].destroyed = true;
				for (Field* field : navy2[i].shipFields)
					field->DestroyedShip = true;
			}
		}
	}
	void shot(int vert,int hor)
	{
		if (!turn)
		{
			map2[hor][vert].hidden = false;
			if (map2[hor][vert].ship == false)
				switchTurn();
		}
		else
		{
			map1[hor][vert].hidden = false;
			if (map1[hor][vert].ship == false)
				switchTurn();
		}
	}
	void draw()
	{
		system("cls");
		for (int hor = 9; hor >= 0; --hor)
		{
			for (int vert = 0; vert <= 9; ++vert)
			{
				if (map1[hor][vert].ship == true && map1[hor][vert].hidden == true)
					cout << "I ";
				else if (map1[hor][vert].ship == true && map1[hor][vert].hidden == false && map1[hor][vert].DestroyedShip == false)
					cout << "X ";
				else if (map1[hor][vert].ship == true && map1[hor][vert].hidden == false && map1[hor][vert].DestroyedShip == true)
					cout << "+ ";
				else if (map1[hor][vert].ship == false && map1[hor][vert].hidden == false)
					cout << "- ";
				else 
					cout << "_ ";
			}
			cout << endl;
		}
		cout << "\n\n";
		for (int hor = 9; hor >= 0; --hor)
		{
			for (int vert = 0; vert <= 9; ++vert)
			{
				if (map2[hor][vert].hidden == true)
					cout << "* ";
				else if (map2[hor][vert].ship == true && map2[hor][vert].hidden == false && map2[hor][vert].DestroyedShip == false)
					cout << "X ";
				else if (map2[hor][vert].ship == true && map2[hor][vert].hidden == false && map2[hor][vert].DestroyedShip == true)
					cout << "+ ";
				else
					cout << "_ ";
			}
			cout << endl;
		}
		cout << "\n\n";
	}
	void play()
	{
		Ship ship1(2, &map1);
		Ship ship2(2, &map2);
		ship1.setShip(2, 3, Direction::Left);
		navy1.push_back(ship1);
		ship2.setShip(8, 6, Direction::Left);
		navy2.push_back(ship2);
		bool gameOver = false;
		srand(time(0));
		draw();
		while (!gameOver)
		{
			if (!turn)
			{
				int vert, hor;
				cin >> hor >> vert;
				shot(hor, vert);
				updateDestroyed();
				draw();
				
			}
			else
			{
				int vert, hor;
				vert = rand() % 10;
				hor = rand() % 10;
				shot(hor, vert);
				updateDestroyed();
				draw();
			}
			int res = victoryCheck();
			if (res == 1)
			{
				cout << "Player wins!\n";
				gameOver = true;
			}
			else if (res == 2)
			{
				cout << "Bot wins\n";
				gameOver = true;
			}
		}
		cout << "Game over\n";
		cin.get();
		cin.get();
	}
};
void main()
{
	SeaFight SF;
	SF.play();
}