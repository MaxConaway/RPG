
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


    if (numChoices < 0 || numChoices > Scene<Moment>::MAX_CHOICES) {
        std::cout << "Error: scene \"" << text << "\" has "
                  << numChoices << " choices. The limit is 3."
                  << std::endl;
        return nullptr;
    }


    Moment* moment = new Moment(text, label, hp, loot, item);
    Scene<Moment>* scene = new Scene<Moment>(moment, numChoices);


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
    std::string line;

    while (true) {
        std::cout << "Your choice: ";

        if (!std::getline(std::cin, line)) {
            return 0;
        }

        try {
            size_t pos = 0;
            int choice = std::stoi(line, &pos);

            if (pos == line.size() &&
                choice >= 1 && choice <= numChoices) {
                return choice;
                }
        }
        catch (...) {
        }

        std::cout << "Please type a number from 1 to "
                  << numChoices << "." << std::endl;
    }
}


// TODO 13: Play the story.

void Adventure::play() {
    Scene<Moment>* current = tree_.getRoot();

    if (current == nullptr) {
        return;
    }

    int hp = startHp_;
    int loot = 0;
    List<Item> bag;

    while (current != nullptr) {
        Moment* moment = current->getData();


        hp += moment->getHpChange();
        loot += moment->getLoot();


        std::cout << "----------------------------------------" << std::endl;
        std::cout << moment->getText() << std::endl;


        if (moment->hasItem()) {
            Item item(moment->getItem());

            if (bag.search(item)) {
                std::cout << "You already have: "
                          << moment->getItem() << std::endl;
            }
            else {
                bag.addFront(new Item(moment->getItem()));
                std::cout << "You picked up: "
                          << moment->getItem() << std::endl;
            }
        }


        std::cout << "HP: " << hp
                  << "  Loot: " << loot << std::endl;


        if (hp <= 0) {
            std::cout << "Your HP hit 0. GAME OVER." << std::endl;
            break;
        }


        if (current->isEnding()) {
            std::cout << "THE END." << std::endl;
            break;
        }


        for (int i = 0; i < current->getNumChoices(); i++) {
            Scene<Moment>* next = current->getChoice(i);

            std::cout << "[" << i + 1 << "] "
                      << next->getData()->getLabel()
                      << std::endl;
        }


        int choice = askChoice(current->getNumChoices());

        if (choice == 0) {
            break;
        }


        current = current->getChoice(choice - 1);
    }


    std::cout << "Final HP: " << hp
              << "  Final loot: " << loot << std::endl;

    std::cout << "Inventory (" << bag.size()
              << " items):" << std::endl;

    bag.print();
}

// ===================== ANALYZING =====================

// TODO 14: Print story stats.

void Adventure::printStats() const {
    std::cout << "Scenes: " << tree_.countScenes() << std::endl;
    std::cout << "Endings: " << tree_.countEndings() << std::endl;
    std::cout << "Longest playthrough: " << tree_.height()
              << " choices" << std::endl;
}

// TODO 15: Print the spoiler guide.

void Adventure::printSpoilers() const {
    if (tree_.isEmpty()) {
        return;
    }

    printEndings(tree_.getRoot(), "");
}

// TODO 16: Print every ending.

void Adventure::printEndings(Scene<Moment>* scene, const std::string& path) const {
    if (scene == nullptr) {
        return;
    }

    if (scene->isEnding()) {
        std::cout << "Path " << path << ": "
                  << scene->getData()->getText()
                  << std::endl;
        return;
    }

    for (int i = 0; i < scene->getNumChoices(); i++) {
        std::string nextPath = extendPath(path, i + 1);

        printEndings(scene->getChoice(i), nextPath);
    }
}

// TODO 17: Greedy advisor.

void Adventure::greedyRun() const {
    Scene<Moment>* current = tree_.getRoot();

    if (current == nullptr) {
        return;
    }

    int hp = startHp_;
    int loot = 0;
    std::string path = "";

    while (current != nullptr) {
        Moment* moment = current->getData();

        hp += moment->getHpChange();
        loot += moment->getLoot();

        if (hp <= 0) {
            break;
        }

        if (current->isEnding()) {
            break;
        }

        int bestChoice = 0;
        int mostLoot = current->getChoice(0)
                              ->getData()->getLoot();

        for (int i = 1; i < current->getNumChoices(); i++) {
            int nextLoot = current->getChoice(i)
                                 ->getData()->getLoot();

            if (nextLoot > mostLoot) {
                mostLoot = nextLoot;
                bestChoice = i;
            }
        }

        path = extendPath(path, bestChoice + 1);
        current = current->getChoice(bestChoice);
    }

    std::cout << "Greedy advisor" << std::endl;
    std::cout << "  Path: " << path << std::endl;
    std::cout << "  Loot: " << loot << std::endl;

    if (hp <= 0) {
        std::cout << "  Result: died" << std::endl;
    }
    else {
        std::cout << "  Result: reached an ending" << std::endl;
    }
}


// TODO 18: Find the best possible loot.

int Adventure::bestLoot(Scene<Moment>* scene, int hp,
                        const std::string& path,
                        std::string& bestPath) const {
    if (scene == nullptr) {
        return -1;
    }

    Moment* moment = scene->getData();

    hp += moment->getHpChange();

    if (hp <= 0) {
        return -1;
    }

    if (scene->isEnding()) {
        bestPath = path;
        return moment->getLoot();
    }

    int mostLoot = -1;
    std::string winningPath = "";

    for (int i = 0; i < scene->getNumChoices(); i++) {
        std::string nextPath = extendPath(path, i + 1);
        std::string childBestPath = "";

        int childLoot = bestLoot(
            scene->getChoice(i),
            hp,
            nextPath,
            childBestPath
        );

        if (childLoot == -1) {
            continue;
        }

        int totalLoot = moment->getLoot() + childLoot;

        if (totalLoot > mostLoot) {
            mostLoot = totalLoot;
            winningPath = childBestPath;
        }
    }

    if (mostLoot != -1) {
        bestPath = winningPath;
    }

    return mostLoot;
}


// TODO 19: Optimal advisor.

void Adventure::optimalRun() const {
    Scene<Moment>* root = tree_.getRoot();

    if (root == nullptr) {
        return;
    }

    std::string bestPath = "";

    int loot = bestLoot(root, startHp_, "", bestPath);

    std::cout << "Optimal advisor" << std::endl;
    std::cout << "  Path: " << bestPath << std::endl;
    std::cout << "  Loot: " << loot << std::endl;

    if (loot == -1) {
        std::cout << "  Result: no surviving path" << std::endl;
    }
    else {
        std::cout << "  Result: reached an ending" << std::endl;
    }
}
