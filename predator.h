//predator.h
class predator : public creature{
public:
	//constructor
	//sets row to the_row, col to the_col, gen to the_gen
	//starve_count_down to PREDATOR_STARVE_PERIOD
	//breed_count_down to PREDATOR_BREED_PERIOD
	//a_prey to false and moved to true
	predator(int the_row, int the_col, char the_gen);

	//if there is an adjacent prey then the predator moves to that cell and eat the prey
	//if there are no adjacent preys, move to a random adjacent cell
	//if no adjacent cells are available, the predator stays where it is
	//if the starve_count_down is 0, the predator starves and dies
	//if the breed_count_down is 0, a new prey is born into an empty adjcent cell
	//if no adjcent cell is empty, the prey doesn't breed until next time_step
	virtual void move(vector<vector<creature*>>& field, int time_step);

	//destrucor
	~predator(){};
private:
	//returns true when a new breed is born into an available adjacent cell
	//else returns false
	bool breed(vector<vector<creature*>>& field, int time_step);

	//count down of how many more time steps are required for a predator to starve to death

	int starve_count_down;
	//count down of how many more time steps are required for a predator to breed

	int breed_count_down;
	//checks the adjacent cell directions in the order of UP, DOWN, LEFT and RIGHT
	//returns the direction if the cell in this direction contains a prey
	//if none of the adjacent cells contains a prey
	//returns a random adjacent cell that's inside field

	direction get_predator_dir(const vector<vector<creature*>>& field) const;
	//moves the predator up by one unit
	//if the location was previously occupied by a prey
	//eat the prey and deallocate the memory used by the prey
	void move_up(vector<vector<creature*>>& field);

	//moves the predator down by one unit
	//if the location was previously occupied by a prey
	//eat the prey and deallocate the memory used by the prey
	void move_down(vector<vector<creature*>>& field);

	//moves the predator left by one unit
	//if the location was previously occupied by a prey
	//eat the prey and deallocate the memory used by the prey
	void move_left(vector<vector<creature*>>& field);

	//moves the predator right by one unit
	//if the location was previously occupied by a prey
	//eat the prey and deallocate the memory used by the prey
	void move_right(vector<vector<creature*>>& field);

	//the number of time steps it takes a predator to breed
	const int PREDATOR_BREED_PERIOD = 8;
	//the number of time steps it takes a predator to starve to death
	const int PREDATOR_STARVE_PERIOD = 3;

	//returns the generation of the prey in time steo time_step
	//when time_step is 0, the generation is 'P'
	//after every other PREDATOR_BREED_PERIOD time steps, 
	//the generation becomes the next letter in the alphabet
	//If the previous generation was represented by a 'Z' the next should be an 'A'.
	char get_gen(int time_step) const;
};

predator::predator(int the_row, int the_col, char the_gen) : creature(the_row, the_col, the_gen){
	starve_count_down = PREDATOR_STARVE_PERIOD;
	breed_count_down = PREDATOR_BREED_PERIOD;
	a_prey = false;
	moved = true;
}

void predator::move_up(vector<vector<creature*>>& field){
	//if upper cell is a prey, eat the prey
	if(field[row - 1][col]!= nullptr && field[row - 1][col]->is_prey()){
		//reset starve count down
		starve_count_down = PREDATOR_STARVE_PERIOD;
		//deallocate memory used by the prey
		delete field[row - 1][col];
	}
	//move down
	row--;
	field[row][col] = this;
	field[row + 1][col] = nullptr;
}

void predator::move_down(vector<vector<creature*>>& field){
	//if lower cell is a prey, eat the prey
	if(field[row + 1][col] != nullptr && field[row + 1][col]->is_prey()){
		//reset starve count down
		starve_count_down = PREDATOR_STARVE_PERIOD;
		//deallocate memory used by the prey
		delete field[row + 1][col];
	}
	//move down
	row++;
	field[row][col] = this;
	field[row - 1][col] = nullptr;
}

void predator::move_left(vector<vector<creature*>>& field){
	//if left cell is a prey, eat the prey
	if(field[row][col - 1] != nullptr && field[row][col - 1]->is_prey()){
		//reset starve count down
		starve_count_down = PREDATOR_STARVE_PERIOD;
		//deallocate memory used by the prey
		delete field[row][col - 1];
	}
	//move left
	col--;
	field[row][col] = this;
	field[row][col + 1] = nullptr;
}

void predator::move_right(vector<vector<creature*>>& field){
	//if right cell is a prey, eat the prey
	if(field[row][col + 1] != nullptr && field[row][col + 1]->is_prey()){
		//reset starve count down
		starve_count_down = PREDATOR_STARVE_PERIOD;
		//deallocate the memory used by the prey
		delete field[row][col + 1];
	}
	//move right
	col++;
	field[row][col] = this;
	field[row][col - 1] = nullptr;
}

void predator::move(vector<vector<creature*>>& field, int time_step){
	starve_count_down--; //decrease starve_count_down by one
	breed_count_down--; //decrease breed_count_down by one
	direction dir = get_predator_dir(field); //get the direction to move to
	if(dir == direction::UP && (field[row - 1][col] == nullptr || field[row - 1][col]->is_prey())){
		//if direction is UP and the upper cell is empty or a prey, move up
		move_up(field);
	}else if(dir == direction::DOWN && (field[row + 1][col] == nullptr || field[row + 1][col]->is_prey())){
		//if direction is DOWN and the lower cell is empty or a prey, move down
		move_down(field);
	}else if(dir == direction::LEFT && (field[row][col - 1] == nullptr || field[row][col - 1]->is_prey())){
		//if direction is LEFT and the left cell is empty or a prey, move left
		move_left(field);
	}else if(dir == direction::RIGHT && (field[row][col + 1] == nullptr || field[row][col + 1]->is_prey())){
		//if direction is RIGHT and the upper cell is empty or a prey, move right
		move_right(field);
	} 
	//if the destination cell is already occupied by a predator, stay where it is

	if(starve_count_down == 0){
		int temp_r = row, temp_c = col;
		//deallocate the memory used by the calling predator
 		delete field[row][col];
		field[temp_r][temp_c] = nullptr;
	}else{ 
		//if the calling predator did not starve to death
		//set moved to true
		moved = true;
		if(breed_count_down == 0){
			//reset breed_count_down if breed is successful 
			if(breed(field, time_step)) breed_count_down = PREDATOR_BREED_PERIOD;
			//else set breed_count_down back to 1 and breed next turn
			else breed_count_down = 1;
		}
	}
}

bool predator::breed(vector<vector<creature*>>& field, int time_step){
	if(row != 1 && field[row - 1][col] == nullptr){
		//if upper_cell is available 
		//put a new predator into the upper cell and return true
		field[row- 1][col] = new predator(row - 1, col, get_gen(time_step));
		return true;
	}else if(row != NUM_ROWS && field[row + 1][col] == nullptr){
		//if lower cell is available 
		//put a new predator into the lower cell and return true
		field[row + 1][col] = new predator(row + 1, col, get_gen(time_step));
		return true;
	}else if(col != 1 && field[row][col - 1] == nullptr){
		//if left cell is available 
		//put a new predator into the left cell and return true
		field[row][col - 1] = new predator(row, col - 1, get_gen(time_step));
		return true;
	}else if(col != NUM_COLS && field[row][col + 1] == nullptr){
		//if right cell is available
		//put a new predator into the right cell and return true
		field[row][col + 1] = new predator(row, col +1, get_gen(time_step));
		return true;
	}else{
		return false;
	}
}

char predator::get_gen(int time_step) const{
	return static_cast<char>('P' + (time_step / PREDATOR_BREED_PERIOD + 15) % 25 - 15);
}

direction predator::get_predator_dir(const vector<vector<creature*>>& field) const{
	if(row != 1 && field[row - 1][col]!= nullptr && field[row - 1][col]->is_prey()){
		//if the predator is not in the first row and the upper row is a prey
		//return UP
		return direction::UP;
	}else if(row != NUM_ROWS && field[row + 1][col] != nullptr && field[row + 1][col]->is_prey()){
		//if the predator is not in the last row and the lower row is a prey
		//return DOWN
		return direction::DOWN;
	}else if(col != 1 && field[row][col - 1] != nullptr && field[row][col - 1]->is_prey()){
		//if the predator is not in the first column and the left column is a prey
		//return LEFT
		return direction::LEFT;
	}else if(col != NUM_COLS && field[row][col + 1] != nullptr && field[row][col + 1]->is_prey()){
		//if the predator is not in the last col and the right col is a prey
		//return RIGHT
		return direction::RIGHT;
	}else{
		//if no adjacent cells are preys
		//returns a random, valid direction
		return get_dir();
	}
}
