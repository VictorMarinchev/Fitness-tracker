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
    int currentBestReps;
    int bestRepsAtTargetWeight;
    double bestWeightAtTargetReps;
    bool trackWeight;

public:
    StrengthGoal(const std::string& d, const std::string& dl,
                 const std::string& exName, double targetW, int targetR,
                 bool trackW = true)
        : Goal(d, dl), exerciseName(exName), targetWeight(targetW),
          targetReps(targetR), currentBestReps(0),
          bestRepsAtTargetWeight(0), bestWeightAtTargetReps(0),
          trackWeight(trackW) {}

    std::string getExerciseName() const { return exerciseName; }
    double getTargetWeight() const { return targetWeight; }
    int getTargetReps() const { return targetReps; }
    bool getTrackWeight() const { return trackWeight; }

    void setCurrentBest(int reps, double weight) {
        if (!trackWeight) {
            if (reps > currentBestReps) currentBestReps = reps;
            if (currentBestReps >= targetReps) markCompleted();
            return;
        }

        if (weight <= 0) return;
        if (reps > currentBestReps) currentBestReps = reps;
        if (reps >= targetReps && weight > bestWeightAtTargetReps)
            bestWeightAtTargetReps = weight;
        if (weight >= targetWeight && reps > bestRepsAtTargetWeight)
            bestRepsAtTargetWeight = reps;
        if (weight >= targetWeight && reps >= targetReps)
            markCompleted();
    }

    void updateBest(int reps, double weight) {
        setCurrentBest(reps, weight);
    }

    double getProgress() const override {
        if (!trackWeight) {
            if (targetReps <= 0) return 0;
            double p = ((double)currentBestReps / targetReps) * 100;
            if (p > 100) p = 100;
            return p;
        }
        double wProg = targetWeight > 0 ? (bestWeightAtTargetReps / targetWeight) * 100 : 100;
        double rProg = targetReps > 0 ? ((double)bestRepsAtTargetWeight / targetReps) * 100 : 100;
        if (wProg > 100) wProg = 100;
        if (rProg > 100) rProg = 100;
        double p = (wProg + rProg) / 2;
        return p;
    }

    void print() const override {
        std::cout << "[Strength] " << description << " - " << exerciseName;
        if (trackWeight) {
            std::cout << ": " << bestRepsAtTargetWeight << " reps @ " << targetWeight
                      << " kg, best " << bestWeightAtTargetReps << " kg for "
                      << targetReps << " reps (" << (int)getProgress() << "% )";
        } else {
            std::cout << ": " << currentBestReps << " / " << targetReps
                      << " reps (" << (int)getProgress() << "% )";
        }
        if (isCompleted) std::cout << " COMPLETED";
    }

    void saveTo(std::ostream& out) const override {
        out << "SGOAL|" << description << "|" << deadline << "|"
            << exerciseName << "|" << targetWeight << "|" << targetReps
            << "|" << (trackWeight ? "1" : "0") << "|"
            << currentBestReps << "|" << bestRepsAtTargetWeight << "|"
            << bestWeightAtTargetReps << "|" << isCompleted << "\n";
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
        std::cout << "[Weight] " << description
                  << " - " << currentWeight << " / " << targetWeight
                  << " kg (" << (int)getProgress() << "%)";
        if (isCompleted) std::cout << " COMPLETED";
    }

    void saveTo(std::ostream& out) const override {
        out << "WGOAL|" << description << "|" << deadline << "|"
            << startWeight << "|" << targetWeight << "|"
            << currentWeight << "|" << isCompleted << "\n";
    }
};

#endif