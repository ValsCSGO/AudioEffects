#pragma once

class Delay {
public:
    Delay() = default;
    Delay(const Delay &);
    Delay(int);
    ~Delay();

    bool empty() { return total == 0; }
    bool full() { return size == 0; }
    void resize(int);
    void push(const double&);
    double pop();
    void clear();
    void set_delay(int);
private:
    double *data = nullptr;
    int size = 0;
    int front = 0;
    int total = 0;
};

