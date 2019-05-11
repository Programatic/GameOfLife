#include "../include/cell.hpp"

bool Cell::is_alive()
{
        return alive;
}

void Cell::set_alive(bool s)
{
        alive = s;
}

void Cell::set_palette(QPalette pale)
{
        pal = pale;
}

void Cell::load_next_state()
{
        set_state(alive_next);
}

void Cell::set_state(bool state)
{
        alive = state;
        update_button();
}

void Cell::set_next_state(bool state)
{
        alive_next = state;
}

bool Cell::get_next_state()
{
        return alive_next;
}

int Cell::get_i()
{
        return i;
}

int Cell::get_j()
{
        return j;
}

void Cell::update_button()
{
        if (alive) {
                pal.setColor(QPalette::Button, QColor(Qt::red));
                setAutoFillBackground(true);
                setPalette(pal);
                update();
        } else {
                setAutoFillBackground(false);
                setPalette(pal);
                update();
                set_alive(false);
        }
}

void Cell::reset()
{
        alive_next = false;
        set_state(false);
}

void Cell::set_coordinates(int i, int j)
{
        this->i = i;
        this->j = j;
}
