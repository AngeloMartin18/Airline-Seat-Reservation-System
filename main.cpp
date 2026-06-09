#include "Flight.h"
#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <new>

// =====================================================================
// Utility: Clear input buffer
// =====================================================================
void clearInputBuffer()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// =====================================================================
// Utility: Get validated integer input
// =====================================================================
int getIntInput(const std::string &prompt, int min, int max)
{
    int value;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value)
        {
            if (value >= min && value <= max)
            {
                clearInputBuffer();
                return value;
            }
            std::cerr << "Error: Please enter a number between "
                      << min << " and " << max << ".\n";
        }
        else
        {
            std::cerr << "Error: Invalid input. Please enter a number.\n";
            clearInputBuffer();
        }
    }
}

// =====================================================================
// Utility: Get validated char input
// =====================================================================
char getCharInput(const std::string &prompt, char minChar, char maxChar)
{
    char value;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value)
        {
            value = std::toupper(value);
            if (value >= minChar && value <= maxChar)
            {
                clearInputBuffer();
                return value;
            }
            std::cerr << "Error: Please enter a letter between "
                      << minChar << " and " << maxChar << ".\n";
        }
        else
        {
            std::cerr << "Error: Invalid input. Please enter a letter.\n";
            clearInputBuffer();
        }
    }
}

// =====================================================================
// Utility: Get non-empty string input
// =====================================================================
std::string getStringInput(const std::string &prompt)
{
    std::string value;
    while (true)
    {
        std::cout << prompt;
        std::getline(std::cin, value);
        if (!value.empty())
        {
            // Trim leading/trailing whitespace
            size_t start = value.find_first_not_of(" \t");
            size_t end = value.find_last_not_of(" \t");
            if (start != std::string::npos)
            {
                return value.substr(start, end - start + 1);
            }
        }
        std::cerr << "Error: Input cannot be empty.\n";
    }
}

// =====================================================================
// Menu Display
// =====================================================================
void displayMenu()
{
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════╗\n";
    std::cout << "║   AIRLINE SEAT RESERVATION SYSTEM        ║\n";
    std::cout << "╠══════════════════════════════════════════╣\n";
    std::cout << "║  1. Display seat map                     ║\n";
    std::cout << "║  2. Reserve a seat                       ║\n";
    std::cout << "║  3. Cancel a reservation                 ║\n";
    std::cout << "║  4. Load reservations from file          ║\n";
    std::cout << "║  5. Display flight statistics            ║\n";
    std::cout << "║  6. Exit                                 ║\n";
    std::cout << "╚══════════════════════════════════════════╝\n";
}

// =====================================================================
// Main
// =====================================================================
int main()
{
    // Get flight configuration from user
    std::cout << "══════════════════════════════════════════\n";
    std::cout << "  AIRLINE SEAT RESERVATION SYSTEM\n";
    std::cout << "══════════════════════════════════════════\n\n";

    std::cout << "Configure your flight:\n";
    int numRows = getIntInput("  Enter number of rows (1-50): ", 1, 50);
    int numCols = getIntInput("  Enter seats per row (1-10): ", 1, 10);

    // Dynamically allocate the Flight object
    Flight *flight = nullptr;
    try
    {
        flight = new Flight(numRows, numCols);
    }
    catch (const std::bad_alloc &e)
    {
        std::cerr << "Error: Memory allocation failed!\n";
        return 1;
    }

    std::cout << "\nFlight configured: " << numRows << " rows x "
              << numCols << " seats/row (" << (numRows * numCols)
              << " total seats)\n";

    // Main menu loop
    bool running = true;
    while (running)
    {
        displayMenu();

        int choice = getIntInput("Select an option (1-6): ", 1, 6);

        switch (choice)
        {
        case 1:
        {
            // Display seat map
            flight->displaySeats();
            break;
        }

        case 2:
        {
            // Reserve a seat
            std::cout << "\n--- Reserve a Seat ---\n";
            int row = getIntInput("  Enter row number: ", 1, flight->getRows());
            char col = getCharInput("  Enter seat letter: ", 'A',
                                    static_cast<char>('A' + flight->getCols() - 1));
            std::string name = getStringInput("  Enter passenger name: ");

            if (flight->reserveSeat(row, col, name))
            {
                std::cout << "Reservation confirmed: Seat " << row << col
                          << " for " << name << "\n";
            }
            break;
        }

        case 3:
        {
            // Cancel reservation
            std::cout << "\n--- Cancel Reservation ---\n";
            int row = getIntInput("  Enter row number: ", 1, flight->getRows());
            char col = getCharInput("  Enter seat letter: ", 'A',
                                    static_cast<char>('A' + flight->getCols() - 1));

            flight->cancelReservation(row, col);
            break;
        }

        case 4:
        {
            // Load from file
            std::cout << "\n--- Load Reservations from File ---\n";
            std::string filename = getStringInput("  Enter filename: ");
            flight->loadFromFile(filename);
            break;
        }

        case 5:
        {
            // Statistics
            flight->displayStatistics();
            break;
        }

        case 6:
        {
            // Exit
            std::cout << "\nThank you for using the Airline Seat "
                      << "Reservation System. Goodbye!\n";
            running = false;
            break;
        }

        default:
            std::cerr << "Error: Invalid option. Please try again.\n";
        }
    }

    // Clean up dynamically allocated Flight object
    delete flight;

    return 0;
}
