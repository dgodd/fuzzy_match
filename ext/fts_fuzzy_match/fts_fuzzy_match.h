// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// VERSION
//   0.2.0  (2017-02-18)  Scored matches perform exhaustive search for best score
//   0.1.0  (2016-03-28)  Initial release
//
// AUTHOR
//   Forrest Smith
//
// NOTES
//   Compiling
//     You MUST add '#define FTS_FUZZY_MATCH_IMPLEMENTATION' before including this header in ONE source file to create implementation.
//
//   fts_fuzzy_match_simple(...)
//     Simplified version of fts_fuzzy_match
//
//   fts_fuzzy_match(...)
//     Returns true if pattern is found AND calculates a score.
//     Performs exhaustive search via recursion to find all possible matches and match with highest score.
//     Scores values have no intrinsic meaning. Possible score range is not normalized and varies with pattern.
//     Recursion is limited internally (default=10) to prevent degenerate cases (pattern="aaaaaa" str="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")
//     Uses uint8_t for match indices. Therefore patterns are limited to 256 characters.
//     Score system should be tuned for YOUR use case. Words, sentences, file names, or method names all prefer different tuning.


#ifndef FTS_FUZZY_MATCH_H
#define FTS_FUZZY_MATCH_H


#include <ctype.h> // tolower, toupper

// Public interface
struct FtsConfig {
    int sequential_bonus;            // bonus for adjacent matches (DEFAULT: 15)
    int separator_bonus;             // bonus if match occurs after a separator
    int camel_bonus;                  // bonus if match is uppercase and prev is lower (DEFAULT: 30)
    int first_letter_bonus;          // bonus if the first letter is matched

    int leading_letter_penalty;      // penalty applied for every letter in str before the first match
    int max_leading_letter_penalty; // maximum penalty for leading letters
    int unmatched_letter_penalty;    // penalty for every letter that doesn't matter
    int string_length_penalty;       // (DEFAULT: 0)
};

struct FtsConfig fts_default_config() {
    struct FtsConfig config = {
        .sequential_bonus = 20,
        .separator_bonus = 30,
        .camel_bonus = 0,
        .first_letter_bonus = 15,
        .leading_letter_penalty = -5,
        .max_leading_letter_penalty = -15,
        .unmatched_letter_penalty = -1,
        .string_length_penalty = -1
    };
    return config;
}

static bool fts_fuzzy_match_simple(char const * pattern, char const * str, struct FtsConfig const * config, int * outScore);
static bool fts_fuzzy_match(char const * pattern, char const * str, struct FtsConfig const * config, int * outScore, uint8_t * matches, int maxMatches);

#ifdef FTS_FUZZY_MATCH_IMPLEMENTATION

// Private interface
static bool fts_fuzzy_match_recursive(const char * pattern, const char * str, struct FtsConfig const * config, int * outScore,
    const char * strBegin, uint8_t const * srcMatches, uint8_t * matches, int maxMatches,
    int nextMatch, int * recursionCount, int recursionLimit);

static bool fts_fuzzy_match_simple(char const * pattern, char const * str, struct FtsConfig const * config, int * outScore) {
    uint8_t matches[256];
    return fts_fuzzy_match(pattern, str, config, outScore, matches, sizeof(matches));
}

static bool fts_fuzzy_match(char const * pattern, char const * str, struct FtsConfig const * config, int * outScore, uint8_t * matches, int maxMatches) {
    int recursionCount = 0;
    int recursionLimit = 10;

    return fts_fuzzy_match_recursive(pattern, str, config, outScore, str, NULL, matches, maxMatches, 0, &recursionCount, recursionLimit);
}

// Private implementation
static bool fts_fuzzy_match_recursive(const char * pattern, const char * str, struct FtsConfig const * config, int * outScore,
    const char * strBegin, uint8_t const * srcMatches, uint8_t * matches, int maxMatches,
    int nextMatch, int * recursionCount, int recursionLimit)
{
    // Count recursions
    ++*recursionCount;
    if (*recursionCount >= recursionLimit)
        return false;

    // Detect end of strings
    if (*pattern == '\0' || *str == '\0')
        return false;

    unsigned long stringLength = strlen(str);

    // Recursion params
    bool recursiveMatch = false;
    uint8_t bestRecursiveMatches[256];
    int bestRecursiveScore = 0;

    // Loop through pattern and str looking for a match
    bool first_match = true;
    while (*pattern != '\0' && *str != '\0') {

        // Found match
        if (tolower(*pattern) == tolower(*str)) {

            // Supplied matches buffer was too short
            if (nextMatch >= maxMatches)
                return false;

            // "Copy-on-Write" srcMatches into matches
            if (first_match && srcMatches) {
                memcpy(matches, srcMatches, nextMatch);
                first_match = false;
            }

            // Recursive call that "skips" this match
            uint8_t recursiveMatches[256];
            int recursiveScore;
            if (fts_fuzzy_match_recursive(pattern, str + 1, config, &recursiveScore, strBegin, matches, recursiveMatches, sizeof(recursiveMatches), nextMatch, recursionCount, recursionLimit)) {

                // Pick best recursive score
                if (!recursiveMatch || recursiveScore > bestRecursiveScore) {
                    memcpy(bestRecursiveMatches, recursiveMatches, 256);
                    bestRecursiveScore = recursiveScore;
                }
                recursiveMatch = true;
            }

            // Advance
            matches[nextMatch++] = (uint8_t)(str - strBegin);
            ++pattern;
        }
        ++str;
    }

    // Determine if full pattern was matched
    bool matched = *pattern == '\0' ? true : false;

    // Calculate score
    if (matched) {
        // Iterate str to end
        while (*str != '\0')
            ++str;

        // Initialize score
        *outScore = 100;

        // Apply length penalty
        *outScore += stringLength * config->string_length_penalty;

        // Apply leading letter penalty
        int penalty = config->leading_letter_penalty * matches[0];
        if (penalty < config->max_leading_letter_penalty)
            penalty = config->max_leading_letter_penalty;
        *outScore += penalty;

        // Apply unmatched penalty
        int unmatched = (int)(str - strBegin) - nextMatch;
        *outScore += config->unmatched_letter_penalty * unmatched;

        // Apply ordering bonuses
        for (int i = 0; i < nextMatch; ++i) {
            uint8_t currIdx = matches[i];

            if (i > 0) {
                uint8_t prevIdx = matches[i - 1];

                // Sequential
                if (currIdx == (prevIdx + 1))
                    *outScore += config->sequential_bonus;
            }

            // Check for bonuses based on neighbor character value
            if (currIdx > 0) {
                // Camel case
                char neighbor = strBegin[currIdx - 1];
                char curr = strBegin[currIdx];
                if (islower(neighbor) && isupper(curr))
                    *outScore += config->camel_bonus;

                // Separator
                bool neighborSeparator = neighbor == '_' || neighbor == ' ';
                if (neighborSeparator)
                    *outScore += config->separator_bonus;
            }
            else {
                // First letter
                *outScore += config->first_letter_bonus;
            }
        }
    }

    // Return best result
    if (recursiveMatch && (!matched || bestRecursiveScore > *outScore)) {
        // Recursive score is better than "this"
        memcpy(matches, bestRecursiveMatches, maxMatches);
        *outScore = bestRecursiveScore;
        return true;
    }
    else if (matched) {
        // "this" score is better than recursive
        return true;
    }
    else {
        // no match
        return false;
    }
}

#endif // FTS_FUZZY_MATCH_IMPLEMENTATION

#endif // FTS_FUZZY_MATCH_H
