#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>
#include <iostream>

class Person {
private:
    std::string name_;
    std::string surname_;
    std::vector<int> hw_;
    int exam_ = 0;

    double finalAvg_ = 0.0;
    double finalMed_ = 0.0;

public:
    // Constructors
    Person() = default;
    Person(std::string name, std::string surname);

    // Rule of Three
    Person(const Person& other);            // copy constructor
    Person& operator=(const Person& other); // copy assignment
    ~Person();                              // destructor

    // Getters
    const std::string& name() const { return name_; }
    const std::string& surname() const { return surname_; }
    double finalAvg() const { return finalAvg_; }
    double finalMed() const { return finalMed_; }

    // Setters
    void setHomework(std::vector<int> hw) { hw_ = std::move(hw); }
    void setExam(int ex) { exam_ = ex; }

    // Calculations
    void calculateFinals();

    static double average(const std::vector<int>& v);
    static double median(std::vector<int> v);

    // Operators
    friend std::istream& operator>>(std::istream& in, Person& p);
    friend std::ostream& operator<<(std::ostream& out, const Person& p);
};

#endif
