#ifndef CONFIG_H
#define CONFIG_H

#include <unordered_map>
#include <typeindex>

enum class EntryType {String, Number};

struct Value
{
    std::string string;
    int number;

    Value(const std::string& string):string(string) {}
    Value(int number):number(number) {}
};

struct Entry
{
    EntryType type;
    Value value;
};

class Config
{
    std::unordered_map<std::string, Entry> values;
public:
    int getValue(const std::string& key, int defaultValue = 0);
    std::string getValue(const std::string& key, std::string defaultValue = "<none>");
    void addValue(const std::string& key, int value);
    void addValue(const std::string& key, const std::string& value);
    bool isEmpty();
};

#endif // CONFIG_H

