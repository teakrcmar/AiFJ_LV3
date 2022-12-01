#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Source.h"
using namespace std;

bool in_array(const std::string& value, const std::vector<std::string>& array)
{
	return std::find(array.begin(), array.end(), value) != array.end();
}

string findingAllWords(vector<string> words, string type) {
	string line;
	vector<string> used(words.size());
	int allCounter = 0;
	for (int i = 0; i < words.size(); i++) {
		if (!words[i].empty())
			allCounter++;
	}
	line.append("- ");
	line.append(type);
	line.append("[");
	line.append(to_string(allCounter));
	line.append("]: ");

	for (int i = 0; i < words.size(); i++) {
		int counter = 1;
		string s = words[i];
		if (!in_array(s, used)) {
			for (int j = i + 1; j < words.size(); j++) {
				if (words[i] == words[j]) {
					counter++;
				}
			}
			line.append(s).append(" [").append(to_string(counter)).append("], ");
		}
		used.push_back(s);
	}
	return line;
}

bool isNumber(const string& str)
{
	for (char const& c : str) {
		if (std::isdigit(c) == 0) return false;
	}
	return true;
}

int main() {
	ifstream inputFile;
	ofstream outputFile;
	inputFile.open("input.txt");
	outputFile.open("output.txt", ios::trunc);
	vector<string> myKeywords = { "break", "case", "char", "const", "default", "do", "double", "else", "float", "for", "if", "int", "long", "return", "short", "switch", "unsigned", "void", "volatile", "while" };
	vector<string> myOperators = { "+", "-", "*", "/", "%", "<", ">", "<=", ">=", "==", "!=", "!","&&", "||", "=" };
	vector<string> mySeparators = { ",", ":", ";", " " };

	if (inputFile.is_open()) {
		string line;
		int counter = 0;
		vector<string> lines;
		while (getline(inputFile, line)) {

			if (line != "")
				lines.push_back(line);
		}
		inputFile.close();

		string delimiter = " ";
		vector<vector<string>> splitLines(lines.size());
		size_t pos = 0;
		string token;
		string delimiter2 = "	";
		size_t pos2 = 0;

		for (int i = 0; i < lines.size(); i++) {
			string s = lines[i];
			if (s.rfind("	", 0) == 0) {
				pos2 = s.find(delimiter2);
				if (pos2 != std::string::npos)
				{
					// If found then erase it from string
					s.erase(pos2, delimiter2.length());
				}
			}
			while ((pos = s.find(delimiter)) != std::string::npos) {
				token = s.substr(0, pos);
				splitLines[i].push_back(token);
				s.erase(0, pos + delimiter.length());
			}
			splitLines[i].push_back(s);
		}

		vector<string> outputText(splitLines.size());

		vector<string> all_separators(outputText.size());
		vector<string> all_operators(outputText.size());
		vector<string> all_keywords(outputText.size());
		vector<string> all_constants(outputText.size());
		vector<string> all_comments(outputText.size());
		vector<string> all_identifiers(outputText.size());
		vector<string> all_unknown(outputText.size());

		for (int i = 0; i < splitLines.size(); i++)
		{
			string line;
			string outputline;
			for (std::vector<std::string>::const_iterator ii = splitLines[i].begin(); ii != splitLines[i].end(); ++ii)
				line += *ii + " ";
			outputline = "line " + to_string(i + 1) + ": " + line;
			if (outputline != " " || outputline != "\n")
				outputText.push_back(outputline);
			for (int j = 0; j < splitLines[i].size(); j++)
			{
				if (in_array(splitLines[i][j], myKeywords))
				{
					outputline = "('" + splitLines[i][j] + "', keyword)";
					outputText.push_back(outputline);
					all_keywords.push_back(splitLines[i][j]);
				}
				else if (in_array(splitLines[i][j], myOperators))
				{
					outputline = "('" + splitLines[i][j] + "', operator)";
					outputText.push_back(outputline);
					all_operators.push_back(splitLines[i][j]);
				}
				else if (in_array(splitLines[i][j], mySeparators))
				{
					outputline = "('" + splitLines[i][j] + "', separator)";
					outputText.push_back(outputline);
					all_separators.push_back(splitLines[i][j]);
				}
				else if (splitLines[i][j].rfind("//", 0) == 0)
				{
					string commentLine;
					int skipCounter = 0;
					for (std::vector<std::string>::const_iterator ii = splitLines[i].begin(); ii != splitLines[i].end(); ++ii) {
						commentLine += *ii + " ";
						skipCounter++;
					}
					outputline = "('" + commentLine + "', comment)";
					outputText.push_back(outputline);
					all_comments.push_back(commentLine);
					j = j + skipCounter - 1;
				}
				else if (splitLines[i][j].rfind("#define", 0) == 0)
				{
					string constLine;
					int skipCounter = 0;
					for (std::vector<std::string>::const_iterator ii = splitLines[i].begin(); ii != splitLines[i].end(); ++ii) {
						constLine += *ii + " ";
						skipCounter++;
					}
					outputline = "('" + constLine + "', constant)";
					outputText.push_back(outputline);
					all_constants.push_back(constLine);
					j = j + skipCounter - 1;
				}
				else if (!in_array(splitLines[i][j], myOperators)) {
					if (!in_array(splitLines[i][j], myKeywords)) {
						if (!in_array(splitLines[i][j], mySeparators)) {
							if (splitLines[i][j].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890") != std::string::npos || isNumber(splitLines[i][j]))
							{
								outputline = "('" + splitLines[i][j] + "', unknown)";
								outputText.push_back(outputline);
								all_unknown.push_back(splitLines[i][j]);
							}
							else {
								outputline = "('" + splitLines[i][j] + "', identifier)";
								outputText.push_back(outputline);
								all_identifiers.push_back(splitLines[i][j]);
							}
						}
					}
				}
			}
			outputText.push_back("\n");

		}
		outputText.push_back("\n");
		outputText.push_back("\n");
		outputText.push_back(findingAllWords(all_separators, "separators"));
		outputText.push_back("\n");
		outputText.push_back(findingAllWords(all_comments, "comments"));
		outputText.push_back("\n");
		outputText.push_back(findingAllWords(all_identifiers, "identifiers"));
		outputText.push_back("\n");
		outputText.push_back(findingAllWords(all_keywords, "keywords"));
		outputText.push_back("\n");
		outputText.push_back(findingAllWords(all_operators, "operators"));
		outputText.push_back("\n");
		outputText.push_back(findingAllWords(all_constants, "constants"));
		outputText.push_back("\n");
		outputText.push_back(findingAllWords(all_unknown, "unknown"));


		for (int i = 0; i < outputText.size(); i++)
		{
			if (!outputText[i].length() == 0) {
				cout << outputText[i] << " ";
				cout << endl;
			}
		}

		if (outputFile.is_open()) {
			for (int i = 0; i < outputText.size(); i++)
			{
				if (!outputText[i].length() == 0) {
					outputFile << outputText[i] << " ";
					outputFile << endl;
				}
			}
			outputFile.close();
		}
	}

	return 0;
}

