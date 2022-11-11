#ifndef CAL_DATA_H
#define CAL_DATA_H

class CalData
{
public:
    bool is_num;
    int val;
    int count;
    void operator=(CalData);
    void print_data_to_k_str();
    CalData();
};

#endif