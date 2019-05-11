#ifndef __CREATURE_H_
#define __CREATURE_H_

#include <QPushButton>
#include <QPalette>

class Cell : public QPushButton {
        Q_OBJECT

    public:
        using QPushButton::QPushButton;
        bool is_alive();
        void set_alive(bool);
        void toggle_alive();
        void set_palette(QPalette);
        void set_next_state(bool);
        void set_state(bool);
        void load_next_state();
        void reset();
    private:
        void update_button();
        bool alive_next = false;
        bool alive = false;
        QPalette pal;
};

#endif // __CREATURE_H_
