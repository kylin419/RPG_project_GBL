#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <vector>
#include<unordered_map>
#include<unistd.h>
#include "pet.h"
using namespace std;

enum class JobType
{
    elder,
    warrior
};
struct Skill
{
    string name;
    int level;
    int damage;
    int manaCost;
    int upgradeCost;

    Skill() : name(""), level(1), damage(0), manaCost(0), upgradeCost(0) {}

    Skill(string n, int d, int m, int u)
        : name(n), level(1), damage(d), manaCost(m), upgradeCost(u) {}
};

class Player
{
private:
    string name;
    int money;
    JobType job;
    int playerHealth = 100;
    int mana = 50;                // 魔力值
    string equippedWeapon = "無"; // 預設無裝備
    int weaponDamage = 0;         // 武器攻擊力
    int weaponHealth= 0;     // 武器耐久度
    vector<string> inventory; //背包
    int level = 1;                         // 玩家等級
    int attributePoints = 0;               // 屬性點數
    int skillPoints = 0;                   // 技能點數
    unordered_map<string, int> attributes; // 儲存各項屬性
    unordered_map<string, Skill> skills;
    vector<Pet> pets;         // 玩家擁有的寵物
    Pet *activePet = nullptr; // 當前攜帶的寵物

public:
int orignalHealth = 100;
int orignalMana = 50;
    Player(string name,JobType job) : money(200),name(name),job(job) {
        if (job == JobType::warrior)
        {
            orignalHealth += orignalHealth * 0.3; // 獸人特性
            playerHealth =orignalHealth; // 獸人特性
            attributes["力量"] = 10;
        }
        else if (job == JobType::elder)
        {
            orignalMana += mana * 0.5; 
            mana = orignalMana;
            // 精靈特性
            attributes["敏捷"] = 10;
        }
    }

    void setName(string name) { this->name = name; }
    string getName() { return this->name; }
    void setMoney(int m){
        this->money = money-m;
    }
    void displayStats()
    {
        cout << "玩家名稱: " << name << endl;
        cout << "職業: " << (job == JobType::warrior ? "哥布林戰士" : "哥布林長老") << endl;
        cout << "目前體力:" << playerHealth << endl;
        cout << "目前魔力:" << mana << endl;
        cout << "金幣餘額: " << money << endl;
        cout << "等級: " << level << " (屬性點數: " << attributePoints << ", 技能點數: " << skillPoints << ")" << endl;
        cout << "當前屬性: " << endl;
        for (auto &attr : attributes)
        {
            cout << "- " << attr.first << ": " << attr.second << endl;
        }
    }
    bool equipWeapon(const string &weapon)
    {
        if ((job == JobType::warrior && weapon == "戰斧") ||
            (job == JobType::elder && weapon == "法杖"))
        {
            inventory.push_back(weapon);
            cout << "成功裝備武器：" << weapon << endl;
            return true;
        }
        else
        {
            cout << "此職業無法裝備該武器。" << endl;
            return false;
        }
    }
    void gainExperience(int exp)
    {
        int levelUpThreshold = 100; // 每 100 經驗升級
        if (exp >= levelUpThreshold)
        {
            level++;
            attributePoints += 5; // 升級獲得 5 屬性點數
            skillPoints += 2;     // 升級獲得 2 技能點數
            cout << "升級成功！目前等級: " << level << endl;
            cout << "獲得屬性點數: 5, 技能點數: 2" << endl;
        }
    }
    void gainAttributePoints(int points)
    {
        attributePoints += points;
        cout << "獲得 " << points << " 屬性點數！當前屬性點數: " << attributePoints << endl;
    }
    void displayInventory()
    {
        cout << "背包內容：" << endl;
        if (inventory.empty())
        {
            cout << "背包是空的。" << endl;
        }
        else
        {
            for (const auto &item : inventory)
            {
                cout << "- " << item << endl;
            }
        }
    }

    void displayMoney()
    {
        cout << "金幣餘額：" << money << " 金幣" << endl;
    }
    void upgradeAttribute(const string &attrName)
    {
        if (attributePoints > 0 && attributes.find(attrName) != attributes.end())
        {
            attributes[attrName]++;
            attributePoints--;
            cout << "屬性 " << attrName << " 提升至 " << attributes[attrName] << "！" << endl;
        }
        else
        {
            cout << "屬性點數不足或無效屬性名稱！" << endl;
        }
    }
    void learnSkillByNumber()
    {
        vector<pair<string, Skill> > skillChoices;
        skillChoices.emplace_back(make_pair("地圖記憶", Skill("地圖記憶", 30, 15, 3)));
        skillChoices.emplace_back(make_pair("眼不見為淨", Skill("眼不見為淨", 25, 10, 2)));
        skillChoices.emplace_back(make_pair("自嘲術", Skill("自嘲術", 40, 20, 4)));

        cout << "可學習的技能列表：" << endl;
        for (size_t i = 0; i < skillChoices.size(); ++i)
        {
            cout << i + 1 << ". " << skillChoices[i].first
                 << " (傷害：" << skillChoices[i].second.damage
                 << ", 魔力消耗：" << skillChoices[i].second.manaCost
                 << ", 升級所需技能點數：" << skillChoices[i].second.upgradeCost << ")" << endl;
        }

        cout << "請輸入要學習的技能編號：" << endl;
        int choice;
        cin >> choice;

        if (choice >= 1 && choice <= skillChoices.size())
        {
            string skillName = skillChoices[choice - 1].first;
            Skill skill = skillChoices[choice - 1].second;

            if (skills.find(skillName) == skills.end())
            {
                skills[skillName] = skill;
                cout << "學習技能：" << skillName << " 成功！" << endl;
            }
            else
            {
                cout << "你已經學習了該技能！" << endl;
            }
        }
        else
        {
            cout << "無效的選擇！" << endl;
        }
    }

void useSkill(const string &skillName) {
        if (skills.find(skillName) != skills.end()) {
            Skill &skill = skills[skillName];
            if (mana >= skill.manaCost) {
                mana -= skill.manaCost;
                cout << "使用技能 " << skill.name << "！造成 " << skill.damage << " 點傷害，消耗 " << skill.manaCost << " 點魔力。" << endl;
            } else {
                cout << "魔力不足，無法使用技能！" << endl;
            }
        } else {
            cout << "你尚未學習該技能！" << endl;
        }
    }

    void upgradeSkill(const string &skillName) {
        if (skills.find(skillName) != skills.end()) {
            Skill &skill = skills[skillName];
            if (skillPoints >= skill.upgradeCost) {
                skillPoints -= skill.upgradeCost;
                skill.level++;
                skill.damage += 10;
                skill.manaCost += 5;
                skill.upgradeCost += 2;
                cout << "技能 " << skill.name << " 升級成功！當前等級：" << skill.level << endl;
            } else {
                cout << "技能點數不足，無法升級！" << endl;
            }
        } else {
            cout << "你尚未學習該技能！" << endl;
        }
    }

    void displaySkills() {
        cout << "當前技能列表：" << endl;
        int count = 1;
        for (const auto &pair : skills)
        {
            const Skill &skill = pair.second;
            cout << count++ << ". " << skill.name
                 << " (等級: " << skill.level
                 << ", 傷害: " << skill.damage
                 << ", 魔力消耗: " << skill.manaCost
                 << ", 升級所需技能點數: " << skill.upgradeCost << ")" << endl;
        }
        if (skills.empty())
        {
            cout << "尚未學習任何技能！" << endl;
        }
    }
    string selectSkill()
    {
        if (skills.empty())
        {
            cout << "你尚未學習任何技能！" << endl;
            return "";
        }

        cout << "==== 選擇技能 ====" << endl;
        int index = 1;
        vector<string> skillNames;

        for (const auto &pair : skills)
        {
            const Skill &skill = pair.second;
            cout << index << ". " << skill.name
                 << " (傷害: " << skill.damage
                 << ", 魔力消耗: " << skill.manaCost
                 << ")" << endl;
            skillNames.push_back(pair.first);
            index++;
        }

        cout << "請輸入技能編號：" << endl;
        int choice;
        cin >> choice;

        if (choice >= 1 && choice <= skillNames.size())
        {
            return skillNames[choice - 1];
        }
        else
        {
            cout << "無效的選擇！" << endl;
            return "";
        }
    }

    int getSkillDamage(const string &skillName)
    {
        if (skills.find(skillName) != skills.end())
        {
            return skills[skillName].damage;
        }
        else
        {
            cout << "你尚未學習技能 " << skillName << "！" << endl;
            return 0;
        }
    }

    void recoverMana(int amount) {
        mana += amount;
        cout << "魔力恢復 " << amount << " 點，當前魔力：" << mana << endl;
    }
    void earnMoney(int amount)
    {
        money += amount;
        cout << "獲得 " << amount << " 金幣！當前金幣餘額: " << money << endl;
    }

    void setPlayerHealth(int event) { playerHealth += event; }
    int getPlayerHealth() { return playerHealth; }
    int &getMoney() { return money; }
    vector<string> &getInventory() { return inventory; }

    void recover()
    {
        auto it = find(inventory.begin(), inventory.end(), "藥水");
        if (it != inventory.end())
        {
            inventory.erase(it);
            setPlayerHealth(5);
            cout << "回復體力中......" << endl;
            usleep(900000);
            cout << "你使用了一瓶藥水，恢復了5點體力。" << endl;
            cout << "當前體力：" << playerHealth << endl;
        }
        else
        {
            cout << "背包中沒有藥水！" << endl;
        }
    }

    void equipWeapon(const string &weapon, int damage)
    {
        equippedWeapon = weapon;
        weaponDamage = damage;
        cout << "已裝備武器：" << equippedWeapon << "（攻擊力：" << weaponDamage << "）" << endl;
    }

    string getEquippedWeapon() { return equippedWeapon; }
    int getWeaponDamage() { return weaponDamage; }
    void setweaponHealth(int h){
        this->weaponHealth = h;
    }
    void removeWeapon()
    {
        if (equippedWeapon != "無")
        {
            auto it = find(inventory.begin(), inventory.end(), equippedWeapon);
            if (it != inventory.end())
            {
                inventory.erase(it); // 從背包中移除武器
            }
            cout << "武器 " << equippedWeapon << " 已損壞，從背包中移除。" << endl;
            equippedWeapon = "無";
            weaponDamage = 0;
            weaponHealth= 0;
        }
    }
    void reduceWeaponHealth()
    {
        if (weaponHealth > 0)
        {
            weaponHealth--;
            cout << "武器 " << equippedWeapon << " 耐久度減少，剩餘耐久度：" << weaponHealth<< endl;
            if (weaponHealth == 0)
            {
                cout << "武器 " << equippedWeapon << " 已損壞！" << endl;
                removeWeapon();
            }
        }
    }
    void addPet(const string &petName, int strength)
    {
        for (auto &pet : pets)
        {
            if (pet.getName() == petName)
            {
                pet.gainExperience(strength * 10); // 獎勵經驗值，提升等級
                cout << "你的寵物 " << petName << " 獲得經驗值並提升等級！" << endl;
                return; // 結束方法，避免新增重複寵物
            }
        }

        // 如果沒有同名寵物，新增新寵物
        pets.emplace_back(petName, strength);
        cout << "獲得新寵物：" << petName << "！" << endl;
        if (!activePet)
        {
            activePet = &pets.back();
            cout << "當前寵物設為：" << petName << endl;
        }
    }

    void displayPets()
    {
        if (pets.empty())
        {
            cout << "你尚未擁有任何寵物！" << endl;
            return;
        }

        cout << "你擁有的寵物列表：" << endl;
        for (size_t i = 0; i < pets.size(); ++i)
        {
            cout << i + 1 << ". " << pets[i].getName()
                 << " (等級：" << pets[i].getLevel()
                 << ", 攻擊加成：" << pets[i].getStrength()<<",經驗值："<<pets[i].getExperience() << ")" << endl;
        }

        if (activePet)
        {
            cout << "當前攜帶的寵物：" << activePet->getName() << endl;
        }
    }

    void switchPet(int index)
    {
        if (index > 0 && index <= pets.size())
        {
            activePet = &pets[index - 1];
            cout << "切換至寵物：" << activePet->getName() << endl;
        }
        else
        {
            cout << "無效的選擇。" << endl;
        }
    }

    int getActivePetBonus()
    {
        return activePet ? activePet->getStrength() : 0;
    }

    void activePetGainExp(int exp)
    {
        if (activePet)
        {
            activePet->gainExperience(exp);
        }
    }
};

#endif