// 12. �������  ���������   ����.    �����������    ���������,
// ��������������   �������   �������  �����  �������  "�������",
// "�������", "�������", "�������", ���� ��� �� �������� ������
// ������ �����������(7).
// �������� ������� ��-24
// IDE: Visual Studio 2022
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main() {
	setlocale(LC_ALL, "rus");

	std::string inputfilename, outputfilename;
	std::vector<std::string> words;
	std::cout << "������� �������� �������� �����: ";
	std::cin >> inputfilename;
	std::cout << "������� �������� ��������� �����: ";
	std::cin >> outputfilename;

	std::ifstream inputfile(inputfilename);
	if (!inputfile.is_open())
	{
		std::cout << "�� ������� ������� ������� ����.";
		return 1;
	}

	std::ofstream outputfile(outputfilename);
	if (!outputfile.is_open())
	{
		std::cout << "�� ������� ������� �������� ����.";
		return 1;
	}

	while (!inputfile.eof())
	{
		std::string line, word;
		bool isFirstLine = true;
		int counter;
		while (getline(inputfile, line))
		{
			counter = 0;
			while (line[counter] != '\0')
			{
				while (line[counter] != ' ' && line[counter] != '\0')
				{
					word += line[counter++];
				}
				if (line[counter] == ' ')
				{
					counter++;
				}

				words.push_back(word);
				word.clear();

				if (words.size() > 2) {
					words[0] = words[1];
					words[1] = words[2];
					words.pop_back();
				}

				if ((words[0] == "�������" || words[0] == "�������" || words[0] == "�������" || words[0] == "�������") && words.size() < 2)
				{
					outputfile << ',' << std::endl;
				}
				else if (words.size() < 2 && !isFirstLine)
				{
					outputfile << std::endl;
				}
				else {
					isFirstLine = false;
				}

				if (words.size() == 2)
				{
					if (words[1] == "�������" || words[1] == "�������" || words[1] == "�������" || words[1] == "�������") 
					{
						if (words[0].back() == ',')
						{
							outputfile << words[0] << ' ';
						}
						else 
						{
							outputfile << words[0] << ", ";
						}
                    }
                    else 
                    {
                        outputfile << words[0] << ' ';
                    }
				}
			}

			if (words.size() < 2)
			{
				outputfile << words[0] << std::endl;
				words.clear();
				break;
			}
			outputfile << words[1];
			words.clear();
		}
	}

	inputfile.close();
	outputfile.close();

	return 0;
}