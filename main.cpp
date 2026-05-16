#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
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

void seedData(FitnessTracker& t) {
    t.addExercise(new StrengthExercise("Leg Press", "Chest", "lever", true));
    t.addExercise(new StrengthExercise("Squat", "Legs", "lever", true));
    t.addExercise(new StrengthExercise("Bicep Curl", "Biceps", "dumbbells", false));
    t.addExercise(new CardioExercise("Running", "Legs", true, "moderate"));
    t.addExercise(new CardioExercise("Cycling", "Legs", true, "low"));

    Workout w("2026-05-10", 45);
    StandardBlock* sb = new StandardBlock(t.findExercise("Leg Press"));
    sb->addSet(Set(8, 75));
    sb->addSet(Set(8, 77.5));
    w.addBlock(sb);
    t.addWorkout(std::move(w));

    t.addGoal(new StrengthGoal("Leg Press 100 kg", "2026-08-01", "Leg Press", 100, 5));
}

void listExercises(const FitnessTracker& t) {
    clearScreen();
    std::cout << "\n============================================\n";
    std::cout << "              EXERCISES (" << t.getExerciseCount() << ")\n";
    std::cout << "============================================\n\n";
    const std::vector<Exercise*>& exs = t.getExercises();
    if (exs.empty()) {
        std::cout << "  No exercises yet.\n\n";
    } else {
        for (size_t i = 0; i < exs.size(); i++) {
            std::cout << "  [" << i << "] ";
            exs[i]->print();
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
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
        std::cout << "Equipment: "; std::string eq = readLine();
        std::cout << "Compound? (1=yes, 0=no): "; int c = readInt();
        t.addExercise(new StrengthExercise(name, muscle, eq, c == 1));
    } else {
        std::cout << "Outdoor? (1=yes, 0=no): "; int o = readInt();
        std::cout << "Intensity: "; std::string intens = readLine();
        t.addExercise(new CardioExercise(name, muscle, o == 1, intens));
    }
    clearScreen();
    std::cout << "\n[OK] Exercise added successfully!\n\n";
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
}

void removeExerciseMenu(FitnessTracker& t) {
    clearScreen();
    listExercises(t);
    clearScreen();
    std::cout << "\n============================================\n";
    std::cout << "         DELETE EXERCISE\n";
    std::cout << "============================================\n\n";
    listExercises(t);
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
        std::cout << "(0) Done\n";
        std::cout << "\nChoice: ";
        int t1 = readInt();
        if (t1 == 0) break;
        clearScreen();
        listExercises(t);
        clearScreen();
        if (t1 == 1) {
            std::cout << "Exercise index: "; int idx = readInt();
            Exercise* ex = t.getExercise(idx);
            if (!ex) { std::cout << "Invalid index.\n"; continue; }
            StandardBlock* sb = new StandardBlock(ex);
            std::cout << "Number of sets: "; int n = readInt();
            for (int i = 0; i < n; i++) {
                std::cout << "Set " << (i+1) << " - reps: "; int r = readInt();
                std::cout << "Set " << (i+1) << " - weight (kg): "; double we = readDouble();
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

void listWorkouts(const FitnessTracker& t) {
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
    std::cout << "       SELECT EXERCISES (-1 to end)\n";
    std::cout << "============================================\n\n";
    WorkoutProgram p(n);
    listExercises(t);
    clearScreen();
    while (true) {
        std::cout << "\nExercise index (-1 = end): "; int idx = readInt();
        if (idx == -1) break;
        Exercise* ex = t.getExercise(idx);
        if (ex) {
            p.addExercise(ex);
            std::cout << "[OK] Added: " << ex->getName() << "\n";
        } else {
            std::cout << "[ERROR] Invalid index.\n";
        }
    }
    t.addProgram(p);
    clearScreen();
    std::cout << "\n[OK] Program added successfully!\n\n";
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
}

void listPrograms(const FitnessTracker& t) {
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
    }
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
        std::cout << "Exercise name: "; std::string en = readLine();
        std::cout << "Target weight (kg): "; double tw = readDouble();
        std::cout << "Target reps: "; int tr = readInt();
        t.addGoal(new StrengthGoal(d, dl, en, tw, tr));
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

void listGoals(const FitnessTracker& t) {
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
    std::cout << "    6. Progress\n";
    std::cout << "    7. Summary\n\n";
    std::cout << "  PROGRAMS\n";
    std::cout << "    8. View\n";
    std::cout << "    9. Add\n\n";
    std::cout << "  GOALS\n";
    std::cout << "   10. View\n";
    std::cout << "   11. Add\n\n";
    std::cout << "  OTHER\n";
    std::cout << "   12. Search\n";
    std::cout << "   13. Save to File\n";
    std::cout << "   14. Load from File\n";
    std::cout << "    0. Exit\n\n";
    std::cout << "============================================\n";
    std::cout << "Choice: ";
}

int main() {
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
            case 6: progressMenu(t); break;
            case 7: 
                clearScreen();
                std::cout << "\n============================================\n";
                std::cout << "            SUMMARY\n";
                std::cout << "============================================\n";
                t.showSummary();
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                break;
            case 8: listPrograms(t); break;
            case 9: addProgramMenu(t); break;
            case 10: listGoals(t); break;
            case 11: addGoalMenu(t); break;
            case 12: searchMenu(t); break;
            case 13:
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
            case 14:
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