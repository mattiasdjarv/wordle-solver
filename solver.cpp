#include <iostream>
#include <cstring>
#include <fstream>
#include <unordered_map>

using namespace std;

void readfile(string filename);

unordered_map<char, int[5]> freq;
unordered_map<char, unordered_map<char, int[5]>> preceeding_letter;
unordered_map<char, unordered_map<char, int[5]>> succeeding_letter;
int main()
{

    int guesscount = 0;
    string guess;
    string secret = "ABOUT";
    readfile("answers.txt");

    while (true)
    {
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
        int wordindex = 0;

        string result[5] = {"Nope", "Nope", "Nope", "Nope", "Nope"};
        for (char i : secret)
        {
            int guessindex = 0;
            for (char j : guess)
            {
                if (i == j)
                {
                    if (wordindex == guessindex)
                    {
                        result[wordindex] = "green";
                    }
                    else
                    {
                        result[guessindex] = "yellow";
                    }
                }
                guessindex++;
            }
            wordindex++;
        }
        cout << "Result of: " << guess << '\n';
        for (string i : result)
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
            char suc = ' ';
            for (char c : tp)
            {
                freq[toupper(c)][i]++;

                if (i > 0)
                {
                    preceeding_letter[toupper(c)][toupper(prec)][i]++;
                    succeeding_letter[toupper(prec)][toupper(c)][i - 1]++;
                }

                prec = c;
                i++;
            }
        }
        newfile.close(); // close the file object.
    }
}