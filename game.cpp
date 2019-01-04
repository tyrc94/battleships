#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <limits>


using namespace std;

struct Game
{
	vector <vector<char>> board = {
		{'?','?','?','?','?','?','?','?','?','?'},
		{'?','?','?','?','?','?','?','?','?','?'},
		{'?','?','?','?','?','?','?','?','?','?'},
		{'?','?','?','?','?','?','?','?','?','?'},
		{'?','?','?','?','?','?','?','?','?','?'},
		{'?','?','?','?','?','?','?','?','?','?'},
		{'?','?','?','?','?','?','?','?','?','?'},
		{'?','?','?','?','?','?','?','?','?','?'},
		{'?','?','?','?','?','?','?','?','?','?'},
		{'?','?','?','?','?','?','?','?','?','?'}
	};

	int score = 0;
	map <int, int> ship_remaining;
};

struct Setter
{
	vector <vector<char>> board = {
		{'O','O','O','O','O','O','O','O','O','O'},
		{'O','O','O','O','O','O','O','O','O','O'},
		{'O','O','O','O','O','O','O','O','O','O'},
		{'O','O','O','O','O','O','O','O','O','O'},
		{'O','O','O','O','O','O','O','O','O','O'},
		{'O','O','O','O','O','O','O','O','O','O'},
		{'O','O','O','O','O','O','O','O','O','O'},
		{'O','O','O','O','O','O','O','O','O','O'},
		{'O','O','O','O','O','O','O','O','O','O'},
		{'O','O','O','O','O','O','O','O','O','O'}
	};
	map<int,int> ships_available;
	multimap<int,pair <int, int>> ships_placed;
	int ship_counter = 1;

};


void show_board(Game* g);
void show_setter(Setter* s);
void determine_ship(Game* g, Setter* s, int row, int col);
void enter_continue();
int game_over(Game* g);
int setter_over(Setter* s);

void place_ships(Game* g, Setter* s, int size, int start_row, int start_col, int end_row, int end_col)
{
	int board_size = s -> board.size();
	if((start_row < 0 || start_row >= board_size)||(end_row < 0 || end_row >= board_size))
	{
		cout << "Out of bounds\n\n";
		throw new exception();
	}
	if((start_col < 0 || start_col >= board_size)||(end_col < 0 || end_col >= board_size))
	{
		cout << "Out of bounds\n\n";
		throw new exception();
	}
	if(abs(end_row - start_row) + 1 != size && abs(end_col - start_col) + 1 != size)
	{
		cout << "You can't do that!\n\n";
		throw new exception();
	}
	if((abs(end_row - start_row) + 1 == size && end_col != start_col) || abs(end_col - start_col) + 1 == size && end_row != start_row) 
	{
		cout << "You can't do that!\n\n";
		throw new exception();
	}

	for(int row = min(start_row, end_row); row <= max(start_row, end_row); row++)
	{
		for (int col = min(start_col, end_col); col <= max(start_col, end_col); col++)
		{
			if(s -> board[row][col] == 'X')
			{
				cout << "There's a ship already here!\n\n";
				throw new exception();
			}
			else
			{
				s -> ships_placed.insert(pair<int,pair<int, int>> (s -> ship_counter, pair <int,int>(row, col)));
				s -> board[row][col] = 'X';
				g -> ship_remaining[s -> ship_counter] = size;
			}
		}
	}
	s -> ships_available[size]--;
	s -> ship_counter++;

	/* For debugging */
	
	multimap<int,pair <int, int>> :: iterator itr;
	cout << "\nThe multimap is : \n"; 
    cout << "\tShipID\tCoordinates\n"; 
    for (itr = s-> ships_placed.begin(); itr != s -> ships_placed.end(); ++itr) 
    { 
        cout  <<  '\t' << itr->first 
              <<  '\t' << "(" << itr->second.first << ", " << itr->second.second << ")" << '\n'; 
    } 
    cout << endl;
	

}

void make_move(Game* g, Setter* s, int row, int col)
{
	int board_size = g -> board.size();

	if(row < 0 || row >= board_size)
	{
		throw new exception();
	}
	if(col < 0 || col >= board_size)
	{
		throw new exception();
	}
	if(g -> board[row][col] != '?')
	{
		cout << "You've already tried here.\n";
		throw new exception();
	}
	if(s -> board[row][col] == 'X' && g -> board[row][col] == '?')
	{
		g -> board[row][col] = 'X';
		g -> score++;
		cout << "\nHit!\n";
		determine_ship(g, s, row, col);
	}
	if(s -> board[row][col] == 'O' && g -> board[row][col] == '?')
	{
		g -> board[row][col] = 'O';
		cout << "\nMiss!\n";
	}
	
}

void play_game()
{
	int game_state = 0;
	int setter_state = 0;
	Game game;
	Setter setter;

	string start_coords;
	int start_row, end_row, start_col, end_col, row, col, size;

	setter.ships_available[2] = 1;
	setter.ships_available[3] = 2;
	setter.ships_available[4] = 1;
	setter.ships_available[5] = 1;

	system("printf \"\033c\""); // Clears the screen completely
	cout << "========================\n";
	cout << " Welcome to Battleships \n";
	cout << "========================\n";
	while(setter_state == 0)
	{
		cout << "\n";
		cout << "Ship size (2, 3, 4, or 5): ";
		cin >> size;
		if(cin.fail()) // There must be a much nicer handling of this!
		{
		    cin.clear();
		    cin.ignore();
		    cout << "Invalid" << endl;
		    continue;
		}
		if((setter.ships_available).count(size) == 0)
		{
			cout << "Invalid size.\n";
			continue;
		}
		if ((setter.ships_available)[size] == 0)
		{
			cout << "No more ships of that size.\n";
			continue;
		}
		cout << "\n";

		cout << "Fore coordinates (Format: <row> <column>) ";
		cin >> start_row >> start_col;
		if(cin.fail())
		{
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Please insert integer values only.\n";
			continue;
		}

		cout << "Aft coordinates (Format: <row> <column>) ";
		cin >> end_row >> end_col;
		if(cin.fail())
		{
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Please insert integer values only.\n";
			continue;
		}

		try
		{
			place_ships(&game, &setter, size, start_row, start_col, end_row, end_col);
		}
		catch(...)
		{
			continue;
		}
		show_setter(&setter);
		setter_state = setter_over(&setter);
	}


	system("printf \"\033c\"");	// Clear the screen completely
	cout << "Let's play!\n\n";
	while(game_state == 0)
	{
		cout << "Score: " << game.score << "\n\n";
		cout << "Coordinates (Format: <row> <column>) ";
		cin >> row >> col;
		if(cin.fail())
		{
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Please insert integer values only.\n";
			continue;
		}

        try
        {
        	make_move(&game, &setter, row, col);
        }
        catch(...)
        {
        	cout << "Invalid move.\n\n";
        	continue;
        }

        cout << endl;

		enter_continue();
		system("printf \"\033c\"");
        show_board(&game);
        game_state = game_over(&game);
	}
}

void show_board(Game* g)
{
	cout << "Current state of play\n\n";
	for(int row = 0; row < g -> board.size(); row++)
	{
		for(int col = 0; col < g -> board[row].size(); col++)
		{
			cout << g -> board[row][col] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

void show_setter(Setter* s)
{
	cout << "Current layout\n\n";
	for(int row = 0; row < s -> board.size(); row++)
	{
		for(int col = 0; col < s -> board[row].size(); col++)
		{
			cout << s -> board[row][col] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

int game_over(Game* g)
{
	if(g -> score == 17)
	{
		cout << "You won!";
		return 1;
	}
	return 0;
}

int setter_over(Setter* s)
{
	for(auto const& x : s -> ships_available)
	{
		if(x.second != 0)
		{
			return 0;
		}
	}
	return 1;
}


void determine_ship(Game* g, Setter* s, int row, int col)	// Determine which ship was hit.
{
	multimap<int,pair <int, int>> :: iterator itr;
    for(itr = s -> ships_placed.begin(); itr != s -> ships_placed.end(); ++itr) 
    { 
        if(itr -> second.first == row && itr -> second.second == col)
		{
			g -> ship_remaining[itr -> first]--;
			if(g -> ship_remaining[itr -> first] == 0)
			{
				cout << "You sunk my battleship!\n";
			}
			break;
		}
    } 
}

void enter_continue()
{
    cout << "\nPress Enter to Continue\n";
    string temp;
    temp = cin.get();
    getline(cin, temp);
}

int main(){
	play_game();
}