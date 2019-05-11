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
        int width;
        int height;
        QPalette de;
        void create_menu_bar();
        int number_neighbors(int, int);
        void update_states();
private slots:
        void next();
        void reset();
        void button_press();
};

#endif // __GAMEWINDOW_H_
