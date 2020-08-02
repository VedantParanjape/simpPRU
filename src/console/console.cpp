#include <iostream>
#include <string.h>
#include <string>
#include "console.hpp"

static int model_beaglebone__ = 0;

int firmware_loader(char* output_filename, int pru_id)
{
    FILE *model = fopen("/proc/device-tree/model", "r");
    if (model == NULL)
    {
        fprintf(stderr, "Error opening /proc/device-tree/model\n");
        return -1;
    }

    char model_name[50];
    for (int i = 0; fread(&model_name[i], sizeof(char), 1, model) == 1 ; i++);

    if (!strcmp(model_name, "TI AM335x BeagleBone Black Wireless"))
    {
        model_beaglebone__ = MODEL_BEAGLEBONE_BLACK_WIRELESS;
        printf("Detected %s\n", model_name);
    }
    // Add BBB also
    else if (!strcmp(model_name, "TI AM335x PocketBeagle"))
    {
        model_beaglebone__ = MODEL_POCKETBEAGLE;
        printf("Detected %s\n", model_name);
    }
    else if (!strcmp(model_name, "BeagleBoard.org BeagleBone AI"))
    {
        model_beaglebone__ = MODEL_BEAGLEBONE_AI;
        printf("Detected %s\n", model_name);
    }
    else
    {
        fprintf(stderr, "Error could not determine beagleboard model\n");
        return -1;
    }

    return 1;
}

using namespace ftxui;

class console : public Component 
{
    public:
        console() 
        {
            Add(&container);
            container.Add(&right_menu);
            container.Add(&input_box);
            container.Add(&pru_start_top);

            right_menu.entries = {
                L"PRU0", L"PRU1",
            };
            input_box.placeholder = L"type here";
            pru_start_top.entries = {
                L"Start", L"Stop"
            };

            right_menu.on_change = [this] {
                pru_id = right_menu.selected;
            };
            input_box.on_enter = [this] {
                try
                {
                    data_sent = std::stoi(input_box.content);
                }
                catch(const std::invalid_argument &err)
                {
                    std::cerr << err.what() << "\n";
                } 
            };
            pru_start_top.on_change = [this] {
                started = pru_start_top.selected;
            };
        }
  
    private:
        Container container = Container::Horizontal();
        RadioBox right_menu;
        Input input_box;
        Toggle pru_start_top;
        int pru_id;
        int data_sent;
        int started = -1;

        Element Render() override 
        {
            return border(vbox({
                // Console and PRU selection
                hbox({
                    hflow({
                        paragraph(std::to_wstring(pru_start_top.selected)),
                    }) | border,
                                        
                    vbox({
                        hcenter(bold(text(L"PRU"))),
                        separator(),
                        right_menu.Render(),
                    }) | border,
                }) | flex,
                
                // Input box and PRU start/stop
                vbox({
                    hbox({
                        text(L" send : "),
                        input_box.Render(),
                        separator(),
                        pru_start_top.Render(),
                    }),
                }) | border,
            }));
        }
};

int main(int argc, const char* argv[]) 
{
    auto screen = ScreenInteractive::Fullscreen();
    console console_simppru;
    // console_simppru.on_enter = screen.ExitLoopClosure();
    screen.Loop(&console_simppru);
}
