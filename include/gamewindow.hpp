#ifndef __GAMEWINDOW_H_
#define __GAMEWINDOW_H_

#include <QMainWindow>
#include <vector>
#include <QPalette>

#include "cell.hpp"

class GameWindow : public QMainWindow {
   Q_OBJECT

public:
        GameWindow();
private:
        std::vector<std::vector<Cell*>> map;
        std::vector<Cell*> curr_alive;
        std::vector<Cell*> updates;

        int width;
        int height;
        QPalette de;
        void create_menu_bar();
        void update_states();
        void remove_element_from_alive(Cell*);
        void add_element_to_alive(Cell*);
        std::vector<Cell*> get_neighbors(Cell*);
        void dump_vec(std::vector<Cell*>);
private slots:
        void next();
        void reset();
        void button_press();
};

#endif // __GAMEWINDOW_H_
