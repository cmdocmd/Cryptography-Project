#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include <unordered_map>
#include <cmath>
#include <limits>

struct Single
{
    int count;
    char c;
};

struct Diagrams
{
    std::string c;
    int count;
};

struct Trigrams
{
    std::string c;
    int count;
};

template <typename T>
bool compareByCount(const T &a, const T &b)
{
    return a.count > b.count;
}

std::vector<std::string> readWordList(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<std::string> wordList;

    if (file.is_open())
    {
        std::string word;
        while (file >> word)
        {
            for (char &c : word)
            {
                c = std::tolower(c); // convert text to lower case
            }
            wordList.push_back(word);
        }
        file.close();
    }
    else
    {
        std::cerr << "Error opening word list file." << std::endl;
    }

    return wordList;
}

std::string Encrypt(std::string str, std::string monoalphabetic)
{
    for (char &c : str)
    {
        c = std::tolower(c); // convert text to lower case
    }

    int shift[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26}; // shifts (a = 1, b = 2, etc...)
    std::string letters = "abcdefghijklmnopqrstuvwxyz";
    int evenOrOdd = 0;
    int letterCounter = 0;
    bool cipher = false;
    int lastLetterShift = 0;

    std::string msg;

    for (int i = 0; i < str.length(); i++)
    {
        evenOrOdd % 2 == 0 ? cipher = false : cipher = true;
        if (cipher)
        {
            if (!isalpha(str[i])) // str[i] == ' ' || str[i] == '.' || str[i] == '-' || str[i] == ';' || str[i] == '!' || str[i] == ',' || str[i] == '_'
            {
                msg += str[i];
            }
            else
            {
                letterCounter++;
                if (letterCounter == 20)
                {
                    letterCounter = 0;
                    evenOrOdd++;
                }

                msg += char(int(str[i] + lastLetterShift - 97) % 26 + 97);
            }
        }
        else
        {
            if (!isalpha(str[i]))
            {
                msg += str[i];
            }
            else
            {
                letterCounter++;
                if (letterCounter == 20)
                {
                    letterCounter = 0;
                    evenOrOdd++;
                }

                for (int j = 0; j < letters.length(); j++)
                {
                    if (str[i] == letters[j])
                    {
                        msg += monoalphabetic[j];

                        if (letterCounter == 0)
                            lastLetterShift = shift[j];
                    }
                }
            }
        }
    }
    return msg;
}

std::string Decrypt(std::string str, std::string monoalphabetic)
{
    for (char &c : str)
    {
        c = std::tolower(c); // convert text to lower case
    }

    int shift[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26}; // shifts (a = 1, b = 2, etc...)
    std::string letters = "abcdefghijklmnopqrstuvwxyz";
    int evenOrOdd = 0;
    int letterCounter = 0;
    bool cipher = false;
    int lastLetterShift = 0;

    std::string msg;

    for (int i = 0; i < str.length(); i++)
    {
        evenOrOdd % 2 == 0 ? cipher = false : cipher = true;
        if (cipher)
        {
            if (!isalpha(str[i]))
            {
                msg += str[i];
            }
            else
            {
                letterCounter++;
                if (letterCounter == 20)
                {
                    letterCounter = 0;
                    evenOrOdd++;
                }

                msg += char(((str[i] - 97) - lastLetterShift + 26) % 26 + 97);
            }
        }
        else
        {
            if (!isalpha(str[i]))
            {
                msg += str[i];
            }
            else
            {
                letterCounter++;
                if (letterCounter == 20)
                {
                    letterCounter = 0;
                    evenOrOdd++;
                }

                for (int j = 0; j < monoalphabetic.length(); j++)
                {
                    if (str[i] == monoalphabetic[j])
                    {
                        msg += letters[j];

                        if (letterCounter == 0)
                            lastLetterShift = shift[j];
                    }
                }
            }
        }
    }
    return msg;
}

std::vector<Diagrams> countTwoLetterWords(const std::string &input)
{
    std::unordered_map<std::string, int> wordCount;

    std::istringstream iss(input);
    std::string word;
    std::vector<Diagrams> diagrams;

    while (iss >> word)
    {
        if (word.length() == 2)
        {
            wordCount[word]++;
        }
    }

    for (const auto &entry : wordCount)
    {
        Diagrams dr;
        dr.c = entry.first;
        dr.count = entry.second;
        diagrams.push_back(dr);
    }

    return diagrams;
}

std::vector<Trigrams> countThreeLetterWords(const std::string &input)
{
    std::unordered_map<std::string, int> wordCount;

    std::istringstream iss(input);
    std::string word;
    std::vector<Trigrams> trigrams;

    while (iss >> word)
    {
        if (word.length() == 3)
        {
            wordCount[word]++;
        }
    }

    for (const auto &entry : wordCount)
    {
        Trigrams dr;
        dr.c = entry.first;
        dr.count = entry.second;
        trigrams.push_back(dr);
    }

    return trigrams;
}

std::string Analysis(std::string str)
{
    for (char &c : str)
    {
        c = std::tolower(c); // convert text to lower case
    }

    std::vector<std::string> wordList = readWordList("/usr/share/dict/british-english"); // /usr/share/dict/british-english

    int letterCounter = 0;
    std::vector<std::string> parts;
    std::string msg;

    //  This is to devide the sentence into 2 parts one for cipher and one for mono-alphabetic
    for (int i = 0; i < str.length(); i++)
    {
        if (!isalpha(str[i]))
        {
            msg += str[i];
        }
        else
        {
            letterCounter++;
            msg += str[i];

            if (letterCounter == 20)
            {
                letterCounter = 0;
                parts.push_back(msg);
                msg = "";
            }

            if (i == str.length() - 1)
            {
                parts.push_back(msg);
            }
        }
    }
    // deviding ends here...

    bool cipher = false;
    msg = "";

    for (int i = 0; i < parts.size(); i++) // parts.size()
    {
        i % 2 == 0 ? cipher = false : cipher = true;

        if (cipher)
        {
            std::vector<int> possible_shifts;
            for (int j = 1; j <= 26; j++)
            {
                for (int k = 0; k < parts[i].length(); k++)
                {
                    if (!isalpha(parts[i][k])) // parts[i][k] == ' ' || parts[i][k] == '.' || parts[i][k] == '-' || parts[i][k] == ';' || parts[i][k] == '!' || parts[i][k] == ',' || parts[i][k] == '_'
                    {
                        msg += parts[i][k];
                    }
                    else
                    {
                        msg += char(((parts[i][k] - 97) - j + 26) % 26 + 97);
                    }
                }

                std::istringstream iss(msg); // input string stream
                std::vector<std::string> words;

                std::string word;
                while (std::getline(iss, word, ' '))
                {
                    words.push_back(word);
                }

                for (int s = 0; s < words.size(); s++)
                {
                    if (words[s].length() >= 4)
                    {
                        auto it = std::find(wordList.begin(), wordList.end(), words[s]);

                        if (it != wordList.end())
                        {
                            possible_shifts.push_back(j);
                        }
                    }
                }

                if (possible_shifts.size() == 0)
                {
                    for (int s = 0; s < words.size(); s++)
                    {
                        if (words[s].length() >= 3)
                        {
                            auto it = std::find(wordList.begin(), wordList.end(), words[s]);

                            if (it != wordList.end())
                            {
                                possible_shifts.push_back(j);
                            }
                        }
                    }
                }
                msg = "";
                words.clear();
            }

            std::map<int, int> countMap;
            std::string correct;
            for (int val : possible_shifts)
            {
                countMap[val]++;
            }

            int maxElement = -1;
            int maxCount = 0;
            bool tie = false;

            for (const auto &pair : countMap)
            {
                if (pair.second > maxCount)
                {
                    maxElement = pair.first;
                    maxCount = pair.second;
                }
                else if (pair.second == maxCount)
                {
                    tie = true;
                }
            }

            if (tie)
            {
                std::cout << "[Brute-Force]: It looks like more than one shift have an english word\n";

                std::sort(possible_shifts.begin(), possible_shifts.end());
                auto last = std::unique(possible_shifts.begin(), possible_shifts.end());
                possible_shifts.erase(last, possible_shifts.end());

                std::string possible;
                std::vector<std::string> possible_strings;

                std::cout << "[Brute-Force]: Choose the correct shift:\n";
                int m = 1;

                for (auto it = possible_shifts.begin(); it != last; ++it)
                {
                    for (int k = 0; k < parts[i].length(); k++)
                    {
                        if (!isalpha(parts[i][k])) // parts[i][k] == ' ' || parts[i][k] == '.' || parts[i][k] == '-' || parts[i][k] == ';' || parts[i][k] == '!' || parts[i][k] == ',' || parts[i][k] == '_'
                        {
                            possible += parts[i][k];
                        }
                        else
                        {
                            possible += char(((parts[i][k] - 97) - *it + 26) % 26 + 97);
                        }
                    }
                    std::cout << "(" << m << ") shift: " << *it << " msg: (" << possible << ")\n";
                    m++;
                    possible_strings.push_back(possible);
                    possible = "";
                }

                int answer;
                std::cin >> answer;
                std::cin.clear();

                parts[i] = possible_strings[answer - 1];
            }
            else
            {
                for (int k = 0; k < parts[i].length(); k++)
                {
                    if (!isalpha(parts[i][k])) // parts[i][k] == ' ' || parts[i][k] == '.' || parts[i][k] == '-' || parts[i][k] == ';' || parts[i][k] == '!' || parts[i][k] == ',' || parts[i][k] == '_'
                    {
                        correct += parts[i][k];
                    }
                    else
                    {
                        correct += char(((parts[i][k] - 97) - maxElement + 26) % 26 + 97);
                    }
                }
                std::string old = parts[i];
                parts[i] = correct;

                std::cout << "[Brute-Force]: encrypted string: (" << old << ") found shift: (" << maxElement << ") " << parts[i] << "\n";
            }

            possible_shifts.clear();
        }
    }

    std::cout << "[Brute-Force]: Done.\n";
    std::cout << "[Analzyer]: Starting Frequency analysis.\n";

    std::string fullMsg;
    msg = "";

    for (int i = 0; i < parts.size(); i++) // parts.size()
    {
        i % 2 == 0 ? cipher = false : cipher = true;
        fullMsg += parts[i];
        if (!cipher) // mono-alphabetic
        {
            msg += parts[i];
        }
    }

    std::vector<Single> fr_list;
    int index = 0;

    while (msg.length() != index)
    {
        char c = msg[index];
        bool found = false;

        if (!isalpha(c)) // c == ' ' || c == '.' || c == '-' || c == ';' || c == '!' || c == ',' || c == '_'
        {
            index++;
            continue;
        }

        for (int j = 0; j < fr_list.size(); j++)
        {
            if (fr_list[j].c == c)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            int counter = 0;
            for (int i = 0; i < msg.length(); i++)
            {
                if (msg[i] == c)
                {
                    counter++;
                }
            }

            Single fr;
            fr.c = c;
            fr.count = counter;
            fr_list.push_back(fr);
        }

        index++;
    }

    std::cout << fullMsg << "\n";
    std::cout << "\n"
              << msg << "\n";

    std::vector<Diagrams> dr_list = countTwoLetterWords(msg);
    std::vector<Trigrams> tr_list = countThreeLetterWords(msg);

    std::sort(fr_list.begin(), fr_list.end(), compareByCount<Single>);
    std::sort(dr_list.begin(), dr_list.end(), compareByCount<Diagrams>);
    std::sort(tr_list.begin(), tr_list.end(), compareByCount<Trigrams>);

    std::map<char, char> changer;

    for (int i = 0; i < fr_list.size(); i++)
    {
        if (i == 0)
        {
            changer[fr_list[i].c] = 'e';
        }
        if (i == 1)
        {
            changer[fr_list[i].c] = 't';
        }
    }

    std::cout << "[Analzyer]: now applying frequency analysis on diagrams\n";

    std::vector<std::string> topTwo{"of", "to", "in", "it", "is", "be", "as", "we", "he", "by", "or", "on", "do", "if", "me", "my", "up", "an", "go", "no", "us", "am"};

    for (int i = 0; i < dr_list.size(); i++)
    {
        std::cout << "[Analzyer]: (" << dr_list[i].c << ") is repeated: (" << dr_list[i].count << ") times\n[Analzyer]: what could it be:\n";
        for (int j = 0; j < topTwo.size(); j++)
        {
            std::cout << "(" << j + 1 << ") " << topTwo[j] << "\n";
        }
        std::cout << "(" << topTwo.size() + 1 << ") Enter the value\n";
        std::cout << "(" << topTwo.size() + 2 << ") Skip\n";
        int answer;
        std::cin >> answer;
        std::cin.clear();
        std::string a;
        if (answer == topTwo.size() + 1)
        {
            std::cin >> a;
            std::cin.clear();
        }
        else if (answer == topTwo.size() + 2)
        {
            continue;
        }
        else
        {
            a = topTwo[answer - 1];
        }

        for (int j = 0; j < dr_list[i].c.length(); j++)
        {
            changer[dr_list[i].c[j]] = a[j];
        }
    }

    std::cout << "[Analzyer]: now applying frequency analysis on trigrams\n";

    std::vector<std::string> topThree{"man", "are", "was", "she", "her", "his", "can", "has", "had", "any", "all", "out", "for", "the", "but", "and", "can", "you", "etc"};

    for (int i = 0; i < tr_list.size(); i++)
    {
        std::cout << "[Analzyer]: (" << tr_list[i].c << ") is repeated: (" << tr_list[i].count << ") times\n[Analzyer]: what could it be:\n";
        for (int j = 0; j < topThree.size(); j++)
        {
            std::cout << "(" << j + 1 << ") " << topThree[j] << "\n";
        }
        std::cout << "(" << topThree.size() + 1 << ") Enter the value\n";
        std::cout << "(" << topThree.size() + 2 << ") Skip\n";
        int answer;
        std::cin >> answer;
        std::cin.clear();
        std::string a;
        if (answer == topThree.size() + 1)
        {
            std::cin >> a;
            std::cin.clear();
        }
        else if (answer == topThree.size() + 2)
        {
            continue;
        }
        else
        {
            a = topThree[answer - 1];
        }

        for (int j = 0; j < tr_list[i].c.length(); j++)
        {
            changer[tr_list[i].c[j]] = a[j];
        }
    }

    for (int i = 0; i < parts.size(); i++)
    {
        i % 2 == 0 ? cipher = false : cipher = true;
        if (!cipher)
        {
            std::vector<std::string> words;
            std::istringstream iss(parts[i]);

            std::string word;
            while (std::getline(iss, word, ' '))
            {
                words.push_back(word);
            }

            for (int j = 0; j < words.size(); j++)
            {
                std::string original = words[j];
                for (int k = 0; k < words[j].length(); k++)
                {
                    if (changer.find(words[j][k]) != changer.end())
                    {
                        words[j][k] = changer[words[j][k]];
                    }
                }

                auto it = std::find(wordList.begin(), wordList.end(), words[j]);

                if (it != wordList.end())
                {
                }
                else
                {
                    std::string ans;
                    std::cout << "[Analzyer]: word: (" << words[j] << ")\n[Analzyer]: Original: (" << original << ")\n[Analzyer]: Enter 0 to skip\n[Analzyer]: what could it be:\n";
                    std::cin >> ans;
                    std::cin.clear();
                    if (ans != "0")
                    {
                        for (int k = 0; k < words[j].length(); k++)
                        {
                            // if (changer.find(words[j][k]) != changer.end())
                            //{
                            // }
                            // else
                            //{
                            // std::cout << words[j][k] << " " << ans[k] << "\n";
                            changer[original[k]] = ans[k];
                            //}
                        }
                    }
                }
            }
        }
    }

    for (const auto &pair : changer)
    {
        std::cout << "[Analzyer]: Found (" << pair.first << ") is (" << pair.second << ")\n";
    }

    fullMsg = "";
    for (int i = 0; i < parts.size(); i++)
    {
        i % 2 == 0 ? cipher = false : cipher = true;
        if (!cipher)
        {
            for (int j = 0; j < parts[i].length(); j++)
            {
                if (changer.find(parts[i][j]) != changer.end())
                {
                    parts[i][j] = changer[parts[i][j]];
                }
            }
        }
        fullMsg += parts[i];
    }

    return fullMsg;
}

void readFile(bool encrypt, bool analyzer)
{
    std::ifstream file("test.txt"); // file name
    std::string key;

    if (!file.is_open()) // failed to open the file
    {
        std::cout << "Failed to open the file\n";
        return;
    }

    if (!analyzer)
    {
        if (std::getline(file, key)) // first line
        {
            std::cout << "Key: " << key << "\n";
            if (key.length() < 26)
            {
                std::cout << "Key must be 26 length\n";
                return;
            }
        }
        else // could not read the key...
        {
            std::cout << "Failed to read the key\n";
            return;
        }

        if (encrypt)
        {
            std::string line;
            while (std::getline(file, line)) // for all other lines call encrypt..
            {
                std::string msg = Encrypt(line, key);
                std::string ans;
                std::cout << msg << "\n\nDo you want to Decrypt it back: Y/N:\n";
                std::cin >> ans;
                if (ans == "y" || ans == "Y")
                {
                    int a;
                    std::cout << "(1) using Key\n(2) without Key\n";
                    std::cin >> a;
                    std::cin.clear();
                    if (a == 1)
                    {
                        std::cout << Decrypt(msg, key) << "\n\n";
                    }
                    if (a == 2)
                    {
                        std::cout << Analysis(msg) << "\n\n";
                    }
                }
            }
        }
        else
        {
            std::string line;
            while (std::getline(file, line))
            {
                std::cout << Decrypt(line, key) << "\n\n"; // for all other lines call decrypt
            }
        }
    }
    else
    {
        std::string line;
        while (std::getline(file, line)) // for all other lines call encrypt..
        {
            std::cout << Analysis(line) << "\n\n";
        }
    }

    file.close(); // closing the file
}

int main()
{

    bool again = true;
    while (again)
    {
        std::string a = "";
        std::cout << "\n\nPlease choose one of the following\n(1)Encrypt\n(2)Decrypt using Key\n(3)Decrypt without Key\n(4)Exit\n";
        std::cin >> a;
        std::cin.clear();

        if (a == "1")
        {
            readFile(true, false);
        }
        else if (a == "2")
        {
            readFile(false, false);
        }
        else if (a == "3")
        {
            readFile(false, true);
        }
        else
        {
            again = false;
        }
    }

    //lkjihgvutsrqponmcbafedzyxw
    //The figures are of two types, full statues and busts. Some of the busts are two-headed. Great effort was put into modelling the heads, with wide-open eyes and bitumen-outlined irises. The statues represent men, women and children; women are recognizable by features resembling breasts and slightly enlarged bellies, but neither male nor female sexual characteristics are emphasized, and none of the statues have genitals, the only part of the statue fashioned with any amount of detail being the faces
    //fuh gtvebha lbh ng fzn fxodr, etkk rszstdr zmc atrfa. anph ng fuh keafa lbh fqi-byuxyx. alyun yzziln qla mef tofn pnihqqtov fum pmila, eqbp eqlm-wxmv mgha loi ktfepho-nefqtohi mvmwiw. xli wxexyiw vitvhahof pho, znpho loi jutqpdqz; iayqz mdq dqoaszullkqh kx ghlfebha bhahpknkpi dtgcuvu cpf unkijvqx hoqlbvhi khqqtha, kef hycnbyl gufy hil zygufy ahyelq julbljfhbtaftjv duh hpskdvlchg, dqg qrqh ng fuh aflfeha uldh vhowhozg, hvs cbzm dofh ct hvh aflfeh glautnohi ztfu ivg iuwcvb wn lmbiqt jmqov fuh gljha

    return 0;
}
