#include "config.h"

int Config::getValue(const std::string& key, int defaultValue)
{
    if (!values.count(key))
        return defaultValue;
    Entry& val = values.at(key);

    return val.type == EntryType::Number ? val.value.number : defaultValue;
}

std::string Config::getValue(const std::string &key, std::string defaultValue)
{
    if (!values.count(key))
        return defaultValue;
    Entry& val = values.at(key);

    return val.type == EntryType::String ? val.value.string : defaultValue;
}

void Config::addValue(const std::string& key, const std::string& value)
{
    if(values.count(key))
    {
        values.at(key).value.string = value;
        return;
    }
    values.insert({key, {EntryType::String, value}});
}

void Config::addValue(const std::string &key, int value)
{
    if(values.count(key))
    {
        values.at(key).value.string = value;
        return;
    }
    values.insert({key, {EntryType::Number, value}});
}

bool Config::isEmpty()
{
    return values.size() == 0;
}
