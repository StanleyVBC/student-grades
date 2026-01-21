# Student Grades (Studentu Zurnalas)

C++ program that calculates students' final grade using either homework **average** or **median** and exam result.
Supports manual input, random generation, and reading from file.

## Final grade formula
Final = 0.4 * HW + 0.6 * Exam

Where HW is either:
- Average of homework marks, or
- Median of homework marks
## How to run

### Build (g++)
From the repository root:

```bash
g++ -std=c++17 -O2 -Wall -Wextra src/main.cpp src/Person.cpp -o app
