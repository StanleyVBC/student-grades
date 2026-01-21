#include "Person.h"

#include <sstream>
#include <algorithm>
#include <numeric>
#include <stdexcept>

// Constructor
Person::Person(std::string name, std::string surname)
    : name_(std::move(name)), surname_(std::move(surname)) {}

// Rule of Three
Person::Person(const Person& other)
    : name_(other.name_),
      surname_(other.surname_),
      hw_(other.hw_),
      exam_(other.exam_),
      finalAvg_(other.finalAvg_),
      finalMed_(other.finalMed_) {}

Person& Person::operator=(const Person& other) {
    if (this != &other) {
        name_ = other.name_;
        surname_ = other.surname_;
        hw_ = other.hw_;
        exam_ = other.exam_;
        finalAvg_ = other.finalAvg_;
        finalMed_ = other.finalMed_;
    }
    return *this;
}

Person::~Person() {
    // nothing special (vector/string clean themselves),
    // but included to satisfy Rule of Three requirement
}

// Average
double Person::average(const std::vector<int>& v) {
    if (v.empty()) return 0.0;
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    return sum / v.size();
}

// Median
double Person::median(std::vector<int> v) {
    if (v.empty()) return 0.0;
    std::sort(v.begin(), v.end());
    size_t n = v.size();
    if (n % 2 == 1) return v[n / 2];
    return (v[n / 2 - 1] + v[n / 2]) / 2.0;
}

// Calculate both finals
void Person::calculateFinals() {
    double hwAvg = average(hw_);
    double hwMed = median(hw_);
    finalAvg_ = 0.4 * hwAvg + 0.6 * exam_;
    finalMed_ = 0.4 * hwMed + 0.6 * exam_;
}

// Input operator: reads one FULL line after name+surname.
// Format works for BOTH manual and file line:
// Name Surname HW1 HW2 ... HWn Exam
std::istream& operator>>(std::istream& in, Person& p) {
    p.hw_.clear();
    p.exam_ = 0;

    if (!(in >> p.name_ >> p.surname_)) return in;

    std::string rest;
    std::getline(in, rest); // read remaining line
    std::istringstream iss(rest);

    std::vector<int> numbers;
    int x;
    while (iss >> x) numbers.push_back(x);

    if (numbers.empty()) {
        // no marks provided
        p.exam_ = 0;
        p.hw_.clear();
        p.calculateFinals();
        return in;
    }

    // last number is exam, others are homework
    p.exam_ = numbers.back();
    numbers.pop_back();
    p.hw_ = std::move(numbers);

    p.calculateFinals();
    return in;
}

// Output operator (simple)
std::ostream& operator<<(std::ostream& out, const Person& p) {
    out << p.name_ << " " << p.surname_
        << " | Avg: " << p.finalAvg_
        << " | Med: " << p.finalMed_;
    return out;
}
