#ifndef META
#define META

#include <string>
#include <chrono>

class MetaInfo
{
    std::string name;
    int primaryVersion;
    int featureVersion;
public:
    explicit MetaInfo(const std::string& name = "", int primaryVersion = 0, int featureVersion = 0);
    const std::string& GetName() const;
    void SetName(const std::string& name);
    int GetPrimaryVersion() const;
    void SetPrimaryVersion(int primaryVersion);
    int GetFeatureVersion() const;
    void SetFeatureVersion(int featureVersion);
    bool operator==(const MetaInfo& rhs) const;
    bool operator!=(const MetaInfo& rhs) const;
};
#endif // META

