#include <iostream>
#include "map.h"
#include "shop.h"
#include "player.h"
#include "monster.h"
#include "company.h"
#include "account.h"
using namespace std;

int main()
{
    // 初始化帳號系統
    Account account;
    string username, password;

    // 登入或創建帳號流程
    while (true)
    {
        cout << "=== RPG 遊戲帳號系統 ===" << endl;
        cout << "1. 登入\n2. 創建帳號\n3. 重設密碼\n4. 刪除帳號\n5. 退出" << endl;
        int choice;
        cin >> choice;

        if (choice == 1)
        {
            cout << "輸入帳號：";
            cin >> username;
            cout << "輸入密碼：";
            cin >> password;
            if (account.login(username, password))
            {
                break; // 成功登入，進入遊戲
            }
        }
        else if (choice == 2)
        {
            cout << "輸入新帳號：";
            cin >> username;
            cout << "輸入新密碼：";
            cin >> password;
            account.createUser(username, password);
        }
        else if (choice == 3)
        {
            cout << "輸入管理員密碼：";
            cin >> password;
            if (account.adminLogin(password))
            {
                cout << "管理員驗證成功。\n輸入要重設密碼的帳號：";
                cin >> username;
                cout << "輸入新密碼：";
                string newPassword;
                cin >> newPassword;
                account.resetPassword(username, newPassword);
            }
            else
            {
                cout << "管理員驗證失敗。" << endl;
            }
        }
        else if (choice == 4)
        {
            cout << "輸入帳號：";
            cin >> username;
            account.deleteUser(username);
        }
        else if (choice == 5)
        {
            cout << "退出系統，感謝使用！" << endl;
            return 0;
        }
        else
        {
            cout << "無效的選項，請重新選擇。" << endl;
        }
    }

    // 遊戲開始
    Company company;
    Shop shop;
    int jobChoice;
    cout << "選擇職業 (1. 哥布林戰士, 2.哥布林長老): ";
    cin >> jobChoice;
    JobType job = (jobChoice == 1) ? JobType::warrior : JobType::elder;
    Player player(username, job); // 使用登入的帳號名稱作為玩家名稱

    while (true)
    {
        int row, col;
        cout << "請設定地圖的大小（列,行）：" << endl;
        cin >> row >> col;
        Map map(player, row, col);
        string cmd;
        while (true)
        {
            for (int i = 0; i < row; i++)
                cout << "- ";
            cout << endl;

            cout << "輸入指令（w/a/s/d移動，pos查看怪物與玩家的位置，fight與怪物戰鬥，work工作，shop進入商店，bag查看背包，recover使用藥水恢復體力，stats查看狀態，upgrade提升屬性，learn學習技能，skills查看技能列表，pets查看寵物，sp選擇寵物，q退出遊戲）:" << endl;
            map.displayMap();

            for (int i = 0; i < row; i++)
                cout << "- ";
            cout << endl;

            cin >> cmd;

            if (cmd == "q")
            {
                cout << "確定要結束遊戲嗎？（y/n）：" << endl;
                string confirm;
                cin >> confirm;
                if (confirm == "y")
                {
                    cout << "遊戲結束！" << endl;
                    break;
                }
                else if (confirm == "n")
                {
                    cout << "遊戲繼續！" << endl;
                    continue;
                }
            }
            else if (cmd == "w" || cmd == "a" || cmd == "s" || cmd == "d")
            {
                map.movePlayer(cmd[0]);
            }
            else if (cmd == "pos")
            {
                cout << player.getName() << "目前位置：(" << map.getPlayerPosition().first << "," << map.getPlayerPosition().second << ")" << endl;
                map.checkMonster();
            }
            else if (cmd == "move")
            {
                string place;
                cout << "請輸入想要去的地方: ";
                cin >> place;
                map.movePlayerInstand(place);
            }
            else if (cmd == "fight")
            {
                pair<int, int> playerPosition = map.getPlayerPosition();
                vector<Monster *> monstersAtLocation = map.getMonstersAtPlayerLocation(playerPosition.first, playerPosition.second);
                if (player.getPlayerHealth() < 10)
                {
                    cout << "警告！！！體力即將耗盡，請至商店購買藥水以回復體力" << endl;
                }
                if (!monstersAtLocation.empty())
                {
                    for (Monster *monster : monstersAtLocation)
                    {
                        map.fightMonster(*monster);
                        if (player.getPlayerHealth() <= 0)
                        {
                            cout << player.getName() << " 用完了所有體力，遊戲結束" << endl;
                            return 0;
                        }
                    }
                }
                else
                {
                    cout << "當前位置沒有怪物。" << endl;
                }
            }
            else if (cmd == "work")
            {
                company.displayWorks();
                cout << "請輸入想要的工作名稱: ";
                string workName;
                cin >> workName;
                company.getWork(workName, player.getMoney(), player);
                map.displayMap();
            }
            else if (cmd == "shop")
            {
                if(map.checkIfPlayerAtShop()){
                    cout << "歡迎光臨商店" << endl;
                    map.enterShop();
                }
                else{
                    cout << "你不在商店附近" << endl;
                }
            }
            else if(cmd=="hospital"){
                if(map.checkIfPlayerAtHospital()){
                    cout << "歡迎光臨醫院" << endl;
                    map.enterHospital();
                }
                else{
                    cout << "你不在醫院附近" << endl;
                }
            }
            else if (cmd == "bag")
            {
                player.displayInventory();
                player.displayMoney();
                cout << "玩家剩餘體力: " << player.getPlayerHealth() << endl;
            }
            else if (cmd == "recover")
            {
                player.recover();
            }
            else if (cmd == "stats")
            {
                player.displayStats();
            }
            else if (cmd == "upgrade")
            {
                cout << "輸入要提升的屬性名稱（如 力量 或 敏捷）：";
                string attrName;
                cin >> attrName;
                player.upgradeAttribute(attrName);
            }
            else if (cmd == "learn")
            {
                player.learnSkillByNumber();
            }
            else if (cmd == "useSkill")
            {
                string skillName;
                cout << "輸入要使用的技能名稱：";
                cin >> skillName;
                player.useSkill(skillName);
            }
            else if (cmd == "upgradeSkill")
            {
                string skillName;
                cout << "輸入要升級的技能名稱：";
                cin >> skillName;
                player.upgradeSkill(skillName);
            }
            else if (cmd == "skills")
            {
                player.displaySkills();
            }
            else if (cmd == "pets")
            {
                player.displayPets();
            }
            else if (cmd == "sp")
            {
                player.displayPets();
                cout << "請輸入要切換的寵物編號：";
                int index;
                cin >> index;
                player.switchPet(index);
            }
            else

            {
                cout << "無效的指令，請重新輸入！" << endl;
            }
        }
    }

    return 0;
}
