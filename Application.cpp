//
// Created by permal on 6/24/17.
//

#include <algorithm>
#include <esp_event.h>
#include <esp_event_loop.h>
#include <nvs_flash.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <smooth/Application.h>
#include <smooth/ipc/Link.h>

using namespace std::chrono;

namespace smooth
{
    Application::Application(const std::string& name, uint32_t stack_depth, UBaseType_t priority,
                             int max_waiting_messages)
            : Task(name, stack_depth, priority, max_waiting_messages, milliseconds(100)),
              systemQueue(name + std::string("-Queue"), 10)
    {
        nvs_flash_init();

        // Setup the system event callback so that we receive events.
        ESP_ERROR_CHECK(esp_event_loop_init(&Application::event_callback, this));
    }

    Application::~Application()
    {
    }

    void Application::loop()
    {
        system_event_t event;
        if (systemQueue.pop(event, std::chrono::seconds(1)))
        {
            // Publish event to listeners
            smooth::ipc::Link<system_event_t>::publish(event);
        }
    }


    esp_err_t Application::event_callback(void* ctx, system_event_t* event)
    {
        Application* app = reinterpret_cast<Application*>( ctx );
        if (app != nullptr)
        {
            if (!app->systemQueue.push(*event))
            {
                ESP_LOGE("Application", "Failed to enqueue event with id %d.", event->event_id);
            }
            else
            {
                ESP_LOGV("Application", "Got event with id %d.", event->event_id);
            }
        }

        return ESP_OK;
    }

    void Application::set_system_log_level(esp_log_level_t level) const
    {
        // Silence wifi logging
        std::vector<const char*> logs{"wifi", "tcpip_adapter"};
        for (auto log : logs)
        {
            esp_log_level_set(log, level);
        }
    }

}