#ifndef PET_H
#define PET_H

#include <iostream>
#include <string>
using namespace std;

class Pet
{
private:
    string name;
    int level;
    int experience;
    int strength; // 攻擊加成

public:
    Pet(string n, int str) : name(n), level(1), experience(0), strength(str) {}

    void gainExperience(int exp)
    {
        experience += exp;
        if (experience >= level * 50)
        {
            experience -= level * 50;
            level++;
            strength += 5; // 每升級增加攻擊加成
            cout << "寵物 " << name << " 升級至等級 " << level << "！攻擊加成提升至 " << strength << endl;
        }
    }
    int getExperience()
    {
        return experience;
    }
    string getName() const { return name; }
    int getLevel() const { return level; }
    int getStrength() const { return strength; }
};

#endif
