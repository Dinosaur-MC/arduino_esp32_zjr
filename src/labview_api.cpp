#include "labview_api.h"

#include <esp_task.h>

#include "event.h"
#include "events.h"

namespace labview {

uint8_t rp = 0;

uint8_t r_buffer[256] = {0};
uint8_t w_buffer[256] = {0};

void _callback(void *arg) {
    while (true) {
        while (LABVIEWSERIAL.available()) {
            r_buffer[rp++] = LABVIEWSERIAL.read();
            delay(1);
        }
        if (rp) {
            r_buffer[rp < 256 ? rp : 255] = 0;

            DEBUGSERIAL.printf("[labview_api] Received cmd: %02X\n[labview_api] All bytes: ", r_buffer[0]);
            uint8_t p = 0;
            while (p < rp) DEBUGSERIAL.printf("%02X ", r_buffer[p++]);
            DEBUGSERIAL.println();

            if (rp == 1) {
                switch (r_buffer[0]) {
                    case R_BTN_START:
                        event::post(events::EVENT_LABVIEW_BTN_START);
                        break;
                    case R_BTN_PAUSE:
                        event::post(events::EVENT_LABVIEW_BTN_PAUSE);
                        break;
                    case R_BTN_RESET:
                        event::post(events::EVENT_LABVIEW_BTN_RESET);
                        break;
                }
            }
            event::post(events::EVENT_LABVIEW_RECEIVE, r_buffer, rp);
            rp = 0;
        }
        delay(100);
    }
    vTaskDelete(NULL);
}

void init() {
    rp = 0;
    LABVIEWSERIAL.begin(LABVIEWSERIAL_BAUD, SERIAL_8N1, PIN_LABVIEW_RX, PIN_LABVIEW_TX);
    xTaskCreate(_callback, "labview_api", 2048, NULL, 1, NULL);
    send_command(Init);
    DEBUGSERIAL.println("[labview_api] Inited");
}

void deinit() {
    LABVIEWSERIAL.end();
    DEBUGSERIAL.println("[labview_api] Deinited");
}

void send_command(uint8_t cmd, uint8_t *data, uint8_t size) {
    w_buffer[0] = cmd;
    if (data != NULL) memcpy(w_buffer + 1, data, size);
    LABVIEWSERIAL.write(w_buffer, size + 1);
    DEBUGSERIAL.printf("[labview_api] Sent cmd: %X\n", cmd);
    event::post(events::EVENT_LABVIEW_SEND, &cmd, 1);
}
void send_i32(uint8_t address, int32_t value) { send_command(address, (uint8_t *)&value, sizeof(int32_t)); }
void send_f32(uint8_t address, double value) { send_command(address, (uint8_t *)&value, sizeof(double)); }

}  // namespace labview
