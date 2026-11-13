//
// Created by tothe on 11/12/2026.
//

#pragma once


int countScenes(Scene<T>* scene) const {
    if (scene == nullptr) {
        return 0;
    }

    int total = 1;

    for (int i = 0; i < scene->getNumChoices(); i++) {
        total += countScenes(scene->getChoice(i));
    }

    return total;
}

int countScenes(Scene<T>* scene) const {
    if (scene == nullptr) {
        return 0;
    }
    if (scene->isEnding()) {
        return 1;
    }
    int total = 0;
    for (int i = 0; i < scene->getNumChoices(); i++) {
        total += countScenes(scene->getChoice(i));
    }
    return total;
}

int height(Scene<T>* scene) const {
    if (scene == nullptr) {
        return -1;
    }
    int maxHeight = -1;
    for (int i = 0; i < scene->getNumChoices(); i++) {
        int childheight = countScenes(scene->getChoice(i));
        if (childheight > maxHeight) {
            maxHeight = childheight;
        }
    }
    return maxHeight + 1;
}


void clear() {
    delete root_;
    root_ = nullptr;
}
