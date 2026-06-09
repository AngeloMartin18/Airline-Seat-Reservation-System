#include "Flight.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cctype>

// =====================================================================
// Constructor
// =====================================================================
Flight::Flight(int r, int c)
    : seats(nullptr), rows(r), cols(c) {
    
    // Dynamically allocate 2D array of Seat objects
    // Exception-safe: if any allocation fails, previously allocated
    // memory is cleaned up before rethrowing
    seats = new Seat*[rows];
    
    // Initialize all row pointers to nullptr for safe cleanup
    for (int i = 0; i < rows; ++i) {
        seats[i] = nullptr;
    }
    
    try {
        for (int i = 0; i < rows; ++i) {
            seats[i] = new Seat[cols];
            // Initialize each seat with its row and column
            for (int j = 0; j < cols; ++j) {
                seats[i][j] = Seat(i + 1, indexToCol(j));
            }
        }
    } catch (...) {
        cleanup();
        throw; // Re-throw the exception
    }
}

// =====================================================================
// Destructor
// =====================================================================
Flight::~Flight() {
    cleanup();
}

// =====================================================================
// Cleanup - frees all dynamically allocated memory
// =====================================================================
void Flight::cleanup() {
    if (seats != nullptr) {
        for (int i = 0; i < rows; ++i) {
            delete[] seats[i];  // Delete each row array
        }
        delete[] seats;         // Delete the array of row pointers
        seats = nullptr;
    }
}

// =====================================================================
// Validation Helpers
// =====================================================================
bool Flight::isValidSeat(int row, char col) const {
    col = std::toupper(col);
    if (row < 1 || row > rows) {
        return false;
    }
    if (col < 'A' || col > indexToCol(cols - 1)) {
        return false;
    }
    return true;
}

int Flight::colToIndex(char col) const {
    return std::toupper(col) - 'A';
}

char Flight::indexToCol(int index) const {
    return static_cast<char>('A' + index);
}

// =====================================================================
// File I/O - Load reservations from file
// =====================================================================
bool Flight::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'" << std::endl;
        return false;
    }

    int loadedCount = 0;
    int errorCount = 0;
    std::string line;
    int lineNum = 0;

    while (std::getline(file, line)) {
        ++lineNum;
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        int row;
        char col;
        std::string name;

        // Parse: row col name
        if (!(iss >> row >> col)) {
            std::cerr << "Warning: Line " << lineNum 
                      << " has invalid format. Skipping." << std::endl;
            ++errorCount;
            continue;
        }

        // Read the rest of the line as the passenger name
        std::getline(iss, name);
        
        // Trim leading and trailing whitespace from name
        size_t start = name.find_first_not_of(" \t");
        size_t end = name.find_last_not_of(" \t");
        if (start != std::string::npos) {
            name = name.substr(start, end - start + 1);
        }

        // Validate input
        if (name.empty()) {
            std::cerr << "Warning: Line " << lineNum 
                      << " missing passenger name. Skipping." << std::endl;
            ++errorCount;
            continue;
        }

        col = std::toupper(col);

        // Attempt reservation
        if (reserveSeat(row, col, name)) {
            ++loadedCount;
        } else {
            ++errorCount;
        }
    }

    file.close();

    std::cout << "File loaded: " << loadedCount << " reservation(s) added";
    if (errorCount > 0) {
        std::cout << ", " << errorCount << " error(s)";
    }
    std::cout << "." << std::endl;

    return true;
}

// =====================================================================
// Display - Visual seat map
// =====================================================================
void Flight::displaySeats() const {
    std::cout << "\n";
    
    // Column headers
    std::cout << "    ";
    for (int j = 0; j < cols; ++j) {
        std::cout << " " << indexToCol(j) << "  ";
    }
    std::cout << "\n";

    // Separator line
    std::cout << "   +";
    for (int j = 0; j < cols; ++j) {
        std::cout << "---+";
    }
    std::cout << "\n";

    // Seat rows
    for (int i = 0; i < rows; ++i) {
        // Row number (right-aligned, 2 digits)
        std::cout << std::setw(2) << (i + 1) << " |";
        
        for (int j = 0; j < cols; ++j) {
            char symbol = seats[i][j].isReserved() ? 'X' : 'O';
            std::cout << " " << symbol << " |";
        }
        std::cout << "\n";

        // Separator between rows
        std::cout << "   +";
        for (int j = 0; j < cols; ++j) {
            std::cout << "---+";
        }
        std::cout << "\n";
    }

    // Legend
    std::cout << "\n   O = Available   X = Reserved\n\n";
}

// =====================================================================
// Reserve a seat
// =====================================================================
bool Flight::reserveSeat(int row, char col, const std::string& name) {
    col = std::toupper(col);
    
    if (!isValidSeat(row, col)) {
        std::cerr << "Error: Invalid seat " << row << col 
                  << ". Valid range: 1" << indexToCol(0) << "-" 
                  << rows << indexToCol(cols - 1) << std::endl;
        return false;
    }

    int r = row - 1;          // Convert to 0-based
    int c = colToIndex(col);

    if (seats[r][c].isReserved()) {
        std::cerr << "Error: Seat " << row << col 
                  << " is already reserved by "
                  << seats[r][c].getPassengerName() << std::endl;
        return false;
    }

    return seats[r][c].reserve(name);
}

// =====================================================================
// Cancel a reservation
// =====================================================================
bool Flight::cancelReservation(int row, char col) {
    col = std::toupper(col);
    
    if (!isValidSeat(row, col)) {
        std::cerr << "Error: Invalid seat " << row << col << std::endl;
        return false;
    }

    int r = row - 1;
    int c = colToIndex(col);

    if (!seats[r][c].isReserved()) {
        std::cerr << "Error: Seat " << row << col 
                  << " is not currently reserved." << std::endl;
        return false;
    }

    std::string passenger = seats[r][c].getPassengerName();
    bool result = seats[r][c].cancel();
    
    if (result) {
        std::cout << "Cancelled reservation for " << passenger 
                  << " on seat " << row << col << std::endl;
    }
    return result;
}

// =====================================================================
// Statistics
// =====================================================================
int Flight::getAvailableSeats() const {
    int count = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!seats[i][j].isReserved()) {
                ++count;
            }
        }
    }
    return count;
}

int Flight::getReservedSeats() const {
    int count = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (seats[i][j].isReserved()) {
                ++count;
            }
        }
    }
    return count;
}

void Flight::displayStatistics() const {
    int total = rows * cols;
    int reserved = getReservedSeats();
    int available = getAvailableSeats();
    double occupancy = (static_cast<double>(reserved) / total) * 100.0;

    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "         FLIGHT STATISTICS\n";
    std::cout << "========================================\n";
    std::cout << "  Configuration: " << rows << " rows x " 
              << cols << " seats/row\n";
    std::cout << "  Total seats:   " << total << "\n";
    std::cout << "  Reserved:      " << reserved << "\n";
    std::cout << "  Available:     " << available << "\n";
    std::cout << "  Occupancy:     " << std::fixed 
              << std::setprecision(1) << occupancy << "%\n";
    std::cout << "========================================\n\n";
}
