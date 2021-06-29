#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <algorithm>

#define MAXINT 1000

using namespace std;

int numberOfStates;
int numberOfLetters;
int counter = 0;
int numberOfTransitions = 0;
int i;
int size;
int tempCount = 0;
string line;
string **transitionTable;

vector<string> inputAlphabets;
vector<string> inputStates;
vector<string> inputFinalStates;
vector<string> inputInitialState;
vector<string> outputAlphabets;
vector<string> outputStates;
vector<string> outputFinalStates;
vector<string> outputInitialState;

ifstream inputfile;
ofstream outputfile;

void calculateStates(string);
string calculate(string state, string alphabet);

int main(void)
{
	inputfile.open("input.txt");
	outputfile.open("output.txt");

	//Initialize transitionTable
	transitionTable = new string *[MAXINT];
	for (i = 0; i < MAXINT;i++)
	{
		transitionTable[i] = new string[3];
	}

	// _ represents an unknown state which will become a trap state in the DFA
	for (int i = 0; i < MAXINT; i++)
	{
		transitionTable[i][2] = "_";
	}

	//Input file
	while (getline(inputfile, line))
	{
		//input the number of states and letters
		if (counter == 0)
		{
			stringstream ss(line);
			string token;
			int count2 = 0;
			while (getline(ss, token, ','))
			{
				if (count2 == 0)
				{
					numberOfStates = stoi(token);
				}
				else if (count2 == 1)
				{
					numberOfLetters = stoi(token);
				}
				count2++;
			}
		}
		//input states
		else if (counter == 1)
		{
			line.erase(0, 5);
			line.erase(line.end() - 1, line.end());
			stringstream ss(line);
			string token;
			while (getline(ss, token, ','))
			{
				inputStates.push_back(token);
			}
		}
		//input initial state
		else if (counter == 2)
		{
			line.erase(0, 5);
			line.erase(line.end() - 1, line.end());
			inputInitialState.push_back(line);
		}
		//input the list of final states
		else if (counter == 3)
		{
			line.erase(0, 5);
			line.erase(line.end() - 1, line.end());
			stringstream ss(line);
			string token;
			while (getline(ss, token, ','))
			{
				inputFinalStates.push_back(token);
			}
		}
		//input alphabets
		else if (counter == 4)
		{
			line.erase(0, 9);
			line.erase(line.end() - 1, line.end());
			stringstream ss(line);
			string token;
			int x = 0;
			while (getline(ss, token, ','))
			{
				inputAlphabets.push_back(token);
			}
		}
		//input delta
		else if (counter >= 5)
		{
			line.erase(0, 6);
			line.replace(4, 4, ",");
			stringstream ss(line);
			string token;
			int k = 0;
			if (numberOfTransitions == 0)
			{
				for (i = 0; i < numberOfStates; i++)
				{
					transitionTable[i][0] = inputStates[i];
					transitionTable[i][2] = inputStates[i];
					numberOfTransitions++;
				}
			}
			while (getline(ss, token, ','))
			{
				{
					transitionTable[numberOfTransitions][k] = token;
					k++;
				}
			}
			numberOfTransitions++;
		}
		counter++;
	}

	/*cout << "NFA Transition table" << endl;
	for(int i = 0; i < numberOfTransitions; i++){
		cout << transitionTable[i][0] << "\t" << transitionTable[i][1] << "\t" << transitionTable[i][2] << endl;
	}*/

	//Computation of the DFA
	string initialState = inputInitialState[0];
	string DFAInitialState = initialState;
	outputStates.push_back(DFAInitialState);
	outputInitialState.push_back(DFAInitialState);

	//Write to the output file
	size = 1;
	while (tempCount < size)
	{
		calculateStates(outputStates[tempCount++]);
		size = outputStates.size();
	}

	vector<string>::iterator i;
	for (i = outputStates.begin(); i != outputStates.end(); i++)
	{
		while (*i == "")
		{
			outputStates.erase(i);
			if (i == outputStates.end())
			{
				break;
			}
		}
		if (i == outputStates.end())
		{
			break;
		}
	}
	outputfile << "Q = {";

	for (i = outputStates.begin(); i != outputStates.end(); i++)
	{
		if (i != outputStates.end() - 1)
		{
			outputfile << *i << ",";
		}
		else
		{
			outputfile << *i;
		}
	}
	outputfile << "}" << endl;
	outputfile << "Init = {" << DFAInitialState << "}\n";
	outputfile << "F = {";

	for (i = outputStates.begin(); i != outputStates.end(); i++)
	{
		string temp1 = *i;
		string token;
		stringstream ss(temp1);
		while (getline(ss, token, '-'))
		{
			vector<string>::iterator j;
			for (j = inputFinalStates.begin(); j != inputFinalStates.end(); j++)
			{
				if (token == *j)
				{
					outputFinalStates.push_back(temp1);
				}
			}
		}
	}
	for (i = outputFinalStates.begin(); i != outputFinalStates.end(); i++)
	{
		if (i != outputFinalStates.end() - 1)
		{
			outputfile << *i << ",";
		}
		else
		{
			outputfile << *i;
		}
	}
	outputfile << "}" << endl;
	outputfile << "Sigma = {";
	for (int i = 0; i < numberOfLetters; i++)
	{
		if (i != numberOfLetters - 1)
		{
			outputfile << inputAlphabets[i] << ",";
		}
		else
		{
			outputfile << inputAlphabets[i];
		}
	}
	outputfile << "}" << endl;
	inputfile.close();
	string outputStateTable[outputStates.size()][numberOfLetters + 1];
	for (int i = 0; i < outputStates.size(); i++)
	{
		for (int j = 0; j < numberOfLetters + 1; j++)
		{
			if (j == 0)
			{
				outputStateTable[i][j] = outputStates[i];
			}
			else
			{
				outputStateTable[i][j] = calculate(outputStateTable[i][0], inputAlphabets[j - 1]);
				if (outputStateTable[i][j] == "")
				{
					outputStateTable[i][j] = "_";
				}
			}
		}
	}

	for (i = outputStates.begin(); i != outputStates.end(); i++)
	{
		for (int j = 0; j < numberOfLetters; j++)
		{
			if (*i != "" && calculate(*i, inputAlphabets[j]) != "")
			{
				outputfile << "Delta(" << *i << ", " << inputAlphabets[j]
						   << ") = {" << calculate(*i, inputAlphabets[j]) << "}" << endl;
			}
		}
	}
	outputfile.close();
	return 0;
}

void calculateStates(string state)
{
	vector<string>::iterator j;

	for (int i = 0; i < numberOfLetters; i++)
	{
		string temp = calculate(state, inputAlphabets[i]);
		if (find(outputStates.begin(), outputStates.end(), temp) == outputStates.end())
		{
			outputStates.push_back(temp);
		}
	}
}

string calculate(string state, string alphabet)
{
	set<int> bla;
	if (state.find('-') == string::npos)
	{
		int count = 0;
		string temp = "";
		for (int i = 0; i < numberOfTransitions; i++)
		{
			if (transitionTable[i][0] == state && transitionTable[i][1] == alphabet && transitionTable[i][2] != "_")
			{
				bla.insert(stoi(transitionTable[i][2].substr(1, transitionTable[i][2].length() - 1)));
			}
		}
		set<int>::iterator it;
		for (it = bla.begin(); it != bla.end(); it++)
		{
			int f = *it;
			if (it == bla.begin())
			{
				temp += "q" + to_string(f);
			}
			else
			{
				temp += "-q" + to_string(f);
			}
		}
		return temp;
	}
	else
	{
		string temp = "";
		int count = 0;
		stringstream ss(state);
		string token;
		while (getline(ss, token, '-'))
		{
			for (int i = 0; i < numberOfTransitions; i++)
			{
				if (transitionTable[i][0] == token && transitionTable[i][1] == alphabet && transitionTable[i][2] != "_")
				{
					bla.insert(stoi(transitionTable[i][2].substr(1, transitionTable[i][2].length() - 1)));
				}
			}
		}
		set<int>::iterator it;
		for (it = bla.begin(); it != bla.end(); it++)
		{
			int f = *it;
			if (it == bla.begin())
			{
				temp += "q" + to_string(f);
			}
			else
			{
				temp += "-q" + to_string(f);
			}
		}
		return temp;
	}
}