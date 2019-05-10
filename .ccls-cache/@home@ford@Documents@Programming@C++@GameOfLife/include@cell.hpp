#ifndef __CREATURE_H_
#define __CREATURE_H_

#include <QPushButton>

class Cell : public QPushButton {
        Q_OBJECT
    public:
        using QPushButton::QPushButton;
        bool is_alive();
        void set_alive(bool);
    private:
        bool alive;
};

#endif // __CREATURE_H_
