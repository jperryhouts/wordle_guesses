#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const uint8_t GREY = 0;
const uint8_t YELLOW = 1;
const uint8_t GREEN = 2;

inline
bool
contains(const vector<char> &word, char c) {
    return (word[0] == c || word[1] == c || word[2] == c || word[3] == c || word[4] == c);
}

inline
string
word_to_str(const vector<char> &word) {
    string ret{word[0], word[1], word[2], word[3], word[4]};
    return ret;
}

int
load_dict(const string fname, vector<vector<char>> &words)
{
    fstream dict_file;
    dict_file.open(fname, ios::in);
    if (dict_file.is_open()) {
        string line;
        while (getline(dict_file, line)) {
            if (line.size() == 5) {
                vector<char> tmp{
                    line.at(0),
                    line.at(1),
                    line.at(2),
                    line.at(3),
                    line.at(4)
                };
                words.push_back(tmp);
            }
        }
        dict_file.close();
        return 0;
    } else {
        cerr << "Unable to open file: " << fname << endl;
    }
    return 1;
}

vector<uint8_t>
get_hints(const vector<char> guess,
          const vector<char> answer)
{
    vector<uint8_t> hints(5, GREY);
    for (int i=0; i<5; ++i) {
        if (guess[i] == answer[i])
            hints[i] = GREEN;
        else if (contains(answer, guess[i]))
            hints[i] = YELLOW;
    }
    return hints;
}

int
n_valid_next_guesses(const vector<char> &first_guess,
                     const vector<char> &answer,
                     const vector<vector<char>> &guessable)
{
    const vector<uint8_t> hints = get_hints(first_guess, answer);
    int n_valid = 0;
    for (int i=0; i<guessable.size(); ++i)
    {
        vector<char> next_guess = guessable[i];
        bool word_is_valid = true;
        for (int i=0; i<5; ++i)
        {
            switch (hints[i]) {
                case GREEN:
                    word_is_valid &= (first_guess[i] == next_guess[i]);
                    break;
                case YELLOW:
                    word_is_valid &= (first_guess[i] != next_guess[i]);
                    word_is_valid &= (contains(next_guess, first_guess[i]));
                    break;
                case GREY:
                default:
                    word_is_valid &= !contains(next_guess, first_guess[i]);
            }

            if (!word_is_valid)
                break;
        }
        if (word_is_valid)
            n_valid++;
    }
    return n_valid;
}

int
parse_args(int argc,
           char** argv,
           int &start_guess,
           int &end_guess,
           string &guesses_file,
           string &answers_file)
{
    start_guess=0;
    end_guess=10000;
    guesses_file="dictionary.txt";
    answers_file="dictionary.txt";
    for (int i=1; i<argc; ++i)
    {
        string arg(argv[i]);
        if (arg == "--help")
        {
            cout << "Usage: " << argv[0] << " [OPTIONS] [DICTIONARY]"
                 << endl
                 << endl
                 << "Options:"
                 << endl
                 << "  --start start_guess (default 0)"
                 << endl
                 << "  --end end_guess (default to all guessable words)"
                 << endl
                 << "  --answers answers_file"
                 << endl;
            return -1;
        }
        else if (arg == "--start")
            start_guess=atoi(argv[++i]);
        else if (arg == "--end")
            end_guess=atoi(argv[++i]);
        else if (arg == "--answers")
        {
            string s(argv[++i]);
            answers_file=s;
        }
        else
            guesses_file=arg;
    }
    return 0;
}

int
main (int argc, char** argv)
{
    int start_guess, end_guess;
    string guesses_file, answers_file;

    int success;
    success = parse_args(argc, argv,
                         start_guess, end_guess,
                         guesses_file, answers_file);
    if (success != 0)
        return success;

    vector<vector<char>> answers;
    success = load_dict(answers_file, answers);
    if (success != 0)
        return success;

    vector<vector<char>> guessable;
    success = load_dict(guesses_file, guessable);
    if (success != 0)
        return success;

    end_guess = min(end_guess, (int) guessable.size());

    cerr << "Guesses dictionary: " << guesses_file << endl;
    cerr << "Answers file: " << answers_file << endl;
    cerr << "Start/end position: "
         << start_guess << "->" << end_guess << endl
         << "-------------------------------" << endl;

    for (int guess=start_guess; guess<end_guess; ++guess)
    {
        float x = 0;
        for (int answer=0; answer<answers.size(); ++answer)
        {
            x += n_valid_next_guesses(
                guessable[guess],
                answers[answer],
                guessable
            ) / float(guessable.size());
        }
        cout << word_to_str(guessable[guess])
             << " "
             << 100 * (1.0 - (x / answers.size()))
             << endl;
    }

    return 0;
}
