#include <iostream>
#include <string>

enum Choice {
    I = 73,
    L = 76,
    D = 68,
    P = 80,
    S = 83,
    E = 69,
    A = 65,
    C = 67,
    Q = 81
};

int to_ascii(string text)
{
    int sum = 0;
    for (size_t i = 0; i < text.length(); i++)
    {
        sum += (int)text[i];
    }
    return sum;
}