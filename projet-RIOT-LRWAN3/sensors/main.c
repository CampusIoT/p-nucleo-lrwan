#include <stdio.h>
#include <stdlib.h>

#include "xtimer.h"
#include "lis3mdl.h"
#include "lis3mdl_params.h"
#include "shell.h"
#include "lpsxxx.h"
#include "lpsxxx_params.h"
#include "hts221.h"
#include "hts221_params.h"

#define SLEEP_USEC  (800 * 800U)

#ifndef SHELL_BUFSIZE
#define SHELL_BUFSIZE       (128U)
#endif

#define SLEEP_S     (2U)

static int run_lis3mdl(int argc, char **argv){
    lis3mdl_t dev;

    if (argc > 1) {
        printf("No argument expected ! usage: %s \n", argv[0]);
        return 1;
    }

    puts("LIS3MDL test application");
    puts("Initializing LIS3MDL sensor");

    if (lis3mdl_init(&dev, &lis3mdl_params[0]) == 0) {
        puts("[ OK ]\n");
    }
    else {
        puts("[ FAIL ]\n");
        return 1;
    }

    int i = 0;

    while(i <= 5) {
        lis3mdl_3d_data_t mag_value;
        lis3mdl_read_mag(&dev, &mag_value);
        printf("Magnetometer [G]:\tX: %2d\tY: %2d\tZ: %2d\n",
               mag_value.x_axis,
               mag_value.y_axis,
               mag_value.z_axis);

        int16_t temp_value;
        lis3mdl_read_temp(&dev, &temp_value);
        printf("Temperature:\t\t%i°C\n", temp_value);

        xtimer_usleep(SLEEP_USEC);
        i++;
    }

    return 0;

}

static int run_lps22hb(int argc, char **argv){
        
    lpsxxx_t dev;

    if (argc > 1) {
        printf("No argument expected ! usage: %s \n", argv[0]);
        return 1;
    }

    printf("Test application for %s pressure sensor\n\n", LPSXXX_SAUL_NAME);
    printf("Initializing %s sensor\n", LPSXXX_SAUL_NAME);
    if (lpsxxx_init(&dev, &lpsxxx_params[0]) != LPSXXX_OK) {
        puts("Initialization failed");
        return 1;
    }

    uint16_t pres;
    int16_t temp;

    int i = 0;
    while (i<=5) {
        lpsxxx_enable(&dev);
        xtimer_sleep(1); /* wait a bit for the measurements to complete */

        lpsxxx_read_temp(&dev, &temp);
        lpsxxx_read_pres(&dev, &pres);
        lpsxxx_disable(&dev);

        int temp_abs = temp / 100;
        temp -= temp_abs * 100;

        printf("Pressure value: %ihPa - Temperature: %2i.%02i°C\n",
               pres, temp_abs, temp);
        i++;
    }

    return 0;
}

static int run_hts221(int argc, char **argv){

    hts221_t dev;

    if (argc > 1) {
        printf("No argument expected ! usage: %s \n", argv[0]);
        return 1;
    }

    printf("Init HTS221 on I2C_DEV(%i)\n", (int)hts221_params[0].i2c);
    if (hts221_init(&dev, &hts221_params[0]) != HTS221_OK) {
        puts("[FAILED]");
        return 1;
    }
    if (hts221_power_on(&dev) != HTS221_OK) {
        puts("[FAILED] to set power on!");
        return 2;
    }
    if (hts221_set_rate(&dev, dev.p.rate) != HTS221_OK) {
        puts("[FAILED] to set continuous mode!");
        return 3;
    }

    int i = 0; 

    while(i<=5) {
        uint16_t hum = 0;
        int16_t temp = 0;
        if (hts221_read_humidity(&dev, &hum) != HTS221_OK) {
            puts(" -- failed to read humidity!");
        }
        if (hts221_read_temperature(&dev, &temp) != HTS221_OK) {
            puts(" -- failed to read temperature!");
        }
        printf("H: %d.%d%%, T: %d.%d°C\n", (hum / 10), (hum % 10),
               (temp / 10), abs(temp % 10));
        xtimer_sleep(SLEEP_S);
        i++;
    }
    return 0;

}

static const shell_command_t shell_commands[] = {
    { "lis3mdl", "Initialize the LIS3MDL sensor and read the values", run_lis3mdl },
    { "lps22hb", "Initialize the LPS22HB sensor and read the values", run_lps22hb },
    { "hts221", "Initialize the HTS221 sensor and read the values", run_hts221 },

    { NULL, NULL, NULL }
};

int main(void){

    xtimer_sleep(5);
    puts("Hello !");

    /* run the shell */
    char line_buf[SHELL_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_BUFSIZE);
    return 0;


}