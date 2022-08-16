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
#define MODEL_BEAGLEBONE_BLUE 5

using namespace ftxui;
class console : public ComponentBase
{
    public:
        console(); 
        static inline auto screen = ScreenInteractive::Fullscreen();
        
    private:
        int pru_id = 0;
        std::vector<std::wstring> pru_id_entries;
        Component pru_id_menu = Menu(&pru_id_entries, &pru_id);

        std::wstring input_box_content;
        Component input_box = Input(&input_box_content, "type here");

        int pru_start_top_selected = 0;
        std::vector<std::wstring> pru_start_top_entries;
        Component pru_start_top = Toggle(&pru_start_top_entries, &pru_start_top_selected);
       
        Component button = Button(" PRU ", NULL);

        int started = 0;
        Elements output_box;
        int data_sent = 0;
        
        Element Render() override;
};

int device_model();
int send_rpmsg_data(int value, int pru_id);
int receive_rpmsg_data(int pru_id, ftxui::Elements &output);
int start_pru(int pru_id);
int stop_pru(int pru_id);
#endif
