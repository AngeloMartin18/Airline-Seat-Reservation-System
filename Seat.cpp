#include "Seat.h"
#include <iostream>

// --- Constructor ---
Seat::Seat(int r, char c)
    : row(r), col(c), reserved(false), passengerName("") {}

// --- Getters ---
int Seat::getRow() const {
    return row;
}

char Seat::getCol() const {
    return col;
}

bool Seat::isReserved() const {
    return reserved;
}

std::string Seat::getPassengerName() const {
    return passengerName;
}

std::string Seat::getSeatNumber() const {
    return std::to_string(row) + col;
}

// --- Reservation Management ---
bool Seat::reserve(const std::string& name) {
    if (reserved) {
        return false; // Already reserved
    }
    reserved = true;
    passengerName = name;
    return true;
}

bool Seat::cancel() {
    if (!reserved) {
        return false; // Not reserved
    }
    reserved = false;
    passengerName = "";
    return true;
}

// --- Display ---
void Seat::display() const {
    std::cout << "Seat " << getSeatNumber() << " | "
              << "Status: " << (reserved ? "Reserved" : "Available");
    if (reserved) {
        std::cout << " | Passenger: " << passengerName;
    }
    std::cout << std::endl;
}
