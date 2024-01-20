#include <iostream>
#include <random>    // atsitiktiniu skaiciu generatorius
#include <chrono>    // pradinei reiksmei ir laikui skaiciuoti
#include <vector>    // duomenu struktura
#include <fstream>
#include <string>

using namespace std;

const int dataSize[5] = { 1000000, 2000000, 4000000, 8000000, 16000000 };  // kiek atsitiktiniu skaiciu sugeneruoti

void generateData(int dataSize, int numCountStart, int numCountEnd)
{
	string file_name = "data_" + to_string(numCountEnd) + "_" + to_string(dataSize) + ".txt";  // failo pavadinimas
	unsigned int seed = std::chrono::steady_clock::now().time_since_epoch().count();   // pradine reiksme - seed - kiek praėjo laiko (s) nuo epochos pradžios iki dabar. Galima naudoti ir pirminį skaičių 19937

	default_random_engine randomNumberGenerator(seed);				//varikliukas, kuris generuoja random skaicius
	uniform_int_distribution<int> distribution(int(pow(10, numCountStart - 1)), int(pow(10, numCountEnd) - 1));

	// *** GENERUOJAME REIKIAMA KIEKI ATSITIKTINIU SKAICIU
	vector <int> data;  // vektorius, kuriame saugosime sugeneruotus skaicius
	for (int i = 0; i < dataSize; i++)
		data.push_back(distribution(randomNumberGenerator));  // sugeneruojamas vienas skaicius ir idedamas i vektoriu

	// *** IRASYMAS AIBES I FAILA
	ofstream output_file;
	output_file.open(file_name);
	for (auto number : data)
	{
		output_file << number << "\n";
	}
	output_file.close();
}

void countingSort(std::vector<int>& array, int place) {
	const int max = 10; // number base

	// sukuriamas output vektorius, kuriame bus laikomi surusiuoti skaiciai
	std::vector<int> output(array.size(), 0);

	// sukuriamas count vektorius, kuriame bus laikomi skaiciu pasikartojimo kiekiai
	std::vector<int> count(max, 0);

	// skaiciuojama kiek kartu pasikartoja kiekvienas skaitmuo
	for (int i = 0; i < array.size(); ++i)
		count[(array[i] / place) % 10]++;

	// atnaujinamas count vektorius, kad laikyti tikrasias skaiciu reiksmes output vektoriuje
	for (int i = 1; i < max; i++)
		count[i] += count[i - 1];

	// sudaromas output vektorius pagal count vektoriu ir skaiciu pasikartojimo kieki
	for (int i = array.size() - 1; i >= 0; i--) {
		output[count[(array[i] / place) % 10] - 1] = array[i];
		count[(array[i] / place) % 10]--;
	}
	//kopijuojami elementai is output i array
	for (int i = 0; i < array.size(); i++)
		array[i] = output[i];
}

void radixSort(std::vector<int>& array, int size) {
	for (int place = 1; size / place > 0; place *= 10)
		countingSort(array, place);
}

std::string generateRandomString(int minLength, int maxLength) {
	static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static std::mt19937 randomEngine(std::chrono::steady_clock::now().time_since_epoch().count());
	static std::uniform_int_distribution<int> distribution(0, sizeof(charset) - 2);

	int length = std::uniform_int_distribution<int>(minLength, maxLength)(randomEngine);
	std::string result;
	result.reserve(length);

	for (int i = 0; i < length; ++i) {
		result += charset[distribution(randomEngine)];
	}

	return result;
}

void generateStringData(int dataSize, int minLength, int maxLength) {
	std::string file_name = "data_" + std::to_string(maxLength) + "_" + std::to_string(dataSize) + "_string.txt";

	std::ofstream output_file(file_name);
	if (!output_file.is_open()) {
		std::cout << "Error opening the file." << std::endl;
		return;
	}

	for (int i = 0; i < dataSize; ++i) {
		std::string randomString = generateRandomString(minLength, maxLength);
		output_file << randomString << "\n";
	}

	output_file.close();
}

void countingSortString(vector<string>& array, int place) {
	const int charRange = 256;

	vector<string> output(array.size());
	vector<int> count(charRange, 0);

	for (int i = 0; i < array.size(); ++i)
		count[array[i].size() >= place ? static_cast<unsigned char>(array[i][array[i].size() - place]) : 0]++;

	for (int i = 1; i < charRange; i++)
		count[i] += count[i - 1];

	for (int i = array.size() - 1; i >= 0; i--) {
		int index = array[i].size() >= place ? static_cast<unsigned char>(array[i][array[i].size() - place]) : 0;
		output[count[index] - 1] = array[i];
		count[index]--;
	}

	for (int i = 0; i < array.size(); i++)
		array[i] = output[i];
}

void radixSortString(vector<string>& array, int maxSize) {
	for (int place = 1; place <= maxSize; place++)
		countingSortString(array, place);
}


int main()
{
	//SKIRTINGAS AIBES DYDIS

	cout << "Generuojami duomenys..." << endl;
	cout << "-----------------------------------------------------" << endl;
	for (int i = 0; i < 5; i++)
	{
		generateData(dataSize[i], 1, 8);
	}
	cout << "Duomenys sugeneruoti" << endl;
	cout << "-----------------------------------------------------" << endl;
	vector<int> temp_data;
	for (int i = 0; i < 5; i++)
	{
		cout << "Rusiuojamu duomenu aibes kiekis: " << dataSize[i] << endl;
		cout << "-----------------------------------------------------" << endl;
		float averageRadix = 0;
		for (int j = 0; j < 6; j++)
		{
			string file_name = "data_8_" + to_string(dataSize[i]) + ".txt";
			ifstream input_file(file_name);
			int temp;
			while (input_file >> temp)
			{
				temp_data.push_back(temp);
			}
			input_file.close();
			cout << j + 1 << " testo laikas: " << endl;
			auto start1 = std::chrono::high_resolution_clock::now();
			radixSort(temp_data, pow(10, 8));
			auto stop1 = std::chrono::high_resolution_clock::now();
			auto durationRadix = std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1);
			cout << durationRadix.count() << " ms" << endl;
			cout << "----------------------------------------------------" << endl;
			averageRadix = averageRadix + durationRadix.count();
			temp_data.clear();  // isvalome vektoriu
		}
		cout << endl << "Vidutinis laikas: " << averageRadix / 6 << " ms" << endl << endl;
		cout << "-----------------------------------------------------" << endl;
	}

	//SKIRTINGAS SKAITMENU KIEKIS

	cout << "Generuojami duomenys..." << endl;
	cout << "-----------------------------------------------------" << endl;
	generateData(dataSize[1], 2, 2);
	generateData(dataSize[1], 4, 4);
	generateData(dataSize[1], 8, 8);
	cout << "Duomenys sugeneruoti" << endl;
	cout << "-----------------------------------------------------" << endl;
	//random skaiciu rusiavimas
	for (int i = 0; i < 3; i++)
	{
		cout << "Rusiuojamu duomenu skaitmenu kiekis: " << pow(2, i + 1) << endl;
		cout << "-----------------------------------------------------" << endl;
		float averageRadix = 0;
		for (int j = 0; j < 6; j++)
		{
			string file_name = "data_" + to_string(int(pow(2, i + 1))) + "_" + to_string(dataSize[1]) + ".txt";
			ifstream input_file(file_name);
			int temp;
			while (input_file >> temp)
			{
				temp_data.push_back(temp);
			}
			input_file.close();
			cout << j + 1 << " testo laikas: " << endl;
			auto start1 = std::chrono::high_resolution_clock::now();
			radixSort(temp_data, pow(10, pow(2, i + 1)));
			auto stop1 = std::chrono::high_resolution_clock::now();
			auto durationRadix = std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1);
			cout << durationRadix.count() << " ms" << endl;
			cout << "----------------------------------------------------" << endl;
			averageRadix = averageRadix + durationRadix.count();
			temp_data.clear();  // isvalome vektoriu
		}
		cout << endl << "Vidutinis laikas: " << averageRadix / 6 << " ms" << endl << endl;
		cout << "-----------------------------------------------------" << endl;
	}


	//STRING RUSIAVIMAS SKIRTINGU AIBES DYDZIU
	cout << "Generuojami duomenys..." << endl;
	cout << "-----------------------------------------------------" << endl;
	for (int i = 0; i < 5; i++)
	{
		generateStringData(dataSize[i], 1, 8);
	}
	cout << "Duomenys sugeneruoti" << endl;
	cout << "-----------------------------------------------------" << endl;
	vector<string> temp_data_string;
	for (int i = 0; i < 5; i++)
	{
		cout << "Rusiuojamu string duomenu aibes kiekis: " << dataSize[i] << endl;
		cout << "-----------------------------------------------------" << endl;
		float averageRadix = 0;
		for (int j = 0; j < 6; j++)
		{
			string file_name = "data_8_" + to_string(dataSize[i]) + "_string.txt";
			ifstream input_file(file_name);
			string temp_string;
			while (input_file >> temp_string)
			{
				temp_data_string.push_back(temp_string);
			}
			input_file.close();
			cout << j + 1 << " testo laikas: " << endl;
			auto start1 = std::chrono::high_resolution_clock::now();
			radixSortString(temp_data_string, 8);
			auto stop1 = std::chrono::high_resolution_clock::now();
			auto durationRadix = std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1);
			cout << durationRadix.count() << " ms" << endl;
			cout << "----------------------------------------------------" << endl;
			averageRadix = averageRadix + durationRadix.count();
			temp_data_string.clear();
		}
		cout << endl << "Vidutinis laikas: " << averageRadix / 6 << " ms" << endl << endl;
		cout << "-----------------------------------------------------" << endl;
	}
	//SKIRTINGAS SKAITMENU KIEKIS

	cout << "Generuojami duomenys..." << endl;
	cout << "-----------------------------------------------------" << endl;
	generateStringData(dataSize[1], 2, 2);
	generateStringData(dataSize[1], 4, 4);
	generateStringData(dataSize[1], 8, 8);
	cout << "Duomenys sugeneruoti" << endl;
	cout << "-----------------------------------------------------" << endl;
	for (int i = 0; i < 3; i++)
	{
		cout << "Rusiuojamu duomenu simboliu kiekis: " << pow(2, i + 1) << endl;
		cout << "-----------------------------------------------------" << endl;
		float averageRadix = 0;
		for (int j = 0; j < 6; j++)
		{
			string file_name = "data_" + to_string(int(pow(2, i + 1))) + "_" + to_string(dataSize[1]) + "_string.txt";
			ifstream input_file(file_name);
			string temp_string;
			while (input_file >> temp_string)
			{
				temp_data_string.push_back(temp_string);
			}
			input_file.close();
			cout << j + 1 << " testo laikas: " << endl;
			auto start1 = std::chrono::high_resolution_clock::now();
			radixSortString(temp_data_string, pow(2, i + 1));
			auto stop1 = std::chrono::high_resolution_clock::now();
			auto durationRadix = std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1);
			cout << durationRadix.count() << " ms" << endl;
			cout << "----------------------------------------------------" << endl;
			averageRadix = averageRadix + durationRadix.count();
			temp_data_string.clear();
		}
		cout << endl << "Vidutinis laikas: " << averageRadix / 6 << " ms" << endl << endl;
		cout << "-----------------------------------------------------" << endl;
	}
}