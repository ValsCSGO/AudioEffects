#include <stdexcept>
#include <cstring>
#include <iostream>

#include "Delay.h"

Delay::Delay(int buf_size) {
    if(buf_size == 0)
        throw std::invalid_argument("size can't be zero");
    resize(buf_size);
}

Delay::Delay(const Delay &other) {
    size = other.size;
    front = other.front;
    total = other.total;
    if(size > 0) {
        data = new double[size];
        memcpy(data, other.data, size * sizeof(double));
    }
}

Delay::~Delay() {
    if(data != nullptr) {
        delete[] data;
        data = nullptr;
    }
}

void Delay::resize(int new_size) {
    if(data != nullptr)
        delete[] data;

    total = new_size;
    size = new_size + 1;
    data = new double[size];
}

void Delay::push(const double &t) {
    int end = (front + total) % size;
    data[end] = t;
    total++;
}

double Delay::pop() {
    if (empty())
        throw std::invalid_argument("can't pop empty buffer");

    double f = data[front];
    front = front == size ? 0 : front + 1;
    total--;
    return f;
}

void Delay::clear() {
    memset(data, 0, size * sizeof(double));
}

void Delay::set_delay(int samples) {
    if(samples > size - 1) {
        resize(samples);
    }
    total = samples;
}
