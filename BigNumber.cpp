#include <cstdint>
#include <cstdio>
#include <cmath>

#include <vector>

template<size_t Base>
using BaseNumber = std::vector<unsigned char>;

template<size_t Base>
void BaseMultiply(BaseNumber<Base>& number, size_t value)
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

        size_t val = (number[i] * value) + (carry % Base);

        number[i] = val % Base;
        
        carry = (carry / Base) + (val / Base);

        i++;
    }
}

template<size_t Base>
void BaseAdd(BaseNumber<Base>& number, size_t value)
{
    size_t carry = 0;
    size_t i = 0;

    while (i < number.size() || carry != 0)
    {
        if (number.size() <= i)
        {
            number.push_back(0);
        }

        size_t val = number[i] + (value % Base) + carry;

        number[i] = val % Base;
        
        carry = (carry / Base) + (val / Base);

        value /= Base;
        i++;
    }
}

template<size_t Base>
void BaseAdd(BaseNumber<Base>& number, const BaseNumber<Base>& value)
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

        size_t val = number[i] + value[i] + (carry % Base);

        number[i] = val % Base;
        
        carry = (carry / Base) + (val / Base);

        i++;
    }
}

template<size_t Base>
BaseNumber<Base> BasePower(size_t value, size_t power)
{
    BaseNumber<Base> number;
    
    constexpr const double log_base = std::log(Base);
    const double log_value = std::log(value) / log_base;

    size_t end_digits = (power * log_value) + 1;

    if (number.size() < end_digits)
    {
        number.resize(end_digits);
    }

    number[0] = 1;

    for (size_t p = 0; p < power; p++)
    {
        BaseMultiply<Base>(number, value);
    }

    return number;
}

void Base10Print(const BaseNumber<10>& number)
{
    for (size_t i = number.size(); i --> 0;)
    {
        printf("%u", number[i]);
    }
    printf("\n");
}

void Base100Print(const BaseNumber<100>& number)
{
    for (size_t i = number.size(); i --> 0;)
    {
        printf("%.2u", number[i]);
    }
    printf("\n");
}

BaseNumber<10> Base256ToBase10(const BaseNumber<256>& number)
{
    BaseNumber<10> total;
    BaseNumber<10> multi;

    multi.resize(1);
    multi[0] = 1;

    for (size_t i = 0; i < number.size(); i++)
    {
        BaseNumber<10> temp_multi = multi;

        unsigned char value = number[i];

        if (value != 0)
        {
            BaseMultiply<10>(temp_multi, value);

            BaseAdd<10>(total, temp_multi);
        }

        BaseMultiply<10>(multi, 256);
    }

    return total;
}

int main()
{
    for (size_t i = 0; i < 33; i++)
    {
        Base10Print(BasePower<10>(2, i));
    }

    for (size_t i = 0; i < 33; i++)
    {
        Base100Print(BasePower<100>(2, i));
    }

    size_t i = 12345125;
    char* ip = (char*)&i;

    BaseNumber<256> number;
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
