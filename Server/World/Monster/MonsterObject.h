#pragma once
#include <iostream>
#include <string>

class MonsterObject
{
public:
    MonsterObject(uint64_t id, const std::string& name, int hp);
    void Tick();
    void TakeDamage(int dmg);
    uint64_t GetId() const;
    const std::string& GetName() const;

private:
    uint64_t _id;
    std::string _name;
    int _hp;
};