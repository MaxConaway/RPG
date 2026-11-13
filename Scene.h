//
// Created by tothe on 11/12/2026.
//


#pragma once


template <typename T>
class Scene {
private:
    T* data_;
    Scene<T>** choices_;
    int numChoices_;

public:
    static const int MAX_CHOICES = 3;

    Scene(T* data, int numChoices) : data_(data), choices_(nullptr), numChoices_(numChoices) {
        // TODO 1
        if (numChoices_ > 0) {
            choices_ = new Scene<T>*[numChoices_];

            for (int i = 0; i < numChoices_; i++) {
                choices_[i] = nullptr;
            }
        }
    }

    ~Scene() {
        // TODO 2
        for (int i = 0; i < numChoices_; i++) {
            delete choices_[i];
        }

        delete[] choices_;
        delete data_;
    }

    T* getData() const {
        return data_;
    }

    int getNumChoices() const {
        return numChoices_;
    }

    bool isEnding() const {
        return numChoices_ == 0;
    }

    Scene<T>* getChoice(int index) const {
        // TODO 4
        if (index < 0 || index >= numChoices_) {
            return nullptr;
        }

        return choices_[index];
    }


    void setChoice(int index, Scene<T>* next) {
        // TODO 5
        if (index < 0 || index >= numChoices_) {
            return;
        }

        choices_[index] = next;
    }
};
