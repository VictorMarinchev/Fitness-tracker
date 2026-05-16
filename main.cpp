#include <iostream>
#include <string>
#include <limits>
#include "FitnessTracker.h"

int readInt() {
    int x;
    while (!(std::cin >> x)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Невалидно число, опитайте пак: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return x;
}

double readDouble() {
    double x;
    while (!(std::cin >> x)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Невалидно число, опитайте пак: ";
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
    t.addExercise(new StrengthExercise("Лег прес", "Гърди", "лост", true));
    t.addExercise(new StrengthExercise("Клек", "Крака", "лост", true));
    t.addExercise(new StrengthExercise("Бицепс", "Бицепс", "дъмбели", false));
    t.addExercise(new CardioExercise("Бягане", "Крака", true, "средна"));
    t.addExercise(new CardioExercise("Колоездене", "Крака", true, "ниска"));

    Workout w("2026-05-10", 45);
    StandardBlock* sb = new StandardBlock(t.findExercise("Лег прес"));
    sb->addSet(Set(8, 75));
    sb->addSet(Set(8, 77.5));
    w.addBlock(sb);
    t.addWorkout(std::move(w));

    t.addGoal(new StrengthGoal("Лег прес 100 кг", "2026-08-01", "Лег прес", 100, 5));
}

void listExercises(const FitnessTracker& t) {
    std::cout << "\nУпражнения: " << t.getExerciseCount() << "\n";
    const std::vector<Exercise*>& exs = t.getExercises();
    for (size_t i = 0; i < exs.size(); i++) {
        std::cout << "  [" << i << "] ";
        exs[i]->print();
        std::cout << "\n";
    }
}

void addExerciseMenu(FitnessTracker& t) {
    std::cout << "Име: "; std::string name = readLine();
    std::cout << "Мускулна група: "; std::string muscle = readLine();
    std::cout << "Тип (1=Силово, 2=Кардио): "; int type = readInt();
    if (type == 1) {
        std::cout << "Оборудване: "; std::string eq = readLine();
        std::cout << "Съставно? (1=да, 0=не): "; int c = readInt();
        t.addExercise(new StrengthExercise(name, muscle, eq, c == 1));
    } else {
        std::cout << "На открито? (1=да, 0=не): "; int o = readInt();
        std::cout << "Интензивност: "; std::string intens = readLine();
        t.addExercise(new CardioExercise(name, muscle, o == 1, intens));
    }
    std::cout << "Добавено.\n";
}

void removeExerciseMenu(FitnessTracker& t) {
    listExercises(t);
    std::cout << "Индекс за изтриване: "; int idx = readInt();
    t.removeExercise(idx);
    std::cout << "Изтрито.\n";
}

void addWorkoutMenu(FitnessTracker& t) {
    if (t.getExerciseCount() == 0) { std::cout << "Първо добавете упражнения.\n"; return; }
    std::cout << "Дата: "; std::string date = readLine();
    std::cout << "Продължителност (мин): "; int dur = readInt();
    Workout w(date, dur);

    while (true) {
        std::cout << "\nДобави блок (1=Стандартен, 2=Кръгов, 0=Готово): ";
        int t1 = readInt();
        if (t1 == 0) break;
        listExercises(t);
        if (t1 == 1) {
            std::cout << "Индекс упражнение: "; int idx = readInt();
            Exercise* ex = t.getExercise(idx);
            if (!ex) { std::cout << "Невалиден индекс.\n"; continue; }
            StandardBlock* sb = new StandardBlock(ex);
            std::cout << "Брой серии: "; int n = readInt();
            for (int i = 0; i < n; i++) {
                std::cout << "Серия " << (i+1) << " - повт: "; int r = readInt();
                std::cout << "Серия " << (i+1) << " - тежест: "; double we = readDouble();
                sb->addSet(Set(r, we));
            }
            w.addBlock(sb);
        } else if (t1 == 2) {
            std::cout << "Брой кръгове: "; int rounds = readInt();
            CircuitBlock* cb = new CircuitBlock(rounds);
            while (true) {
                std::cout << "Индекс упражнение (-1 = край): "; int idx = readInt();
                if (idx == -1) break;
                Exercise* ex = t.getExercise(idx);
                if (!ex) { std::cout << "Невалиден индекс.\n"; continue; }
                std::cout << "Повторения: "; int r = readInt();
                cb->addItem(ex, r);
            }
            w.addBlock(cb);
        }
    }

    int prs = t.detectPRs(w);
    if (prs > 0) std::cout << "*** Открити " << prs << " нови PR-и! ***\n";
    t.addWorkout(std::move(w));
    t.updateGoalsFromWorkouts();
    std::cout << "Тренировката е записана.\n";
}

void listWorkouts(const FitnessTracker& t) {
    std::cout << "\nТренировки: " << t.getWorkoutCount() << "\n";
    const std::vector<Workout>& ws = t.getWorkouts();
    for (size_t i = 0; i < ws.size(); i++) {
        std::cout << "\n[" << i << "] ";
        ws[i].print();
    }
}

void addProgramMenu(FitnessTracker& t) {
    if (t.getExerciseCount() == 0) { std::cout << "Първо добавете упражнения.\n"; return; }
    std::cout << "Име на програма: "; std::string n = readLine();
    WorkoutProgram p(n);
    listExercises(t);
    while (true) {
        std::cout << "Индекс упражнение (-1 = край): "; int idx = readInt();
        if (idx == -1) break;
        Exercise* ex = t.getExercise(idx);
        if (ex) p.addExercise(ex);
    }
    t.addProgram(p);
    std::cout << "Програмата е добавена.\n";
}

void listPrograms(const FitnessTracker& t) {
    std::cout << "\nПрограми: " << t.getProgramCount() << "\n";
    const std::vector<WorkoutProgram>& ps = t.getPrograms();
    for (size_t i = 0; i < ps.size(); i++) { std::cout << "[" << i << "] "; ps[i].print(); }
}

void addGoalMenu(FitnessTracker& t) {
    std::cout << "Описание: "; std::string d = readLine();
    std::cout << "Краен срок: "; std::string dl = readLine();
    std::cout << "Тип (1=Силова, 2=Тегло): "; int type = readInt();
    if (type == 1) {
        std::cout << "Ime упражнение: "; std::string en = readLine();
        std::cout << "Целева тежест: "; double tw = readDouble();
        std::cout << "Целеви повторения: "; int tr = readInt();
        t.addGoal(new StrengthGoal(d, dl, en, tw, tr));
    } else {
        std::cout << "Начално тегло: "; double sw = readDouble();
        std::cout << "Целево тегло: "; double tw = readDouble();
        t.addGoal(new WeightGoal(d, dl, sw, tw));
    }
    t.updateGoalsFromWorkouts();
    std::cout << "Целта е добавена.\n";
}

void listGoals(const FitnessTracker& t) {
    std::cout << "\nЦели: " << t.getGoalCount() << "\n";
    const std::vector<Goal*>& gs = t.getGoals();
    for (size_t i = 0; i < gs.size(); i++) {
        std::cout << "  [" << i << "] "; gs[i]->print(); std::cout << "\n";
    }
}

void searchMenu(const FitnessTracker& t) {
    std::cout << "Търсене: "; std::string q = readLine();
    std::vector<Exercise*> r = t.searchExercises(q);
    std::cout << "Намерени " << r.size() << ":\n";
    for (size_t i = 0; i < r.size(); i++) { std::cout << "  "; r[i]->print(); std::cout << "\n"; }
}

void progressMenu(const FitnessTracker& t) {
    std::cout << "Ime на упражнение: "; std::string n = readLine();
    t.showProgress(n);
}

void printMenu() {
    std::cout << "\n========================================\n";
    std::cout << "          FITNESS TRACKER\n";
    std::cout << "========================================\n";
    std::cout << "  1. Упражнения - преглед\n";
    std::cout << "  2. Упражнения - добавяне\n";
    std::cout << "  3. Упражнения - изтриване\n";
    std::cout << "  4. Тренировка - нова\n";
    std::cout << "  5. Тренировки - преглед\n";
    std::cout << "  6. Прогрес по упражнение\n";
    std::cout << "  7. Обобщение\n";
    std::cout << "  8. Програми - преглед\n";
    std::cout << "  9. Програми - нова\n";
    std::cout << " 10. Цели - преглед\n";
    std::cout << " 11. Цели - нова\n";
    std::cout << " 12. Търсене на упражнение\n";
    std::cout << " 13. Запис във файл\n";
    std::cout << " 14. Зареждане от файл\n";
    std::cout << "  0. Изход\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Избор: ";
}

int main() {
    FitnessTracker t;
    seedData(t);
    std::cout << "Заредени са примерни данни.\n";

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
                if (t.saveToFile("data.txt")) std::cout << "Записано в data.txt\n";
                else std::cout << "Грешка при запис.\n";
                break;
            case 14:
                if (t.loadFromFile("data.txt")) std::cout << "Заредено от data.txt\n";
                else std::cout << "Грешка при зареждане.\n";
                break;
            case 0:
                std::cout << "Довиждане!\n";
                return 0;
            default:
                std::cout << "Невалиден избор.\n";
        }
    }
}