#include <iostream>
#include <string>
#include <vector>

// size_t TABLE_SIZE = 16;

enum Choice {
    I = 73,
    L = 76,
    D = 68,
    P = 80,
    S = 83,
    E = 69,
    A = 65,
    C = 67,
    Q = 81,
    R = 82
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

unsigned int sdbmHash(string str) {
	unsigned int hash = 0;
	unsigned int i = 0;
	unsigned int len = str.length();

	for (i = 0; i < len; i++)
	{
		hash = (str[i]) + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

int hashf(string key, int m)
{
    return sdbmHash(key) % m;
}


