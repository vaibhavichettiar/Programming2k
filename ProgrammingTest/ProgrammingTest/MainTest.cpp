// Copyright Mass Media. All rights reserved. DO NOT redistribute.

////////////////////////////////////////////////////////////////////////////////////////////////////
// Task List
////////////////////////////////////////////////////////////////////////////////////////////////////
// Notes
//	* This test requires a compiler with C++17 support and was built for Visual Studio 2017.
// 		* Tested on Linux (Ubuntu 20.04) with: g++ -Wall -Wextra -pthread -std=c++17 MainTest.cpp
//		* Tested on Mac OS Big Sur, 11.0.1 and latest XCode updates.
//	* Correct output for all three sorts is in the CorrectOutput folder. BeyondCompare is recommended for comparing output.
//	* Functions, classes, and algorithms can be added and changed as needed.
//	* DO NOT use std::sort().
// Objectives
//	* 20 points - Make the program produce a SingleAscending.txt file that matches CorrectOutput/SingleAscending.txt.
//	* 10 points - Make the program produce a SingleDescending.txt file that matches CorrectOutput/SingleDescending.txt.
//	* 10 points - Make the program produce a SingleLastLetter.txt file that matches CorrectOutput/SingleLastLetter.txt.
//	* 20 points - Write a brief report on what you found, what you did, and what other changes to the code you'd recommend.
//	* 10 points - Make the program produce three MultiXXX.txt files that match the equivalent files in CorrectOutput; it must be multi-threaded.
//	* 20 points - Improve performance as much as possible on both single-threaded and multi-threaded versions; speed is more important than memory usage.
//	* 10 points - Improve safety and stability; fix memory leaks and handle unexpected input and edge cases.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <ostream>
#include <fstream>
#include <thread>
#include <atomic>
#include <ctime>
#include <vector>
#include <experimental/filesystem>
const int MAX_CHAR = 26;

#ifndef INCLUDE_STD_FILESYSTEM_EXPERIMENTAL
#   if defined(__cpp_lib_filesystem)
#       define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 0
#   elif defined(__cpp_lib_experimental_filesystem)
#       define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1
#   elif !defined(__has_include)
#       define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1
#   elif __has_include(<filesystem>)
#       ifdef _MSC_VER
#           if __has_include(<yvals_core.h>)
#               include <yvals_core.h>
#               if defined(_HAS_CXX17) && _HAS_CXX17
#                   define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 0
#               endif
#           endif
#           ifndef INCLUDE_STD_FILESYSTEM_EXPERIMENTAL
#               define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1
#           endif
#       else
#           define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 0
#       endif
#   elif __has_include(<experimental/filesystem>)
#       define INCLUDE_STD_FILESYSTEM_EXPERIMENTAL 1
#   else
#       error Could not find system header "<filesystem>" or "<experimental/filesystem>"
#   endif
#   if INCLUDE_STD_FILESYSTEM_EXPERIMENTAL
#       include <experimental/filesystem>
     	namespace fs = std::experimental::filesystem;
#   else
#       include <filesystem>
#		if __APPLE__
			namespace fs = std::__fs::filesystem;
#		else
			namespace fs = std::filesystem;
#		endif
#   endif
#endif

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Definitions and Declarations
////////////////////////////////////////////////////////////////////////////////////////////////////
#define MULTITHREADED_ENABLED 0

enum class ESortType { AlphabeticalAscending, AlphabeticalDescending, LastLetterAscending };

std::string to_string(ESortType sortType) {
    switch (sortType) {
        case ESortType::AlphabeticalAscending:
            return "AlphabeticalAscending";
        case ESortType::AlphabeticalDescending:
            return "AlphabeticalDescending";
        case ESortType::LastLetterAscending:
            return "LastLetterAscending";
        default:
            return "Unknown";
    }
}


class IStringComparer {
public:
	virtual bool IsFirstAboveSecond(string _first, string _second) = 0;
};

class AlphabeticalAscendingStringComparer : public IStringComparer {
public:
	virtual bool IsFirstAboveSecond(string _first, string _second);
};
class TrieNode {
public:
    // Initialize your data structure here.
    bool is_word;
    int index;
	int countofwords = 0;
    TrieNode *children[26];
    
    TrieNode() {
        is_word = false;
        for (int i = 0; i < 26; i++)
            children[i] = NULL;
        index=-1;

    }
};

class Trie {
private:
public:
	TrieNode* root;
	int dummyCount = 0;
    /** Initialize your data structure here. */
    Trie() {
        root = new TrieNode();
    }
    
    /** Inserts a word into the trie. */
    void insert(string word,int count) {
        TrieNode* node = root;
		cout<<"\n inserting lol lol ol: \n"<<word<<' '<<count;
        for (int i = 0; i < word.size(); i++) {
            cout<<" "<<word.size();
            int index = word[i] - 'a';
            
            if (node->children[index] == NULL) {
                node->children[index] = new TrieNode();
            }
            if(i+1 == word.size())
			{
				break;
			}
            node = node->children[index];
        }
        for(int i  =0;i<26;i++){

			if (root->children[i]!= NULL){
				cout<<" -- "<<char('a'+i);
			} else {
				cout<<" -- 0";
			}
		}
		cout<<endl<<"+++++++++++++++++++++++++++++++++++++"<<endl;
        node->is_word = true;
		node->countofwords+=1;
    }
    vector<string> getOrderedList(ESortType _sortType){
		vector<string> result;
		cout<<"\n\n-------------------------------------------------\n\n";
		if (_sortType == ESortType::LastLetterAscending) 
		{
			preorderHelper(root,"",result);
		}
		// else if
		// {(_sortType == ESortType::) preorderHelper(root,"",result);
		// }
		// else 
		// {preorderHelper(root,"",result);
		// }
		return result;
	}

    void preorderHelper(TrieNode* node, string word,vector<string>& result) {
        if (node == NULL) {
            return;
        }
        if (node->is_word) {
			int count = node->countofwords;
        	while (count > 0) { // add the word to the result vector count times
            	result.push_back(word);
            	count--;
				dummyCount += 1;
			}
        }
        for (int i = 0; i <26; i++) {
            if (node->children[i] != NULL) {
                char c = 'a'+i;
				string newWord = word + c; // append current character to word
            	// cout<<"\nCalling again: "<<newWord<<endl;
                preorderHelper(node->children[i],newWord,result);
            }
        }
    }
	void postorderHelper(TrieNode* node, string word,vector<string>& result) {
        if (node == NULL) {
            return;
        }
		if (node->is_word) {
			int count = node->countofwords;
        	while (count > 0) { // add the word to the result vector count times
            	result.push_back(word);
            	count--;
			}
        }
        for (int i = 25; i >=0; i--) {
            if (node->children[i] != NULL) {
                char c = 'a' + i;

                postorderHelper(node->children[i],word + c, result);
            }
        }
    }
	
};

void DoSingleThreaded(vector<string> _fileList, ESortType _sortType, string _outputName);
// void DoMultiThreaded(vector<string> _fileList, ESortType _sortType, string _outputName);
vector<string> ReadFile(string _fileName);
// void ThreadedReadFile(string _fileName, vector<string>* _listOut,mutex& _mtx);
vector<string> TriePrint(vector<string> _listToSort, ESortType _sortType);
// vector<string> BubbleSort(vector<string> _listToSort, ESortType _sortType);
void WriteAndPrintResults(const vector<string>& _masterStringList, string _outputName, int _clocksTaken);


////////////////////////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
	// cout <<"execution"<<endl;
	// Enumerate the directory for input files
	vector<string> fileList;
    string inputDirectoryPath = "/Users/vaibhavichettiar/Downloads/ProgrammingTest/InputFiles";
	// std::cout << inputDirectoryPath << endl;
    for (const auto & entry : fs::directory_iterator(inputDirectoryPath)) {
		// std::cout<<"entry:"<<entry.path()<<endl;
		if (!fs::is_directory(entry)) {
			// std::cout<<"--------"<<entry.path()<<endl;
			fileList.push_back(entry.path().string());	
		}
	}
	// Do the stuff
	// DoSingleThreaded(fileList, ESortType::AlphabeticalAscending,	"SingleAscending");
	// DoSingleThreaded(fileList, ESortType::AlphabeticalDescending,	"SingleDescending");
	DoSingleThreaded(fileList, ESortType::LastLetterAscending,	"SingleLastLetter");
#if MULTITHREADED_ENABLED
	// DoMultiThreaded(fileList, ESortType::AlphabeticalAscending,		"MultiAscending");
	// DoMultiThreaded(fileList, ESortType::AlphabeticalDescending,	"MultiDescending");
	// DoMultiThreaded(fileList, ESortType::LastLetterAscending,		"MultiLastLetter");
#endif

	// Wait
	cout << endl << "Finished...";
	getchar();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// The Stuff
////////////////////////////////////////////////////////////////////////////////////////////////////
void DoSingleThreaded(vector<string> _fileList, ESortType _sortType, string _outputName) {
	clock_t startTime = clock();
	vector<string> masterStringList;
	// cout <<"Hello"<<endl;
	for (unsigned int i = 0; i < _fileList.size(); ++i) {
	// for (unsigned int i = 2; i < 3; ++i) {
		// cout<<"\n\n-------\n filename:"<<_fileList[i]<<"\n\n---------\n\n";
		vector<string> fileStringList = ReadFile(_fileList[i]);

		for (unsigned int j = 0; j < fileStringList.size(); ++j) {
			masterStringList.push_back(fileStringList[j]);
		}

		
	}
	masterStringList = TriePrint(masterStringList,_sortType);
	cout << masterStringList.size()<<endl;
	// for (vector<string>::iterator i = masterStringList.begin(); i != masterStringList.end(); ++i)
	// 	{
    // 		cout << *i <<"  : " << endl;
	// 	}

	clock_t endTime = clock();

	WriteAndPrintResults(masterStringList, _outputName, endTime - startTime);
}

// void DoMultiThreaded(vector<string> _fileList, ESortType _sortType, string _outputName) {
// 	clock_t startTime = clock();
// 	vector<string> masterStringList;
// 	vector<thread> workerThreads(_fileList.size());
// 	mutex mtx;
// 	for (unsigned int i = 0; i < _fileList.size() ; ++i) {
// 		workerThreads[i] = thread(ThreadedReadFile, _fileList[i], &masterStringList,ref(mtx));
// 		cout << "Launched thread for file: " << _fileList[i] << endl;
// 	}
// 	for (unsigned int i = 0; i < _fileList.size(); ++i) {
// 		if (workerThreads[i].joinable())
// 		{
// 			workerThreads[i].join();
// 			cout << "Joined thread for file: " << _fileList[i] << endl;
// 		}
// 	}
	
// 	// workerThreads[workerThreads.size() - 1].join(); 
// 	cout << "Size of masterStringList: " << masterStringList.size() << endl;
	
// 	// masterStringList = BubbleSort(masterStringList, _sortType);
// 	clock_t endTime = clock();

// 	WriteAndPrintResults(masterStringList, _outputName, endTime - startTime);
// }

////////////////////////////////////////////////////////////////////////////////////////////////////
// File Processing
////////////////////////////////////////////////////////////////////////////////////////////////////
vector<string> ReadFile(string _fileName) {
	vector<string> listOut;
	streampos positionInFile = 0;
	bool endOfFile = false;
	int count = 0;
	ifstream fileIn(_fileName, ifstream::in);
	while (!endOfFile) {
		fileIn.clear();
		fileIn.seekg(positionInFile, ios::beg);

		string* tempString = new string();
		getline(fileIn, *tempString);
		count+=1;
		if (!endOfFile)
			listOut.push_back(*tempString);
		endOfFile = fileIn.peek() == EOF;
		positionInFile = endOfFile ? ios::beg : fileIn.tellg();

	}
	fileIn.close();
	return listOut; 
}

// void ThreadedReadFile(string _fileName, vector<string>* _listOut, mutex& _mtx) {
//     vector<string> fileContents = ReadFile(_fileName);
    
//     // Lock the mutex to protect the shared vector
//     _mtx.lock();
    
//     // Append the file contents to the shared vector
//     _listOut->insert(_listOut->end(), fileContents.begin(), fileContents.end());
    
//     // Unlock the mutex
//     _mtx.unlock();
// }

////////////////////////////////////////////////////////////////////////////////////////////////////
// Sorting
////////////////////////////////////////////////////////////////////////////////////////////////////
bool AlphabeticalAscendingStringComparer::IsFirstAboveSecond(string _first, string _second) {
	unsigned int i = 0;
	while (i < _first.length() && i < _second.length()) {
		if (_first[i] < _second[i])
			return true;
		else if (_first[i] > _second[i])
			return false;
		++i;
	}
	return (i == _second.length());
}

vector<string> TriePrint(vector<string> _listToSort, ESortType _sortType) {
    Trie trie;
	vector<string> sortedList;
	// cout <<"print trie words"<<endl;

	if (_sortType != ESortType::LastLetterAscending)
	{
		for (string& word : _listToSort) {
			transform(word.begin(), word.end(), word.begin(), ::tolower);
			//trie.insert(word);
		}
		sortedList = trie.getOrderedList(_sortType);
	
		for (string& word : sortedList) {
			word[0] = toupper(word[0]);
		}
	}
	else{
		int count =0;
		for (string& word : _listToSort) {
			transform(word.begin(), word.end(), word.begin(), ::tolower);
			reverse(word.begin(),word.end());
			//cout << word << endl;
			count += 1;
			trie.insert(word,count);
		}
		for(int i  =0;i<26;i++){

			if (trie.root->children[i]!= NULL){
				cout<<" -- "<<char('a'+i);
			} else {
				cout<<" -- 0";
			}
		}
		//cout<<"----- count "<<count<<"-------"<<endl;
		sortedList = trie.getOrderedList(_sortType);
		cout << "sortedList.siez():"<<sortedList.size()<<endl;
		cout<<"\ndummyCount is: "<<trie.dummyCount<<endl;
	}

	return sortedList;
	
}


// vector<string> BubbleSort(vector<string> _listToSort, ESortType _sortType) {
// 	AlphabeticalAscendingStringComparer* stringSorter = new AlphabeticalAscendingStringComparer();
// 	vector<string> sortedList = _listToSort;
// 	for (unsigned int i = 0; i < sortedList.size() - 1; ++i) {
// 		for (unsigned int j = 0; j < sortedList.size() - 1; ++j) {
// 			if (!stringSorter->IsFirstAboveSecond(sortedList[j], sortedList[j+1])) {
// 				string tempString = sortedList[j];
// 				sortedList[j] = sortedList[j+1];
// 				sortedList[j+1] = tempString;
// 			}
// 		}
// 	}
// 	return sortedList; 
// }

////////////////////////////////////////////////////////////////////////////////////////////////////
// Output
////////////////////////////////////////////////////////////////////////////////////////////////////
void WriteAndPrintResults(const vector<string>& _masterStringList, string _outputName, int _clocksTaken) {

	// for (auto i = _masterStringList.begin(); i != _masterStringList.end(); ++i)
	// 	{
    // 		cout << *i <<"   Master " <<endl;
	// 	}
	cout << endl << _outputName << "\t- Clocks Taken: " << _clocksTaken << endl;
	
	ofstream fileOut(_outputName + ".txt", ofstream::trunc);
	for (unsigned int i = 0; i < _masterStringList.size(); ++i) {
		fileOut << _masterStringList[i] << endl;
	}
	fileOut.close();
}
