#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <unordered_map>
#include <list>
#include <bits/stdc++.h>
#include <algorithm>

using namespace std;

void readfile(string filename);
void evaluate_words(int guesscount);
string sort(map<string, double> &M);
bool cmp(pair<string, double> &a,
         pair<string, double> &b);
int gameloop(string secret);
void autoplay();
void manual_game();

list<char> must_contain;
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
list<char> alphabet{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
map<string, double> scores;
list<string> all_words;
char gamemode;

void reset()
{
    must_contain.clear();
    alphabet = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    for (string word : all_words)
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

    while (gamemode != 'm' && gamemode != 'a')
    {
        cout << "Choose game mode: manual(m) or automatic(a)" << endl;
        cin >> gamemode;
        if (gamemode == 'a')
        {
            cout << "Autosolving all words" << endl;
            autoplay();
        }
        else if (gamemode == 'm')
        {
            manual_game();
        }
        else
        {
            cout << gamemode << " is not a valid gamemode, try again" << endl;
        }
    }
}

void autoplay()
{
    double avg_guesses;
    double guesses_to_win = 0;
    int guessnmr = 0;
    int maxguesses = 0;
    int currentwordnum = 0;
    int numofwords = all_words.size();
    string maxguessword;
    for (string secret : all_words)
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
    cout << endl
         << "avg: " << guesses_to_win / numofwords << endl;
    cout << "Hardest word was " << maxguessword << " in " << maxguesses << " guesses" << endl;
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
                alphabet.remove(letter);
            }
        }

        for (char i : alphabet)
        {
            cout << i << ", ";
        }
        cout << '\n';
    }
}

int gameloop(string secret)
{
    int guesscount = 0;
    string guess;
    while (true)
    {
        evaluate_words(guesscount);
        guess = sort(scores);
        guesscount++;
        if (guess == secret)
        {
            break;
        }

        /* for manual input
        cout << freq['Q'][2];
        cout << "hello worldle! Type guess number " << guesscount << '\n';
        cin >> guess;
        if (guess.length() != 5)
        {
            std::cout << "Please type a 5 letter word \n";
            guesscount--;
            continue;
        }
        */

        for (auto &c : guess)
            c = toupper(c);
        // cout << "You guessed: " << guess << '\n';
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

                    alphabet.remove(guess_c);
                }
                guessindex++;
            }
        }

        /*cout << "Result of: " << guess << '\n';

        for (string i : results[guesscount])
        {
            cout << i << ", ";
        }
        for (char i : alphabet)
        {
            cout << i << ", ";
        }
        cout << '\n';*/
    }
    return guesscount;
}

void readfile(string filename)
{
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
    list<string> impossiblewords;
    list<char> illegal_dupes;
    unordered_map<char, string[5]> correct_letters;

    int must_contain_letter_index = 0;
    for (string s : results[guesscount])
    {
        if (s == "yellow" && !(find(must_contain.begin(), must_contain.end(), guesses[guesscount][must_contain_letter_index]) != must_contain.end()))
        {
            must_contain.push_back(guesses[guesscount][must_contain_letter_index]);
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
        list<char> must_contain_copy;
        for (char must_letter : must_contain)
        {
            must_contain_copy.push_back(must_letter);
        }

        unordered_map<char, int> letter_count;

        for (char c : word)

        {
            if ((find(must_contain_copy.begin(), must_contain_copy.end(), c) != must_contain_copy.end()))
            {
                must_contain_copy.remove(c);
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
            }
            for (int r = 0; r <= guesscount; r++)
            {
                if (results[r][wordindex] == "yellow")

                {

                    if (c == guesses[r][wordindex])
                    {
                        correct_letters[c][wordindex] = "incorrect";
                        score -= 500000;
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
                }
            }

            if (guesscount > 0)
            {
                if (results[guesscount][wordindex] == "Nope" && c == guesses[guesscount][wordindex])
                {
                    score -= 500000;
                }
            }

            if (wordindex > 0)
            {
                // score += preceeding_letter[c][prec][wordindex];
                // score += succeeding_letter[prec][c][wordindex - 1];
            }

            score += freq.at(c)[wordindex];
            wordindex++;
            prec = c;
            scores[word] = score;
        }

        scores[word] = scores[word] * (letter_count.size());
        for (auto const &[letter, count] : letter_count)
        {
            if (count > 1 && (find(illegal_dupes.begin(), illegal_dupes.end(), letter) != illegal_dupes.end()))
            {
                scores[word] -= 500000;
            }
        }
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
}

bool cmp(pair<string, double> &a,
         pair<string, double> &b)
{
    return a.second > b.second;
}

// Function to sort the map according
// to value in a (key-value) pairs
string sort(map<string, double> &M)
{
    string best_answer;
    // Declare vector of pairs
    vector<pair<string, double>> A;

    // Copy key-value pair from Map
    // to vector of pairs
    for (auto &it : M)
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

        if (gamemode == 'm')
        {
            cout << it.first << ' '
                 << it.second << endl;
        }

        printnum++;
        if (printnum >= 5)
            break;
    }
    return best_answer;
}