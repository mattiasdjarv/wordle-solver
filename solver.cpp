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
void evaluate_words();
void sort(map<string, double> &M);
bool cmp(pair<string, double> &a,
         pair<string, double> &b);

unordered_map<char, double[5]> freq;
unordered_map<char, unordered_map<char, double[5]>> preceeding_letter;
unordered_map<char, unordered_map<char, double[5]>> succeeding_letter;
string guesses[7];
string results[7][5] = {
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"},
    {"Nope", "Nope", "Nope", "Nope", "Nope"}};
list<char> alphabet{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
map<string, double> scores;
int main()
{

    int guesscount = 0;
    string guess;
    string secret = "ABOUT";
    readfile("answers.txt");

    while (true)
    {
        evaluate_words();
        sort(scores);
        guesscount++;
        cout << freq['Q'][2];
        cout << "hello worldle! Type guess number " << guesscount << '\n';
        cin >> guess;
        if (guess.length() != 5)
        {
            std::cout << "Please type a 5 letter word \n";
            guesscount--;
            continue;
        }

        for (auto &c : guess)
            c = toupper(c);
        cout << "You guessed: " << guess << '\n';
        guesses[guesscount - 1] = guess;

        for (int i = 0; i < 2; i++)
        {
            int guessindex = 0;
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
                        }
                        else if (i == 1)
                        {
                            results[guesscount][guessindex] = "yellow";
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

        cout << "Result of: " << guess << '\n';

        for (string i : results[guesscount])
        {
            cout << i << ", ";
        }
        for (char i : alphabet)
        {
            cout << i << ", ";
        }
        cout << '\n';
    }
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

            for (char c : tp)
            {
                freq[toupper(c)][i]++;

                if (i > 0)
                {
                    preceeding_letter[toupper(c)][toupper(prec)][i]++;
                    succeeding_letter[toupper(prec)][toupper(c)][i - 1]++;
                }
                scores[tp] = 0;

                prec = c;
                i++;
            }
        }
        newfile.close(); // close the file object.
    }
}

void evaluate_words()
{
    list<string> impossiblewords;
    for (auto const &[word, _] : scores)
    {
        int i = 0;
        double score = 0;
        char prec = ' ';
        bool found;
        for (char c : word)
        {
            c = toupper(c);
            found = (find(alphabet.begin(), alphabet.end(), c) != alphabet.end());
            if (!found)
            {
                score -= 500000;
            }

            if (i > 0)
            {
                score += preceeding_letter[c][prec][i];
                score += succeeding_letter[prec][c][i - 1];
            }

            score += freq.at(c)[i] / 2;
            i++;
            prec = c;
            scores[word] = score;
        }
        if (score < 0)
        {
            impossiblewords.push_back(word);
        }
    }
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
void sort(map<string, double> &M)
{

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

        cout << it.first << ' '
             << it.second << endl;
        printnum++;
        if (printnum >= 5)
            break;
    }
}