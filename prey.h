//prey.h
//inherits from the creature class
class prey : public creature{
public:
	//constructor
	//sets row to the_row, col to the_col, gen to the_gen
	//breed_count_down to PREY_BREED_PERIOD
	//a_prey to true and moved to true
	prey(int the_row, int the_col, char the_gen);

	//move the prey to a random adjcent cell 
	//if its chosen destination is occupied the prey stays where it is
	//if the breed_count_down is 0, a new prey is born into an empty adjcent cell
	//if no adjcent cell is empty, the prey doesn't breed until next time_step
	virtual void move(vector<vector<creature*>>& field, int time_step);

	//destructor
	~prey(){};

private:
	//returns true when a new prey is born into an available adjcent cell
	//if no adjcent cell is empty, returns false
	bool breed(vector<vector<creature*>>& field, int time_step);

	//count down of how many more time steps are required for a prey to breed
	int breed_count_down;

	//the number of time steps the prey needs to have a new breed
	const int PREY_BREED_PERIOD = 3;

	//returns the generation of the prey in time steo time_step
	//when time_step is 0, the generation is 'a'
	//after every other PREY_BREED_PERIOD time steps,
	//the generation becomes the next letter in the alphabet
	//If the previous generation was represented by a 'z' the next should be an 'a'.
	char get_gen(int time_step) const;
};

prey::prey(int the_row, int the_col, char the_gen) : creature(the_row, the_col, the_gen){
	breed_count_down = PREY_BREED_PERIOD; 
	a_prey = true;
	moved = true;
}

void prey::move(vector<vector<creature*>>& field, int time_step){
	//decrease breed_count_down by one
	breed_count_down--;
	//gets the direction the prey is moving to
	direction dir = get_dir();
	//if direction is up and upper cell is empty
	if(dir == direction::UP && field[row - 1][col] == nullptr){
		row--; //move one row up
		field[row][col] = this; //sets current position pointer to the calling object
		field[row + 1][col] = nullptr; //set previous position as nullptr
	}
	//if direction is down and lower cell is empty
	else if(dir == direction::DOWN && field[row + 1][col] == nullptr){
		row++; //move one row down
		field[row][col] = this;
		field[row - 1][col] = nullptr;
	}
	//if direction is left and left cell is empty
	else if(dir == direction::LEFT && field[row][col - 1] == nullptr){
		col--; //move one col to the left
		field[row][col] = this;
		field[row][col + 1] = nullptr;
	}
	//if direction is right and right cell is empty
	else if(dir == direction::RIGHT && field[row][col + 1] == nullptr){
		col++; //move one col to the right
		field[row][col] = this;
		field[row][col - 1] = nullptr;
	}

	if(breed_count_down == 0){
		//reset breed_count_down if breed is successful 
		if(breed(field, time_step)) breed_count_down = PREY_BREED_PERIOD;
		//else set breed_count_down back to 1 and breed next turn
		else breed_count_down = 1;
	}
	//set moved to true
	moved = true;
}

bool prey::breed(vector<vector<creature*>>& field, int time_step){
	if(row != 1 && field[row - 1][col] == nullptr){
		//if upper_cell is available 
		//put a new prey into the upper cell and return true
		field[row- 1][col] = new prey(row - 1, col, get_gen(time_step));
		return true;
	}else if(row != NUM_ROWS && field[row + 1][col] == nullptr){
		//if lower cell is available 
		//put a new prey into the lower cell and return true
		field[row + 1][col] = new prey(row + 1, col, get_gen(time_step));
		return true;
	}else if(col != 1 && field[row][col - 1] == nullptr){
		//if left cell is available 
		//put a new prey into the left cell and return true
		field[row][col - 1] = new prey(row, col - 1, get_gen(time_step));
		return true;
	}else if(col != NUM_COLS && field[row][col + 1] == nullptr){
		//if right cell is available
		//put a new prey into the right cell and return true
		field[row][col + 1] = new prey(row, col +1, get_gen(time_step));
		return true;
	}else{
		return false;
	}
}

char prey::get_gen(int time_step) const{
	return static_cast<char>(static_cast<int>('a') + (time_step / PREY_BREED_PERIOD) % 25);
}