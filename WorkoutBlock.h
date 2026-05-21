#ifndef WORKOUT_BLOCK_H
#define WORKOUT_BLOCK_H

#include <iostream>
#include <vector>
#include <string>
#include "Exercise.h"
#include "Set.h"

class WorkoutBlock {
protected:
    std::string notes;

public:
    virtual ~WorkoutBlock() {}

    void setNotes(const std::string& n) { notes = n; }
    std::string getNotes() const { return notes; }

    virtual std::string getType() const = 0;
    virtual double getTotalVolume() const = 0;
    virtual void display() const = 0;
    virtual void saveTo(std::ostream& out) const = 0;
    virtual bool isStandard() const = 0;
};

class StandardBlock : public WorkoutBlock {
private:
    Exercise* exercise;
    std::vector<Set> sets;

public:
    StandardBlock(Exercise* ex) : exercise(ex) {}

    Exercise* getExercise() const { return exercise; }
    const std::vector<Set>& getSets() const { return sets; }
    std::vector<Set>& getSetsMutable() { return sets; }

    void addSet(const Set& s) { sets.push_back(s); }
    int getSetCount() const { return sets.size(); }

    std::string getType() const override { return "Standard"; }
    bool isStandard() const override { return true; }

    double getTotalVolume() const override {
        double total = 0;
        for (size_t i = 0; i < sets.size(); i++) total += sets[i].getVolume();
        return total;
    }

    void display() const override {
        if (!exercise) {
            std::cout << "<missing exercise> - " << sets.size() << " sets:\n";
        } else {
            std::cout << exercise->getName() << " - " << sets.size() << " sets:\n";
        }
        for (size_t i = 0; i < sets.size(); i++) {
            std::cout << "      " << (i + 1) << ") "
                      << sets[i].getReps() << " reps x "
                      << sets[i].getWeight() << " kg";
            if (sets[i].getRpe() > 0) std::cout << " (RPE " << sets[i].getRpe() << ")";
            if (sets[i].getIsPR()) std::cout << " *** PR ***";
            std::cout << "\n";
        }
    }

    void saveTo(std::ostream& out) const override {
        out << "STD|" << exercise->getName() << "|" << sets.size() << "\n";
        for (size_t i = 0; i < sets.size(); i++) {
            out << sets[i].getReps() << "|" << sets[i].getWeight()
                << "|" << sets[i].getRpe() << "|" << sets[i].getIsPR() << "\n";
        }
    }
};

struct CircuitItem {
    Exercise* exercise;
    int reps;
    CircuitItem(Exercise* ex = nullptr, int r = 0) : exercise(ex), reps(r) {}
};

class CircuitBlock : public WorkoutBlock {
private:
    int rounds;
    std::vector<CircuitItem> items;

public:
    CircuitBlock(int r = 1) : rounds(r) {}

    int getRounds() const { return rounds; }
    const std::vector<CircuitItem>& getItems() const { return items; }

    void addItem(Exercise* ex, int reps) {
        items.push_back(CircuitItem(ex, reps));
    }

    void removeExercise(Exercise* ex) {
        items.erase(std::remove_if(items.begin(), items.end(), [&](const CircuitItem& it){
            return it.exercise == ex;
        }), items.end());
    }

    bool isEmpty() const { return items.empty(); }

    std::string getType() const override { return "Circuit"; }
    bool isStandard() const override { return false; }

    double getTotalVolume() const override {
        double total = 0;
        for (size_t i = 0; i < items.size(); i++) total += items[i].reps;
        return total * rounds;
    }

    void display() const override {
        std::cout << "Circuit workout - " << rounds << " rounds:\n";
        for (size_t i = 0; i < items.size(); i++) {
            std::cout << "      " << (i + 1) << ") ";
            if (!items[i].exercise) std::cout << "<missing exercise>";
            else std::cout << items[i].exercise->getName();
            std::cout << " x " << items[i].reps << " reps\n";
        }
    }

    void saveTo(std::ostream& out) const override {
        out << "CIRC|" << rounds << "|" << items.size() << "\n";
        for (size_t i = 0; i < items.size(); i++) {
            out << items[i].exercise->getName() << "|" << items[i].reps << "\n";
        }
    }
};

class Workout {
private:
    std::string date;
    int duration;
    std::vector<WorkoutBlock*> blocks;

public:
    Workout(const std::string& d = "", int dur = 0) : date(d), duration(dur) {}

    ~Workout() {
        for (size_t i = 0; i < blocks.size(); i++) delete blocks[i];
    }

    Workout(const Workout&) = delete;
    Workout& operator=(const Workout&) = delete;

    Workout(Workout&& other) noexcept
        : date(other.date), duration(other.duration), blocks(std::move(other.blocks)) {
        other.blocks.clear();
    }

    Workout& operator=(Workout&& other) noexcept {
        if (this != &other) {
            for (size_t i = 0; i < blocks.size(); i++) delete blocks[i];
            date = other.date;
            duration = other.duration;
            blocks = std::move(other.blocks);
            other.blocks.clear();
        }
        return *this;
    }

    std::string getDate() const { return date; }
    int getDuration() const { return duration; }
    const std::vector<WorkoutBlock*>& getBlocks() const { return blocks; }

    void setDate(const std::string& d) { date = d; }
    void setDuration(int d) { if (d >= 0) duration = d; }

    void addBlock(WorkoutBlock* b) { blocks.push_back(b); }
    int getBlockCount() const { return blocks.size(); }

    void removeBlocksWithExercise(Exercise* ex) {
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

    double getTotalVolume() const {
        double total = 0;
        for (size_t i = 0; i < blocks.size(); i++) total += blocks[i]->getTotalVolume();
        return total;
    }

    void print() const {
        std::cout << "Workout on " << date << " (" << duration << " min):\n";
        for (size_t i = 0; i < blocks.size(); i++) {
            std::cout << "  [" << (i + 1) << "] ";
            blocks[i]->display();
        }
        std::cout << "  Total volume: " << getTotalVolume() << "\n";
    }
};

#endif