#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>
#include "Net.h"
#include "TrainingData.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <random>
#include "Connector.h"
#include <chrono>
#include <thread>
#include <string>
#include <Windows.h>
#include "GraphicFunctions.h"



using namespace sf;
using namespace std;









void sleep(int milliseconds) {

	std::chrono::milliseconds timespan(milliseconds);

	std::this_thread::sleep_for(timespan);

}







int generate_random_number(int start, int end) {

	std::mt19937 rng;
	rng.seed(chrono::high_resolution_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(start, end); // distribution in range [1, 6]

	return (int)dist6(rng);

}


void show_vector_vals(string label, vector<double> &v)
{

	cout << label << " ";
	for (unsigned i = 0; i < v.size(); i++) {

		cout << v[i] << " ";

	}

	cout << endl;



}

struct BoardData {
	vector<double> board_state;
	int			times_played;
	double		value;

};
	

short check_for_win(vector <double> &board_state) {

	if ((board_state[0] == board_state[1]) && (board_state[0] == board_state[2])) {
		if (board_state[0] == 1) {

			return 1;

		}
		else if (board_state[0] == -1) {

			return -1;

		}

	}

	else if ((board_state[3] == board_state[4]) && (board_state[3] == board_state[5])) {
		if (board_state[3] == 1) {

			return 1;

		}
		else if (board_state[3] == -1) {

			return -1;

		}

	}

	else if ((board_state[6] == board_state[7]) && (board_state[6] == board_state[8])) {
		if (board_state[6] == 1) {

			return 1;

		}
		else if (board_state[6] == -1) {

			return -1;

		}

	}

	else if ((board_state[0] == board_state[3]) && (board_state[0] == board_state[6])) {
		if (board_state[0] == 1) {

			return 1;

		}
		else if (board_state[0] == -1) {

			return -1;

		}

	}

	else if ((board_state[1] == board_state[4]) && (board_state[1] == board_state[7])) {
		if (board_state[1] == 1) {

			return 1;

		}
		else if (board_state[1] == -1) {

			return -1;

		}

	}

	else if ((board_state[2] == board_state[5]) && (board_state[2] == board_state[8])) {
		if (board_state[2] == 1) {

			return 1;

		}
		else if (board_state[2] == -1) {

			return -1;

		}

	}

	else if ((board_state[0] == board_state[4]) && (board_state[0] == board_state[8])) {
		if (board_state[0] == 1) {

			return 1;

		}
		else if (board_state[0] == -1) {

			return -1;

		}

	}

	else if ((board_state[2] == board_state[4]) && (board_state[2] == board_state[6])) {
		if (board_state[2] == 1) {

			return 1;

		}
		else if (board_state[2] == -1) {

			return -1;

		}

	}


	return 0;

}
	




int main() {



	sf::RenderWindow window(sf::VideoMode(450, 450), "MattseTicTacToe!");
	sf::Vector2i window_pos;
	window_pos.x = 0;
	window_pos.y = 0;
	window.setPosition(window_pos);
	graphic_functions::load_textures();
	graphic_functions::init_board_graphics(3, 150);
	graphic_functions::draw_raw_board(window, 3);
	
	//****************for neural net********************************
	vector<unsigned> topology = { 9,18,1 };
	Net my_net(topology);
	vector<double> input_vals, target_vals, result_vals;
	int training_pass = 0;

	//****************for neural net********************************


	//*************for change in values *************************
	double momentum=;


	//*************for change in values *************************



	//container for all free fields
	vector<int> free_fields;
	//container for all fields with symbols , cross or circle
	vector<bool> occupied_fields;
	//current board state
	vector<double> board_state;
	//container for all states during one game
	vector<vector<double>> board_all_states;
	vector<Sprite> sprites;
	vector<BoardData> board_data_base;
	vector<vector<double>> possible_states;

	static int crosswon = 0;
	static int circlewon = 0;

	BoardData start_state;
	start_state.board_state = { 0,0,0,0,0,0,0,0,0 };
	start_state.value = 0;
	start_state.times_played = 0;
	board_data_base.push_back(start_state);
	
	while (window.isOpen())
	{
		//		//start with circle or with cross?
		int select_player = generate_random_number(0, 1);
		sprites.clear();
		board_all_states.clear();

		occupied_fields.clear();
		for (int i = 0; i < 9; i++) {

			occupied_fields.push_back(false);

		}

		board_state.clear();
		for (int i = 0; i < 9; i++) {

			board_state.push_back(0);

		}


		for (int move = 0; move < 9; move++) {

			select_player++;

			//clear free fields for new initialization of the game
			free_fields.clear();
			possible_states.clear();

			//iterate through size of the occupied_fields vector
			for (int i = 0; i < 9; i++) {
				if (occupied_fields[i] == false) {
					free_fields.push_back(i);
				}
			}

			int random = generate_random_number(0, free_fields.size() - 1);
			int random_position = free_fields[random];

			Sprite new_sprite;
			(select_player % 2 == 0) ? new_sprite.setTexture(graphic_functions::_circle) : new_sprite.setTexture(graphic_functions::_cross);



			double free_index;
			vector<int> free_indices;
			free_indices.clear();
			if (select_player % 2 == 0) {



				vector<double> possible_next_state;
				possible_next_state = board_state;
				for (int i = 0; i < free_fields.size(); i++) {
					possible_next_state = board_state;
					free_index = free_fields[i];
					possible_next_state[free_index] = 1;
					free_indices.push_back(free_index);
					possible_states.push_back(possible_next_state);

				}
				double max = -10000000000000000;
				int position_of_best_move;

				for (int i = 0; i < possible_states.size(); i++) {


					input_vals = possible_states[i];


					my_net.feed_forward(input_vals);
					my_net.get_results(result_vals);



					if (result_vals[0] > max) {

						max = result_vals[0];
						position_of_best_move = free_indices[i];

					}


				}


				//board_state = possible_states[random_state];
				
				//THIS random position is not random anymore, just for now
				//random_position = position_of_best_move;
				random_position = position_of_best_move;
				board_state[position_of_best_move] = 1;

				occupied_fields[position_of_best_move] = true;
			}

			else {

				board_state[random_position] = -1;

				occupied_fields[random_position] = true;

			}

			new_sprite.setPosition(random_position % 3 * 150, random_position / 3 * 150);
			sprites.push_back(new_sprite);



			board_all_states.push_back(board_state);
			BoardData insert_state;


			int tester = 0;
			for (int i = 0; i < board_data_base.size(); i++) {



				if (board_state == board_data_base[i].board_state) {

					board_data_base[i].times_played++;
					break;

				}

				else if (i == board_data_base.size() - 1) {
					insert_state.board_state = board_state;
					insert_state.times_played = 1;
					insert_state.value = 0;
					board_data_base.push_back(insert_state);

					//cout << tester << endl;
				}




			}





			//for dragging the window around
			sf::Event e;
			while (window.pollEvent(e)) {

				if (e.type == sf::Event::Closed)
					window.close();
				break;

			}

			if(circlewon%100==0){
			graphic_functions::draw_raw_board(window, 3);
			graphic_functions::draw_figures(window, sprites);
			window.display();
			sleep(500);
			}
		
				
		

			int who_won = check_for_win(board_state);

			

			

			if (who_won == 1) {


				BoardData last_state;
				int number_of_states = board_all_states.size();

				for (int i = 0; i < number_of_states; i++){
					target_vals.clear();
					//setting REWARDS for the former states
					last_state.board_state = board_all_states[board_all_states.size() - 1 - i];
					last_state.value = 1 - double(i) / number_of_states;

					input_vals = last_state.board_state;
				
					cout << "*********************************************" << endl;
					show_vector_vals(": inputs:", input_vals);
					
					my_net.feed_forward(input_vals);
					
					my_net.get_results(result_vals);
					
						show_vector_vals("Outputs: ", result_vals);
					
					target_vals.push_back(last_state.value);
					
						show_vector_vals("Targets: ", target_vals);
					
					assert(target_vals.size() == topology.back());
					
					my_net.back_prop(target_vals);
					
						cout << "Net recent average error: "
							<< my_net.get_recent_average_error() << endl;
						cout << "Done!" << endl;
					
					
				}

				circlewon++;



				
				break;
			}
			else if (who_won == -1) {

				//BoardData last_state;
				//int number_of_states = board_all_states.size();

				//for (int i = 0; i < number_of_states; i++) {
				//	target_vals.clear();
				//	//setting REWARDS for the former states
				//	last_state.board_state = board_all_states[board_all_states.size() - 1 - i];
				//	last_state.value = -1 +(double(i) / number_of_states);

				//	input_vals = last_state.board_state;
				//
				//		cout << "*********************************************" << endl;
				//		show_vector_vals(": inputs:", input_vals);
				//	
				//	my_net.feed_forward(input_vals);

				//	my_net.get_results(result_vals);
				//	
				//		show_vector_vals("Outputs: ", result_vals);
				//	
				//	target_vals.push_back(last_state.value);
				//
				//		show_vector_vals("Targets: ", target_vals);
				//	
				//	assert(target_vals.size() == topology.back());

				//	my_net.back_prop(target_vals);
				//	
				//		cout << "Net recent average error: "
				//			<< my_net.get_recent_average_error() << endl;
				//		cout << "Done!" << endl;
				//	

				//}

				
				crosswon++;

				
				break;
			}

			else if (who_won == 0) {

				
			}

			cout << "circle won: " << circlewon << endl;
			cout << "cross won: " << crosswon << endl;
			cout << "*******************" << endl;
			double relative_winrate = circlewon / (((double)circlewon + (double)crosswon)+1);
			cout << "relative_winrate: " << relative_winrate << endl;
			cout << "*******************" << endl;



			
		}


		


	}
	window.close();
	CloseConnection();

}



//************** neural net stuff*****************************************

//TrainingData train_data("TrainingData.txt");
//
//vector<unsigned> topology;
//
//train_data.get_topology(topology);
//Net my_net(topology);
//
//
//vector<double> input_vals, target_vals, result_vals;
//int training_pass = 0;
//
//while (training_pass<10000) {
//
//	training_pass++;
//	cout << endl << "Pass" << training_pass;
//
//	if (train_data.get_next_inputs(input_vals) != topology[0]) {
//
//		break;
//
//	}
//
//	show_vector_vals(": Inputs:", input_vals);
//	my_net.feed_forward(input_vals);
//
//	my_net.get_results(result_vals);
//	show_vector_vals("Outputs: ", result_vals);
//
//	train_data.get_target_outputs(target_vals);
//	show_vector_vals("Targets: ", target_vals);
//	assert(target_vals.size() == topology.back());
//
//	my_net.back_prop(target_vals);
//
//	cout << "Net recent average error: "
//		<< my_net.get_recent_average_error() << endl;
//
//	cout << "Done!" << endl;
//
//
//
//
//
//
//
//
//
//}
//
//for (int layer = 0; layer < topology.size() - 1; layer++) {
//	cout << "Layer: " << layer << endl;
//	for (int neuron = 0; neuron < my_net.m_layers[layer].size() - 1; neuron++) {
//		cout << "Neuron: " << neuron << endl;
//		for (int neuron2 = 0; neuron2 < my_net.m_layers[layer + 1].size() - 1; neuron2++) {
//
//
//			cout << " next Neuron: " << neuron2 << " weight: " << my_net.m_layers[layer][neuron].m_output_weights[neuron2].weight << endl;
//		}
//
//	}
//
//
//}




//************** neural net stuff*****************************************

// ******************** OLD MAIN **************************************
//	sf::RenderWindow window(sf::VideoMode(450, 450), "MattseTicTacToe!");
//	sf::Vector2i window_pos;
//	window_pos.x = 0;
//	window_pos.y = 0;
//	window.setPosition(window_pos);
//
//
//
//	
//	sf::Sprite board[3][3];
//	
//	
//	//set sprites for fields
//	for (int i = 0; i < 3; i++) {
//		for (int j = 0; j < 3; j++) {
//	
//			if ((i + j) % 2 == 0) {
//				board[i][j] = sf::Sprite(_t1);
//			}
//			else {
//				board[i][j] = sf::Sprite(_t2);
//			}
//	
//			board[i][j].setPosition(i * 150, j * 150);
//	
//		}
//	
//	}
//
//	vector<int> free_fields;
//	vector<bool> occupied_fields;
//	vector<int> board_state;
//	vector<vector<int>> board_all_states;
//	vector<vector<vector<int>>> board_all_states_many_games;
//	int random_position_circle = generate_random_number(0, 9);
//	
//	vector<Sprite> sprites;
//	sf::Sprite cross = sf::Sprite(_cross);
//	sf::Sprite circle = sf::Sprite(_circle);
//
//
//	
////	cross.setPosition(300, 300);
//	
//	
//	
//	
//	while (window.isOpen())
//	{
//		//start with circle or with cross?
//		int select_player = generate_random_number(0, 1);
//		sprites.clear();
//
//		occupied_fields.clear();
//		for (int i = 0; i < 9; i++) {
//
//			occupied_fields.push_back(false);
//
//		}
//
//		board_state.clear();
//		for (int i = 0; i < 9; i++) {
//
//			board_state.push_back(0);
//
//		}
//
//
//		
//		//move counter for iterating many moves
//		for (int move = 0; move < 9; move++) {
//	
//			select_player++;
//
//			//clear free fields for new initialization of the game
//			free_fields.clear();
//
//			//iterate through size of the occupied_fields vector
//			for (int i = 0; i < 9; i++) {
//				if (occupied_fields[i] == false) {
//					free_fields.push_back(i);
//				}
//			}
//
//			int random = generate_random_number(0, free_fields.size() - 1);
//			int random_position = free_fields[random];
//
//			Sprite new_sprite;
//			(select_player % 2 == 0) ? new_sprite.setTexture(_circle) : new_sprite.setTexture(_cross);
//
//			new_sprite.setPosition(random_position % 3 * 150, random_position / 3 * 150);
//			sprites.push_back(new_sprite);
//
//			occupied_fields[random_position] = true;
//			(select_player % 2 == 0) ? board_state[random_position]=1 : board_state[random_position] = -1;
//
//			board_all_states.push_back(board_state);
//
//			sf::Event e;
//
//			while (window.pollEvent(e)) {
//
//				if (e.type == sf::Event::Closed)
//					window.close();
//				break;
//
//			}
//
//
//
//			for (int i = 0; i < 3; i++) {
//				for (int j = 0; j < 3; j++) {
//					window.draw(board[i][j]);
//				}
//			}
//
//			for (int i = 0; i < sprites.size(); i++) {
//
//				window.draw(sprites[i]);
//
//			}
//
//			//	window.draw(cross);
//
//
//			for (int i = 0; i < board_state.size(); i++) {
//
//				cout << "Field: " << i << endl;
//				cout << board_state[i] << endl;
//
//			}
//
//			cout << "*************************************" << endl;
//
//			
//			window.display();
//			
//
//		}
//
//		board_all_states_many_games.push_back(board_all_states);
//
//
//
//
//		CloseConnection();
//
//
//
//	}
	

//}

//
//if (Keyboard::isKeyPressed(Keyboard::Right))
//{
//
//
//}
//
//if (Keyboard::isKeyPressed(Keyboard::Left))
//{
//
//
//}
//}*/

//
//


//win condition : 

/*	for (int i = 0; i < 3; i + 3) {

if (Field[i].status == Field[i + 1].status == Field[i + 2].status)
}
for (int i = 0; i < 3; i++) {
if(Field[i].status == Field[i+3].status == Field[i+6].status)



}

if (Field[0].status == Field[4].status == Field[8].status) {


}

if (Field[2].status == Field[4].status == Field[6].status) {


}*/



