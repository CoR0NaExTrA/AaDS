// 12. Имеется  текстовый   файл.    Разработать    программу,
// обеспечивающую   вставку   запятой  перед  словами  "который",
// "которая", "которое", "которые", если они не являются первым
// словом предложения(7).
// Трофимов Ярослав ПС-24
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
	std::cout << "Введите название входного файла: ";
	std::cin >> inputfilename;
	std::cout << "Введите название выходного файла: ";
	std::cin >> outputfilename;

	std::ifstream inputfile(inputfilename);
	if (!inputfile.is_open())
	{
		std::cout << "Не удалось открыть входной файл.";
		return 1;
	}

	std::ofstream outputfile(outputfilename);
	if (!outputfile.is_open())
	{
		std::cout << "Не удалось открыть выходной файл.";
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

				if ((words[0] == "который" || words[0] == "которая" || words[0] == "которое" || words[0] == "которые") && words.size() < 2)
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
					if (words[1] == "который" || words[1] == "которая" || words[1] == "которое" || words[1] == "которые") 
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