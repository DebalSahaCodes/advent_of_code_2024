#pragma once

#include<iostream>

bool is_safe_incr_or_decr(const int chng)
{
    const int i = (chng<0) ? -1*chng : chng;
    return i>0 && i<=3;
}

bool is_same_sign(const int i1, const int i2)
{
    return i1<0 && i2<0 || i1>=0 && i2>=0;
}

template<typename T>
void print_vec(const std::vector<T>& vec)
{
    std::cout << "\n";
    for(const T e : vec)
    {
        std::cout << e << " ";
    }
    std::cout << "\n";
}


bool day_2_report_safety_handler(const std::vector<int>& v1, int& pos)
{
    bool is_safe = true;

    for(int i=2; i<v1.size(); ++i)
    {
        const int chng1 = v1[i-1] - v1[i-2];
        const int chng2 = v1[i] - v1[i-1];
        
        is_safe = is_safe && is_same_sign(chng1, chng2) &&
                  is_safe_incr_or_decr(chng1) && is_safe_incr_or_decr(chng2);

        if(!is_safe)
        {
            pos = i;// return the highest index/position used
            return false;
        }
    }
    return true;
}


bool day_2_report_safety_analyser(const std::vector<int>& v1)
{
    bool is_safe = true;

    for(int i=2; i<v1.size(); ++i)
    {
        const int chng1 = v1[i-1] - v1[i-2];
        const int chng2 = v1[i] - v1[i-1];
        
        is_safe = is_safe && is_same_sign(chng1, chng2) &&
                  is_safe_incr_or_decr(chng1) && is_safe_incr_or_decr(chng2);
    }

    return is_safe;
}
