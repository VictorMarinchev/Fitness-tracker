#ifndef WORKOUT_PROGRAM_H
#define WORKOUT_PROGRAM_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Exercise.h"

class WorkoutProgram {
private:
    std::string name;
    std::vector<Exercise*> planned;

public:
    WorkoutProgram(const std::string& n = "") : name(n) {}

    std::string getName() const { return name; }
    const std::vector<Exercise*>& getPlanned() const { return planned; }

    void setName(const std::string& n) { name = n; }
    void addExercise(Exercise* ex) { planned.push_back(ex); }
    int getExerciseCount() const { return planned.size(); }

    void removeExercise(Exercise* ex) {
        planned.erase(std::remove(planned.begin(), planned.end(), ex), planned.end());
    }

    void print() const {
        std::cout << "Program: " << name << " (" << planned.size() << " exercises)\n";
        for (size_t i = 0; i < planned.size(); i++) {
            std::cout << "  " << (i + 1) << ") " << planned[i]->getName() << "\n";
        }
    }

    void saveTo(std::ostream& out) const {
        out << "PROG|" << name << "|" << planned.size() << "\n";
        for (size_t i = 0; i < planned.size(); i++) {
            out << planned[i]->getName() << "\n";
        }
    }
};

#endif