#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here

/**
 * generates all possible strings with letters a-z for any non-fixed location
 */
void generate(
    size_t idx, 
    std::string in, // making it mutable
    const std::string& floating, 
    const std::set<std::string>& dict,
    std::set<std::string>& out 
);

/**
 * Checks if the in string includes all floating letters at least once
 */
bool hasAllFloating(const std::string& in, const std::string& floating);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // output set of all valid strings
    std::set<std::string> out;
    generate(0, in, floating, dict, out);

    return out;
}

// Define any helper functions here
void generate(
    size_t idx, 
    std::string in, // making it mutable
    const std::string& floating, 
    const std::set<std::string>& dict,
    std::set<std::string>& out 
)
{
    // Base Case : reached the end of in string
    if (idx == in.size()) { 
        if (hasAllFloating(in, floating) && dict.count(in)) {
            out.insert(in);
        }
        return;
    }

    // Recursive Case : step thorugh all configurations of a-z in "empty" spots
    if (in[idx] != '-') {   // spot already filled 
        generate(idx + 1, in, floating, dict, out);
    }
    else {  // spot is '-'
        for (char c = 'a'; c <= 'z'; ++c) {
            in[idx] = c; // fill spot
            generate(idx + 1, in, floating, dict, out); // go to next spot
            in[idx] = '-'; // reset the spot for the next letter
        }
    }
}

bool hasAllFloating(const std::string& in, const std::string& floating)
{
    std::map<char, int> flCount; // stores # occurence for each ch in floating 
    std::map<char, int> inCount; // stores # occurence for each ch in 'in'

    // feed floating chars and their count to map container
    for (size_t i = 0; i < floating.size(); ++i) {
        char c = floating[i];
        flCount[c]++;
    }

    // feed 'in' chars and their count to map container
    for (size_t i = 0; i < in.size(); ++i) {
        char c = in[i];
        inCount[c]++;
    }

    // loop through maximum of alphabet length,
    // return false if floating letters appear less in 'in' string
    for (char c = 'a'; c <= 'z'; ++c) {
        if (inCount[c] < flCount[c]) return false; // directly compare values
    }
    return true;
}
