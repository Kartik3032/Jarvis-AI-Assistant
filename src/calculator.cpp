#include "../include/calculator.h"

#include <iostream>
#include <sstream>

void Calculator::Calculate(const std::string &expression)
{
    std::stringstream ss(expression);

    double a;
    double b;
    char op;

    ss >> a >> op >> b;

    if (ss.fail())
    {
        std::cout << "Invalid calculation\n";
        return;
    }

    double result = 0;

    switch (op)
    {
    case '+':
        result = a + b;
        break;

    case '-':
        result = a - b;
        break;

    case '*':
        result = a * b;
        break;

    case '/':
        if (b == 0)
        {
            std::cout << "Cannot divide by zero\n";
            return;
        }

        result = a / b;
        break;

    default:
        std::cout << "Unknown operator\n";
        return;
    }

    std::cout << "Result : " << result << std::endl;
}