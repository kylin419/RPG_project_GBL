#ifndef MONSTER_H
#define MONSTER_H

#include <string>
#include <iostream>
using namespace std;
class Monster
{
public:
    std::string name;    // 怪物名稱
    int x;               // 怪物所在的 X 座標
    int y;               // 怪物所在的 Y 座標
    int strength;        // 怪物的力量
    string mapName;      // 怪物所在地圖名稱

    // 建構子
    Monster(const std::string &name, int x, int y, int strength, const string &mapName)
        : name(name), x(x), y(y), strength(strength),mapName(mapName) {}

    // 預設建構子，防止初始化錯誤
    Monster() : name(""), x(-1), y(-1), strength(0) {}
    string getMapName() const // 修正方法以返回 mapName
    {
        return mapName;
    }
    void setMapName(const std::string &newMapName) // 修正方法以設定 mapName
    {
        mapName = newMapName;
    }
    void display() const
    {
        cout << "怪物名稱: " << name << ", 位置: (" << x << ", " << y
                  << "), 力量: " << strength << ", 所屬地圖: " << mapName << std::endl;
    }
    // 獲取怪物名稱
    const std::string &getName() const
    {
        return name;
    }

    // 設置怪物名稱
    void setName(const std::string &newName)
    {
        name = newName;
    }

    // 獲取怪物座標
    std::pair<int, int> getPosition() const
    {
        return make_pair(x, y);
    }

    // 設置怪物座標
    void setPosition(int newX, int newY)
    {
        x = newX;
        y = newY;
    }

    // 獲取怪物力量
    int getStrength() const
    {
        return strength;
    }

    // 設置怪物力量
    void setStrength(int newStrength)
    {
        strength = newStrength;
    }
    // 減少怪物力量
    void reduceStrength(int amount)
    {
        strength -= amount;
        if (strength < 0)
        {
            strength = 0;
        }
    }

    // 輸出怪物資訊
    void displayInfo() const
    {
        std::cout << "怪物名稱: " << name << ", 座標: (" << x << ", " << y
                  << "), 力量: " << strength << ", 所屬地圖: " << mapName << std::endl;
    }
};

#endif // MONSTER_H
