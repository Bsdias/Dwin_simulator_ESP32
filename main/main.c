/* 
OUTPUT SIMULATOR - DISPLAY DWIN
*/
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"             
#include "freertos/task.h"     
#include "driver/gpio.h"  
#include "esp_err.h"
#include "esp_system.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "esp_vfs_fat.h"
#include "soc/rtc.h"
#include "esp_log.h"
#include "driver/uart.h"


static const int RX_BUF_SIZE = 1024;
#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_16)

void init(void) {
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}
void dwin_write(int vp[2], int content ){
       //5a a5 -> Header, 06 -> Size 
      const char message[9]={0x5a,0xa5,0x06,0x83,vp[0],vp[1],0x00,0x00,content};
      for (int i=0; i<9; i++){ 
            static const char *DWIN_WRITE_TAG = "DWIN_WRITE_";// escrever em hexadeciamal
            esp_log_level_set(DWIN_WRITE_TAG, ESP_LOG_INFO);
            uart_write_bytes(UART_NUM_1,  (const char*)&message[i],1);
            //sendData(DWIN_WRITE_TAG, message[i]);
        }
}
void config_esp_console(){ 
    
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    ESP_ERROR_CHECK(uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM, 256, 0, 0, NULL, 0));
    esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);
    esp_vfs_dev_uart_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
    esp_vfs_dev_uart_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);
}
void data_idf_monitor(int *VP_0,int *VP_1, int *value){
    printf("\n");
    for(int i = 0; i<50;i++){
        printf("-");
    }
    printf("\nDIWN SIMULATOR\nInsert the VPs (HEX) and content (INT):\n");
    printf("VP[0]:\n");
    scanf("%x",&*VP_0);
    printf("VP[1]:\n");
    scanf("%x", &*VP_1);
    printf("value:\n");
    scanf("%d", &*value);
    printf("VP ---> %x %x\nValue:%d\n",*VP_0,*VP_1, *value);
}
void dwin_output(void){

    int VP_0,VP_1,value;
    while(1){
        data_idf_monitor(&VP_0,&VP_1,&value);
        printf("\nDwin Parameters: VP->%x %x VALUE: %d", VP_0,VP_1, value);
        int VP_test[2]={VP_0,VP_1};
        dwin_write(VP_test,value);

    }
}
void app_main(void)
{
    init();
    config_esp_console();
    dwin_output();

}

