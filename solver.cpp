#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <unordered_map>
#include <list>
#include <bits/stdc++.h>
#include <algorithm>
#include <random>
#include <vector>
#include <chrono>
#include <unordered_set>

using namespace std;
using namespace std::chrono;

void readfile(string const &filename);
void evaluate_words(int guesscount);
string sort(unordered_map<string, double> &M);
bool cmp(pair<string, double> &a,
         pair<string, double> &b);
int gameloop(string const &secret);
double autoplay();
void manual_game();

unordered_set<char> must_contain;
unordered_map<char, double[5]> freq;
unordered_map<char, unordered_map<char, double[5]>> preceeding_letter;
unordered_map<char, unordered_map<char, double[5]>> succeeding_letter;
string guesses[15];
string results[15][5] = {
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"}};
unordered_set<char> alphabet{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
unordered_map<string, double> scores;
list<string> all_words;
char gamemode;
int startindex = 0;
string startword;
string startwordactual;

void reset()
{
    must_contain.clear();
    alphabet = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    for (string const &word : all_words)
    {
        scores[word] = 0;
    }
    for (int row = 0; row < end(results) - begin(results); row++)
    {
        for (int col = 0; col < end(results[row]) - begin(results[row]); col++)
        {
            results[row][col] = "Nope";
        }
    };
}

int main()
{

    readfile("answers.txt");

    vector<string> v(all_words.cbegin(), all_words.cend());
    random_device rd;
    mt19937 generator(rd());
    shuffle(v.begin(), v.end(), generator);
    bool invalid_mode = true;
    int final_score;
    double startwordavg;
    string beststart = " ";
    double beststartavg = MAXFLOAT;

    while (invalid_mode)
    {
        cout << "Choose game mode: manual(m), automatic(a), or full game(f)" << endl;
        string input;
        cin >> input;
        gamemode = input[0];
        cin.ignore();
        switch (gamemode)
        {
        case 'a':
            cout << "Autosolving all words" << endl;
            for (int i = 0; i < all_words.size(); i++)
            {
                startwordavg = autoplay();
                startindex++;
                if (startwordavg < beststartavg)
                {
                    beststart = startwordactual;
                    beststartavg = startwordavg;
                }
                cout << "Solved with start word " << startwordactual << " with avg " << startwordavg << " guesses" << endl;

                cout << "Best starting word yet: " << beststart << " with an avg of " << beststartavg << endl;
            }

            invalid_mode = false;
            break;
        case 'm':
            manual_game();
            invalid_mode = false;
            break;

        case 'f':

            final_score = gameloop(v[0]);
            cout << "Congrats, solved " << v[0] << " in " << final_score << " guesses" << endl;
            invalid_mode = false;
            break;
        default:

            cout << gamemode << " is not a valid gamemode, try again" << endl;
            break;
        }
    }
}

double autoplay()
{
    double guesses_to_win = 0;
    int guessnmr;
    int maxguesses = 0;
    int currentwordnum = 0;
    int numofwords = all_words.size();
    string maxguessword;

    for (string const &secret : all_words)
    {
        currentwordnum++;
        reset();
        guessnmr = gameloop(secret);
        // cout << "Solved " << secret << " in " << guessnmr << " guesses" << "\t\r" << flush;
        if (guessnmr > maxguesses)
        {
            maxguesses = guessnmr;
            maxguessword = secret;
        }
        guesses_to_win += guessnmr;
        cout << "Solving word number " << currentwordnum << " out of " << numofwords << "\t\r" << flush;
    }
    /*
    cout << endl
         << "avg: " << guesses_to_win / numofwords << endl;
    cout << "Hardest word was " << maxguessword << " in " << maxguesses << " guesses" << endl;
*/
    return guesses_to_win / numofwords;
}

void manual_game()
{

    int guesscount = 0;
    string guess;
    string res;
    while (true)
    {
        evaluate_words(guesscount);
        guess = sort(scores);
        guesscount++;
        res = " ";

        cout << "hello worldle! Type guess number " << guesscount << '\n';
        cin >> guess;

        if (guess.length() != 5)
        {
            std::cout << "Please type a 5 letter word \n";
            guesscount--;
            continue;
        }
        bool correct_format = false;
        while (res.length() != 5 || !correct_format)
        {
            cout << "type the result in the following format: grey(n), yellow(y) or green(g). e.g. ynngn" << endl;
            cin >> res;

            for (char c : res)
            {
                if (c == 'n' || c == 'g' || c == 'y')
                {
                    correct_format = true;
                }
            }
            if (res.length() != 5 || !correct_format)
            {
                cout << "Invalid format, please try again" << endl;
            }
        }
        for (auto &c : guess)
            c = toupper(c);
        // cout << "You guessed: " << guess << '\n';
        guesses[guesscount] = guess;
        int resindex = 0;
        list<char> letters_to_delete;
        list<char> correct_letters;
        for (char c : res)
        {
            switch (c)
            {
            case 'g':
                results[guesscount][resindex] = "green";
                correct_letters.push_back(guess[resindex]);
                break;

            case 'y':
                results[guesscount][resindex] = "yellow";
                correct_letters.push_back(guess[resindex]);
                break;

            case 'n':
                letters_to_delete.push_back(guess[resindex]);
                break;

            default:
                cout << "big formatting error" << endl;
                break;
            }
            resindex++;
        }
        for (char letter : letters_to_delete)
        {
            bool found = find(correct_letters.begin(), correct_letters.end(), letter) != correct_letters.end();
            if (!found)
            {
                alphabet.erase(letter);
            }
        }

        for (char i : alphabet)
        {
            cout << i << ", ";
        }
        cout << '\n';
    }
}

int gameloop(string const &secret)
{
    int guesscount = 0;
    string guess;
    while (true)
    {
        evaluate_words(guesscount);
        guesscount++;
        if (gamemode == 'a')
        {
            guess = sort(scores);
            if (guesscount == 1)
            {
                guess = startword;
                startwordactual = startword;
            }
        }
        else if (gamemode == 'f')
        {
            sort(scores);
            guess = " ";
            while (guess.length() != 5)
            {
                cout << "hello worldle! Type guess number " << guesscount << '\n';
                cin >> guess;
                if (guess.length() != 5)
                {
                    std::cout << "Please type a 5 letter word \n";
                }
            }
        }

        for (auto &c : guess)
            c = toupper(c);

        if (guess == secret)
        {
            break;
        }

        guesses[guesscount] = guess;
        unordered_map<char, int> letters;
        for (int i = 0; i < 2; i++)
        {
            int guessindex = 0;
            if (i == 0)
            {
                for (char s : secret)
                {
                    letters[s]++;
                }
            }

            for (char guess_c : guess)
            {

                int secretindex = 0;
                bool correct_letter = false;
                for (char secret_c : secret)
                {
                    if (secret_c == guess_c)
                    {

                        correct_letter = true;
                        if (secretindex == guessindex)
                        {
                            results[guesscount][secretindex] = "green";
                            letters[secret_c]--;
                        }
                        else if (i == 1 && letters[guess_c] > 0 && results[guesscount][guessindex] != "green")
                        {
                            results[guesscount][guessindex] = "yellow";
                            letters[secret_c]--;
                        }
                    }
                    secretindex++;
                }
                if (!correct_letter)
                {

                    alphabet.erase(guess_c);
                }
                guessindex++;
            }
        }

        if (gamemode != 'a')
        {
            cout << "Result of: " << guess << '\n';

            for (string const &i : results[guesscount])
            {
                cout << i << ", ";
            }
            cout << endl;
        }

        /*
        for (char i : alphabet)
        {
            cout << i << ", ";
        }
        */
    }
    return guesscount;
}

void readfile(string const &filename)
{
    auto start = high_resolution_clock::now();
    fstream newfile;

    newfile.open(filename, ios::in); // open a file to perform read operation using file object
    if (newfile.is_open())
    { // checking whether the file is open
        string tp;
        while (getline(newfile, tp))
        { // read data from file object and put it into string.
            int i = 0;
            char prec = ' ';

            for (auto &c : tp)
                c = toupper(c);

            for (char c : tp)
            {
                freq[c][i]++;

                if (i > 0)
                {
                    preceeding_letter[c][prec][i]++;
                    succeeding_letter[prec][c][i - 1]++;
                }

                prec = c;
                i++;
            }
            scores[tp] = 0;
            all_words.push_back(tp);
        }
        newfile.close(); // close the file object.
    }
}

void evaluate_words(int guesscount)
{
    // auto start = high_resolution_clock::now();

    list<string> impossiblewords;
    list<char> illegal_dupes;
    unordered_map<char, string[5]> correct_letters;

    int must_contain_letter_index = 0;
    for (string const &s : results[guesscount])
    {
        if (s == "yellow" && !(find(must_contain.begin(), must_contain.end(), guesses[guesscount][must_contain_letter_index]) != must_contain.end()))
        {
            must_contain.insert(guesses[guesscount][must_contain_letter_index]);
        }
        must_contain_letter_index++;
    }

    for (int g_idx = 0; g_idx < guesses[guesscount].length(); g_idx++)
    {
        if (results[guesscount][g_idx] == "Nope")
        {
            for (int idx = 0; idx < guesses[guesscount].length(); idx++)
            {
                if ((results[guesscount][idx] == "green" || results[guesscount][idx] == "yellow") && guesses[guesscount][idx] == guesses[guesscount][g_idx])
                {
                    illegal_dupes.push_back(guesses[guesscount][g_idx]);
                }
            }
        }
    }

    for (auto const &[word, _] : scores)
    {
        int wordindex = 0;
        double score = 0;
        char prec = ' ';
        bool found;
        bool delete_word = true;
        unordered_set<char> must_contain_copy;
        for (char must_letter : must_contain)
        {
            must_contain_copy.insert(must_letter);
        }

        unordered_map<char, int> letter_count;

        for (char c : word)

        {
            if ((find(must_contain_copy.begin(), must_contain_copy.end(), c) != must_contain_copy.end()))
            {
                must_contain_copy.erase(c);
            }
            if (must_contain_copy.empty())
            {
                delete_word = false;
            }

            letter_count[c]++;

            found = (find(alphabet.begin(), alphabet.end(), c) != alphabet.end());
            if (!found)
            {
                score -= 500000;
                goto jump;
            }
            for (int r = 0; r <= guesscount; r++)
            {
                if (results[r][wordindex] == "yellow")

                {

                    if (c == guesses[r][wordindex])
                    {
                        correct_letters[c][wordindex] = "incorrect";
                        score -= 500000;
                        goto jump;
                    }
                }
            }
            if (results[guesscount][wordindex] == "green")
            {

                if (c == guesses[guesscount][wordindex])
                {
                    correct_letters[c][wordindex] = "correct";
                }
                else
                {
                    score -= 50000;
                    goto jump;
                }
            }

            if (guesscount > 0)
            {
                if (results[guesscount][wordindex] == "Nope" && c == guesses[guesscount][wordindex])
                {
                    score -= 500000;
                    goto jump;
                }
            }
            /******** Better solverate with these turned off. Was giving points for common letter combos
            if (wordindex > 0)
            {
                // score += preceeding_letter[c][prec][wordindex];
                // score += succeeding_letter[prec][c][wordindex - 1];
            } *****/

            score += freq.at(c)[wordindex];
            wordindex++;
            prec = c;
            scores[word] = score;
        }

        scores[word] = scores[word] * (letter_count.size() * letter_count.size());
        for (auto const &[letter, count] : letter_count)
        {
            if (count > 1 && (find(illegal_dupes.begin(), illegal_dupes.end(), letter) != illegal_dupes.end()))
            {
                scores[word] -= 500000;
            }
        }
    jump:
        if (score < 0 || delete_word == true)
        {
            impossiblewords.push_back(word);
        }
    }
    impossiblewords.push_back(guesses[guesscount]);
    for (string delword : impossiblewords)
    {
        scores.erase(delword);
    }
    /*
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);

        cout << "Time taken by function: "
             << duration.count() << " microseconds" << endl;*/
}

bool cmp(pair<string, double> &a,
         pair<string, double> &b)
{
    return a.second > b.second;
}

// Function to sort the map according
// to value in a (key-value) pairs
string sort(unordered_map<string, double> &M)
{
    string best_answer;
    // Declare vector of pairs
    vector<pair<string, double>> A;

    // Copy key-value pair from Map
    // to vector of pairs
    for (auto const &it : M)
    {
        A.push_back(it);
    }

    // Sort using comparator function
    sort(A.begin(), A.end(), cmp);

    int printnum = 0;
    // Print the sorted value
    for (auto &it : A)
    {
        if (printnum == 0)
            best_answer = it.first;
        if (printnum == startindex && gamemode == 'a')
        {
            startword = it.first;
            break;
        }

        if (gamemode != 'a')
        {
            cout << it.first << ' '
                 << it.second << endl;
        }

        printnum++;
        if (printnum >= 5 && gamemode != 'a')
            break;
    }
    return best_answer;
}