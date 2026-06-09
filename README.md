# # ✈️ Airline Seat Reservation System

A console-based airline seat reservation system written in C++17. The project demonstrates core C++ concepts including dynamic memory allocation, object-oriented design, file I/O, and input validation.

---

## Features

- **Interactive seat map** — visual grid display with `O` (available) and `X` (reserved) markers
- **Reserve & cancel seats** — validated input with informative error messages
- **Load from file** — bulk-import reservations from a formatted text file
- **Flight statistics** — total/reserved/available seat counts and occupancy percentage
- **Configurable flight size** — set any number of rows (1–50) and seats per row (1–10) at startup
- **Safe memory management** — Rule of Three enforced; exception-safe 2D array allocation/cleanup

---

## Project Structure

```
.
├── main.cpp          # Entry point, menu loop, input utilities
├── Flight.h/.cpp     # Flight class — manages the seat grid
├── Seat.h/.cpp       # Seat class — represents a single seat
├── Makefile          # Build configuration
└── reservations.txt  # Sample reservation data file
```

---

## Getting Started

### Prerequisites

- A C++17-compatible compiler (e.g., `g++` 7+)
- `make`

### Build

```bash
make
```

### Run

```bash
./airline_reservation
# or
make run
```

### Clean

```bash
make clean
```

---

## Usage

On launch, you'll be prompted to configure the flight dimensions:

```
Configure your flight:
  Enter number of rows (1-50): 10
  Enter seats per row (1-10): 6
```

You'll then enter the main menu:

```
╔══════════════════════════════════════════╗
║   AIRLINE SEAT RESERVATION SYSTEM        ║
╠══════════════════════════════════════════╣
║  1. Display seat map                     ║
║  2. Reserve a seat                       ║
║  3. Cancel a reservation                 ║
║  4. Load reservations from file          ║
║  5. Display flight statistics            ║
║  6. Exit                                 ║
╚══════════════════════════════════════════╝
```

### Loading from a File

Select option **4** and provide a path to a reservation file. The file format is:

```
# Lines starting with # are comments
# Format: row col passenger_name
1 A John Smith
1 B Maria Garcia
3 C Jane Doe
```

A sample file (`reservations.txt`) is included in the repository.

---

## Class Overview

### `Seat`

Represents a single airplane seat. Encapsulates:

| Member | Description |
|--------|-------------|
| `row`, `col` | Seat position (e.g., row 3, column C) |
| `reserved` | Reservation status |
| `passengerName` | Name of the passenger, or empty if available |

Key methods: `reserve(name)`, `cancel()`, `isReserved()`, `getSeatNumber()`

### `Flight`

Manages a dynamically allocated 2D grid of `Seat` objects. Handles:

| Responsibility | Methods |
|----------------|---------|
| Seat map display | `displaySeats()` |
| Reservation management | `reserveSeat()`, `cancelReservation()` |
| File loading | `loadFromFile(filename)` |
| Statistics | `displayStatistics()`, `getAvailableSeats()`, `getReservedSeats()` |

Copy constructor and copy assignment are deleted to prevent accidental shallow copies of the heap-allocated seat grid.

---

## C++ Concepts Demonstrated

- **Dynamic memory allocation** — `new`/`delete` for a 2D array of objects
- **Rule of Three** — destructor + deleted copy constructor/assignment operator
- **Exception safety** — constructor cleans up partial allocations on failure
- **File I/O** — `std::ifstream`, `std::getline`, `std::istringstream`
- **Input validation** — range checks, type checking, whitespace trimming
- **Encapsulation** — private data with public getter/setter interfaces

---

## License

This project is released for educational use. Feel free to adapt it for learning purposes.
