#ifndef WORKOUT_PROGRAM_H
#define WORKOUT_PROGRAM_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Exercise.h"
#include "WorkoutBlock.h"

class WorkoutProgram {
private:
    std::string name;
    std::vector<WorkoutBlock*> blocks;

public:
    WorkoutProgram(const std::string& n = "") : name(n) {}

    ~WorkoutProgram() {
        for (size_t i = 0; i < blocks.size(); i++) delete blocks[i];
    }

    std::string getName() const { return name; }
    const std::vector<WorkoutBlock*>& getBlocks() const { return blocks; }

    void setName(const std::string& n) { name = n; }
    void addBlock(WorkoutBlock* b) { blocks.push_back(b); }
    int getBlockCount() const { return blocks.size(); }

    void removeExercise(Exercise* ex) {
        for (size_t i = 0; i < blocks.size(); ) {
            WorkoutBlock* b = blocks[i];
            if (b->isStandard()) {
                StandardBlock* sb = (StandardBlock*)b;
                if (sb->getExercise() == ex) {
                    delete b;
                    blocks.erase(blocks.begin() + i);
                    continue;
                }
            } else {
                CircuitBlock* cb = dynamic_cast<CircuitBlock*>(b);
                if (cb) {
                    cb->removeExercise(ex);
                    if (cb->isEmpty()) {
                        delete cb;
                        blocks.erase(blocks.begin() + i);
                        continue;
                    }
                }
            }
            ++i;
        }
    }

    void print() const {
        std::cout << "Program: " << name << " (" << blocks.size() << " blocks)\n";
        for (size_t i = 0; i < blocks.size(); i++) {
            std::cout << "  [" << (i+1) << "] "; blocks[i]->display();
        }
    }

    void saveTo(std::ostream& out) const {
        out << "PROG|" << name << "|" << blocks.size() << "\n";
        for (size_t i = 0; i < blocks.size(); i++) blocks[i]->saveTo(out);
    }
};

#endif