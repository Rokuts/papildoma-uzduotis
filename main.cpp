#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <regex>
#include <string>
#include <locale>
#include <algorithm>

using std::string;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::unordered_map;
using std::unordered_set;
using std::vector;
using std::regex;
using std::sregex_iterator;
using std::cerr;
using std::endl;

regex urlRegex(R"((https?://[^\s]+|www\.[^\s]+|[^\s]+\.[a-zA-Z]{2,}))");

void processText(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Nepavyko atidaryti failo: " << filename << endl;
        return;
    }

    string line;
    unordered_map<string, int> wordCount;
    unordered_map<string, unordered_set<int>> wordLocations;
    unordered_set<string> urls;

    int lineNumber = 0;

    // Nustatome lokalę, kad palaikytume lietuviškus simbolius
    std::locale::global(std::locale("lt_LT.UTF-8"));

    while (getline(file, line)) {
        lineNumber++;
        istringstream iss(line);
        string word;

        // Rasti URL
        auto urlBegin = sregex_iterator(line.begin(), line.end(), urlRegex);
        auto urlEnd = sregex_iterator();
        for (sregex_iterator i = urlBegin; i != urlEnd; ++i) {
            urls.insert(i->str());
        }

        // Skaičiuoti žodžius
        while (iss >> word) {
            // Konvertuoti į mažąsias raides
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);

            // Pašalinti skyrybos ženklus iš žodžio pradžios/pabaigos
            word.erase(0, word.find_first_not_of(".,;:!?\"'()[]{}<>"));
            word.erase(word.find_last_not_of(".,;:!?\"'()[]{}<>") + 1);

            if (!word.empty()) {
                wordCount[word]++;
                wordLocations[word].insert(lineNumber);
            }
        }
    }

    // Išvestis žodžių dažniams
    ofstream freqOutput("word_frequencies.txt");
    freqOutput << "Zodis: daznis\n";
    freqOutput << "----------------\n";
    for (const auto& pair : wordCount) {
        if (pair.second > 1) {
            freqOutput << pair.first << ": " << pair.second << "\n";
        }
    }
    freqOutput.close();

    // Išvestis kryžminėms nuorodoms
    ofstream crossRefOutput("cross_reference.txt");
    crossRefOutput << "Zodis: eilutes kuriose jis yra\n";
    crossRefOutput << "---------------------------------\n";
    for (const auto& pair : wordLocations) {
        if (pair.second.size() > 1) {
            crossRefOutput << pair.first << ": ";
            for (int loc : pair.second) {
                crossRefOutput << loc << " ";
            }
            crossRefOutput << "\n";
        }
    }
    crossRefOutput.close();

    // Išvestis URL adresams
    ofstream urlOutput("urls.txt");
    for (const auto& url : urls) {
        urlOutput << url << "\n";
    }
    urlOutput.close();
}

int main() {
    processText("input.txt");
    return 0;
}

