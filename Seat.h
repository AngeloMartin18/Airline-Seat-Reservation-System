#ifndef SEAT_H
#define SEAT_H

#include <string>

/**
 * Seat class - Represents a single seat on an airplane.
 * Demonstrates encapsulation with private data members
 * and public getter/setter methods.
 */
class Seat {
private:
    int row;                   // Row number (1-based)
    char col;                  // Column letter (A-F)
    bool reserved;             // Reservation status
    std::string passengerName; // Name of passenger (empty if not reserved)

public:
    /**
     * Constructor - Initializes a seat with given row and column.
     * @param r Row number (default: 0)
     * @param c Column letter (default: 'A')
     */
    Seat(int r = 0, char c = 'A');

    // --- Getters ---
    int getRow() const;
    char getCol() const;
    bool isReserved() const;
    std::string getPassengerName() const;

    /**
     * Returns the human-readable seat number (e.g., "12C").
     */
    std::string getSeatNumber() const;

    /**
     * Reserves this seat for a passenger.
     * @param name Passenger name
     * @return true if reservation succeeded, false if already reserved
     */
    bool reserve(const std::string& name);

    /**
     * Cancels the reservation on this seat.
     * @return true if cancellation succeeded, false if seat was not reserved
     */
    bool cancel();

    /**
     * Displays seat information to the console.
     */
    void display() const;
};

#endif // SEAT_H
