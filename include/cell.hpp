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
        void set_palette(QPalette);
        void set_next_state(bool);
        void set_state(bool);
        void load_next_state();
        void reset();
        void set_coordinates(int, int);
        bool get_next_state();
        int get_i();
        int get_j();
    private:
        int i;
        int j;
        void update_button();
        bool alive_next = false;
        bool alive = false;
        QPalette pal;
};

#endif // __CREATURE_H_
