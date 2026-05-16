#ifndef SET_H
#define SET_H

class Set {
private:
    int reps;
    double weight;
    bool isPR;

public:
    Set(int r = 0, double w = 0) : reps(r), weight(w), isPR(false) {}

    int getReps() const { return reps; }
    double getWeight() const { return weight; }
    bool getIsPR() const { return isPR; }

    void setReps(int r) { if (r >= 0) reps = r; }
    void setWeight(double w) { if (w >= 0) weight = w; }
    void markAsPR() { isPR = true; }

    double getVolume() const { return reps * weight; }
};

#endif