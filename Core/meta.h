#ifndef META
#define META

#include <string>
#include <chrono>

class MetaInfo
{
    std::string name;
    std::string version;
    std::time_t timestamp;
public:
    MetaInfo(std::string name = "<none>", std::string version = "0.0.0", std::time_t timestamp = time(nullptr));
    const std::string& getName() const;
    const std::string& getVersion() const;
    std::time_t getTimestamp() const;
    bool operator ==(const MetaInfo& second) const;
};

namespace std {

template<>
struct hash<MetaInfo>
{
    typedef MetaInfo argument_type;
    typedef std::size_t result_type;

    result_type operator()(const argument_type& s) const
    {
        const result_type h1(std::hash<std::string>()(s.getName()));

        return h1;
    }
};

}
#endif // META

