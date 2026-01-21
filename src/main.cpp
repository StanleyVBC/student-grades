#include "Person.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <random>
#include <string>

static void printTable(const std::vector<Person>& students) {
    std::cout << std::left
              << std::setw(15) << "Name"
              << std::setw(20) << "Surname"
              << std::right
              << std::setw(15) << "Final (Avg.)"
              << " | "
              << std::setw(15) << "Final (Med.)"
              << "\n";

    std::cout << std::string(15 + 20 + 15 + 3 + 15, '-') << "\n";

    std::cout << std::fixed << std::setprecision(2);
    for (const auto& s : students) {
        std::cout << std::left
                  << std::setw(15) << s.name()
                  << std::setw(20) << s.surname()
                  << std::right
                  << std::setw(15) << s.finalAvg()
                  << " | "
                  << std::setw(15) << s.finalMed()
                  << "\n";
    }
}

static void sortStudents(std::vector<Person>& students, int mode) {
    if (mode == 1) { // sort by name
        std::sort(students.begin(), students.end(),
            [](const Person& a, const Person& b) {
                if (a.name() == b.name()) return a.surname() < b.surname();
                return a.name() < b.name();
            });
    } else { // sort by surname
        std::sort(students.begin(), students.end(),
            [](const Person& a, const Person& b) {
                if (a.surname() == b.surname()) return a.name() < b.name();
                return a.surname() < b.surname();
            });
    }
}

static std::vector<Person> readFromFile(const std::string& path) {
    std::ifstream fin(path);
    if (!fin) {
        throw std::runtime_error("Cannot open file: " + path);
    }

    std::vector<Person> students;
    std::string headerLine;
    std::getline(fin, headerLine); // skip header

    Person p;
    while (fin >> p) {
        students.push_back(p);
    }
    return students;
}

static std::vector<Person> randomGenerate(int count, int hwCount) {
    std::vector<Person> students;
    students.reserve(count);

    std::mt19937 rng((unsigned)std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 10);

    for (int i = 1; i <= count; ++i) {
        Person p("Name" + std::to_string(i), "Surname" + std::to_string(i));

        std::vector<int> hw;
        hw.reserve(hwCount);
        for (int j = 0; j < hwCount; ++j) hw.push_back(dist(rng));

        p.setHomework(std::move(hw));
        p.setExam(dist(rng));
        p.calculateFinals();

        students.push_back(p);
    }
    return students;
}

static std::vector<Person> manualInput() {
    int count;
    std::cout << "How many students? ";
    std::cin >> count;
    std::cin.ignore(10000, '\n');

    std::vector<Person> students;
    students.reserve(count);

    std::cout << "\nEnter each student in ONE LINE:\n";
    std::cout << "Name Surname HW1 HW2 ... HWn Exam\n";
    std::cout << "Example: John Doe 8 9 10 6 9\n\n";

    for (int i = 0; i < count; ++i) {
        std::cout << "Student " << (i + 1) << ": ";
        Person p;
        std::cin >> p;
        students.push_back(p);
    }
    return students;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    try {
        std::cout << "=== Student Grades (v0.1) ===\n";
        std::cout << "1) Manual input\n";
        std::cout << "2) Random generate\n";
        std::cout << "3) Read from file (data/Students.txt)\n";
        std::cout << "Choose: ";

        int choice;
        std::cin >> choice;

        std::vector<Person> students;

        if (choice == 1) {
            students = manualInput();
        } else if (choice == 2) {
            int n, hwN;
            std::cout << "How many students? ";
            std::cin >> n;
            std::cout << "How many homework marks per student? ";
            std::cin >> hwN;
            students = randomGenerate(n, hwN);
        } else if (choice == 3) {
            students = readFromFile("data/Students.txt");
        } else {
            std::cout << "Wrong option.\n";
            return 0;
        }

        int sortMode;
        std::cout << "\nSort by: 1) Name  2) Surname : ";
        std::cin >> sortMode;
        if (sortMode != 1) sortMode = 2;

        sortStudents(students, sortMode);

        std::cout << "\nComputed finals:\n";
        std::cout << "Final = 0.4 * HW + 0.6 * Exam\n";
        std::cout << "HW uses BOTH: average and median\n\n";

        printTable(students);

        std::cout << "\nDone. Students: " << students.size() << "\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "\nERROR: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
