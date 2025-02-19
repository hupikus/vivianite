#ifndef ANY_TAB
#define ANY_TAB

class Tab
{
public:

    virtual ~Tab() = default;
protected:
    Tab() = default;
    Tab(const Tab&) = delete;
    Tab& operator=(const Tab&) = delete;
};

#endif
