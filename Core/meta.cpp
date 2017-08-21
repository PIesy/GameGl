#include <tuple>
#include "meta.h"

MetaInfo::MetaInfo(const std::string& name, int primaryVersion, int featureVersion) :
        name(name), primaryVersion(primaryVersion), featureVersion(featureVersion) {}

const std::string& MetaInfo::GetName() const
{
    return name;
}

void MetaInfo::SetName(const std::string& name)
{
    MetaInfo::name = name;
}

int MetaInfo::GetPrimaryVersion() const
{
    return primaryVersion;
}

void MetaInfo::SetPrimaryVersion(int primaryVersion)
{
    MetaInfo::primaryVersion = primaryVersion;
}

int MetaInfo::GetFeatureVersion() const
{
    return featureVersion;
}

void MetaInfo::SetFeatureVersion(int featureVersion)
{
    MetaInfo::featureVersion = featureVersion;
}

bool MetaInfo::operator==(const MetaInfo& rhs) const
{
    return std::tie(name, primaryVersion, featureVersion) == std::tie(rhs.name, rhs.primaryVersion, rhs.featureVersion);
}

bool MetaInfo::operator!=(const MetaInfo& rhs) const
{
    return !(rhs == *this);
}

namespace std
{

    template<>
    struct hash<MetaInfo>
    {
        using argument_type = MetaInfo;
        using result_type = std::size_t;

        result_type operator()(const argument_type& s) const
        {
            const result_type h1{std::hash<std::string>()(s.GetName() + std::to_string(s.GetPrimaryVersion()) + std::to_string(s.GetFeatureVersion()))};

            return h1;
        }
    };

}
