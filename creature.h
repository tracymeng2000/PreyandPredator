//creature.h
#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

enum class direction{UP, RIGHT, DOWN, LEFT};
const int NUM_ROWS = 10;
const int NUM_COLS = 10; 

class creature{
public:
	//constructor
	//sets row as the_row, col as the_col and gen as the_gen
	creature(int the_row, int the_col, char the_gen);
	//returns col
	int get_col() const;
	//returns row
	int get_row() const;
	//returns gen
	char get_gen() const;
	//returns a_prey
	bool is_prey() const;
	//returns moved
	bool has_moved() const;
	//set the moved attribute as bool_var
	void set_moved(bool bool_var);
	//virtual function
	virtual void move(vector<vector<creature*>>& field, int time_step) = 0;
	//virtual destructor
	virtual ~creature(){};

protected:
	//the creature's column
	int col;
	//the creature's row
	int row;
	//generation
	char gen;
	//true if the creature is a prey
	//else false
	bool a_prey;
	//true if the creature has moved for the current turn
	//else false
	bool moved;
	//returns true if dir is a valid direction (the destination cell is on the grid)
	//else returns false	
	bool valid_dir(direction dir) const;
	//returns a random and valid direction
	direction get_dir() const;
};

//constructor
//sets row as the_row, col as the_col and gen as the_gen
creature::creature(int the_row, int the_col, char the_gen){
	row = the_row;
	col = the_col;
	gen = the_gen;
}

//returns col
int creature::get_col() const{
	return col;
}

//returns row
int creature::get_row() const{
	return row;
}

//returns gen
char creature::get_gen() const{
	return gen;
}

//set the moved attribute as bool_var
void creature::set_moved(bool bool_var){
	moved = bool_var;
}

//returns moved
bool creature::has_moved() const{
	return moved;
}

//returns a_prey
bool creature::is_prey() const{
	return a_prey;
}

//returns true if dir is a valid direction (the destination cell is on the grid)
//else returns false
bool creature::valid_dir(direction dir) const{
	return (dir == direction::UP && row != 1) //direction is up and not in the first row
			|| (dir == direction::RIGHT && col != NUM_COLS) //direction is right and not in the last col
			|| (dir == direction::DOWN && row != NUM_ROWS) //direction is down and not in the last row
			|| (dir ==direction::LEFT && col != 1); //direction is left and not in the first col
}

//returns a random and valid direction
direction creature::get_dir() const{
	direction dir = static_cast<direction>(rand() % 4);
	//while the random direction is not valid, generate another random direction
	while(!valid_dir(dir)){
		dir = static_cast<direction>(rand() % 4);
	}
	return dir;
}
