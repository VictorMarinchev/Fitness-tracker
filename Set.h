#ifndef SET_H
#define SET_H

class Set {
private:
    int reps;
    double weight;
    int rpe;        // Rate of Perceived Exertion (0-10)
    bool isPR;

public:
    Set(int r = 0, double w = 0, int rpeVal = 0)
        : reps(r), weight(w), rpe(rpeVal), isPR(false) {}

    int getReps() const { return reps; }
    double getWeight() const { return weight; }
    int getRpe() const { return rpe; }
    bool getIsPR() const { return isPR; }

    void setReps(int r) { if (r >= 0) reps = r; }
    void setWeight(double w) { if (w >= 0) weight = w; }
    void setRpe(int r) { if (r >= 0 && r <= 10) rpe = r; }
    void markAsPR() { isPR = true; }

    double getVolume() const { return reps * weight; }
};

#endif