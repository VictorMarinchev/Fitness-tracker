#ifndef FITNESS_TRACKER_H
#define FITNESS_TRACKER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Exercise.h"
#include "WorkoutBlock.h"
#include "Goal.h"
#include "WorkoutProgram.h"

class FitnessTracker {
private:
    std::vector<Exercise*> exercises;
    std::vector<Workout> workouts;
    std::vector<WorkoutProgram> programs;
    std::vector<Goal*> goals;

public:
    FitnessTracker() {}

    ~FitnessTracker() {
        for (size_t i = 0; i < exercises.size(); i++) delete exercises[i];
        for (size_t i = 0; i < goals.size(); i++) delete goals[i];
    }

    // CRUD exercises
    void addExercise(Exercise* ex) { exercises.push_back(ex); }

    void removeExercise(int idx) {
        if (idx < 0 || idx >= (int)exercises.size()) return;
        Exercise* ex = exercises[idx];

        // Remove references from programs
        for (size_t i = 0; i < programs.size(); i++) programs[i].removeExercise(ex);

        // Remove references from workouts (blocks/items)
        for (size_t i = 0; i < workouts.size(); i++) workouts[i].removeBlocksWithExercise(ex);

        // Finally delete the exercise
        delete ex;
        exercises.erase(exercises.begin() + idx);
    }

    Exercise* getExercise(int idx) {
        if (idx < 0 || idx >= (int)exercises.size()) return nullptr;
        return exercises[idx];
    }

    Exercise* findExercise(const std::string& name) {
        for (size_t i = 0; i < exercises.size(); i++)
            if (exercises[i]->getName() == name) return exercises[i];
        return nullptr;
    }

    int getExerciseCount() const { return exercises.size(); }
    const std::vector<Exercise*>& getExercises() const { return exercises; }

    // CRUD workouts
    void addWorkout(Workout w) { workouts.push_back(std::move(w)); }

    void removeWorkout(int idx) {
        if (idx < 0 || idx >= (int)workouts.size()) return;
        workouts.erase(workouts.begin() + idx);
    }

    int getWorkoutCount() const { return workouts.size(); }
    const std::vector<Workout>& getWorkouts() const { return workouts; }
    Workout& getWorkout(int idx) { return workouts[idx]; }

    // Feature 3: PR detection
    int detectPRs(Workout& w) {
        int newPRs = 0;
        const std::vector<WorkoutBlock*>& blocks = w.getBlocks();
        for (size_t i = 0; i < blocks.size(); i++) {
            if (!blocks[i]->isStandard()) continue;
            StandardBlock* sb = (StandardBlock*)blocks[i];
            Exercise* ex = sb->getExercise();

            double maxWeight = 0;
            for (size_t j = 0; j < workouts.size(); j++) {
                const std::vector<WorkoutBlock*>& pb = workouts[j].getBlocks();
                for (size_t k = 0; k < pb.size(); k++) {
                    if (!pb[k]->isStandard()) continue;
                    StandardBlock* psb = (StandardBlock*)pb[k];
                    if (psb->getExercise() != ex) continue;
                    const std::vector<Set>& sets = psb->getSets();
                    for (size_t s = 0; s < sets.size(); s++)
                        if (sets[s].getWeight() > maxWeight) maxWeight = sets[s].getWeight();
                }
            }

            std::vector<Set>& newSets = sb->getSetsMutable();
            for (size_t s = 0; s < newSets.size(); s++) {
                if (newSets[s].getWeight() > maxWeight) {
                    newSets[s].markAsPR();
                    maxWeight = newSets[s].getWeight();
                    newPRs++;
                }
            }
        }
        return newPRs;
    }

    // Feature 4: Exercise progress
    void showProgress(const std::string& exerciseName) const {
        std::cout << "\nProgress for: " << exerciseName << "\n";
        bool found = false;
        for (size_t i = 0; i < workouts.size(); i++) {
            const std::vector<WorkoutBlock*>& blocks = workouts[i].getBlocks();
            double maxW = 0;
            for (size_t j = 0; j < blocks.size(); j++) {
                if (!blocks[j]->isStandard()) continue;
                StandardBlock* sb = (StandardBlock*)blocks[j];
                if (sb->getExercise()->getName() != exerciseName) continue;
                const std::vector<Set>& sets = sb->getSets();
                for (size_t s = 0; s < sets.size(); s++)
                    if (sets[s].getWeight() > maxW) maxW = sets[s].getWeight();
            }
            if (maxW > 0) {
                std::cout << "  " << workouts[i].getDate()
                          << " - max weight: " << maxW << " kg\n";
                found = true;
            }
        }
        if (!found) std::cout << "  No records.\n";
    }

    // Feature 5: Summary
    void showSummary() const {
        std::cout << "\nSummary:\n";
        std::cout << "  Number of workouts: " << workouts.size() << "\n";
        double totalVolume = 0;
        int totalDuration = 0, totalPRs = 0;
        for (size_t i = 0; i < workouts.size(); i++) {
            totalVolume += workouts[i].getTotalVolume();
            totalDuration += workouts[i].getDuration();
            const std::vector<WorkoutBlock*>& blocks = workouts[i].getBlocks();
            for (size_t j = 0; j < blocks.size(); j++) {
                if (!blocks[j]->isStandard()) continue;
                StandardBlock* sb = (StandardBlock*)blocks[j];
                const std::vector<Set>& sets = sb->getSets();
                for (size_t s = 0; s < sets.size(); s++)
                    if (sets[s].getIsPR()) totalPRs++;
            }
        }
        std::cout << "  Total volume: " << totalVolume << "\n";
        std::cout << "  Total time: " << totalDuration << " min\n";
        std::cout << "  Number of PRs: " << totalPRs << "\n";
        if (workouts.size() > 0)
            std::cout << "  Average duration: "
                      << (totalDuration / (int)workouts.size()) << " min\n";
    }

    // CRUD programs
    void addProgram(WorkoutProgram&& p) { programs.push_back(std::move(p)); }
    int getProgramCount() const { return programs.size(); }
    const std::vector<WorkoutProgram>& getPrograms() const { return programs; }

    void removeProgram(int idx) {
        if (idx < 0 || idx >= (int)programs.size()) return;
        programs.erase(programs.begin() + idx);
    }

    // CRUD goals
    void addGoal(Goal* g) { goals.push_back(g); }
    int getGoalCount() const { return goals.size(); }
    const std::vector<Goal*>& getGoals() const { return goals; }

    void removeGoal(int idx) {
        if (idx < 0 || idx >= (int)goals.size()) return;
        delete goals[idx];
        goals.erase(goals.begin() + idx);
    }

    void updateGoalsFromWorkouts() {
        for (size_t g = 0; g < goals.size(); g++) {
            StrengthGoal* sg = dynamic_cast<StrengthGoal*>(goals[g]);
            if (!sg) continue;
            for (size_t i = 0; i < workouts.size(); i++) {
                const std::vector<WorkoutBlock*>& blocks = workouts[i].getBlocks();
                for (size_t j = 0; j < blocks.size(); j++) {
                    if (!blocks[j]->isStandard()) continue;
                    StandardBlock* sb = (StandardBlock*)blocks[j];
                    if (sb->getExercise()->getName() != sg->getExerciseName()) continue;
                    const std::vector<Set>& sets = sb->getSets();
                    for (size_t s = 0; s < sets.size(); s++)
                        sg->updateBest(sets[s].getReps(), sets[s].getWeight());
                }
            }
        }
    }

    // Feature 8: Search
    std::vector<Exercise*> searchExercises(const std::string& query) const {
        std::vector<Exercise*> results;
        if (query.empty()) return results;
        for (size_t i = 0; i < exercises.size(); i++) {
            if (exercises[i]->getName().find(query) != std::string::npos ||
                exercises[i]->getMuscleGroup().find(query) != std::string::npos ||
                exercises[i]->getType().find(query) != std::string::npos)
                results.push_back(exercises[i]);
        }
        return results;
    }

    // Feature 9: Save to file
    bool saveToFile(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out) return false;

        // Exercises
        out << exercises.size() << "\n";
        for (size_t i = 0; i < exercises.size(); i++) exercises[i]->saveTo(out);

        // Programs
        out << programs.size() << "\n";
        for (size_t i = 0; i < programs.size(); i++) programs[i].saveTo(out);

        // Goals
        out << goals.size() << "\n";
        for (size_t i = 0; i < goals.size(); i++) goals[i]->saveTo(out);

        // Workouts
        out << workouts.size() << "\n";
        for (size_t i = 0; i < workouts.size(); i++) {
            out << "WORKOUT|" << workouts[i].getDate() << "|"
                << workouts[i].getDuration() << "|"
                << workouts[i].getBlocks().size() << "\n";
            const std::vector<WorkoutBlock*>& blocks = workouts[i].getBlocks();
            for (size_t j = 0; j < blocks.size(); j++) blocks[j]->saveTo(out);
        }

        return true;
    }

    // Feature 9: Load from file
    bool loadFromFile(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) return false;

        // Clear everything
        for (size_t i = 0; i < exercises.size(); i++) delete exercises[i];
        exercises.clear();
        for (size_t i = 0; i < goals.size(); i++) delete goals[i];
        goals.clear();
        programs.clear();
        workouts.clear();

        // Helper function for splitting by '|'
        auto split = [](const std::string& line) {
            std::vector<std::string> parts;
            std::string cur;
            for (size_t j = 0; j < line.size(); j++) {
                if (line[j] == '|') { parts.push_back(cur); cur.clear(); }
                else cur += line[j];
            }
            if (!cur.empty()) parts.push_back(cur);
            return parts;
        };

        int n;
        std::string line;

        // === Exercises ===
        in >> n; in.ignore();
        for (int i = 0; i < n; i++) {
            std::getline(in, line);
            std::vector<std::string> parts = split(line);
            if (parts.size() < 5) continue;
            if (parts[0] == "STRENGTH")
                exercises.push_back(new StrengthExercise(
                    parts[1], parts[2], parts[3], parts[4] == "1"));
            else if (parts[0] == "CARDIO")
                exercises.push_back(new CardioExercise(
                    parts[1], parts[2], parts[3] == "1", parts[4]));
        }

        // === Programs ===
        in >> n; in.ignore();
        for (int i = 0; i < n; i++) {
            std::getline(in, line);
            std::vector<std::string> parts = split(line);
            if (parts.size() < 3) continue;
            WorkoutProgram p(parts[1]);
            int blockCount = std::stoi(parts[2]);
            for (int b = 0; b < blockCount; b++) {
                std::getline(in, line);
                std::vector<std::string> bp = split(line);
                if (bp.empty()) continue;

                if (bp[0] == "STD" && bp.size() >= 3) {
                    Exercise* ex = findExercise(bp[1]);
                    if (!ex) continue;
                    StandardBlock* sb = new StandardBlock(ex);
                    int setCnt = std::stoi(bp[2]);
                    for (int s = 0; s < setCnt; s++) {
                        std::getline(in, line);
                        std::vector<std::string> sp = split(line);
                        if (sp.size() < 4) continue;
                        Set st(std::stoi(sp[0]), std::stod(sp[1]), std::stoi(sp[2]));
                        if (sp[3] == "1") st.markAsPR();
                        sb->addSet(st);
                    }
                    p.addBlock(sb);
                } else if (bp[0] == "CIRC" && bp.size() >= 3) {
                    int rounds = std::stoi(bp[1]);
                    int itemCnt = std::stoi(bp[2]);
                    CircuitBlock* cb = new CircuitBlock(rounds);
                    for (int it = 0; it < itemCnt; it++) {
                        std::getline(in, line);
                        std::vector<std::string> ip = split(line);
                        if (ip.size() < 2) continue;
                        Exercise* ex = findExercise(ip[0]);
                        if (ex) cb->addItem(ex, std::stoi(ip[1]));
                    }
                    p.addBlock(cb);
                }
            }
            programs.push_back(std::move(p));
        }

        // === Goals ===
        in >> n; in.ignore();
        for (int i = 0; i < n; i++) {
            std::getline(in, line);
            std::vector<std::string> parts = split(line);
            if (parts.size() < 7) continue;
            if (parts[0] == "SGOAL") {
                bool trackWeight = true;
                int currentBestReps = 0;
                int bestRepsAtTargetWeight = 0;
                double bestWeightAtTargetReps = 0;
                bool completed = false;
                if (parts.size() >= 11) {
                    trackWeight = (parts[6] == "1");
                    currentBestReps = std::stoi(parts[7]);
                    bestRepsAtTargetWeight = std::stoi(parts[8]);
                    bestWeightAtTargetReps = std::stod(parts[9]);
                    completed = (parts[10] == "1");
                } else if (parts.size() == 10) {
                    double targetWeight = std::stod(parts[4]);
                    trackWeight = (targetWeight > 0);
                    if (trackWeight) {
                        currentBestReps = std::stoi(parts[6]);
                        bestRepsAtTargetWeight = std::stoi(parts[7]);
                        bestWeightAtTargetReps = std::stod(parts[8]);
                    } else {
                        currentBestReps = std::stoi(parts[6]);
                    }
                    completed = (parts[9] == "1");
                }
                StrengthGoal* sg = new StrengthGoal(
                    parts[1], parts[2], parts[3],
                    std::stod(parts[4]), std::stoi(parts[5]), trackWeight);
                if (trackWeight) {
                    sg->setCurrentBest(currentBestReps, bestWeightAtTargetReps);
                    sg->setCurrentBest(bestRepsAtTargetWeight, bestWeightAtTargetReps);
                } else {
                    sg->setCurrentBest(currentBestReps, 0);
                }
                if (completed) sg->markCompleted();
                goals.push_back(sg);
            } else if (parts[0] == "WGOAL") {
                WeightGoal* wg = new WeightGoal(
                    parts[1], parts[2],
                    std::stod(parts[3]), std::stod(parts[4]));
                wg->updateCurrent(std::stod(parts[5]));
                if (parts.size() > 6 && parts[6] == "1") wg->markCompleted();
                goals.push_back(wg);
            }
        }

        // === Workouts ===
        in >> n; in.ignore();
        for (int i = 0; i < n; i++) {
            std::getline(in, line);
            std::vector<std::string> parts = split(line);
            if (parts.size() < 4 || parts[0] != "WORKOUT") continue;
            Workout w(parts[1], std::stoi(parts[2]));
            int blockCount = std::stoi(parts[3]);

            for (int b = 0; b < blockCount; b++) {
                std::getline(in, line);
                std::vector<std::string> bp = split(line);
                if (bp.empty()) continue;

                if (bp[0] == "STD" && bp.size() >= 3) {
                    Exercise* ex = findExercise(bp[1]);
                    if (!ex) continue;
                    StandardBlock* sb = new StandardBlock(ex);
                    int setCnt = std::stoi(bp[2]);
                    for (int s = 0; s < setCnt; s++) {
                        std::getline(in, line);
                        std::vector<std::string> sp = split(line);
                        if (sp.size() < 4) continue;
                        Set st(std::stoi(sp[0]), std::stod(sp[1]), std::stoi(sp[2]));
                        if (sp[3] == "1") st.markAsPR();
                        sb->addSet(st);
                    }
                    w.addBlock(sb);
                } else if (bp[0] == "CIRC" && bp.size() >= 3) {
                    int rounds = std::stoi(bp[1]);
                    int itemCnt = std::stoi(bp[2]);
                    CircuitBlock* cb = new CircuitBlock(rounds);
                    for (int it = 0; it < itemCnt; it++) {
                        std::getline(in, line);
                        std::vector<std::string> ip = split(line);
                        if (ip.size() < 2) continue;
                        Exercise* ex = findExercise(ip[0]);
                        if (ex) cb->addItem(ex, std::stoi(ip[1]));
                    }
                    w.addBlock(cb);
                }
            }
            workouts.push_back(std::move(w));
        }

        return true;
    }
};

#endif