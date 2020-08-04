#include "UpdateGroup.hpp"

void UpdateGroup::Init() {
    Objects = vector<Object*>();
    thrd = thread(&UpdateGroup::Work, this);
}

void UpdateGroup::Work() {

    Working = true;

    while (Working) {
        
        while (!Update) {}
        
        auto Pre = high_resolution_clock::now();

        size_t Size = Objects.size();
        
        for (size_t Index = 0; Index < Size; Index++)
            Objects[Index]->ExecuteCode(UPDATE);

        Update = false;
        
        auto Now = high_resolution_clock::now();
        UpdateTime = duration_cast<milliseconds>(Now - Pre);
    }
}