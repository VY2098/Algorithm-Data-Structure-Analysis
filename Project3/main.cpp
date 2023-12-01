#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main()
{
	string statues[26];
	string slot[26];
	string input, temp;
	getline(cin, input);
	
	vector<string> words;

	size_t pos = 0;
	while ((pos = input.find(' ')) != string::npos)
	{
		temp = input.substr(0, pos);
		words.push_back(temp);
		input.erase(0, pos + 1);
	}
	words.push_back(input);


	fill(begin(statues), end(statues), "never used");

	for (int i = 0; i < words.size(); i++)
	{
		bool repeat = false;
		
		//Add
		if (words[i][0] == 'A')
		{
			words[i] = words[i].erase(0, 1);
			int check = 0;
			for (int j = 0; j < 26; j++)
			{
				if (words[i] == slot[j])
					repeat = true;
			}
			if (!repeat)
			{
				char c = words[i][words[i].length() - 1];
				for (int j = 0; j < 26; j++)
				{
					if (c - 97 + j <= 25)
					{
						if (slot[c - 97 + j].size() == 0)
						{
							slot[c - 97 + j] = words[i];
							statues[c - 97 + j] = "occupied";
							break;
						}
					}
					else
					{
						if (slot[check].size() == 0)
						{
							slot[check] = words[i];
							statues[check] = "occupied";
							break;
						}
						else
						{
							check++;
						}
					}
				}
			}
		}
		
		//Delete
		else if (words[i][0] == 'D')
		{
			words[i] = words[i].erase(0, 1);
			char a = words[i][words[i].size() - 1];
			for (int j = 0; j < 26; j++)
			{
				if (slot[j] == words[i])
				{
					slot[j] = "";
					statues[j] = "tombstone";
				}
			}
		}
	}
	for (int i = 0; i < 26; i++)
	{
		if (slot[i].size())
			cout << slot[i] << " ";
	}
	return 0;
}