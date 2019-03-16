// InvertedIndex.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "dirent.h"
#include <vector>
#include <sstream>
#include <string>
#include <list>
#include <fstream>
#include <stack>

using std::vector;
using std::string;
using std::list;
using std::fstream;
using std::istringstream;
using std::stack;

struct Node {
	string word;
	list<int> indices;
};


class index {
public:
	index(const char * path = NULL) {
		if (path == NULL) return;
		openDir(path);
	}
	void openDir(const char * path) {
		this->path = path;
		this->path += "\\";
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir(path)) != NULL) {
			while ((ent = readdir(dir)) != NULL) {
				files.push_back(ent->d_name);
			}
			closedir(dir);
		}
		files.erase(files.begin());
		files.erase(files.begin());
		parse();
		
		
	}
	void listAll() const {
		auto iter = library.cbegin();
		for (iter; iter != library.cend(); ++iter) {
			std::cout << (*iter).word << " -> ";
			for (auto iter2 = (*iter).indices.cbegin(); iter2 != (*iter).indices.cend(); ++iter2) {
				std::cout << files[*iter2 - 1] << ", ";
			}
			std::cout << std::endl;
		}
	}
	void search(const string & word) {
		int index = binarySearch(library, 0, library.size() - 1, word);
		if (index == -1) {
			std::cout << "No results found" << std::endl;
			return;
		}
		std::cout <<  word << " -> ";
		printFiles(library[index].indices);
	}
	void advancedSearch(const string & query) {
		string word;
		istringstream str(query);
		stack<list<int>> values;
		stack<char> operators;
		while (str >> word) {
			if (word == "(") {
				operators.push(word[0]);
			}
			else if (word == ")")
			{
				while (!operators.empty() && operators.top() != '(')
				{
					list<int> val2 = values.top();
					values.pop();

					list<int> val1 = values.top();
					values.pop();

					char op = operators.top();
					operators.pop();

					values.push(applyOper(val1, val2, op));
				}

				// pop opening brace. 
				operators.pop();
			}
			else if (word.size() >= 1 && word != "*" && word != "+" && word != "-") {
				list<int> lst;
				int index = binarySearch(library, 0, library.size() - 1, word);
				lst = library[index].indices;
				values.push(lst);
				lst.clear();
			}
			else {
				while (!operators.empty() && weight(operators.top()) >= weight(word[0])) {
					list<int> val2 = values.top();
					values.pop();
					list<int> val1 = values.top();
					values.pop();
					char op = operators.top();
					operators.pop();
					values.push(applyOper(val1, val2, op));
				}
				operators.push(word[0]);
			}
		}
		while (!operators.empty()) {
			list<int> val2 = values.top();
			values.pop();
			list<int> val1 = values.top();
			values.pop();
			char op = operators.top();
			operators.pop();
			values.push(applyOper(val1, val2, op));
		}
		if (values.top().size() == 0) {
			std::cout << query << " -> No results found" << std::endl;
			return;
		}
		std::cout << query << " -> ";
		printFiles(values.top());
		std::cout << std::endl;
	}

private:
	string path;
	vector<string> files;
	vector<Node> library;
	

	void toLowerCase(string & word) {
		auto iter = word.begin();
		while (iter != word.end()) {
			if (*iter <= 'Z' && *iter >= 'A')
				*iter = *iter - ('Z' - 'z');
			++iter;
		}
	}
	int binarySearch(const vector<Node> & arr, int left, int right, string word) {
		if (right > 1)
			if (arr[right].word == word) {
				return right;
			}
		if (right >= left) {
			int mid = left + (right - left) / 2;
			if (arr[mid].word == word)
				return mid;
			if (arr[mid].word > word)
				return binarySearch(arr, left, mid - 1, word);
			return binarySearch(arr, mid + 1, right, word);
		}
		return -1;
	}
	int binaryInsert(const vector<Node> & arr, int left, int right, string word) {
		if (arr.size() == 1) {
			if (arr[0].word > word)
				return 0;
			else return 1;
		}
		if (right >= left) {
			int mid = left + (right - left) / 2;
			if (mid == right) {
				if (word > arr[mid].word)
					return mid + 1;
				else return mid;
			}
			if (word > arr[mid].word && word < arr[mid + 1].word) {
				return mid + 1;
			}
			if (arr[mid].word > word)
				return binaryInsert(arr, left, mid - 1, word);
			return binaryInsert(arr, mid + 1, right, word);
		}
		return 0;
	}
	void parse() {
		auto iter = files.cbegin();
		int counter = 1;
		bool flag = false;
		string word;
		Node elem;
		elem.indices.push_back(counter);
		while (iter != files.cend()) {
			fstream file(this->path + *iter);
			if (flag) {
				elem.indices.pop_front();
				elem.indices.push_back(counter);
				flag = false;
			}
			while (file >> word) {
				toLowerCase(word);
				if (library.size() == 0) {
					elem.word = word;
					library.push_back(elem);
				}
				else {
					int isInVector = binarySearch(library, 0, library.size() - 1, word);
					if (isInVector == -1) {
						int position = binaryInsert(library, 0, library.size() - 1, word);
						elem.word = word;
						if (position == 0)
							library.insert(library.begin(), elem);
						else {
							auto it = library.begin();
							it = it + position;
							library.insert(it, elem);
						}
					}
					else {
						if (library[isInVector].indices.back() != counter)
							library[isInVector].indices.push_back(counter);
					}
				}
			}
			++counter;
			++iter;
			file.close();
			flag = true;
		}
	}
	int weight(char op) {
		if (op == '+' || op == '-')
			return 1;
		if (op == '*')
			return 2;
		return 0;
	}
	list<int> applyOper(list<int> a, list<int> b, char op) {
		list<int> copy = a;
		copy.merge(b);
		if (op == '-') {
			auto it = copy.begin();
			int last = *it;
			size_t counter = 1;
			++it;
			for (it; it != copy.end(); ++it) {
				if (*it == last)
					counter++;
				else {
					if (counter != 1) {
						copy.remove(last);
					}
					counter = 1;
					last = *it;
				}
			}
			auto iter1 = copy.end();
			--iter1;
			auto iter2 = iter1;
			--iter2;
			if (*iter1 == *iter2) {
				copy.remove(*iter1);
			}
		}
		if (op == '*') {
			auto it = copy.begin();
			int last = *it;
			size_t counter = 1;
			++it;
			for (it; it != copy.end(); ++it) {
				if (*it == last)
					counter++;
				else {
					if (counter == 1) {
						copy.remove(last);
					}
					counter = 1;
					last = *it;
				}
			}
			if (copy.size() % 2 == 1) {
				copy.pop_back();
			}
		}
		copy.unique();
		return copy;
	};
	void printFiles(const list<int> & indices) {
		int counter = 0;
		for (auto iter = indices.cbegin(); iter != indices.cend(); ++iter) {
			std::cout << files[*iter - 1];
			if (counter + 1 != indices.size()) {
				std::cout << ", ";
				++counter;
			}
		}
	}
};
int main()
{
	index lst("PATH TO FILES");
	string line;
	//gets a line until when give it an empty line
	while (getline(std::cin, line) && line.size() > 0)
	{
		lst.advancedSearch(line);
	}
	return 0;
}
