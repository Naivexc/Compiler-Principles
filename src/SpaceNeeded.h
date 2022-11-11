#ifndef SPACE_NEEDED_H
#define SPACE_NEEDED_H

struct SpaceNeeded
{
    int total_needed;
    int return_address_needed;
    int params_needed;
    int variate_needed;
    SpaceNeeded();
};

#endif