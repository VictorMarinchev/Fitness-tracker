#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include "FitnessTracker.h"

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int readInt() {
    int x;
    while (!(std::cin >> x)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid number, try again: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return x;
}

double readDouble() {
    double x;
    while (!(std::cin >> x)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid number, try again: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return x;
}

std::string readLine() {
    std::string s;
    std::getline(std::cin, s);
    return s;
}

std::string normalizeEquipment(std::string eq) {
    std::transform(eq.begin(), eq.end(), eq.begin(), [](unsigned char c){ return std::tolower(c); });
    if (eq == "lever") return "barbell";
    if (eq == "dumbbells") return "dumbbell";
    if (eq == "dumbbell") return "dumbbell";
    if (eq == "machine") return "machine";
    if (eq == "cable") return "cable";
    if (eq == "bodyweight") return "bodyweight";
    return eq;
}

void seedData(FitnessTracker& t) {
    // === Calisthenics (bodyweight) ===
    // Push - pushing movements
    t.addExercise(new StrengthExercise("Push-Up", "Chest", "bodyweight", true));
    t.addExercise(new StrengthExercise("Diamond Push-Up", "Triceps", "bodyweight", true));
    t.addExercise(new StrengthExercise("Pike Push-Up", "Shoulders", "bodyweight", true));
    t.addExercise(new StrengthExercise("Handstand Push-Up", "Shoulders", "bodyweight", true));
    t.addExercise(new StrengthExercise("Archer Push-Up", "Chest", "bodyweight", true));

    // Pull - pulling movements
    t.addExercise(new StrengthExercise("Chin-Up", "Biceps", "bar", true));
    t.addExercise(new StrengthExercise("Muscle-Up", "Back", "bar", true));
    t.addExercise(new StrengthExercise("Australian Pull-Up", "Back", "bar", true));
    t.addExercise(new StrengthExercise("Archer Pull-Up", "Back", "bar", true));

    // Legs
    t.addExercise(new StrengthExercise("Pistol Squat", "Legs", "bodyweight", true));
    t.addExercise(new StrengthExercise("Bulgarian Split Squat", "Legs", "bodyweight", true));
    t.addExercise(new StrengthExercise("Jump Squat", "Legs", "bodyweight", true));
    t.addExercise(new StrengthExercise("Lunges", "Legs", "bodyweight", true));

    // Core
    t.addExercise(new StrengthExercise("L-Sit", "Abs", "bodyweight", true));
    t.addExercise(new StrengthExercise("Hanging Leg Raise", "Abs", "bar", false));
    t.addExercise(new StrengthExercise("Hollow Body Hold", "Abs", "bodyweight", false));
    t.addExercise(new StrengthExercise("Russian Twist", "Abs", "bodyweight", false));

    // Skills - static strength skills
    t.addExercise(new StrengthExercise("Handstand", "Shoulders", "bodyweight", true));
    t.addExercise(new StrengthExercise("Front Lever", "Back", "bar", true));
    t.addExercise(new StrengthExercise("Back Lever", "Back", "bar", true));
    t.addExercise(new StrengthExercise("Human Flag", "Full Body", "pole", true));
    t.addExercise(new StrengthExercise("Planche", "Shoulders", "bodyweight", true));

    // === Compound strength exercises ===
    t.addExercise(new StrengthExercise("Bench Press", "Chest", "barbell", true));
    t.addExercise(new StrengthExercise("Squat", "Legs", "barbell", true));
    t.addExercise(new StrengthExercise("Deadlift", "Back", "barbell", true));
    t.addExercise(new StrengthExercise("Overhead Press", "Shoulders", "barbell", true));
    t.addExercise(new StrengthExercise("Pull-Up", "Back", "bodyweight", true));
    t.addExercise(new StrengthExercise("Dips", "Chest", "bodyweight", true));
    t.addExercise(new StrengthExercise("Bent-Over Row", "Back", "barbell", true));
    t.addExercise(new StrengthExercise("Leg Press", "Legs", "machine", true));

    // === Isolation strength exercises ===
    t.addExercise(new StrengthExercise("Bicep Curl", "Biceps", "dumbbell", false));
    t.addExercise(new StrengthExercise("Tricep Extension", "Triceps", "cable", false));
    t.addExercise(new StrengthExercise("Lateral Raise", "Shoulders", "dumbbell", false));
    t.addExercise(new StrengthExercise("Leg Curl", "Hamstrings", "machine", false));
    t.addExercise(new StrengthExercise("Leg Extension", "Quads", "machine", false));
    t.addExercise(new StrengthExercise("Calf Raise", "Calves", "machine", false));
    t.addExercise(new StrengthExercise("Cable Fly", "Chest", "cable", false));
    t.addExercise(new StrengthExercise("Plank", "Abs", "bodyweight", false));

    // === Cardio exercises ===
    t.addExercise(new CardioExercise("Running", "Legs", true, "moderate"));
    t.addExercise(new CardioExercise("Cycling", "Legs", true, "moderate"));
    t.addExercise(new CardioExercise("Treadmill", "Legs", false, "low"));
    t.addExercise(new CardioExercise("Rowing", "Back", false, "high"));
    t.addExercise(new CardioExercise("Jump Rope", "Full Body", false, "high"));
    t.addExercise(new CardioExercise("Swimming", "Full Body", false, "moderate"));
    t.addExercise(new CardioExercise("Stair Climber", "Legs", false, "moderate"));
}

void listExercises(const FitnessTracker& t, bool pause = true) {
    clearScreen();
    std::cout << "\n============================================\n";
    std::cout << "              EXERCISES (" << t.getExerciseCount() << ")\n";
    std::cout << "============================================\n\n";
    const std::vector<Exercise*>& exs = t.getExercises();
    if (exs.empty()) {
        std::cout << "  No exercises yet.\n\n";
    } else {
        // Build vector of (exercise, original index) and sort by type then name
        std::vector<std::pair<Exercise*, size_t>> items;
        for (size_t i = 0; i < exs.size(); ++i) items.emplace_back(exs[i], i);
        // Sort by original index (ID) so view is ordered by ID
        std::sort(items.begin(), items.end(), [](const auto& a, const auto& b){
            return a.second < b.second;
        });

        std::cout << std::left << std::setw(6) << "#"
                  << std::setw(12) << "Type"
                  << std::setw(28) << "Exercise"
                  << std::setw(12) << "Muscle"
                  << std::setw(12) << "Category"
                  << "Equipment/Info\n";
        std::cout << std::string(100, '-') << "\n";

        for (const auto& p : items) {
            Exercise* ex = p.first;
            size_t orig = p.second;
            std::cout << std::left << std::setw(6) << ("[" + std::to_string(orig) + "]")
                      << std::setw(12) << ex->getType()
                      << std::setw(28) << ex->getName()
                      << std::setw(12) << ex->getMuscleGroup();
            if (StrengthExercise* s = dynamic_cast<StrengthExercise*>(ex)) {
                std::string category = s->getIsCompound() ? "compound" : "isolation";
                std::cout << std::setw(12) << category
                          << s->getEquipment();
            } else if (CardioExercise* c = dynamic_cast<CardioExercise*>(ex)) {
                std::string category = "cardio";
                std::string where = c->getIsOutdoor() ? "outdoor" : "indoor";
                std::cout << std::setw(12) << category
                          << where << " " << c->getIntensity();
            } else {
                std::cout << std::setw(12) << "";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    if (pause) {
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
    }
}

void addExerciseMenu(FitnessTracker& t) {
    clearScreen();
    std::cout << "\n============================================\n";
    std::cout << "           ADD NEW EXERCISE\n";
    std::cout << "============================================\n\n";
    std::cout << "Name: "; std::string name = readLine();
    std::cout << "Muscle group: "; std::string muscle = readLine();
    std::cout << "Type (1=Strength, 2=Cardio): "; int type = readInt();
    if (type == 1) {
        std::cout << "Equipment: "; std::string eq = normalizeEquipment(readLine());
        std::cout << "Compound? (1=yes, 0=no): "; int c = readInt();
        t.addExercise(new StrengthExercise(name, muscle, eq, c == 1));
    } else {
        std::cout << "Outdoor? (1=yes, 0=no): "; int o = readInt();
        std::cout << "Intensity (1=high, 2=mid, 3=low): "; int it = readInt();
        std::string intens;
        if (it == 1) intens = "high";
        else if (it == 2) intens = "moderate";
        else intens = "low";
        t.addExercise(new CardioExercise(name, muscle, o == 1, intens));
    }
    clearScreen();
    std::cout << "\n[OK] Exercise added successfully!\n\n";
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
}

void removeExerciseMenu(FitnessTracker& t) {
    clearScreen();
    std::cout << "\n============================================\n";
    std::cout << "         DELETE EXERCISE\n";
    std::cout << "============================================\n\n";
    listExercises(t, false);
    std::cout << "Index to delete (or -1 to cancel): ";
    int idx = readInt();
    if (idx == -1) return;
    t.removeExercise(idx);
    clearScreen();
    std::cout << "\n[DONE] Exercise deleted successfully!\n\n";
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
}

void addWorkoutMenu(FitnessTracker& t) {
    clearScreen();
    if (t.getExerciseCount() == 0) { 
        std::cout << "\n[ERROR] Add exercises first.\n\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        return; 
    }
    std::cout << "\n============================================\n";
    std::cout << "         ADD NEW WORKOUT\n";
    std::cout << "============================================\n\n";
    std::cout << "Date (YYYY-MM-DD): "; std::string date = readLine();
    std::cout << "Duration (minutes): "; int dur = readInt();
    Workout w(date, dur);

    while (true) {
        clearScreen();
        std::cout << "\n============================================\n";
        std::cout << "        ADD WORKOUT BLOCK\n";
        std::cout << "============================================\n\n";
        std::cout << "(1) Standard Block\n";
        std::cout << "(2) Circuit Block\n";
        std::cout << "(3) Insert Program\n";
        std::cout << "(0) Done\n";
        std::cout << "\nChoice: ";
        int t1 = readInt();
        if (t1 == 0) break;
        if (t1 == 3) {
            if (t.getProgramCount() == 0) {
                std::cout << "No programs available.\n";
                std::cout << "Press Enter to continue..."; std::cin.ignore();
                continue;
            }
            // show brief program list
            const std::vector<WorkoutProgram>& _ps = t.getPrograms();
            for (size_t pi = 0; pi < _ps.size(); ++pi) std::cout << "[" << pi << "] " << _ps[pi].getName() << "\n";
            std::cout << "Program index to insert (-1 to cancel): ";
            int pidx = readInt();
            if (pidx == -1) continue;
            if (pidx < 0 || pidx >= t.getProgramCount()) {
                std::cout << "Invalid index.\n";
                std::cout << "Press Enter to continue..."; std::cin.ignore();
                continue;
            }
            const WorkoutProgram& prog = t.getPrograms()[pidx];
            const std::vector<WorkoutBlock*>& pblocks = prog.getBlocks();
            for (size_t bi = 0; bi < pblocks.size(); ++bi) {
                WorkoutBlock* pb = pblocks[bi];
                if (!pb) continue;
                if (pb->isStandard()) {
                    StandardBlock* sb = (StandardBlock*)pb;
                    Exercise* ex = sb->getExercise();
                    StandardBlock* copy = new StandardBlock(ex);
                    const std::vector<Set>& sets = sb->getSets();
                    for (size_t si = 0; si < sets.size(); ++si) copy->addSet(sets[si]);
                    w.addBlock(copy);
                } else {
                    CircuitBlock* cb = dynamic_cast<CircuitBlock*>(pb);
                    if (!cb) continue;
                    CircuitBlock* ccopy = new CircuitBlock(cb->getRounds());
                    const std::vector<CircuitItem>& items = cb->getItems();
                    for (size_t ii = 0; ii < items.size(); ++ii) {
                        ccopy->addItem(items[ii].exercise, items[ii].reps);
                    }
                    w.addBlock(ccopy);
                }
            }
            clearScreen();
            std::cout << "[OK] Program inserted into workout.\n";
            std::cout << "Press Enter to continue..."; std::cin.ignore();
            continue;
        }
        listExercises(t, false);
        if (t1 == 1) {
            std::cout << "Exercise index: "; int idx = readInt();
            Exercise* ex = t.getExercise(idx);
            if (!ex) { std::cout << "Invalid index.\n"; continue; }
            StandardBlock* sb = new StandardBlock(ex);
            std::cout << "Number of sets: "; int n = readInt();
            std::cout << "Track weight for this block? (1=Yes, 2=No): ";
            int trackChoice = readInt();
            bool trackWeight = (trackChoice == 1);
            for (int i = 0; i < n; i++) {
                std::cout << "Set " << (i+1) << " - reps: "; int r = readInt();
                double we = 0;
                if (trackWeight) {
                    std::cout << "Set " << (i+1) << " - weight (kg): ";
                    we = readDouble();
                }
                sb->addSet(Set(r, we));
            }
            w.addBlock(sb);
            clearScreen();
            std::cout << "\n[OK] Standard block added.\n";
        } else if (t1 == 2) {
            std::cout << "Number of rounds: "; int rounds = readInt();
            CircuitBlock* cb = new CircuitBlock(rounds);
            while (true) {
                std::cout << "Exercise index (-1 = end): "; int idx = readInt();
                if (idx == -1) break;
                Exercise* ex = t.getExercise(idx);
                if (!ex) { std::cout << "Invalid index.\n"; continue; }
                std::cout << "Reps: "; int r = readInt();
                cb->addItem(ex, r);
            }
            w.addBlock(cb);
            clearScreen();
            std::cout << "\n[OK] Circuit block added.\n";
        }
    }

    int prs = t.detectPRs(w);
    t.addWorkout(std::move(w));
    t.updateGoalsFromWorkouts();
    clearScreen();
    if (prs > 0) {
        std::cout << "\n============================================\n";
        std::cout << "     NEW PERSONAL RECORDS DETECTED!\n";
        std::cout << "              " << prs << " new PR(s)!\n";
        std::cout << "============================================\n\n";
    } else {
        std::cout << "\n[OK] Workout saved successfully!\n\n";
    }
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
}

void listWorkouts(const FitnessTracker& t, bool pause = true) {
    clearScreen();
    std::cout << "\n============================================\n";
    std::cout << "              WORKOUTS (" << t.getWorkoutCount() << ")\n";
    std::cout << "============================================\n\n";
    const std::vector<Workout>& ws = t.getWorkouts();
    if (ws.empty()) {
        std::cout << "  No workouts yet.\n\n";
    } else {
        for (size_t i = 0; i < ws.size(); i++) {
            std::cout << "\n[" << i << "] ";
            ws[i].print();
        }
        std::cout << "\n";
    }
    if (pause) {
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
    }
}

void removeWorkoutMenu(FitnessTracker& t) {
    clearScreen();
    std::cout << "\n============================================\n";
    std::cout << "         DELETE WORKOUT\n";
    std::cout << "============================================\n\n";
    if (t.getWorkoutCount() == 0) {
        std::cout << "  No workouts available.\n\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        return;
    }
    listWorkouts(t, false);
    std::cout << "Index to delete (or -1 to cancel): ";
    int idx = readInt();
    if (idx == -1) return;
    if (idx < 0 || idx >= t.getWorkoutCount()) {
        clearScreen();
        std::cout << "\n[ERROR] Invalid index.\n\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        return;
    }
    t.removeWorkout(idx);
    clearScreen();
    std::cout << "\n[DONE] Workout deleted successfully!\n\n";
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
}

void addProgramMenu(FitnessTracker& t) {
    clearScreen();
    if (t.getExerciseCount() == 0) { 
        std::cout << "\n[ERROR] Add exercises first.\n\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        return; 
    }
    std::cout << "\n============================================\n";
    std::cout << "          ADD NEW PROGRAM\n";
    std::cout << "============================================\n\n";
    std::cout << "Program name: "; std::string n = readLine();
    clearScreen();
    std::cout << "\n============================================\n";
    std::cout << "        ADD PROGRAM BLOCKS\n";
    std::cout << "============================================\n\n";
    WorkoutProgram p(n);

    while (true) {
        clearScreen();
        std::cout << "\n============================================\n";
        std::cout << "        ADD PROGRAM BLOCK\n";
        std::cout << "============================================\n\n";
        std::cout << "(1) Standard Block\n";
        std::cout << "(2) Circuit Block\n";
        std::cout << "(0) Done\n";
        std::cout << "\nChoice: ";
        int t1 = readInt();
        if (t1 == 0) break;
        listExercises(t, false);
        if (t1 == 1) {
            std::cout << "Exercise index: "; int idx = readInt();
            Exercise* ex = t.getExercise(idx);
            if (!ex) { std::cout << "Invalid index.\n"; std::cout << "Press Enter to continue..."; std::cin.ignore(); continue; }
            StandardBlock* sb = new StandardBlock(ex);
            std::cout << "Number of sets: "; int nsets = readInt();
            std::cout << "Track weight for this block? (1=Yes, 2=No): ";
            int trackChoice = readInt();
            bool trackWeight = (trackChoice == 1);
            for (int i = 0; i < nsets; i++) {
                std::cout << "Set " << (i+1) << " - reps: "; int r = readInt();
                double we = 0;
                if (trackWeight) {
                    std::cout << "Set " << (i+1) << " - weight (kg): ";
                    we = readDouble();
                }
                sb->addSet(Set(r, we));
            }
            p.addBlock(sb);
            clearScreen();
            std::cout << "\n[OK] Standard block added to program.\n";
            std::cout << "Press Enter to continue..."; std::cin.ignore();
        } else if (t1 == 2) {
            std::cout << "Number of rounds: "; int rounds = readInt();
            CircuitBlock* cb = new CircuitBlock(rounds);
            while (true) {
                std::cout << "Exercise index (-1 = end): "; int idx = readInt();
                if (idx == -1) break;
                Exercise* ex = t.getExercise(idx);
                if (!ex) { std::cout << "Invalid index.\n"; continue; }
                std::cout << "Reps: "; int r = readInt();
                cb->addItem(ex, r);
            }
            p.addBlock(cb);
            clearScreen();
            std::cout << "\n[OK] Circuit block added to program.\n";
            std::cout << "Press Enter to continue..."; std::cin.ignore();
        }
    }

    t.addProgram(std::move(p));
    clearScreen();
    std::cout << "\n[OK] Program added successfully!\n\n";
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
}

void listPrograms(const FitnessTracker& t, bool pause = true) {
    clearScreen();
    std::cout << "\n============================================\n";
    std::cout << "            PROGRAMS (" << t.getProgramCount() << ")\n";
    std::cout << "============================================\n\n";
    const std::vector<WorkoutProgram>& ps = t.getPrograms();
    if (ps.empty()) {
        std::cout << "  No programs yet.\n\n";
    } else {
        for (size_t i = 0; i < ps.size(); i++) {
            std::cout << "[" << i << "] "; ps[i].print();
        }
        std::cout << "\n";
    }
    if (pause) {
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
    }
}

void removeProgramMenu(FitnessTracker& t) {
    clearScreen();
    std::cout << "\n============================================\n";
    std::cout << "        DELETE PROGRAM\n";
    std::cout << "============================================\n\n";
    if (t.getProgramCount() == 0) {
        std::cout << "  No programs available.\n\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        return;
    }
    listPrograms(t, false);
    std::cout << "Index to delete (or -1 to cancel): ";
    int idx = readInt();
    if (idx == -1) return;
    if (idx < 0 || idx >= t.getProgramCount()) {
        clearScreen();
        std::cout << "\n[ERROR] Invalid index.\n\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        return;
    }
    t.removeProgram(idx);
    clearScreen();
    std::cout << "\n[DONE] Program deleted successfully!\n\n";
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
}

void addGoalMenu(FitnessTracker& t) {
    clearScreen();
    std::cout << "\n============================================\n";
    std::cout << "            ADD NEW GOAL\n";
    std::cout << "============================================\n\n";
    std::cout << "Description: "; std::string d = readLine();
    std::cout << "Deadline (YYYY-MM-DD): "; std::string dl = readLine();
    std::cout << "Type (1=Strength, 2=Weight): "; int type = readInt();
    if (type == 1) {
        clearScreen();
        std::cout << "\n============================================\n";
        std::cout << "         SELECT EXERCISE FOR GOAL\n";
        std::cout << "============================================\n\n";
        listExercises(t, false);
        std::cout << "\nExercise index: "; int idx = readInt();
        Exercise* ex = t.getExercise(idx);
        if (!ex) {
            clearScreen();
            std::cout << "\n[ERROR] Invalid index. Goal not added.\n\n";
            std::cout << "Press Enter to continue...";
            std::cin.ignore();
            return;
        }
        std::string en = ex->getName();
        std::cout << "Track weight for this goal? (1=Yes, 2=No): "; int track = readInt();
        bool trackWeight = (track == 1);
        double tw = 0;
        if (trackWeight) {
            std::cout << "Target weight (kg): "; tw = readDouble();
        }
        std::cout << "Target reps: "; int tr = readInt();
        t.addGoal(new StrengthGoal(d, dl, en, tw, tr, trackWeight));
    } else {
        std::cout << "Starting weight (kg): "; double sw = readDouble();
        std::cout << "Target weight (kg): "; double tw = readDouble();
        t.addGoal(new WeightGoal(d, dl, sw, tw));
    }
    t.updateGoalsFromWorkouts();
    clearScreen();
    std::cout << "\n[OK] Goal added successfully!\n\n";
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
}

void listGoals(const FitnessTracker& t, bool pause = true) {
    clearScreen();
    std::cout << "\n============================================\n";
    std::cout << "               GOALS (" << t.getGoalCount() << ")\n";
    std::cout << "============================================\n\n";
    const std::vector<Goal*>& gs = t.getGoals();
    if (gs.empty()) {
        std::cout << "  No goals yet.\n\n";
    } else {
        for (size_t i = 0; i < gs.size(); i++) {
            std::cout << "  [" << i << "] "; gs[i]->print(); std::cout << "\n";
        }
        std::cout << "\n";
    }
    if (pause) {
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
    }
}

void removeGoalMenu(FitnessTracker& t) {
    clearScreen();
    std::cout << "\n============================================\n";
    std::cout << "          DELETE GOAL\n";
    std::cout << "============================================\n\n";
    if (t.getGoalCount() == 0) {
        std::cout << "  No goals available.\n\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        return;
    }
    listGoals(t, false);
    std::cout << "Index to delete (or -1 to cancel): ";
    int idx = readInt();
    if (idx == -1) return;
    if (idx < 0 || idx >= t.getGoalCount()) {
        clearScreen();
        std::cout << "\n[ERROR] Invalid index.\n\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        return;
    }
    t.removeGoal(idx);
    clearScreen();
    std::cout << "\n[DONE] Goal deleted successfully!\n\n";
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
}

void updateWeightMenu(FitnessTracker& t) {
    clearScreen();
    if (t.getGoalCount() == 0) {
        std::cout << "\n[ERROR] No goals available.\n\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        return;
    }

    listGoals(t, false);
    std::cout << "\nIndex of weight goal: "; int idx = readInt();
    if (idx < 0 || idx >= (int)t.getGoals().size()) {
        std::cout << "Invalid index.\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        return;
    }

    WeightGoal* wg = dynamic_cast<WeightGoal*>(t.getGoals()[idx]);
    if (!wg) {
        std::cout << "This goal is not a weight goal.\n";
        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        return;
    }

    std::cout << "New current weight (kg): "; double w = readDouble();
    wg->updateCurrent(w);
    clearScreen();
    std::cout << "\nUpdated.\n\n";
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
}

void searchMenu(const FitnessTracker& t) {
    clearScreen();
    std::cout << "\n============================================\n";
    std::cout << "         SEARCH EXERCISES\n";
    std::cout << "============================================\n\n";
    std::cout << "Search term: "; std::string q = readLine();
    clearScreen();
    std::cout << "\n============================================\n";
    std::cout << "         SEARCH RESULTS\n";
    std::cout << "============================================\n\n";
    std::vector<Exercise*> r = t.searchExercises(q);
    if (r.empty()) {
        std::cout << "  No results found.\n\n";
    } else {
        std::cout << "Found " << r.size() << " exercise(s):\n\n";
        for (size_t i = 0; i < r.size(); i++) {
            std::cout << "  "; r[i]->print(); std::cout << "\n";
        }
        std::cout << "\n";
    }
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
}

void progressMenu(const FitnessTracker& t) {
    clearScreen();
    std::cout << "\n============================================\n";
    std::cout << "         EXERCISE PROGRESS\n";
    std::cout << "============================================\n\n";
    std::cout << "Exercise name: "; std::string n = readLine();
    clearScreen();
    t.showProgress(n);
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
}

void printMenu() {
    clearScreen();
    std::cout << "\n";
    std::cout << "============================================\n";
    std::cout << "          FITNESS TRACKER MENU\n";
    std::cout << "============================================\n\n";
    std::cout << "  EXERCISES\n";
    std::cout << "    1. View\n";
    std::cout << "    2. Add\n";
    std::cout << "    3. Delete\n\n";
    std::cout << "  WORKOUTS\n";
    std::cout << "    4. New\n";
    std::cout << "    5. View\n";
    std::cout << "    6. Delete\n";
    std::cout << "    7. Progress\n";
    std::cout << "    8. Summary\n\n";
    std::cout << "  PROGRAMS\n";
    std::cout << "    9. View\n";
    std::cout << "   10. Add\n";
    std::cout << "   11. Delete\n\n";
    std::cout << "  GOALS\n";
    std::cout << "   12. View\n";
    std::cout << "   13. Add\n";
    std::cout << "   14. Delete\n";
    std::cout << "   15. Update current weight\n\n";
    std::cout << "  OTHER\n";
    std::cout << "   16. Search\n";
    std::cout << "   17. Save to File\n";
    std::cout << "   18. Load from File\n";
    std::cout << "    0. Exit\n\n";
    std::cout << "============================================\n";
    std::cout << "Choice: ";
}

int main() {
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    FitnessTracker t;
    seedData(t);
    
    while (true) {
        printMenu();
        int c = readInt();
        switch (c) {
            case 1: listExercises(t); break;
            case 2: addExerciseMenu(t); break;
            case 3: removeExerciseMenu(t); break;
            case 4: addWorkoutMenu(t); break;
            case 5: listWorkouts(t); break;
            case 6: removeWorkoutMenu(t); break;
            case 7: progressMenu(t); break;
            case 8: 
                clearScreen();
                std::cout << "\n============================================\n";
                std::cout << "            SUMMARY\n";
                std::cout << "============================================\n";
                t.showSummary();
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                break;
            case 9: listPrograms(t); break;
            case 10: addProgramMenu(t); break;
            case 11: removeProgramMenu(t); break;
            case 12: listGoals(t); break;
            case 13: addGoalMenu(t); break;
            case 14: removeGoalMenu(t); break;
            case 15: updateWeightMenu(t); break;
            case 16: searchMenu(t); break;
            case 17:
                if (t.saveToFile("data.txt")) {
                    clearScreen();
                    std::cout << "\n[OK] Data saved to data.txt\n\n";
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();
                }
                else {
                    clearScreen();
                    std::cout << "\n[ERROR] Error saving file.\n\n";
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();
                }
                break;
            case 18:
                if (t.loadFromFile("data.txt")) {
                    clearScreen();
                    std::cout << "\n[OK] Data loaded from data.txt\n\n";
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();
                }
                else {
                    clearScreen();
                    std::cout << "\n[ERROR] Error loading file.\n\n";
                    std::cout << "Press Enter to continue...";
                    std::cin.ignore();
                }
                break;
            case 0:
                clearScreen();
                std::cout << "\n============================================\n";
                std::cout << "     Thank you for using Fitness Tracker!\n";
                std::cout << "              Goodbye!\n";
                std::cout << "============================================\n\n";
                return 0;
            default:
                clearScreen();
                std::cout << "\n[ERROR] Invalid choice. Please try again.\n\n";
                std::cout << "Press Enter to continue...";
                std::cin.ignore();
        }
    }
}