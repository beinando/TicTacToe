
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>
#include "Net.h"
#include "TrainingData.h"


using namespace std;


void show_vector_vals(string label, vector<double> &v)
{

	cout << label << " ";
	for (unsigned i = 0; i < v.size(); i++) {

		cout << v[i] << " ";

	}

	cout << endl;



}



int main() {


	TrainingData train_data("TrainingData.txt");
	
	vector<unsigned> topology;

	train_data.get_topology(topology);
	Net my_net(topology);


	vector<double> input_vals, target_vals, result_vals;
	int training_pass = 0;

	while (training_pass<10000) {

		training_pass++;
		cout << endl << "Pass" << training_pass;

		if (train_data.get_next_inputs(input_vals) != topology[0]) {

			break;

		}

		show_vector_vals(": Inputs:", input_vals);
		my_net.feed_forward(input_vals);

		my_net.get_results(result_vals);
		show_vector_vals("Outputs: ", result_vals);
		
		train_data.get_target_outputs(target_vals);
		show_vector_vals("Targets: ", target_vals);
		assert(target_vals.size() == topology.back());

		my_net.back_prop(target_vals);

		cout << "Net recent average error: "
			<< my_net.get_recent_average_error() << endl;

		cout << "Done!" << endl;


		
			





	}

	for (int layer = 0; layer < topology.size() - 1; layer++) {
		cout << "Layer: " << layer << endl;
		for (int neuron = 0; neuron < my_net.m_layers[layer].size()-1; neuron++) {
			cout << "Neuron: " << neuron << endl;
			for (int neuron2 = 0; neuron2 < my_net.m_layers[layer + 1].size()-1; neuron2++) {


				cout << " next Neuron: " << neuron2 << " weight: " << my_net.m_layers[layer][neuron].m_output_weights[neuron2].weight << endl;
			}

		}


	}


	

}

//
//
//sf::RenderWindow window(sf::VideoMode(450, 450), "MattseTicTacToe!");
//sf::Vector2i window_pos;
//window_pos.x = 0;
//window_pos.y = 0;
//window.setPosition(window_pos);
//
//
//
//
//
//
//
//
//sf::Texture _t1;
//sf::Texture _t2;
//_t1.loadFromFile("images/Field.png");
//_t2.loadFromFile("images/Field.png");
//sf::Texture _cross;
//sf::Texture _circle;
//_cross.loadFromFile("images/Cross.png");
//_circle.loadFromFile("images/Circle.png");
//
//
//sf::Sprite board[3][3];
//
//
////set sprites for fields
//for (int i = 0; i < 3; i++) {
//	for (int j = 0; j < 3; j++) {
//
//		if ((i + j) % 2 == 0) {
//			board[i][j] = sf::Sprite(_t1);
//		}
//		else {
//			board[i][j] = sf::Sprite(_t2);
//		}
//
//		board[i][j].setPosition(i * 150, j * 150);
//
//	}
//
//}
//
//
//sf::Sprite circle = sf::Sprite(_circle);
//sf::Sprite cross = sf::Sprite(_cross);
//circle.setPosition(150, 150);
//cross.setPosition(300, 300);
//
////
//
//
//while (window.isOpen())
//{
//	auto start = chrono::steady_clock::now();
//	sf::Event e;
//
//
//
//	while (window.pollEvent(e)) {
//
//		if (e.type == sf::Event::Closed)
//			window.close();
//		break;
//
//	}
//
//
//
//	for (int i = 0; i < 3; i++) {
//		for (int j = 0; j < 3; j++) {
//			window.draw(board[i][j]);
//		}
//	}
//
//	window.draw(circle);
//	window.draw(cross);
//
//
//	window.display();
//	//window.close();
//
//	//win condition : 
//
//	/*	for (int i = 0; i < 3; i + 3) {
//
//	if (Field[i].status == Field[i + 1].status == Field[i + 2].status)
//	}
//	for (int i = 0; i < 3; i++) {
//	if(Field[i].status == Field[i+3].status == Field[i+6].status)
//
//
//
//	}
//
//	if (Field[0].status == Field[4].status == Field[8].status) {
//
//
//	}
//
//	if (Field[2].status == Field[4].status == Field[6].status) {
//
//
//	}*/
//
//
//
//	auto end = chrono::steady_clock::now();
//	double elapsed_time = double(chrono::duration_cast<chrono::nanoseconds>(end - start).count());
//	//cout << "elapsed_time    " << elapsed_time << endl;
//
//
//
//	if (Keyboard::isKeyPressed(Keyboard::Right))
//	{
//
//
//	}
//
//	if (Keyboard::isKeyPressed(Keyboard::Left))
//	{
//
//
//	}
//
//	CloseConnection();
//}
