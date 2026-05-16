#ifndef WORKOUT_PROGRAM_H
#define WORKOUT_PROGRAM_H

#include <iostream>
#include <string>
#include <vector>
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

    void print() const {
        std::cout << "Програма: " << name << " (" << planned.size() << " упражнения)\n";
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