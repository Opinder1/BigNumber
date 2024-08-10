#include <cstdint>
#include <cstdio>
#include <cmath>

#include <vector>

using Base10Number = std::vector<unsigned char>;
using Base256Number = std::vector<unsigned char>;

void Base10Multiply(Base10Number& number, size_t value)
{
    if (value == 0)
    {
        number.clear();
        return;
    }

    size_t carry = 0;
    size_t i = 0;

    while (i < number.size() || carry != 0)
    {
        if (number.size() <= i)
        {
            number.push_back(0);
        }

        size_t val = (number[i] * value) + (carry % 10);

        number[i] = val % 10;
        
        carry = (carry / 10) + (val / 10);

        i++;
    }
}

void Base10Add(Base10Number& number, size_t value)
{
    size_t carry = 0;
    size_t i = 0;

    while (i < number.size() || carry != 0)
    {
        if (number.size() <= i)
        {
            number.push_back(0);
        }

        size_t val = number[i] + (value % 10) + carry;

        number[i] = val % 10;
        
        carry = (carry / 10) + (val / 10);

        value /= 10;
        i++;
    }
}

void Base10Add(Base10Number& number, const Base10Number& value)
{
    if (number.size() < value.size())
    {
        number.resize(value.size());
    }

    size_t carry = 0;
    size_t i = 0;
    
    while (i < number.size() || carry != 0)
    {
        if (number.size() <= i)
        {
            number.push_back(0);
        }

        size_t val = number[i] + value[i] + (carry % 10);

        number[i] = val % 10;
        
        carry = (carry / 10) + (val / 10);

        i++;
    }
}

Base10Number Base10Power(size_t value, size_t power)
{
    Base10Number number;
    
    const double log10_v = std::log10(value);

    size_t end_digits = (power * log10_v) + 1;

    if (number.size() < end_digits)
    {
        number.resize(end_digits);
    }

    number[0] = 1;

    for (size_t p = 0; p < power; p++)
    {
        Base10Multiply(number, value);
    }

    return number;
}

void Base10Print(const Base10Number& number)
{
    for (size_t i = number.size(); i --> 0;)
    {
        printf("%u", number[i]);
    }
    printf("\n");
}

Base10Number Base256ToBase10(const Base256Number& number)
{
    Base10Number total;
    Base10Number multi;

    multi.resize(1);
    multi[0] = 1;

    for (size_t i = 0; i < number.size(); i++)
    {
        Base10Number temp_multi = multi;

        unsigned char value = number[i];

        if (value != 0)
        {
            Base10Multiply(temp_multi, value);

            Base10Add(total, temp_multi);
        }

        Base10Multiply(multi, 256);
    }

    return total;
}

int main()
{
    for (size_t i = 0; i < 33; i++)
    {
        Base10Print(Base10Power(2, i));
    }

    size_t i = 12345125;
    char* ip = (char*)&i;

    Base256Number number;
    number.resize(8);
    std::copy(ip, ip + sizeof(size_t), number.data());

    try
    {
        Base10Print(Base256ToBase10(number));
    }
    catch (std::exception& e)
    {

    }
}
