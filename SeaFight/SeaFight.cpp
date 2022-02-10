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
		void setShip(int hor, int vert, Direction dir)
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
		bool geometryCheck(int hor, int vert, Direction dir)
		{
			if (hor < 0 || hor > 9 || vert < 0 || vert > 9)
				return false;
			if (size == 1)
				return true;
			if (size == 2)
			{
				if (dir == Direction::Up)
				{
					if (hor == 9)
						return false;
					return true;
				}
				if (dir == Direction::Down)
				{
					if (hor == 0)
						return false;
					return true;
				}
				if (dir == Direction::Left)
				{
					if (vert == 0)
						return false;
					return true;
				}
				if (dir == Direction::Right)
				{
					if (vert == 9)
						return false;
					return true;
				}
			}
			if (size == 3)
			{
				if (dir == Direction::Up)
				{
					if (hor == 9 || hor == 8)
						return false;
					return true;
				}
				if (dir == Direction::Down)
				{
					if (hor == 0 || hor == 1)
						return false;
					return true;
				}
				if (dir == Direction::Left)
				{
					if (vert == 0 || vert == 1)
						return false;
					return true;
				}
				if (dir == Direction::Right)
				{
					if (vert == 9 || vert == 8)
						return false;
					return true;
				}
			}
			if (size == 4)
			{
				if (dir == Direction::Up)
				{
					if (hor == 9 || hor == 8 || hor == 7)
						return false;
					return true;
				}
				if (dir == Direction::Down)
				{
					if (hor == 0 || hor == 1 || hor == 2)
						return false;
					return true;
				}
				if (dir == Direction::Left)
				{
					if (vert == 0 || vert == 1 || vert == 2)
						return false;
					return true;
				}
				if (dir == Direction::Right)
				{
					if (vert == 9 || vert == 8 || vert == 7)
						return false;
					return true;
				}
			}
		}
		bool shipClosenessCheck(int hor, int vert, Direction dir)
		{
			for (int i = hor - 1 - (size - 1)*(int)(dir == Direction::Down);i <= hor + 1 + (size - 1)*(int)(dir == Direction::Up); ++i)
				for (int j = vert - 1 - (size - 1) * (int)(dir == Direction::Left); j <= vert + 1 + (size - 1) * (int)(dir == Direction::Right); ++j)
				{
					if (i < 0 || i > 9 || j < 0 || j > 9)
						continue;
					if ((*map)[i][j].ship)
						return false;
				}
			return true;
		}
		bool setCheck(int hor, int vert, Direction dir)
		{
			if (geometryCheck(hor, vert, dir) && shipClosenessCheck(hor, vert, dir))
				return true;
			return false;
		}
	};
	class Bot
	{
		vector<pair<int, int>> shotMap;
		vector<vector<Field>>* map;
		vector<pair<int, int>> lastHits;
		bool woundedShip = false;
	public:
		Bot(vector<vector<Field>> *m)
		{
			map = m;
			for (int i = 0; i <= 9; ++i)
				for (int j = 0; j <= 9; ++j)
					shotMap.push_back(make_pair(i,j));
		}
		void clearShipCash()
		{
			if (lastHits.front().first == lastHits.back().first)
			{
				int min = 9, max = 0;
				for (auto p : lastHits)
				{
					if (p.second < min)
						min = p.second;
					if (p.second > max)
						max = p.second;
				}
				for (int i = lastHits.front().first - 1; i <= lastHits.front().first + 1;++i)
					for (int j = min - 1; j <= max + 1; ++j)
					{
						if (i < 0 || i > 9 || j < 0 || j > 9)
							continue;
						auto iter = find(shotMap.begin(), shotMap.end(), make_pair(i, j));
						if (iter != shotMap.end())
							shotMap.erase(iter);
					}
			}
			if (lastHits.front().second == lastHits.back().second)
			{
				int min = 9, max = 0;
				for (auto p : lastHits)
				{
					if (p.first < min)
						min = p.first;
					if (p.first > max)
						max = p.first;
				}
				for (int i = min - 1; i <= max + 1; ++i)
					for (int j = lastHits.front().second - 1; j <= lastHits.front().second + 1; ++j)
					{
						if (i < 0 || i > 9 || j < 0 || j > 9)
							continue;
						auto iter = find(shotMap.begin(), shotMap.end(), make_pair(i, j));
						if (iter != shotMap.end())
							shotMap.erase(iter);
					}
			}
			lastHits.clear();
			woundedShip = false;
		}
		void aim(int& hor, int& vert)
		{
			if (!woundedShip)
			{
				auto target = shotMap[rand() % (shotMap.size())];
				hor = target.first;
				vert = target.second;
				shotMap.erase(find(shotMap.begin(), shotMap.end(), target));
				return;
			}
			else
			{
				if (lastHits.size() == 1)
				{
					while (1)
					{
						int r = rand() % 4;
						if (r == 0)
						{
							auto iter = find(shotMap.begin(), shotMap.end(), make_pair(lastHits[0].first - 1, lastHits[0].second));
							if (iter == shotMap.end())
								continue;
							shotMap.erase(iter);
							hor = lastHits[0].first - 1;
							vert = lastHits[0].second;
							return;
						}
						else if (r == 1)
						{
							auto iter = find(shotMap.begin(), shotMap.end(), make_pair(lastHits[0].first, lastHits[0].second - 1));
							if (iter == shotMap.end())
								continue;
							shotMap.erase(iter);
							hor = lastHits[0].first;
							vert = lastHits[0].second - 1;
							return;
						}
						else if (r == 2)
						{
							auto iter = find(shotMap.begin(), shotMap.end(), make_pair(lastHits[0].first + 1, lastHits[0].second));
							if (iter == shotMap.end())
								continue;
							shotMap.erase(iter);
							hor = lastHits[0].first + 1;
							vert = lastHits[0].second;
							return;
						}
						else if (r == 3)
						{
							auto iter = find(shotMap.begin(), shotMap.end(), make_pair(lastHits[0].first, lastHits[0].second + 1));
							if (iter == shotMap.end())
								continue;
							shotMap.erase(iter);
							hor = lastHits[0].first;
							vert = lastHits[0].second + 1;
							return;
						}
					}
				}
				else
				{
					if (lastHits.front().first == lastHits.back().first)
					{
						int min = 9, max = 0;
						for (auto p : lastHits)
						{
							if (p.second < min)
								min = p.second;
							if (p.second > max)
								max = p.second;
						}
						while (1)
						{
							int r = rand() % 2;
							if (r == 0)
							{
								auto iter = find(shotMap.begin(), shotMap.end(), make_pair(lastHits[0].first, min - 1));
								if (iter == shotMap.end())
									continue;
								shotMap.erase(iter);
								hor = lastHits[0].first;
								vert = min - 1;
								return;
							}
							else
							{
								auto iter = find(shotMap.begin(), shotMap.end(), make_pair(lastHits[0].first, max + 1));
								if (iter == shotMap.end())
									continue;
								shotMap.erase(iter);
								hor = lastHits[0].first;
								vert = max + 1;
								return;
							}
						}
					}
					else
					{
						int min = 9, max = 0;
						for (auto p : lastHits)
						{
							if (p.first < min)
								min = p.first;
							if (p.first > max)
								max = p.first;
						}						
						while (1)
						{
							int r = rand() % 2;
							if (r == 0)
							{
								auto iter = find(shotMap.begin(), shotMap.end(), make_pair(min-1, lastHits[0].second));
								if (iter == shotMap.end())
									continue;
								shotMap.erase(iter);
								hor = min - 1;
								vert = lastHits[0].second;
								return;
							}
							else
							{
								auto iter = find(shotMap.begin(), shotMap.end(), make_pair(max + 1, lastHits[0].second));
								if (iter == shotMap.end())
									continue;
								shotMap.erase(iter);
								hor = max + 1;
								vert = lastHits[0].second;
								return;
							}
						}
					}
				}
			}
		}
		void hit(int hor, int vert)
		{
			lastHits.push_back(make_pair(hor, vert));
			woundedShip = true;
		}
	};
	vector<vector<Field>> map1;
	vector<vector<Field>> map2;

	vector<Ship> navy1;
	vector<Ship> navy2;

	Bot bot;
	bool turn = false;

public:
	SeaFight():map1(10,vector<Field>(10)), map2(10, vector<Field>(10)),bot(&map2)
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
	int shot(int hor,int vert)
	{
		if (!turn)
		{
			map2[hor][vert].hidden = false;
			if (map2[hor][vert].ship == false)
			{
				switchTurn();
				return 0;
			}
			return 1;
		}
		else
		{
			map1[hor][vert].hidden = false;
			if (map1[hor][vert].ship == false)
			{
				switchTurn();
				return 0;
			}
			return 1;
		}
	}
	void draw()
	{
		system("cls");
		for (int hor = 9; hor >= 0; --hor)
		{
			cout << hor << "  ";
			for (int vert = 0; vert <= 9; ++vert)
			{
				if (map1[hor][vert].ship == true && map1[hor][vert].hidden == true)
					cout << "I ";
				else if (map1[hor][vert].ship == true && map1[hor][vert].hidden == false && map1[hor][vert].DestroyedShip == false)
					cout << "X ";
				else if (map1[hor][vert].ship == true && map1[hor][vert].hidden == false && map1[hor][vert].DestroyedShip == true)
					cout << "+ ";
				else if (map1[hor][vert].ship == false && map1[hor][vert].hidden == false)
					cout << "0 ";
				else 
					cout << "_ ";
			}
			cout << endl;
		}
		cout << endl << "   0 1 2 3 4 5 6 7 8 9\n";
		cout << "\n\n";
		for (int hor = 9; hor >= 0; --hor)
		{
			cout << hor << "  ";
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
		cout << endl << "   0 1 2 3 4 5 6 7 8 9\n";
		cout << "\n\n";
	}
	void playerShipsPlacement()
	{
		for (int num = 0; num < navy1.size(); ++num)
		{
			bool ready = false;
			bool firstTime = true;
			while (!ready)
			{
				draw();
				if (!firstTime)
					cout << "Wrong input or ship location\n";
				cout << "Place a ";
				if (navy1[num].size == 1)
					cout << "single";
				else if (navy1[num].size == 2)
					cout << "double";
				else if (navy1[num].size == 3)
					cout << "three";
				else cout << "four";
				cout << "-deck ship\n";
				cout << "Input coordinates and direction char(U,D,L,R)\n";
				int hor, vert;
				char d;
				cin >> hor >> vert >> d;
				Direction dir;
				if (d == 'U')
					dir = Direction::Up;
				else if (d == 'D')
					dir = Direction::Down;
				else if (d == 'L')
					dir = Direction::Left;
				else if (d == 'R')
					dir = Direction::Right;
				else
				{
					firstTime = false;
					continue;				
				}
				if (navy1[num].setCheck(hor, vert, dir))
				{
					navy1[num].setShip(hor, vert, dir);
					ready = true;
				}
				else
					firstTime = false;
			}
			
		}
		
	}
	void botShipsPlacement()
	{
		for (int num = 0; num < navy2.size(); ++num)
		{
			bool ready = false;
			while (!ready)
			{
				int hor, vert;
				Direction dir;
				hor = rand() % 10;
				vert = rand() % 10;
				dir = Direction(rand() % 4);
				if (navy2[num].setCheck(hor, vert, dir))
				{
					navy2[num].setShip(hor, vert, dir);
					ready = true;
				}
			}

		}
	}
	int countDeadShips(vector<Ship>* navy)
	{
		return count_if(navy->begin(), navy->end(), [](Ship& ship) {return ship.destroyed; });
	}
	void play()
	{
		bool gameOver = false;
		playerShipsPlacement();
		botShipsPlacement();
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
				int vert, hor,result,deadShipsBeforeUpd,deadShipsAfterUpd;
				deadShipsBeforeUpd = countDeadShips(&navy1);
				bot.aim(hor, vert);
				result = shot(hor, vert);
				if (result == 1)
					bot.hit(hor, vert);
				updateDestroyed();
				deadShipsAfterUpd = countDeadShips(&navy1);
				if (deadShipsAfterUpd > deadShipsBeforeUpd)
					bot.clearShipCash();
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