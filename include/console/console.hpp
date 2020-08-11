#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/radiobox.hpp>
#include <ftxui/component/container.hpp>
#include <ftxui/component/input.hpp>
#include <ftxui/component/toggle.hpp>

#define MODEL_POCKETBEAGLE 1
#define MODEL_BEAGLEBONE_BLACK 2
#define MODEL_BEAGLEBONE_BLACK_WIRELESS 3
#define MODEL_BEAGLEBONE_AI 4

using namespace ftxui;
class console : public Component 
{
    public:
        console(); 
        
    private:
        Container container = Container::Horizontal();
        RadioBox right_menu;
        Input input_box;
        Toggle pru_start_top;
        Elements output_box;
        int pru_id = 0;
        int data_sent = 0;
        int started = -1;
        
        Element Render() override;
};

int device_model();
int send_rpmsg_data(int value, int pru_id);
int receive_rpmsg_data(int pru_id, ftxui::Elements &output);
int start_pru(int pru_id);
int stop_pru(int pru_id);
#endif