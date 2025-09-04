#ifndef _PICO_LIB_UTILS_H
#define _PICO_LIB_UTILS_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName)          \
    TypeName(const TypeName&) = delete;             \
    TypeName(TypeName&&) = delete;                  \
    TypeName& operator=(const TypeName&) = delete;  \
    TypeName& operator=(TypeName&&) = delete;


#include <inttypes.h>
#include <pico/stdlib.h>

static const char *gpio_irq_str[] = {
    "LEVEL_LOW",  // 0x1
    "LEVEL_HIGH", // 0x2
    "EDGE_FALL",  // 0x4
    "EDGE_RISE"   // 0x8
};

class Utils
{
    public:
        static inline long map(long x, long in_min, long in_max, long out_min, long out_max) {
            return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        }

        static inline float lerp(uint16_t a, uint16_t b, float ratio) {
            return (a + ((b - a) * ratio));
        }

        static inline uint16_t adcGetSampleAvgNDeleteX(uint8_t n, uint8_t x, uint16_t *samples) {
            uint32_t avgSample = 0x00;
            
            sortTab(samples, n);
            for (int i = x / 2; i < n - x / 2; i++) {
                avgSample += samples[i];
            }

            avgSample /= (n - x);
            return avgSample;
        }

        static inline void sortTab(uint16_t tab[], uint8_t length) {
            uint8_t l = 0x00, exchange = 0x01;
            uint16_t tmp = 0x00;

            while (exchange == 1) {
                exchange = 0;
                for (l = 0; l < length - 1; l++) {
                    if (tab[l] > tab[l + 1]) {
                        tmp = tab[l];
                        tab[l] = tab[l + 1];
                        tab[l + 1] = tmp;
                        exchange = 1;
                    }
                }
            }
        }

        static inline uint32_t millis() {
            return to_ms_since_boot(get_absolute_time());
        }

        static inline uint64_t micros() {
            return to_us_since_boot(get_absolute_time());
        }

        // Convert interrupt event to string
        // declare this var and use for buf 
        // static char event_str[128];
        // Example: https://github.com/raspberrypi/pico-examples/blob/master/gpio/hello_gpio_irq/hello_gpio_irq.c
        void gpio_event_string(char *buf, uint32_t events) {
            for (uint i = 0; i < 4; i++) {
                uint mask = (1 << i);
                if (events & mask) {
                    // Copy this event string into the user string
                    const char *event_str = gpio_irq_str[i];
                    while (*event_str != '\0') {
                        *buf++ = *event_str++;
                    }
                    events &= ~mask;

                    // If more events add ", "
                    if (events) {
                        *buf++ = ',';
                        *buf++ = ' ';
                    }
                }
            }
            *buf++ = '\0';
        }
};

#endif