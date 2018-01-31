#ifndef EVENTSHANDLER_INTERFACES
#define EVENTSHANDLER_INTERFACES

/**
 * @brief Marker event class
 */
class EventInterface
{
    int hint = 0;
public:
    virtual void SetHint(int hint) { this->hint = hint; }
    virtual int GetHint() const { return hint; }
    virtual ~EventInterface() = default;
};

#endif // EVENTSHANDLER_INTERFACES

