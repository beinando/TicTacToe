
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>



using namespace std;


class TrainingData {

public:
	TrainingData(const string filename);
	bool isEof(void) { return m_training_data_file.eof(); }
	void get_topology(vector<unsigned> &topology);

	//returns the number of input values read from the file:
	unsigned get_next_inputs(vector<double> &input_vals);
	unsigned get_target_out;
	unsigned get_target_outputs(vector<double> &target_output_vals);

private:
	ifstream m_training_data_file;

};

void TrainingData::get_topology(vector<unsigned> &topology)
{

	string line;
	string label;

	getline(m_training_data_file, line);
	stringstream ss(line);
	ss >> label;
	if (this->isEof() || label.compare("topology:") != 0) {

		abort();
	}

	while (!ss.eof()) {

		unsigned n;
		ss >> n;
		topology.push_back(n);

	}

	return;

}

unsigned TrainingData::get_next_inputs(vector<double> &input_vals)
{

	input_vals.clear();

	string line;
	getline(m_training_data_file, line);
	stringstream ss(line);
	

	string label;
	ss >> label;
	if (label.compare("in: ") == 0) {

		double one_value;
		while (ss >> one_value) {

			input_vals.push_back(one_value);

		}

	}

}


unsigned TrainingData::get_target_outputs(vector<double> &target_output_vals)
{

	target_output_vals.clear();
	string line;
	getline(m_training_data_file, line);
	stringstream ss(line);
	

	string label;
	ss >> label;
	
	if (label.compare("out: ") == 0) {

		double one_value;
		while (ss >> one_value) {

			target_output_vals.push_back(one_value);

		}

	}


	return target_output_vals.size();



}







struct Connection {

	double weight;
	double delta_weight;

};


#pragma once
class Neuron;
typedef vector<Neuron> Layer;

// ********************** class neuron **************
class Neuron {

public:
	Neuron(unsigned num_outputs, unsigned my_index);
	void set_output_val(double val) { m_output_val = val; };
	double get_output_val(void) const { return m_output_val; };
	void feed_forward(const Layer &prev_layer);
	void calc_output_gradients(double target_val);
	void calc_hidden_gradients(const Layer &next_layer);
	void update_input_weights(Layer &prev_layer);
	void get_results(vector<double> &result_vals) const;

private:

	static double eta; //[0.0 ... 1.0] overall net training rate
	static double alpha; //[0.0 ... n] multiplier of last weight change (momentum)

	static double transfer_function(double x);
	static double transfer_function_derivative(double x);
	double sum_dow(const Layer &next_layer) const;
	unsigned m_my_index;
	double m_output_val;
	vector<Connection> m_output_weights;

	static double random_weight(void) {return rand() / double(RAND_MAX);}
	double m_gradient;

};

double Neuron::eta = 0.15;
double Neuron::alpha = 0.5;

void Neuron::update_input_weights(Layer &prev_layer) {

	//the wights to be updated are in the connection container 
	//in the neurons in the preceeding layer

	for (unsigned n = 0; n < prev_layer.size(); n++) {
		Neuron &neuron = prev_layer[n];
		double old_delta_weight = neuron.m_output_weights[m_my_index].delta_weight;
		double new_delta_weight =
			//Individual input, magnified by the gradient and train rate
			eta
			* neuron.get_output_val()
			*m_gradient
			//also add momentum = a fraction of the previous delta weight
			+ alpha
			* old_delta_weight;

		neuron.m_output_weights[m_my_index].delta_weight = new_delta_weight;
		neuron.m_output_weights[m_my_index].weight += new_delta_weight;

	}

}

double Neuron::sum_dow(const Layer &next_layer) const {

	double sum = 0.0;

	//sum of our contributions of the errors at the nodes we feed

	for (unsigned n = 0; n < next_layer.size() - 1; n++) {

		sum += m_output_weights[n].weight*next_layer[n].m_gradient;
	}

	return sum;

}


void Neuron::calc_hidden_gradients(const Layer &next_layer)
{

	double dow = sum_dow(next_layer);
	m_gradient = dow*Neuron::transfer_function_derivative(m_output_val);

}


void Neuron::calc_output_gradients(double target_val) 
{

	double delta = target_val - m_output_val;
	m_gradient = delta*Neuron::transfer_function_derivative(m_output_val);



}





double Neuron::transfer_function(double x)
{

	return tanh(x);


}

// x := tanh(x) from transfer_function, so since d/dx tanh(x) = 1-tanh^2(x) it makes sense
double Neuron::transfer_function_derivative(double x)
{

	return 1.0 - x*x;


}


void Neuron::feed_forward(const Layer &prev_layer)
{

	double sum = 0.0;

	//sum up the previous layers outputs (which are our inputs)
	//include the bias node from the previous layer

	for (unsigned n = 0; n < prev_layer.size(); n++) {
		sum += prev_layer[n].get_output_val()*
			prev_layer[n].m_output_weights[m_my_index].weight;

	}

	m_output_val = Neuron::transfer_function(sum);


}
Neuron::Neuron(unsigned num_outputs, unsigned my_index) 
{

	for (unsigned c = 0; c < num_outputs; c++) {

		m_output_weights.push_back(Connection());
		m_output_weights.back().weight = random_weight();

	}

	m_my_index = my_index;

}

// ********************** class neuron **************

class Net
{
public:
	Net(const vector<unsigned> &topology);
	//& : reference
	void feed_forward(const vector<double> &input_vals);
	void back_prop(const vector<double> &target_vals);
	void get_results(vector<double> &result_vals) const;
	double get_recent_average_error(void) const { return m_recent_average_error;}


private:
	vector<Layer> m_layers;
	double m_error;
	double m_recent_average_error;
	double m_recent_average_smoothing_factor;
};


void Net::get_results(vector<double> &result_vals) const
{
	result_vals.clear();

	for (unsigned n = 0; n < m_layers.back().size() - 1; n++) {

		result_vals.push_back(m_layers.back()[n].get_output_val());

	}


}


//********* back prop **************

void Net::back_prop(const vector<double> &target_vals) {

	//calculate overall net error (RMS of output neuron errors)
	Layer &output_layer = m_layers.back();
	m_error = 0.0;

	for (unsigned n = 0; n < output_layer.size() - 1; n++) {

		double delta = target_vals[n] - output_layer[n].get_output_val();
		m_error += delta*delta;

	}

	m_error /= output_layer.size() - 1; // get average error squared
	m_error = sqrt(m_error);			//get RMS

	//implement a recent average measurement 
	m_recent_average_error = (m_recent_average_error * m_recent_average_smoothing_factor + m_error)
	/ (m_recent_average_smoothing_factor + 1.0);


	//calculate output layer gradients
	for (unsigned n = 0; n < output_layer.size() - 1; n++) {

		output_layer[n].calc_output_gradients(target_vals[n]);


	}


	//calculate gradients on hidden layers

	for (unsigned layer_num = m_layers.size() - 2; layer_num > 0; layer_num--) {

		Layer &hidden_layer = m_layers[layer_num];
		Layer &next_layer = m_layers[layer_num+1];
		for (unsigned n = 0; n < hidden_layer.size(); n++) {

			hidden_layer[n].calc_hidden_gradients(next_layer);

		}
	}

	//for all layers from outputs to first hidden layer
	//update connection weights

	for (unsigned layer_num = m_layers.size() - 1; layer_num > 0; layer_num--) {

		Layer &layer = m_layers[layer_num];
		Layer &prev_layer = m_layers[layer_num - 1];

		for (unsigned n = 0; n < layer.size() - 1; n++) {

			layer[n].update_input_weights(prev_layer);

		}

	}


}








//********* back prop **************



//********** feed forward ************
void Net::feed_forward(const vector<double> &input_vals)
{

	assert(input_vals.size() == m_layers[0].size() - 1);

	//Assign (latch) the input values into the input neurons

	for (unsigned i = 0; i < input_vals.size(); i++) {

		m_layers[0][i].set_output_val(input_vals[i]);

	}

	//forward propagation
	for (unsigned layer_num = 1; layer_num < m_layers.size(); layer_num++) {
		Layer &prev_layer = m_layers[layer_num - 1];
		
		for (unsigned n = 0; n < m_layers[layer_num].size() - 1; n++) {
			m_layers[layer_num][n].feed_forward(prev_layer);

		}

	}

}



//********** feed forward ************


Net::Net(const vector<unsigned> &topology)
{

	unsigned num_layers = topology.size();
	for (unsigned layer_num = 0; layer_num < num_layers; layer_num++) {

		unsigned num_outputs = layer_num == topology.size() - 1 ? 0 : topology[layer_num + 1];

		m_layers.push_back(Layer());
		//We have made a new layer, now fill it with neurons, 
		//and a bias neuron to the layer:
		for (unsigned neuron_num = 0; neuron_num <= topology[layer_num]; neuron_num++) {

			m_layers.back().push_back(Neuron(num_outputs, neuron_num));
			cout << "made a neuron!" << endl;
		}

		//force the bias nodes output values to 1.0. it's the last neuron created above
		m_layers.back().back().set_output_val(1.0);

	}

}


void show_vector_vals(string label, vector<double> &v)
{

	cout << label << " ";
	for (unsigned i = 0; i < v.size(); i++) {

		cout << v[i] << " ";

	}



}




int main() {


	TrainingData train_data("TrainingData.txt");
	
	vector<unsigned> topology;
	
	topology.push_back(1);
	topology.push_back(4);
	topology.push_back(1);
	Net my_net(topology);


	vector<double> input_vals, target_vals, result_vals;
	int training_pass = 0;

	while (!train_data.isEof()) {

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


	vector<double> input_vals;
	input_vals.push_back(1.0);
	my_net.feed_forward(input_vals);

	const vector<double> target_vals = { 0.0 };
	
	my_net.back_prop(target_vals);

	vector<double> result_vals;
	my_net.get_results(result_vals);

	

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
