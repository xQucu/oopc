#include <iostream>
#include <string>

class Employee {
public:
    Employee(std::string name, std::string position, int age)
        : name(name)
        , position(position)
        , age(age)
    {
    }

    std::string name;
    std::string position;
    int age;
};

inline std::ostream& operator<<(std::ostream& os, const Employee& emp)
{
    os << emp.name << ", " << emp.position << ", " << emp.age;
    return os;
}
