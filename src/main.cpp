#include "Person.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

static void printHeader() {
    std::cout << std::left << std::setw(12) << "Name"
              << std::left << std::setw(15) << "Surname"
              << std::right << std::setw(12) << "Final(Avg.)"
              << " | "
              << std::setw(12) << "Final(Med.)"
              << "\n";
    std::cout << std::string(12 + 15 + 12 + 3 + 12, '-') << "\n";
}

static void sortStudents(std::vector<Person>& v) {
    std::sort(v.begin(), v.end(), [](const Person& a, const Person& b) {
        if (a.surname() != b.surname()) return a.surname() < b.surname();
        return a.name() < b.name();
    });
}

static std::vector<Person> readFromFile(const std::string& filePath, bool freeHwForBigData) {
    std::ifstream fin(filePath);
    if (!fin) throw std::runtime_error("Cannot open file: " + filePath);

    std::vector<Person> students;
    students.reserve(10000);

    // skip header line (e.g. "Name Surname ND1 ND2 ... Egz.")
    std::string header;
    std::getline(fin, header);

    Person p;
    while (fin >> p) {
        if (freeHwForBigData) {
            // optional memory saver for 100k/1M
            p.clearHwToSaveMemory();
        }
        students.push_back(p);
    }
    return students;
}

static Person readOneInteractive() {
    std::cout << "\nEnter ONE student in this format:\n";
    std::cout << "Name Surname HW1 HW2 ... 0 Exam\n";
    std::cout << "Example: Jonas Jonaitis 8 9 10 7 0 9\n\n";
    Person p;
    std::cin >> p;
    return p;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cout << "===== Student Final Grade Calculator (v0.1) =====\n";
    std::cout << "1) Enter students manually\n";
    std::cout << "2) Generate random students\n";
    std::cout << "3) Read from file (data/Students.txt)\n";
    std::cout << "Choose: ";

    int choice = 0;
    std::cin >> choice;

    std::vector<Person> students;

    try {
        if (choice == 1) {
            int k;
            std::cout << "How many students? ";
            std::cin >> k;
            students.reserve(k);

            for (int i = 0; i < k; i++) {
                students.push_back(readOneInteractive());
            }
        } else if (choice == 2) {
            int k;
            std::size_t hwCount;
            std::cout << "How many students to generate? ";
            std::cin >> k;
            std::cout << "How many homework marks per student? ";
            std::cin >> hwCount;

            students.reserve(k);
            for (int i = 0; i < k; i++) {
                Person p("Name" + std::to_string(i + 1), "Surname" + std::to_string(i + 1));
                p.generateRandom(hwCount);
                students.push_back(p);
            }
        } else if (choice == 3) {
            std::string path;
            std::cout << "File path (example: data/Students.txt): ";
            std::cin >> path;

            // For very large datasets, freeing HW vectors saves a lot of memory.
            bool freeHwForBigData = true;
            students = readFromFile(path, freeHwForBigData);
        } else {
            std::cout << "Invalid choice.\n";
            return 0;
        }

        sortStudents(students);

        printHeader();

        // For 100k/1M, better redirect output:
        // ./app > results.txt
        for (const auto& s : students) {
            std::cout << s << "\n";
        }

        std::cout << "\nDone. Students: " << students.size() << "\n";
    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << "\n";
    }

    return 0;
}
