#include "meta.h"

MetaInfo::MetaInfo(std::string name, std::string version, time_t timestamp)
{
    this->name = name;
    this->version = version;
    this->timestamp = timestamp;
}

const std::string& MetaInfo::getName() const
{
    return name;
}

const std::string& MetaInfo::getVersion() const
{
    return version;
}

std::time_t MetaInfo::getTimestamp() const
{
    return timestamp;
}

bool MetaInfo::operator ==(const MetaInfo& second) const
{
    return name == second.name && version == second.version && timestamp == second.getTimestamp();
}
