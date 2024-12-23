#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <tuple>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include "shop.h"
#include "player.h"
#include "monster.h"
using namespace std;

class Map
{
private:
    unordered_map<string, vector<vector<char>>> maps;
    unordered_map<string, unordered_map<string, string>> mapConnections;
    string currentMap;
    vector<vector<char>> mapGrid;
    vector<Monster> monsters;
    vector<string> petName = {
        "小火龍",
        "影狼",
        "狂獅",
        "狂狼",
        "狂虎"};
    unordered_map<string, tuple<string, int, int>> locationCoords; // No changes here
    unordered_map<string, vector<pair<int, int>>> shopLocations;   // Shop locations grouped by map
    unordered_map<string, vector<vector<char>>> landmarkMap;       // Stores landmark symbols for each map

    int playerX, playerY;
    int row, col;
    Player &player;
    Shop shop;
    int monstersDefeated = 0;

    const int refreshWhenAllDefeat;

public:
    Map(Player &p, int row, int col) : currentMap("洞穴一"), row(row), col(col), playerX(1), playerY(1), player(p), refreshWhenAllDefeat((row / 2) * 3)
    {
        maps["洞穴一"] = vector<vector<char>>(row, vector<char>(col, '.'));
        maps["洞穴二"] = vector<vector<char>>(row, vector<char>(col, '.'));
        maps["洞穴三"] = vector<vector<char>>(row, vector<char>(col, '.'));
        generateMonsters((row / 2) * 3);
        maps["洞穴一"][4][4] = 'M';
        maps["洞穴二"][5][8] = 'T';
        maps["洞穴三"][4][5] = 'H';
        locationCoords["商店"] = make_tuple("洞穴一", 4, 4);
        locationCoords["市集"] = make_tuple("洞穴二", 5, 8);
        locationCoords["醫院"] = make_tuple("洞穴三", 4, 5);
        shopLocations["洞穴一"].emplace_back(4, 4);
        shopLocations["洞穴二"].emplace_back(5, 8);

        // Set map connections
        mapConnections["洞穴一"]["右"] = "洞穴二";
        mapConnections["洞穴二"]["左"] = "洞穴一";
        mapConnections["洞穴二"]["右"] = "洞穴三";
        mapConnections["洞穴三"]["左"] = "洞穴二";

        landmarkMap["洞穴一"] = maps["洞穴一"];
        landmarkMap["洞穴二"] = maps["洞穴二"];
        landmarkMap["洞穴三"] = maps["洞穴三"];

        // Set landmarks in the landmark map
        landmarkMap["洞穴一"][4][4] = 'M';
        landmarkMap["洞穴二"][5][8] = 'T';
        landmarkMap["洞穴三"][4][5] = 'H';

        // Place player on the initial map
        maps[currentMap][playerX][playerY] = '@';
    }

    void displayMap()
    {
        auto &mapGrid = maps[currentMap];
        for (int i = 0; i < mapGrid.size(); ++i)
        {
            for (int j = 0; j < mapGrid[i].size(); ++j)
            {
                if (i == playerX && j == playerY)
                {
                    cout << '@' << " "; // 玩家位置
                }
                else
                {
                    cout << mapGrid[i][j] << " "; // 顯示地圖內容
                }
            }
            cout << endl;
        }
    }

    string currentMapName()
    {
        return currentMap;
    }
    void switchMap(char direction)
    {
        string nextMap = "";
        int newPlayerX = playerX, newPlayerY = playerY;

        if (direction == 'w' && mapConnections[currentMap].count("上"))
        {
            nextMap = mapConnections[currentMap]["上"];
            newPlayerX = row - 1;
        }
        else if (direction == 's' && mapConnections[currentMap].count("下"))
        {
            nextMap = mapConnections[currentMap]["下"];
            newPlayerX = 0;
        }
        else if (direction == 'a' && mapConnections[currentMap].count("左"))
        {
            nextMap = mapConnections[currentMap]["左"];
            newPlayerY = col - 1;
        }
        else if (direction == 'd' && mapConnections[currentMap].count("右"))
        {
            nextMap = mapConnections[currentMap]["右"];
            newPlayerY = 0;
        }

        if (!nextMap.empty())
        {
            cout << "切換至地圖：" << nextMap << endl;

            maps[currentMap][playerX][playerY] = '.'; // 清空舊地圖玩家位置

            currentMap = nextMap;
            playerX = newPlayerX;
            playerY = newPlayerY;

            maps[currentMap][playerX][playerY] = '@'; // 更新新地圖玩家位置

            cout << "目前地圖怪物數量：" << monsters.size() << endl;
        }
        else
        {
            cout << "無法移動到該方向，已達地圖邊界！" << endl;
        }
    }

    void movePlayer(char direction)
    {
        auto &mapGrid = maps[currentMap];
        int newX = playerX, newY = playerY;

        // Calculate new position based on direction
        switch (direction)
        {
        case 'w':
            newX--;
            break;
        case 'a':
            newY--;
            break;
        case 's':
            newX++;
            break;
        case 'd':
            newY++;
            break;
        default:
            cout << "無效指令！" << endl;
            return;
        }

        // If new position is out of bounds, switch map
        if (newX < 0 || newX >= row || newY < 0 || newY >= col)
        {
            switchMap(direction);
            return;
        }

        // Restore the original cell value at the current player position
        mapGrid[playerX][playerY] = landmarkMap[currentMap][playerX][playerY];

        // Update player position
        playerX = newX;
        playerY = newY;

        // Place player at the new position
        mapGrid[playerX][playerY] = '@';
    }

    void hosipital()
    {
        if (player.getMoney() < 50)
        {
            cout << "你的金幣不足以支付醫療費用！" << endl;
        }
        else
        {
            cout << "你現在的體力還剩" << player.getPlayerHealth() << "，需要回復體力嗎？（Y/N）" << endl;
            string choice;
            cin >> choice;
            if (choice == "Y" || choice == "y")
            {
                if (player.getPlayerHealth() < player.orignalHealth)
                {
                    player.setPlayerHealth(player.orignalHealth);
                    player.setMoney(100);
                    cout << "你的體力已經回復滿了！" << endl;
                }
                else if (player.getPlayerHealth() == player.orignalHealth)
                {
                    cout << "你的體力已經滿了！" << endl;
                }
            }
            else
            {
                cout << "你選擇不回復體力！" << endl;
            }
        }
    }
    bool checkIfPlayerAtShop()
    {
        // Check if there are shops on the current map
        if (shopLocations.find(currentMap) != shopLocations.end())
        {
            // Get the list of shops on the current map
            const auto &shops = shopLocations[currentMap];

            // Check if the player's position matches any shop location
            for (const auto &[shopX, shopY] : shops)
            {
                if (playerX == shopX && playerY == shopY)
                {
                    cout << "你在商店位置，可以進入商店！" << endl;
                    return true;
                }
            }
        }

        // If no match is found
        return false;
    }
    bool isShopLocation(int x, int y)
    {
        if (currentMap == "洞穴一")
        {
            if (x == 4 && y == 4)
            {
                return true;
            }
        }
        else if (currentMap == "洞穴二")
        {
            if (x == 5 && y == 8)
            {
                return true;
            }
        }
        return false;
    }
    bool checkIfPlayerAtHospital()
    {
        if (currentMap == "洞穴三")
        {
            if (playerX == 4 && playerY == 5)
            {
                return true;
            }
        }
        return false;
    }
    bool
    hasMonsterAt(int x, int y)
    {
        for (Monster &monster : monsters)
        {
            if (monster.x == x && monster.y == y && monster.getMapName() == currentMap) // 檢測地圖名稱
            {
                return true;
            }
        }
        return false;
    }
    pair<int, int> getPlayerPosition() const
    {
        return make_pair(playerX, playerY);
    }

    void movePlayerInstand(const string &locationName)
    {
        if (locationCoords.find(locationName) != locationCoords.end())
        {
            // Retrieve target map and coordinates
            const auto &[targetMap, targetX, targetY] = locationCoords[locationName];

            // Check if coordinates are valid
            if (targetX < 0 || targetX >= row || targetY < 0 || targetY >= col)
            {
                cout << "傳送座標無效，請檢查地圖設置！" << endl;
                return;
            }

            // Restore the original cell value at the current player position
            auto &mapGrid = maps[currentMap];
            mapGrid[playerX][playerY] = landmarkMap[currentMap][playerX][playerY]; // Restore landmark

            // Update player position and current map
            currentMap = targetMap;
            playerX = targetX;
            playerY = targetY;

            // Place player at the new position
            maps[currentMap][playerX][playerY] = '@';

            cout << "已傳送至 " << locationName << " (" << currentMap << ": " << playerX << ", " << playerY << ")" << endl;
        }
        else
        {
            cout << "地點 " << locationName << " 不存在。有效地點有：" << endl;
            for (const auto &location : locationCoords)
            {
                cout << "- " << location.first << endl;
            }
        }
    }

    void enterShop()
    {

        cout << "歡迎來到商店！" << endl;

        shop.displayItems(); // 顯示商品列表

        while (true)
        {
            cout << "請輸入要購買的商品編號 (輸入 0 離開商店)：";
            int itemIndex;
            cin >> itemIndex;

            if (itemIndex == 0)
            {
                cout << "離開商店，歡迎下次光臨！" << endl;
                break;
            }

            if (!shop.buyItem(itemIndex, player.getMoney(), player.getInventory(), player))
            {
                cout << "購買失敗，請重新選擇！" << endl;
            }
            player.displayMoney(); // 顯示剩餘金幣
        }
    }
    void enterHospital()
    {
        hosipital();
    }
    void generateMonsters(int totalCount)
    {
        vector<string> mapsList = {"洞穴一", "洞穴二", "洞穴三"};
        vector<string> monsterNames = {"女冒險者", "女精靈", "背叛者哥布林"};

        int monstersPerMap = totalCount / mapsList.size();
        int remainingMonsters = totalCount % mapsList.size();

        for (const string &mapName : mapsList)
        {
            int count = monstersPerMap + (remainingMonsters > 0 ? 1 : 0);
            if (remainingMonsters > 0)
                remainingMonsters--;

            auto &mapGrid = maps[mapName];

            for (int i = 0; i < count; i++)
            {
                int x, y;
                do
                {
                    x = rand() % row;
                    y = rand() % col;
                } while (mapGrid[x][y] != '.'); // 確保生成位置有效

                string name = monsterNames[rand() % monsterNames.size()];
                int strength = rand() % 25 + 1;

                Monster monster(name, x, y, strength, mapName);
                monsters.push_back(monster); // 確保加入到 monsters 列表中

                mapGrid[x][y] = 'i'; // 標記怪物位置
            }
            cout << "在 " << mapName << " 生成了 " << count << " 隻怪物。" << endl;
        }
    }

    void removeMonster(Monster *monster)
    {
        auto it = std::find_if(monsters.begin(), monsters.end(), [&](const Monster &m)
                               { return &m == monster; }); // 比較指針

        if (it != monsters.end())
        {
            // 清除地圖上的標記
            maps[currentMap][it->x][it->y] = '.';

            // 同步更新 landmarkMap
            landmarkMap[currentMap][it->x][it->y] = '.';

            // 從列表中移除怪物
            monsters.erase(it);

            cout << "怪物已被移除。" << endl;
        }
        else
        {
            cout << "錯誤：未找到要移除的怪物。" << endl;
        }
    }

    void fightMonster(Monster &monster)
    {
        while (monster.strength > 0 && player.getPlayerHealth() > 0)
        {
            cout << "怪物：" << monster.name << "（剩餘強度：" << monster.strength << "）" << endl;
            cout << "選擇攻擊方式：1. 普通攻擊 2. 技能攻擊 3. 使用武器 4. 恢復體力" << endl;

            string choice;
            cin >> choice;
            int damage = 0;

            if (choice == "1")
            {
                damage = rand() % 5 + 1;
                cout << "你對 " << monster.name << " 造成了 " << damage << " 點傷害。" << endl;
            }
            else if (choice == "2")
            {
                string skillName = player.selectSkill();
                if (!skillName.empty())
                {
                    player.useSkill(skillName);
                    int skillDamage = player.getSkillDamage(skillName);
                    if (skillDamage > 0)
                    {
                        monster.strength -= skillDamage;
                    }
                }
            }
            else if (choice == "3")
            {
                if (player.getWeaponDamage() > 0)
                {
                    damage = player.getWeaponDamage();
                    cout << "你使用武器 " << player.getEquippedWeapon() << " 對 " << monster.name << " 造成了 " << damage << " 點傷害！" << endl;
                    player.reduceWeaponHealth();
                }
                else
                {
                    cout << "你未裝備武器，無法使用武器攻擊。" << endl;
                    continue;
                }
            }
            else if (choice == "4")
            {
                player.recover();
                continue;
            }
            else
            {
                cout << "無效的選擇，請重新選擇。" << endl;
                continue;
            }

            monster.strength -= damage;

            if (monster.strength <= 0)
            {
                cout << "怪物 " << monster.name << " 已被擊敗！" << endl;
                monstersDefeated++;
                if (monstersDefeated == refreshWhenAllDefeat)
                {
                    refreshMonsters();
                    monstersDefeated = 0;
                }
                if (rand() % 100 < 80) // 寵物獲取機率
                {
                    player.addPet(petName[rand() % 5], abs(monster.strength / 2));
                }

                int expGained = rand() % 50 + 10;
                int moneyEarned = rand() % 20 + 5;
                cout << "恭喜你成功防止自己被騙出洞穴！獲得經驗值 " << expGained << " 和金幣 " << moneyEarned << endl;

                player.gainExperience(expGained);
                player.earnMoney(moneyEarned);

                removeMonster(&monster); // 使用指針
                break;
            }

            // 怪物反擊
            int monsterDamage = rand() % 4 + 1;
            player.setPlayerHealth(-monsterDamage);
            cout << monster.name << " 使出反擊，造成了 " << monsterDamage << " 點的傷害。你剩餘體力：" << player.getPlayerHealth() << endl;

            if (player.getPlayerHealth() <= 0)
            {
                cout << "你已無法繼續戰鬥。" << endl;
                break;
            }
        }
    }

    // 刷新怪物資訊
    void refreshMonsters()
    {
        monsters.clear();
        generateMonsters(row / 2);
    }
    void checkMonster()
    {
        cout << "怪物所在位置：" << endl;

        // 遍歷所有怪物
        for (const Monster &monster : monsters)
        {
            cout << "- [" << monster.getMapName() << "](" << monster.x << "," << monster.y << ")" << endl;
        }
    }

    vector<Monster *> getMonstersAtPlayerLocation(int playerX, int playerY)
    {
        vector<Monster *> monstersAtLocation;

        for (Monster &monster : monsters)
        {
            if (monster.x == playerX && monster.y == playerY)
            {
                monstersAtLocation.push_back(&monster);
            }
        }

        return monstersAtLocation;
    }
};
