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
    std::string floating, 
    const std::set<std::string>& dict,
    std::set<std::string>& out 
);

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
    std::string in, // mutable
    std::string floating, 
    const std::set<std::string>& dict,
    std::set<std::string>& out)
{
    if (idx == in.size()) {
        if (floating.empty() && dict.count(in)) {
            out.insert(in);
        }
        return;
    }

    size_t remaining_dashes = 0;
    for (size_t i = idx; i < in.size(); ++i) {
        if (in[i] == '-') remaining_dashes++;
    }
    if (floating.size() > remaining_dashes) return;

    if (in[idx] != '-') {
        generate(idx + 1, in, floating, dict, out);
    }
    else {
        std::set<char> tried;

        // Try all characters from floating
        for (size_t i = 0; i < floating.size(); ++i) {
            char c = floating[i];
            if (tried.count(c)) continue;
            tried.insert(c);

            in[idx] = c;
            std::string newFloating = floating;
            newFloating.erase(newFloating.find(c), 1);  // remove one instance
            generate(idx + 1, in, newFloating, dict, out);
        }

        // Try all other characters not in floating
        for (char c = 'a'; c <= 'z'; ++c) {
            if (floating.find(c) != std::string::npos) continue;
            if (tried.count(c)) continue;
            tried.insert(c);

            in[idx] = c;
            generate(idx + 1, in, floating, dict, out);
        }
    }
}
