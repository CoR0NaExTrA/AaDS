#include <iostream>
#include <fstream>

bool isStartOfSentence(const std::string& word, const std::string& prevWord) {
    return (prevWord.empty() || prevWord.back() == '.' || prevWord.back() == '!' || prevWord.back() == '?');
}

bool isTargetWord(const std::string& word) {
    return (word == "который" || word == "которая" || word == "которое" || word == "которые");
}

int main() {
    
}

