#ifndef FLIGHT_H
#define FLIGHT_H

#include "Seat.h"
#include <string>

/**
 * Flight class - Manages the seat grid for a single flight.
 * Demonstrates dynamic memory allocation with new/delete,
 * file I/O operations, and input validation.
 */
class Flight {
private:
    Seat** seats;   // Dynamic 2D array: seats[row][col]
    int rows;       // Number of rows in the flight
    int cols;       // Number of columns (seats per row)

    /**
     * Validates that a row and column are within bounds.
     * @return true if valid, false otherwise
     */
    bool isValidSeat(int row, char col) const;

    /**
     * Converts a column letter to a zero-based index.
     * 'A' -> 0, 'B' -> 1, etc.
     */
    int colToIndex(char col) const;

    /**
     * Converts a zero-based index to a column letter.
     * 0 -> 'A', 1 -> 'B', etc.
     */
    char indexToCol(int index) const;

    /**
     * Deallocates the dynamic seat array.
     * Called by destructor and when reinitializing.
     */
    void cleanup();

public:
    /**
     * Constructor - Allocates a rows x cols grid of Seat objects.
     * @param r Number of rows (default: 10)
     * @param c Number of columns (default: 6, for seats A-F)
     */
    Flight(int r = 10, int c = 6);

    /**
     * Destructor - Frees all dynamically allocated memory.
     */
    ~Flight();

    // --- Rule of Three: prevent copying ---
    Flight(const Flight&) = delete;
    Flight& operator=(const Flight&) = delete;

    /**
     * Loads reservation data from a file.
     * File format: one reservation per line:
     *   row col passenger_name
     * Example:
     *   1 A John Doe
     *   3 C Jane Smith
     *
     * @param filename Path to the input file
     * @return true if file loaded successfully, false on error
     */
    bool loadFromFile(const std::string& filename);

    /**
     * Displays a visual seat map to the console.
     * Reserved seats are marked with 'X', available with 'O'.
     */
    void displaySeats() const;

    /**
     * Reserves a specific seat for a passenger.
     * @param row Row number (1-based)
     * @param col Column letter (A-F)
     * @param name Passenger name
     * @return true if reservation succeeded
     */
    bool reserveSeat(int row, char col, const std::string& name);

    /**
     * Cancels a reservation on a specific seat.
     * @return true if cancellation succeeded
     */
    bool cancelReservation(int row, char col);

    /**
     * Returns the number of available (unreserved) seats.
     */
    int getAvailableSeats() const;

    /**
     * Returns the number of reserved seats.
     */
    int getReservedSeats() const;

    /**
     * Displays summary statistics about the flight.
     */
    void displayStatistics() const;

    // --- Accessors ---
    int getRows() const { return rows; }
    int getCols() const { return cols; }
};

#endif // FLIGHT_H
