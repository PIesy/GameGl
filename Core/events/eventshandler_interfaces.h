#ifndef EVENTSHANDLER_INTERFACES
#define EVENTSHANDLER_INTERFACES

/**
 * @brief Marker event class
 */
class EventInterface
{
    int hint;
public:
    virtual void setHint(int hint) { this->hint = hint; }
    virtual int getHint() { return hint; }
    virtual ~EventInterface() {}
};

#endif // EVENTSHANDLER_INTERFACES

