#include <iostream>
#include <string.h>
#include <string>
#include <errno.h>
#include "console.hpp"
#include "ftxui/component/component.hpp"       
#include "ftxui/component/component_base.hpp" 
#include "ftxui/component/screen_interactive.hpp" 
#include "ftxui/dom/elements.hpp" 
#include "ftxui/screen/color.hpp" 

static int model_beaglebone__ = 0;
static int rpmsg_fd = -1;
std::thread read_rpmsg_thread;
std::timed_mutex rpmsg_mutex;
std::mutex output_box_mutex;
std::atomic_bool stop_read_signal(false);
std::atomic_bool update_screen(false);
FILE *fd = NULL;

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
    else if (!strcmp(model_name, "TI AM335x BeagleBone Black"))
    {
        model_beaglebone__ = MODEL_BEAGLEBONE_BLACK;
        printf("Detected %s\n", model_name);
    }
    else if (!strcmp(model_name, "TI AM335x BeagleBone Blue"))
    {
        model_beaglebone__ = MODEL_BEAGLEBONE_BLUE;
        printf("Detected %s\n", model_name);
    }
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

int send_rpmsg_data(int value, int pru_id)
{
    int rpmsg_fd = -1;
    while(rpmsg_fd < 0)
    {
       char path[30] = " ";
       snprintf(path, 30, "/dev/rpmsg_pru%d", pru_id%4+30);
       rpmsg_fd = open(path, O_RDWR);
    }
    if (rpmsg_fd < 0)
    {
        fprintf(stderr, "Error could not open /dev/rpmsg%d\n", pru_id%4+30);
        fprintf(fd, "Error could not open /dev/rpmsg%d\n", pru_id%4+30);
        
        return -1;
    }

    std::string value_str = std::to_string(value);
    const char *data = value_str.c_str();

    std::unique_lock<std::timed_mutex> rpmsg_lock(rpmsg_mutex, std::defer_lock);

    if (rpmsg_lock.try_lock_for(std::chrono::milliseconds(1000)))
    {
        fprintf(fd, "%s", "fd locked\n");

        int data_sent = write(rpmsg_fd, data, sizeof(char)*strlen(data));
        fprintf(fd, "%s: %d\n", data, (int)strlen(data));

        rpmsg_lock.unlock();
        fprintf(fd, "%s", "fd unlocked\n");

        close(rpmsg_fd);
        return data_sent;
    }

    close(rpmsg_fd);
    return -1;
}

int receive_rpmsg_data(int pru_id, ftxui::Elements &output)
{    
    while(!stop_read_signal.load())
    {
        char buffer[512];
        for (int i = 0; i < 512; i++)
        {
            buffer[i] = '\0';
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::unique_lock<std::timed_mutex> rpmsg_lock(rpmsg_mutex, std::defer_lock);
        if (rpmsg_lock.try_lock_for(std::chrono::milliseconds(1000)))
        {   
            int rpmsg_fd = -1;
            while(rpmsg_fd < 0 && !stop_read_signal.load())
            {
                char path[30] = " ";
                snprintf(path, 30, "/dev/rpmsg_pru%d", pru_id%4+30);
                rpmsg_fd = open(path, O_RDWR | O_NONBLOCK);
            }
            if (rpmsg_fd < 0)
            {
                fprintf(stderr, "Error could not open /dev/rpmsg%d\n", pru_id%4+30);
                fprintf(fd, "Error could not open /dev/rpmsg%d\n", pru_id%4+30);
            }

            int data_read = read(rpmsg_fd, buffer, 512);
            rpmsg_lock.unlock();
            
            if (data_read > 0)
            {
                buffer[strlen(buffer)-1] = '\0';
                update_screen.store(true, std::memory_order_relaxed);
                fprintf(fd, "recv: %s: %d\n", buffer, (int)strlen(buffer));
                
                std::lock_guard<std::mutex> output_box_lock(output_box_mutex);
                output.push_back(ftxui::text(std::wstring(&buffer[0], &buffer[strlen(buffer)])));                
            }
            close(rpmsg_fd);
        }
        else
        {
            fprintf(fd, "error recv: %s\n", "couldn't unlock mutex");
        }
    }
    stop_read_signal.store(false, std::memory_order_relaxed);

    return 1;
}

int start_pru(int pru_id)
{
    int remoteproc_start = -1;

    if (model_beaglebone__ == MODEL_POCKETBEAGLE)
    {
        remoteproc_start = pru_id == 0 ? open("/sys/class/remoteproc/remoteproc1/state", O_RDWR) : open("/sys/class/remoteproc/remoteproc2/state", O_RDWR);
    }
    else if (model_beaglebone__ == MODEL_BEAGLEBONE_AI)
    {
        char path[100] = " ";
        snprintf(path, 100, "/sys/class/remoteproc/remoteproc%d/state", pru_id%4+4);
        remoteproc_start = open(path, O_RDWR);
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
    else
    {
        return -1;
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
    else if (model_beaglebone__ == MODEL_BEAGLEBONE_AI)
    {
        char path[100] = " ";
        snprintf(path, 100, "/sys/class/remoteproc/remoteproc%d/state", pru_id%4+4);
        remoteproc_stop = open(path, O_RDWR);
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
    else
    {
        return -1;
    }
    
    return 1;
}

using namespace ftxui;
console::console() 
{
    auto container = Container::Vertical({
      pru_id_menu,
      Container::Horizontal({
        input_box,
        pru_start_top,
        quit_button,
      }),
    });
    Add(container);

    if (model_beaglebone__ == MODEL_BEAGLEBONE_AI)
    {
        pru_id_entries = {
            L"PRU0-0",
            L"PRU0-1",
            L"PRU1-0",
            L"PRU1-1",
        };
    }
    else
    {
        pru_id_entries = {
            L"PRU0",
            L"PRU1",
        };
    }
    pru_start_top_entries = {
        L"Start",
        L"Stop",
    };

    InputBase::From(input_box)->on_enter = [this] {
        try
        {
            data_sent = std::stoi(input_box_content);
            input_box_content = L"";
            if (started == 0)
            {
                send_rpmsg_data(data_sent, pru_id);
            }
        }
        catch(const std::invalid_argument &err)
        {
            std::cerr << err.what() << "\n";
        } 
    };
    ToggleBase::From(pru_start_top)->on_enter = [this] {
        started = pru_start_top_selected;

        if (started == 0)
        {
            if (start_pru(pru_id) == 1)
            {
                send_rpmsg_data(-1, pru_id);

                read_rpmsg_thread = std::thread(receive_rpmsg_data, pru_id, std::ref(output_box));
                read_rpmsg_thread.detach();
            }
        }
        else if (started == 1)
        {
            if (stop_pru(pru_id) == 1)
            {
                stop_read_signal.store(true, std::memory_order_relaxed);
                output_box.clear();
            }
        }
    };
    
}
  
Element console::Render() 
{
    std::lock_guard<std::mutex> output_box_lock(output_box_mutex);
    
    return border(vbox({
        // Console and PRU selection
        hbox({
            hbox({
                vbox({
                    vbox({output_box}),
                    text(L"   ") | ftxui::select,
                }) | frame,                                        
            }) | flex | border,
            vbox({
                hcenter(bold(text(L"PRU"))),
                separator(),
                pru_id_menu->Render(),
            }) | border,
        }) | flex,
        
        // Input box , PRU start/stop
        vbox({
            hbox({
                text(L" send : "),
                input_box->Render(),
                separator(),
                pru_start_top->Render(),
                separator(),
                quit_button->Render() | bold | color(Color::Red),
            }),
        }) | border,
    }));
}

int main(int argc, const char* argv[]) 
{
    Component quit_button;
    fd = fopen("/tmp/log.txt", "w+");

    int model = device_model();
    if (model == -1)
    {
        fprintf(stderr, "Not a beagleboard device\n");
        // return 0;
    }

    stop_pru(0);
    stop_pru(1);
    stop_pru(2);
    stop_pru(3);
    
    auto screen = ScreenInteractive::Fullscreen();
   
    std::thread update([&screen]() {
    for (;;) {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      if(update_screen.load())
      {
          update_screen.store(false, std::memory_order_relaxed);
          screen.PostEvent(Event::Custom);
      }
    }
    });
    screen.Loop(ftxui::Make<console>());
    

}
