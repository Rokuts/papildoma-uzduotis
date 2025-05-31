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

void processText(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nepavyko atidaryti failo: " << filename << std::endl;
        return;
    }

    std::string line;
    std::unordered_map<std::string, int> wordCount;
    std::unordered_map<std::string, std::unordered_set<int>> wordLocations;
    std::unordered_set<std::string> urls;
    std::regex urlRegex(R"((https?://[^\s]+|www\.[^\s]+|[^\s]+\.[a-zA-Z]{2,}))");

    int lineNumber = 0;

    // Nustatome lokalę, kad palaikytume lietuviškus simbolius
    std::locale::global(std::locale("lt_LT.UTF-8"));

    while (std::getline(file, line)) {
        lineNumber++;
        std::istringstream iss(line);
        std::string word;

        // Rasti URL
        auto urlBegin = std::sregex_iterator(line.begin(), line.end(), urlRegex);
        auto urlEnd = std::sregex_iterator();
        for (std::sregex_iterator i = urlBegin; i != urlEnd; ++i) {
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
    std::ofstream freqOutput("word_frequencies.txt");
    for (const auto& pair : wordCount) {
        if (pair.second > 1) {
            freqOutput << pair.first << ": " << pair.second << "\n";
        }
    }
    freqOutput.close();

    // Išvestis kryžminėms nuorodoms
    std::ofstream crossRefOutput("cross_reference.txt");
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
    std::ofstream urlOutput("urls.txt");
    for (const auto& url : urls) {
        urlOutput << url << "\n";
    }
    urlOutput.close();
}

int main() {
    processText("input.txt");
    return 0;
}

