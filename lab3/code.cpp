#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <queue>

class QNS
{
    public:
        QNS(int powerLevel = 5000, int durability = 500, int energyStorage = 300, int heatLevel = 0);
        QNS(const QNS& qns);
        
        //getters
        inline int getPowerLevel();
        inline int getDurability();
        inline int getEnergyStorage();
        inline int getHeatLevel();

        //methods
        void boostPower(int factor);
        void boostPower(QNS& otherSuit);

        //operator overloading
        QNS& operator+(QNS& otherSuit);
        QNS& operator-(int factor);
        QNS& operator*(int factor);
        QNS& operator/(int factor);
        bool operator==(QNS& otherSuit);
        bool operator<(QNS& otherSuit);

    private:
        int powerLevel_;
        int durability_;
        int energyStorage_;
        int heatLevel_;
};

std::queue<QNS> suits;

class Avenger
{
    public:
        Avenger(std::string avName, QNS avSuit, int strength);
        Avenger() : suit_() {}

        //methods
        void attack(Avenger& enemy);
        void upgradeSuit();
        void repair(int x);
        void printStatus();

        //getters
        QNS& getSuit()
        {
            return suit_;
        }
        std::string getName()
        {
            return name_;
        }

    private:
        std::string name_;
        QNS suit_;
        int attackStrength_;
};

class Battle
{
    public:
        Battle() {}

        //methods
        void startBattle();
        void printBattleLog();
        int result();

    private:
        std::vector<Avenger> avengers_;
        std::map<std::string,Avenger*> av;
        std::vector<Avenger> enemies_;
        std::vector<std::string> battleLog_;
};

//qns methods
QNS::QNS(int powerLevel, int durability, int energyStorage, int heatLevel)
{
    this->powerLevel_ = std::min(powerLevel, 5000);
    this->durability_ = durability;
    this->energyStorage_ = energyStorage;
    this->heatLevel_ = std::max(heatLevel, 0);
}

QNS::QNS(const QNS& otherSuit)
{
    this->powerLevel_ = std::min(otherSuit.powerLevel_, 5000);
    this->durability_ = otherSuit.durability_;
    this->energyStorage_ = otherSuit.energyStorage_;
    this->heatLevel_ = std::max(otherSuit.heatLevel_, 0);
}

int QNS::getPowerLevel()
{
    return this->powerLevel_;
}

int QNS::getDurability()
{
    return this->durability_;
}

int QNS::getEnergyStorage()
{
    return this->energyStorage_;
}

int QNS::getHeatLevel()
{
    return this->heatLevel_;
}

void QNS::boostPower(int factor)
{
    (*this) * factor;
}

void QNS::boostPower(QNS& otherSuit)
{
    (*this) + otherSuit;
}

QNS& QNS::operator+(QNS& otherSuit)
{
    powerLevel_ = std::min(5000, powerLevel_ + otherSuit.energyStorage_);
    durability_ += otherSuit.durability_;
    energyStorage_ += otherSuit.powerLevel_;
    return *this;
}

QNS& QNS::operator-(int factor)
{
    durability_ -= factor;
    energyStorage_ += factor;
    heatLevel_ += factor;
    heatLevel_ = std::max(heatLevel_, 0);
    return *this;
}

QNS& QNS::operator*(int factor)
{
    powerLevel_ += (powerLevel_*factor)/100;
    powerLevel_ = std::min(powerLevel_, 5000);
    energyStorage_ += 5*factor;
    heatLevel_ += factor;
    heatLevel_ = std::max(heatLevel_, 0);
    return *this;
}

QNS& QNS::operator/(int factor)
{
    durability_ += factor;
    heatLevel_ -= factor;
    heatLevel_ = std::max(0, heatLevel_);
    return *this;
}

bool QNS::operator==(QNS& otherSuit)
{
    return (powerLevel_ == otherSuit.powerLevel_) && (durability_ == otherSuit.durability_);
}

bool QNS::operator<(QNS& otherSuit)
{
    return powerLevel_ + durability_ < otherSuit.powerLevel_ + otherSuit.durability_;
}

//avenger methods
Avenger::Avenger(std::string avName, QNS avSuit, int strength)
{
    name_ = avName;
    suit_ = avSuit;
    attackStrength_ = strength;
}

void Avenger::attack(Avenger& enemy)
{
    enemy.suit_ - attackStrength_;
}

void Avenger::upgradeSuit()
{
    suit_ + suits.front();
    suits.pop();
}

void Avenger::repair(int x)
{
    suit_/x;
}

void Avenger::printStatus()
{
    std::cout << name_ << " " << suit_.getPowerLevel() << " " << suit_.getDurability() << " " << suit_.getEnergyStorage() << " " << suit_.getHeatLevel() << std::endl;
}

//battle methods
void Battle::startBattle()
{
    int k, n, m;
    std::cin >> k >> n >> m;
    avengers_.reserve(n);
    enemies_.reserve(m);
    for (int i = 0; i < k; i++)
    {
        int P, D, E, H;
        std::cin >> P >> D >> E >> H;
        QNS newSuit(P, D, E, H);
        suits.push(newSuit);
    }

    for (int i = 0; i < n; i++)
    {
        std::string name;
        int strength;
        std::cin >> name >> strength;
        if(suits.size() == 0)
        {
            std::cout << name << " is out of fight" << std::endl;
            continue;
        }
        auto suit = suits.front();
        suits.pop();
        Avenger newAv(name, suit, strength);
        this->avengers_.push_back(newAv);
        av[name] = &avengers_[avengers_.size() - 1]; 
    }
    for (int i = 0; i < m; i++)
    {
        std::string name;
        int strength;
        std::cin >> name >> strength;
        if (suits.size() == 0)
        {
            std::cout << name << " is out of fight" << std::endl;
            continue;
        }
        auto suit = suits.front();
        suits.pop();
        Avenger newAv(name, suit, strength);
        this->enemies_.push_back(newAv);
        av[name] = &enemies_[enemies_.size() - 1];
    }
    
    std::string battleString;
    std::cin >> battleString;

    while (true)
    {
        std::cin >> battleString;

        if (battleString == "Attack")
        {
            std::string av1, av2;
            std::cin >> av1 >> av2;
            if (av.find(av1) == av.end() || av.find(av2) == av.end()) continue;
            if ((*av[av1]).getSuit().getDurability() <= 0 || (*av[av1]).getSuit().getHeatLevel() > 500 || (*av[av2]).getSuit().getDurability() <= 0) continue;

            battleLog_.push_back(av1 + " attacks " + av2);
            (*av[av1]).attack((*av[av2]));
            if ((*av[av2]).getSuit().getDurability() <= 0) battleLog_.push_back(av2 + " suit destroyed");
            else if ((*av[av2]).getSuit().getHeatLevel() > 500) battleLog_.push_back(av2 + " suit overheated");
        }
        else if (battleString == "Repair")
        {
            std::string av1;
            std::cin >> av1;
            int x;
            std::cin >> x;
            if (av.find(av1) == av.end()) continue;
            (*av[av1]).repair(x);
            battleLog_.push_back(av1 + " repaired");
        }
        else if (battleString == "BoostPowerByFactor")
        {
            std::string av1;
            std::cin >> av1;
            int factor;
            std::cin >> factor;
            if (av.find(av1) == av.end()) continue;
            (*av[av1]).getSuit().boostPower(factor);
            battleLog_.push_back(av1 + " boosted");
            if ((*av[av1]).getSuit().getHeatLevel() > 500)
            {
                battleLog_.push_back(av1 + " suit overheated");
            }
        }
        else if (battleString == "BoostPower")
        {
            std::string av1;
            std::cin >> av1;
            int P, D, E, H;
            std::cin >> P >> D >> E >> H;
            if (av.find(av1) == av.end()) continue;
            QNS newSuit(P, D, E, H);
            battleLog_.push_back(av1 + " boosted");
            (*av[av1]).getSuit().boostPower(newSuit);
        }
        else if (battleString == "AvengerStatus")
        {
            std::string av1;
            std::cin >> av1;
            // std::cout << (*av[av1]).getName() << std::endl;
            if (av.find(av1) == av.end()) continue;

            (*av[av1]).printStatus();
        }
        else if (battleString == "Upgrade")
        {
            std::string av1;
            std::cin >> av1;
            if (av.find(av1) == av.end()) continue;
            if (suits.size() == 0)
            {
                battleLog_.push_back(av1 + " upgrade Fail");
                continue;
            }
            (*av[av1]).upgradeSuit();
            battleLog_.push_back(av1 + " upgraded");
        }
        else if (battleString == "PrintBattleLog")
        {
            printBattleLog();
        }
        else if (battleString == "BattleStatus")
        {
            int res = result();
            if (res == 1) 
            {
                std::cout << "heroes are winning" << std::endl;
            }
            else if (res == -1)
            {
                std::cout << "enemies are winning" << std::endl;
            }
            else std::cout << "tie" << std::endl;
        }
        else break;
    }

}

void Battle::printBattleLog()
{
    for(auto &x : battleLog_)
    {
        std::cout << x << std::endl;
    }
}

int Battle::result()
{
    int res = 0;
    for (auto &x: avengers_)
    {
        if (x.getSuit().getDurability() > 0)
        {
            res += x.getSuit().getPowerLevel() + x.getSuit().getDurability();
        }
    }

    for (auto &x : enemies_)
    {
        if (x.getSuit().getDurability() > 0)
        {
            res -= x.getSuit().getPowerLevel() + x.getSuit().getDurability();
        }
    }

    if (res > 0) return 1;
    else if (res < 0) return -1;
    else return 0;
}

int main()
{
    Battle newBattle;
    newBattle.startBattle();    
}