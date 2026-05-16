#include <iostream>
#include <string>
#include <limits>
#include "FitnessTracker.h"

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
    std::cout << "\nExercises: " << t.getExerciseCount() << "\n";
    const std::vector<Exercise*>& exs = t.getExercises();
    for (size_t i = 0; i < exs.size(); i++) {
        std::cout << "  [" << i << "] ";
        exs[i]->print();
        std::cout << "\n";
    }
}

void addExerciseMenu(FitnessTracker& t) {
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
    std::cout << "Added.\n";
}

void removeExerciseMenu(FitnessTracker& t) {
    listExercises(t);
    std::cout << "Index to delete: "; int idx = readInt();
    t.removeExercise(idx);
    std::cout << "Deleted.\n";
}

void addWorkoutMenu(FitnessTracker& t) {
    if (t.getExerciseCount() == 0) { std::cout << "Add exercises first.\n"; return; }
    std::cout << "Date: "; std::string date = readLine();
    std::cout << "Duration (minutes): "; int dur = readInt();
    Workout w(date, dur);

    while (true) {
        std::cout << "\nAdd block (1=Standard, 2=Circuit, 0=Done): ";
        int t1 = readInt();
        if (t1 == 0) break;
        listExercises(t);
        if (t1 == 1) {
            std::cout << "Exercise index: "; int idx = readInt();
            Exercise* ex = t.getExercise(idx);
            if (!ex) { std::cout << "Invalid index.\n"; continue; }
            StandardBlock* sb = new StandardBlock(ex);
            std::cout << "Number of sets: "; int n = readInt();
            for (int i = 0; i < n; i++) {
                std::cout << "Set " << (i+1) << " - reps: "; int r = readInt();
                std::cout << "Set " << (i+1) << " - weight: "; double we = readDouble();
                sb->addSet(Set(r, we));
            }
            w.addBlock(sb);
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
        }
    }

    int prs = t.detectPRs(w);
    if (prs > 0) std::cout << "*** Found " << prs << " new PRs! ***\n";
    t.addWorkout(std::move(w));
    t.updateGoalsFromWorkouts();
    std::cout << "Workout saved.\n";
}

void listWorkouts(const FitnessTracker& t) {
    std::cout << "\nWorkouts: " << t.getWorkoutCount() << "\n";
    const std::vector<Workout>& ws = t.getWorkouts();
    for (size_t i = 0; i < ws.size(); i++) {
        std::cout << "\n[" << i << "] ";
        ws[i].print();
    }
}

void addProgramMenu(FitnessTracker& t) {
    if (t.getExerciseCount() == 0) { std::cout << "Add exercises first.\n"; return; }
    std::cout << "Program name: "; std::string n = readLine();
    WorkoutProgram p(n);
    listExercises(t);
    while (true) {
        std::cout << "Exercise index (-1 = end): "; int idx = readInt();
        if (idx == -1) break;
        Exercise* ex = t.getExercise(idx);
        if (ex) p.addExercise(ex);
    }
    t.addProgram(p);
    std::cout << "Program added.\n";
}

void listPrograms(const FitnessTracker& t) {
    std::cout << "\nPrograms: " << t.getProgramCount() << "\n";
    const std::vector<WorkoutProgram>& ps = t.getPrograms();
    for (size_t i = 0; i < ps.size(); i++) { std::cout << "[" << i << "] "; ps[i].print(); }
}

void addGoalMenu(FitnessTracker& t) {
    std::cout << "Description: "; std::string d = readLine();
    std::cout << "Deadline: "; std::string dl = readLine();
    std::cout << "Type (1=Strength, 2=Weight): "; int type = readInt();
    if (type == 1) {
        std::cout << "Exercise name: "; std::string en = readLine();
        std::cout << "Target weight: "; double tw = readDouble();
        std::cout << "Target reps: "; int tr = readInt();
        t.addGoal(new StrengthGoal(d, dl, en, tw, tr));
    } else {
        std::cout << "Starting weight: "; double sw = readDouble();
        std::cout << "Target weight: "; double tw = readDouble();
        t.addGoal(new WeightGoal(d, dl, sw, tw));
    }
    t.updateGoalsFromWorkouts();
    std::cout << "Goal added.\n";
}

void listGoals(const FitnessTracker& t) {
    std::cout << "\nGoals: " << t.getGoalCount() << "\n";
    const std::vector<Goal*>& gs = t.getGoals();
    for (size_t i = 0; i < gs.size(); i++) {
        std::cout << "  [" << i << "] "; gs[i]->print(); std::cout << "\n";
    }
}

void searchMenu(const FitnessTracker& t) {
    std::cout << "Search: "; std::string q = readLine();
    std::vector<Exercise*> r = t.searchExercises(q);
    std::cout << "Found " << r.size() << ":\n";
    for (size_t i = 0; i < r.size(); i++) { std::cout << "  "; r[i]->print(); std::cout << "\n"; }
}

void progressMenu(const FitnessTracker& t) {
    std::cout << "Exercise name: "; std::string n = readLine();
    t.showProgress(n);
}

void printMenu() {
    std::cout << "\n========================================\n";
    std::cout << "          FITNESS TRACKER\n";
    std::cout << "========================================\n";
    std::cout << "  1. Exercises - View\n";
    std::cout << "  2. Exercises - Add\n";
    std::cout << "  3. Exercises - Delete\n";
    std::cout << "  4. Workout - New\n";
    std::cout << "  5. Workouts - View\n";
    std::cout << "  6. Progress by Exercise\n";
    std::cout << "  7. Summary\n";
    std::cout << "  8. Programs - View\n";
    std::cout << "  9. Programs - New\n";
    std::cout << " 10. Goals - View\n";
    std::cout << " 11. Goals - New\n";
    std::cout << " 12. Search Exercise\n";
    std::cout << " 13. Save to File\n";
    std::cout << " 14. Load from File\n";
    std::cout << "  0. Exit\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Choice: ";
}

int main() {
    FitnessTracker t;
    seedData(t);
    std::cout << "Sample data loaded.\n";

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
            case 7: t.showSummary(); break;
            case 8: listPrograms(t); break;
            case 9: addProgramMenu(t); break;
            case 10: listGoals(t); break;
            case 11: addGoalMenu(t); break;
            case 12: searchMenu(t); break;
            case 13:
                if (t.saveToFile("data.txt")) std::cout << "Saved to data.txt\n";
                else std::cout << "Error saving.\n";
                break;
            case 14:
                if (t.loadFromFile("data.txt")) std::cout << "Loaded from data.txt\n";
                else std::cout << "Error loading.\n";
                break;
            case 0:
                std::cout << "Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid choice.\n";
        }
    }
}