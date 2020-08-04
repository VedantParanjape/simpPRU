#include <iostream>
#include <string.h>
#include <string>
#include "console.hpp"

static int model_beaglebone__ = 0;

int device_model()
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

    fclose(model);
    return 1;
}

int send_data(int value, int pru_id)
{
    int rpmsg_handle = pru_id == 0 ? open("/dev/rpmsg_pru30", O_WRONLY) : open("/dev/rpmsg_pru31", O_WRONLY);
    if (rpmsg_handle < 0)
    {
        fprintf(stderr, "Error could not open /dev/rpmsg%d\n", pru_id == 0 ? 30 : 31);
        return -1;
    }

    const char* data = std::to_string(value).c_str();
    int data_sent = write(rpmsg_handle, data, sizeof(char)*strlen(data));
    close(rpmsg_handle);

    return data_sent;
}

// int receive_data(int pru_id)
// {
    
// }

int start_pru(int pru_id)
{
    int remoteproc_start = -1;

    if (model_beaglebone__ == MODEL_POCKETBEAGLE)
    {
        remoteproc_start = pru_id == 0 ? open("/sys/class/remoteproc/remoteproc1/state", O_RDWR) : open("/sys/class/remoteproc/remoteproc2/state", O_RDWR);
    }
    else
    {
        remoteproc_start = pru_id == 0 ? open("/sys/class/remoteproc/remoteproc0/state", O_RDWR) : open("/sys/class/remoteproc/remoteproc1/state", O_RDWR);
    }

    if (remoteproc_start < 0)
    {
        fprintf(stderr, "Error could not open /sys/class/remoteproc/");
        return -1;
    }

    char state[20] = " ";
    int bits_read = read(remoteproc_start, state, sizeof(char)*7);

    if (!strcmp(state, "offline") && bits_read > 0)
    {
        if (write(remoteproc_start, "start", 6*sizeof(char)) > 0)
        {
            close(remoteproc_start);
            return 1;
        }
        else
        {
            close(remoteproc_start);
            return -1;
        }
    }
    
    return 1;
}

int stop_pru(int pru_id)
{
    int remoteproc_stop = -1;

    if (model_beaglebone__ == MODEL_POCKETBEAGLE)
    {
        remoteproc_stop = pru_id == 0 ? open("/sys/class/remoteproc/remoteproc1/state", O_RDWR) : open("/sys/class/remoteproc/remoteproc2/state", O_RDWR);
    }
    else
    {
        remoteproc_stop = pru_id == 0 ? open("/sys/class/remoteproc/remoteproc0/state", O_RDWR) : open("/sys/class/remoteproc/remoteproc1/state", O_RDWR);
    }

    if (remoteproc_stop < 0)
    {
        fprintf(stderr, "Error could not open /sys/class/remoteproc/");
        return -1;
    }

    char state[20] = " ";
    int bits_read = read(remoteproc_stop, state, sizeof(char)*7);

    if (!strcmp(state, "running") && bits_read > 0)
    {
        if (write(remoteproc_stop, "stop", 5*sizeof(char)) > 0)
        {
            close(remoteproc_stop);
            return 1;
        }
        else
        {
            close(remoteproc_stop);
            return -1;
        }
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

            if (model_beaglebone__ == MODEL_BEAGLEBONE_AI)
            {
                right_menu.entries = {
                    L"PRU0-0", L"PRU0-1", L"PRU1-0", L"PRU1-1",
                };
            }
            else
            {
                right_menu.entries = {
                    L"PRU0", L"PRU1",
                };
            }
            
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
                    send_data(data_sent, pru_id);
                    output_box.push_back(select(text(input_box.content)));
                    input_box.content = L"";

                }
                catch(const std::invalid_argument &err)
                {
                    std::cerr << err.what() << "\n";
                } 
            };
            pru_start_top.on_change = [this] {
                started = pru_start_top.selected;
                if (started == 0)
                {
                    start_pru(pru_id);
                }
                else if (started == 1)
                {
                    stop_pru(pru_id);
                }
            };
        }
  
    private:
        Container container = Container::Horizontal();
        RadioBox right_menu;
        Input input_box;
        RadioBox pru_start_top;
        Elements output_box;
        int pru_id = 0;
        int data_sent = 0;
        int started = -1;
        
        Element Render() override 
        {
            return border(vbox({
                // Console and PRU selection
                hbox({
                    hflow({
                        vbox({
                            output_box ,
                        }),
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
    int model = device_model();
    if (model == -1)
    {
        fprintf(stderr, "Not a beagleboard device\n");
        // return 0;
    }

    auto screen = ScreenInteractive::Fullscreen();
    console console_simppru;
    screen.Loop(&console_simppru);
}
