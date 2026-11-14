
#include "Adventure.h"
#include <iostream>
#include "Scene.h"



// ===================== GIVEN HELPERS (do not change) =====================

// Reads the next line that is not blank.
// Spaces at the start of the line are removed.
// Returns false when the file has no more lines.
bool Adventure::nextLine(std::ifstream& in, std::string& line) {
    std::string raw;
    while (std::getline(in, raw)) {
        int start = 0;
        while (start < (int)raw.size() && (raw[start] == ' ' || raw[start] == '\t')) {
            start++;
        }
        line = raw.substr(start);

        if (!line.empty() && line[line.size() - 1] == '\r') {
            line = line.substr(0, line.size() - 1);
        }

        if (!line.empty()) {
            return true;
        }
    }
    return false;
}

// Reads the next line and checks that it starts with keyword.
bool Adventure::readField(std::ifstream& in, const std::string& keyword, std::string& value) {
    std::string line;

    if (!nextLine(in, line)) {
        std::cout << "Error: expected a " << keyword
                  << " line but the file ended." << std::endl;
        return false;
    }

    std::string prefix = keyword + " ";

    if (line.substr(0, prefix.size()) != prefix) {
        std::cout << "Error: expected a " << keyword
                  << " line but found: " << line << std::endl;
        return false;
    }

    value = line.substr(prefix.size());
    return true;
}

// Adds one choice number to a path.
std::string Adventure::extendPath(const std::string& path, int choiceNumber) const {
    if (path.empty()) {
        return std::to_string(choiceNumber);
    }

    return path + "-" + std::to_string(choiceNumber);
}

// ===================== LOADING =====================

// TODO 10: Load the story file.
bool Adventure::load(const std::string& fileName) {
    std::ifstream in(fileName);

    if (!in.is_open()) {
        std::cout << "Error: could not open file "
                  << fileName << std::endl;
        return false;
    }

    std::string value;

    if (!readField(in, "START_HP", value)) {
        return false;
    }

    startHp_ = std::stoi(value);

    Scene<Moment>* root = readScene(in, "START");

    if (root == nullptr) {
        return false;
    }

    tree_.setRoot(root);

    return true;
}

// TODO 11: Read a scene and its children.

Scene<Moment>* Adventure::readScene(std::ifstream& in, const std::string& label) {
    std::string text;
    std::string value;

    // Read the scene information
    if (!readField(in, "SCENE", text)) {
        return nullptr;
    }

    if (!readField(in, "HP", value)) {
        return nullptr;
    }
    int hp = std::stoi(value);

    if (!readField(in, "LOOT", value)) {
        return nullptr;
    }
    int loot = std::stoi(value);

    if (!readField(in, "ITEM", value)) {
        return nullptr;
    }
    std::string item = value;

    if (!readField(in, "CHOICES", value)) {
        return nullptr;
    }
    int numChoices = std::stoi(value);

    // Make sure the number of choices is allowed
    if (numChoices < 0 || numChoices > Scene<Moment>::MAX_CHOICES) {
        std::cout << "Error: scene \"" << text << "\" has "
                  << numChoices << " choices. The limit is 3."
                  << std::endl;
        return nullptr;
    }

    // Create the scene
    Moment* moment = new Moment(text, label, hp, loot, item);
    Scene<Moment>* scene = new Scene<Moment>(moment, numChoices);

    // Read and connect each choice
    for (int i = 0; i < numChoices; i++) {
        std::string choiceLabel;

        if (!readField(in, "LABEL", choiceLabel)) {
            delete scene;
            return nullptr;
        }

        Scene<Moment>* child = readScene(in, choiceLabel);

        if (child == nullptr) {
            delete scene;
            return nullptr;
        }

        scene->setChoice(i, child);
    }

    return scene;
}

// ===================== PLAYING =====================

// TODO 12: Ask the player for a choice.
int Adventure::askChoice(int numChoices) {
    return 0;
}

// TODO 13: Play the story.
void Adventure::play() {
}

// ===================== ANALYZING =====================

// TODO 14: Print story stats.
void Adventure::printStats() const {
}

// TODO 15: Print the spoiler guide.
void Adventure::printSpoilers() const {
}

// TODO 16: Print every ending.
void Adventure::printEndings(Scene<Moment>* scene, const std::string& path) const {
}

// TODO 17: Greedy advisor.
void Adventure::greedyRun() const {
}

// TODO 18: Find the best possible loot.
int Adventure::bestLoot(Scene<Moment>* scene, int hp, const std::string& path, std::string& bestPath) const {
    return -1;
}

// TODO 19: Optimal advisor.
void Adventure::optimalRun() const {
}
