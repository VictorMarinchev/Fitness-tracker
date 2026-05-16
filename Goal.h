#ifndef GOAL_H
#define GOAL_H

#include <iostream>
#include <string>
#include <cmath>

class Goal {
protected:
    std::string description;
    std::string deadline;
    bool isCompleted;

public:
    Goal(const std::string& d, const std::string& dl)
        : description(d), deadline(dl), isCompleted(false) {}
    virtual ~Goal() {}

    std::string getDescription() const { return description; }
    std::string getDeadline() const { return deadline; }
    bool getIsCompleted() const { return isCompleted; }

    void markCompleted() { isCompleted = true; }

    virtual double getProgress() const = 0;
    virtual void print() const = 0;
    virtual void saveTo(std::ostream& out) const = 0;
};

class StrengthGoal : public Goal {
private:
    std::string exerciseName;
    double targetWeight;
    int targetReps;
    double currentBest;

public:
    StrengthGoal(const std::string& d, const std::string& dl,
                 const std::string& exName, double targetW, int targetR)
        : Goal(d, dl), exerciseName(exName), targetWeight(targetW),
          targetReps(targetR), currentBest(0) {}

    std::string getExerciseName() const { return exerciseName; }
    double getTargetWeight() const { return targetWeight; }

    void updateBest(double w) {
        if (w > currentBest) currentBest = w;
        if (currentBest >= targetWeight) markCompleted();
    }

    double getProgress() const override {
        if (targetWeight <= 0) return 0;
        double p = (currentBest / targetWeight) * 100;
        if (p > 100) p = 100;
        return p;
    }

    void print() const override {
        std::cout << "[Силова] " << description
                  << " - " << exerciseName << ": " << currentBest
                  << "/" << targetWeight << " кг x " << targetReps
                  << " повт. (" << (int)getProgress() << "%)";
        if (isCompleted) std::cout << " ЗАВЪРШЕНА";
    }

    void saveTo(std::ostream& out) const override {
        out << "SGOAL|" << description << "|" << deadline << "|"
            << exerciseName << "|" << targetWeight << "|" << targetReps
            << "|" << currentBest << "|" << isCompleted << "\n";
    }
};

class WeightGoal : public Goal {
private:
    double startWeight;
    double targetWeight;
    double currentWeight;

public:
    WeightGoal(const std::string& d, const std::string& dl,
               double start, double target)
        : Goal(d, dl), startWeight(start), targetWeight(target),
          currentWeight(start) {}

    double getTargetWeight() const { return targetWeight; }
    double getCurrentWeight() const { return currentWeight; }

    void updateCurrent(double w) {
        currentWeight = w;
        if (std::abs(currentWeight - targetWeight) < 0.5) markCompleted();
    }

    double getProgress() const override {
        double diff = targetWeight - startWeight;
        if (std::abs(diff) < 0.001) return 100;
        double curDiff = currentWeight - startWeight;
        double p = (curDiff / diff) * 100;
        if (p < 0) p = 0;
        if (p > 100) p = 100;
        return p;
    }

    void print() const override {
        std::cout << "[Тегло] " << description
                  << " - " << currentWeight << " / " << targetWeight
                  << " кг (" << (int)getProgress() << "%)";
        if (isCompleted) std::cout << " ЗАВЪРШЕНА";
    }

    void saveTo(std::ostream& out) const override {
        out << "WGOAL|" << description << "|" << deadline << "|"
            << startWeight << "|" << targetWeight << "|"
            << currentWeight << "|" << isCompleted << "\n";
    }
};

#endif