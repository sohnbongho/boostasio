#pragma once
#include <iostream>
#include <string>

class MonsterObject
{
public:
    MonsterObject(uint64_t id, const std::string& name, int hp = 100)
        : _id(id), _name(name), _hp(hp) {}

    void Tick()
    {
        std::cout << "[Monster Tick] ID: " << _id << ", HP: " << _hp << std::endl;
    }

    void TakeDamage(int dmg)
    {
        _hp -= dmg;
        if (_hp <= 0)
        {
            _hp = 0;
            std::cout << "[Monster Dead] " << _name << " (ID: " << _id << ")\n";
        }
        else
        {
            std::cout << "[Monster Hit] " << _name << " took " << dmg << " damage. HP: " << _hp << "\n";
        }
    }

    uint64_t GetId() const { return _id; }
    const std::string& GetName() const { return _name; }

private:
    uint64_t _id;
    std::string _name;
    int _hp;
};