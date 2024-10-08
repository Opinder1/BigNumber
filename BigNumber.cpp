#include <cstdint>
#include <cstdio>
#include <cmath>

#include <vector>

template<uint64_t Base>
struct BaseNumber : std::vector<uint64_t> {};

const uint64_t k_max_base = UINT32_MAX - 1;
const uint64_t k_max_pow_10 = 1000000000u;

template<uint64_t Base>
BaseNumber<Base> BaseMake(uint64_t value)
{
    BaseNumber<Base> out;

    while (value != 0)
    {
        out.push_back(value % Base);

        value /= Base;
    }

    return out;
}

template<uint64_t Base>
int8_t BaseCompare(const BaseNumber<Base>& first, const BaseNumber<Base>& second)
{
    if (first.size() > second.size())
    {
        return 1;
    }

    if (first.size() < second.size())
    {
        return -1;
    }

    for (size_t i = 0; i < first.size(); i++)
    {
        if (first[i] > second[i])
        {
            return 1;
        }

        if (first[i] < second[i])
        {
            return -1;
        }
    }

    return 0;
}

template<size_t Base>
void BaseMultiply(BaseNumber<Base>& number, uint64_t value)
{
    if (value == 0)
    {
        number.clear();
        return;
    }

    uint64_t carry = 0;
    size_t i = 0;

    while (i < number.size() || carry != 0)
    {
        if (number.size() <= i)
        {
            number.push_back(0);
        }

        uint64_t val = (number[i] * value) + (carry % Base);

        number[i] = val % Base;
        
        carry = (carry / Base) + (val / Base);

        i++;
    }
}

template<size_t Base>
void BaseMultiply(BaseNumber<Base>& number, const BaseNumber<Base>& value)
{

}

template<uint64_t Base>
void BaseAdd(BaseNumber<Base>& number, uint64_t value)
{
    uint64_t carry = 0;
    size_t i = 0;

    while (i < number.size() || carry != 0)
    {
        if (number.size() <= i)
        {
            number.push_back(0);
        }

        uint64_t val = number[i] + (value % Base) + carry;

        number[i] = val % Base;
        
        carry = (carry / Base) + (val / Base);

        value /= Base;
        i++;
    }
}

template<uint64_t Base>
void BaseAdd(BaseNumber<Base>& number, const BaseNumber<Base>& value)
{
    if (number.size() < value.size())
    {
        number.resize(value.size());
    }

    uint64_t carry = 0;
    size_t i = 0;
    
    while (i < number.size() || carry != 0)
    {
        if (number.size() <= i)
        {
            number.push_back(0);
        }

        uint64_t val = number[i] + value[i] + (carry % Base);

        number[i] = val % Base;
        
        carry = (carry / Base) + (val / Base);

        i++;
    }
}

template<uint64_t Base>
void BaseSubtract(BaseNumber<Base>& number, uint64_t value)
{

}

template<uint64_t Base>
void BaseSubtract(BaseNumber<Base>& number, const BaseNumber<Base>& value)
{

}

template<uint64_t Base>
BaseNumber<Base> BasePower(uint64_t value, uint64_t power)
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

template<uint64_t Base>
void BaseShift(BaseNumber<Base>& number, uint64_t shift_amount)
{

}

template<uint64_t Base>
BaseNumber<Base> BaseDivide(const BaseNumber<Base>& number, const BaseNumber<Base>& value)
{
    BaseNumber<Base> out;
    BaseNumber<Base> attempt;
    const BaseNumber<Base> one = BaseMake<Base>(1);

    BaseNumber<Base> pow2 = BasePower<Base>(2, number.size());

    while (BaseCompare<Base>(pow2, one) == 1)
    {
        BaseNumber<Base> attempt_change = pow2;
        BaseMultiply<Base>(attempt_change, value);

        if (BaseCompare<Base>(attempt, number) == 1)
        {
            BaseAdd<Base>(out, pow2);
            BaseAdd<Base>(attempt, attempt_change);
        }

        if (BaseCompare<Base>(attempt, number) == -1)
        {
            BaseSubtract<Base>(out, pow2);
            BaseSubtract<Base>(attempt, attempt_change);
        }

        BaseShift<Base>(pow2, 1);
    }

    return out;
}

template<uint64_t BaseFrom, uint64_t BaseTo>
BaseNumber<BaseTo> BaseConvert(const BaseNumber<BaseFrom>& number)
{
    BaseNumber<BaseTo> total;
    BaseNumber<BaseTo> multi;

    multi.resize(1);
    multi[0] = 1;

    for (size_t i = 0; i < number.size(); i++)
    {
        BaseNumber<BaseTo> temp_multi = multi;

        uint64_t value = number[i];

        if (value != 0)
        {
            BaseMultiply<BaseTo>(temp_multi, value);

            BaseAdd<BaseTo>(total, temp_multi);
        }

        BaseMultiply<BaseTo>(multi, BaseFrom);
    }

    return total;
}

template<uint64_t Base>
void BasePrint(const BaseNumber<Base>& number)
{
    constexpr const size_t zeros = std::log10(Base);

    size_t i = number.size();

    printf("%u", number[--i]);

    for (; i --> 0;)
    {
        printf("%.*u", zeros, number[i]);
    }
    printf("\n");
}

int main()
{
    for (size_t i = 1; i < 512; i *= 2)
    {
        BasePrint<10>(BasePower<10>(2, i));
    }

    for (size_t i = 1; i < 512; i *= 2)
    {
        BasePrint<k_max_pow_10>(BasePower<k_max_pow_10>(2, i));
    }

    BaseNumber<k_max_base> number = BaseMake<k_max_base>(12345125);

    BaseDivide(number, BaseMake<k_max_base>(123));

    BasePrint<k_max_pow_10>(BaseConvert<k_max_base, k_max_pow_10>(number));
}
