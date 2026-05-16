#ifndef EXERCISE_H
#define EXERCISE_H

#include <iostream>
#include <string>

class Exercise {
protected:
    std::string name;
    std::string muscleGroup;

public:
    Exercise(const std::string& n, const std::string& m) : name(n), muscleGroup(m) {}
    virtual ~Exercise() {}

    std::string getName() const { return name; }
    std::string getMuscleGroup() const { return muscleGroup; }

    void setName(const std::string& n) { name = n; }
    void setMuscleGroup(const std::string& m) { muscleGroup = m; }

    virtual std::string getType() const = 0;
    virtual void print() const = 0;
    virtual void saveTo(std::ostream& out) const = 0;
};

class StrengthExercise : public Exercise {
private:
    std::string equipment;
    bool isCompound;

public:
    StrengthExercise(const std::string& n, const std::string& m,
                     const std::string& eq, bool compound)
        : Exercise(n, m), equipment(eq), isCompound(compound) {}

    std::string getEquipment() const { return equipment; }
    bool getIsCompound() const { return isCompound; }

    std::string getType() const override { return "Силово"; }

    void print() const override {
        std::cout << "[Силово] " << name << " (" << muscleGroup << ")";
        std::cout << (isCompound ? " - съставно" : " - изолиращо");
        std::cout << ", оборудване: " << equipment;
    }

    void saveTo(std::ostream& out) const override {
        out << "STRENGTH|" << name << "|" << muscleGroup << "|"
            << equipment << "|" << isCompound << "\n";
    }

    double estimate1RM(int reps, double weight) const {
        if (reps <= 0 || weight <= 0) return 0;
        if (reps == 1) return weight;
        return weight * (1.0 + reps / 30.0);
    }
};

class CardioExercise : public Exercise {
private:
    bool isOutdoor;
    std::string intensity;

public:
    CardioExercise(const std::string& n, const std::string& m,
                   bool outdoor, const std::string& intens)
        : Exercise(n, m), isOutdoor(outdoor), intensity(intens) {}

    bool getIsOutdoor() const { return isOutdoor; }
    std::string getIntensity() const { return intensity; }

    std::string getType() const override { return "Кардио"; }

    void print() const override {
        std::cout << "[Кардио] " << name;
        std::cout << (isOutdoor ? " (на открито)" : " (на закрито)");
        std::cout << ", интензивност: " << intensity;
    }

    void saveTo(std::ostream& out) const override {
        out << "CARDIO|" << name << "|" << muscleGroup << "|"
            << isOutdoor << "|" << intensity << "\n";
    }
};

#endif