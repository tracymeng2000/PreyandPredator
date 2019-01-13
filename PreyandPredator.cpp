//prey and predator stimulation

#include <iostream>
#include <vector>
#include <iomanip>
#include "creature.h"
#include "prey.h"
#include "predator.h"
using namespace std;

const string EMPTY = "   ";

//draw the field
//each predator is represented by an upper case letter
//each prey is represented by a lower case letter
//each empty space is represented by the EMPTY string

void draw_field(const vector<vector<creature*>>& field){
	cout << setw(6);
	for(int c = 1 ; c <= NUM_COLS; c++){
		cout << c << "   ";
	}
	cout << endl << "   -----------------------------------------" << endl;
	for(int r = 1; r <= NUM_ROWS; r++){
		cout << r << "  ";
		if(r < 10) cout << " ";
		for(int c = 1; c <= NUM_COLS; c++){
			if(field[r][c] != nullptr){
				cout << " " << field[r][c]-> get_gen() << " |";
			}else{
				cout << EMPTY << "|";
			}
		}
		cout << endl << "   -----------------------------------------" << endl;
	}
}

//generates a random int between 1 and the max number of rows
int rand_row(){
	return rand() % NUM_ROWS  + 1;
}

//generates a random int between 1 and the max number of columns
int rand_col(){
	return rand() % NUM_COLS + 1;
}

//if rand_prey is true, put a prey onto a random spot on the field
//else put a predator onto a random spot on the field
void put_rand_creature(vector<vector<creature*>>& field, bool rand_prey){
	int r = rand_row(), c = rand_col();
	//while the chosen position is not empty
	while(field[r][c] != nullptr){
		r = rand_row();
		c = rand_col();
	}
	if(rand_prey){
		//put a new prey into the chosen position
		field[r][c] = new prey(r, c, 'a');
	}else{
		//put a new predator into the chosen position
		field[r][c] = new predator(r, c, 'P');
	}
}

//put 25 preys and 5 predators onto random and different locations on the field
void initialize_field(vector<vector<creature*>>& field){
	for(int i = 0; i < 25; i++){
		put_rand_creature(field, true); // put 25 preys
	}
	for(int i = 0; i < 5; i++){
		put_rand_creature(field, false); //put 5 predators
	}
}

//if count_for_prey is true, returns the number of prey objects in field
//else returns the number of predators object in field
int count_quantity(vector<vector<creature*>> field, bool count_for_prey){
	int num_prey = 0, num_creature = 0;
	for(int r = 1; r <= NUM_ROWS; r++){
		for(int c = 1; c <= NUM_ROWS; c++){
			if(field[r][c] != nullptr){
				num_creature++;
				if(field[r][c] -> is_prey()) num_prey++;
			}
		}
	}
	//if the function is called to count for prey
	if(count_for_prey) return num_prey;
	//if the function is called to count for predatora
	else return num_creature - num_prey;
}

//deallocate all the memories in use by field
void deallocate_field(vector<vector<creature*>> field){
	for(int r = 1; r <= NUM_ROWS; r++){
		for(int c = 1; c <= NUM_ROWS; c++){
			delete field[r][c];
		}
	}
}

//asks the user whether they want to continue
//returns the char that the user inputs
char ask_user_input(){
	char user_char;
	cout << "enter q to quit or any other key to continue: ";
	cin >> user_char;
	return user_char; 
}

//prints the timestep, number of prey and the number of predators in the current timestep
void print_quantities(int time_step, const vector<vector<creature*>>& field){
	int num_prey = count_quantity(field, true);
	int num_predator = count_quantity(field, false);
	cout << "t: " << time_step << " prey: " << num_prey
		 << " predator: " << num_predator << endl;
}

int main(){
	//creats a new (NUM_ROWS + 2) * (NUM_COLS +2) vector of pointers to creature
	//the first row, first col, last row and last col are imaginary boarders 
	//this is to make indexes to positions more readable (e.g. row 1 col 1 is represented by field[1][1])
	vector<vector<creature*>> field(NUM_ROWS + 2, vector<creature*>(NUM_COLS + 2, nullptr));
	//initialize time_step to 0
	int time_step = 0;
	srand(time(0));
	//put 25 preys and 5 predators onto the field
	initialize_field(field);
	//draw the field
	draw_field(field);
	print_quantities(time_step, field);
	//ask the user whether they want to quit or continue
	char user = ask_user_input();

	//while the user chooses to continue
	while(user != 'q' && user != 'Q'){
		time_step++; //increase time step by 1

		//reset every creature's moved attribute to false 
		//to allow the creature to move in the current turn
		for(int r = 1;  r <= NUM_ROWS; r++){
			for(int c = 1; c <= NUM_COLS; c++){
				if(field[r][c] != nullptr){
					field[r][c]->set_moved(false);
				}
			}
		}

		//move every craeture that hasn't moved on the field	
		for(int r = 1;  r <= NUM_ROWS; r++){
			for(int c = 1; c <= NUM_COLS; c++){
				if(field[r][c] != nullptr && !field[r][c]-> has_moved()){
					field[r][c] -> move(field, time_step);
				}
			}
		}

		//draw the field
		draw_field(field);
		print_quantities(time_step, field);
		//ask if the user wants to continue or quit
		user = ask_user_input();
	} 

	//when the user chooses to quit, deallocate all the memory in use
	deallocate_field(field);
}
