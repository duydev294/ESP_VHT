#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "esp_wifi.h"
#include "esp_wpa2.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_smartconfig.h"
#include "driver/gpio.h"
#include "mqtt_client.h"
#include <esp_http_server.h>
#include "esp_eth.h"
#include "../common/cJSON/cJSON.h"
#include "stdbool.h"
#include "tcpip_adapter.h"
#include <string.h>

static nvs_handle_t nvsHandle;
static nvs_handle_t nvsHandle1;
static EventGroupHandle_t s_wifi_event_group;
static esp_mqtt_event_handle_t mqtt_client;

int32_t flag_restart  = 0;

typedef enum {
	WIFI_ERROR,
	SMART_CONFIG,
	MQTT_CONNECTED,
	WIFI_CONNECT_NOT_MQTT

}led_t;
char* mqtt_pass;
char* device_id;
char device_id1[8];
char device_id2[4];
char device_id3[4];
char device_id4[4];
char device_id5[12];
char *device_id12;
char *device_id22;
char *device_id32;
char *device_id42;
char *device_id52;
static const int CONNECTED_BIT = BIT0;
static const int ESPTOUCH_DONE_BIT = BIT1;
static const char *TAG = "smartconfig_example";

#define BUTTON_PIN GPIO_NUM_0
#define GPIO_IN_PIN (1ULL << BUTTON_PIN)
#define LED GPIO_NUM_2
#define GPIO_OUT_PIN (1ULL << LED)
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MQTT_BROKER "mqtt://mqtt.innoway.vn"


QueueHandle_t qClick = NULL;
QueueHandle_t qBtSt = NULL;
QueueHandle_t qledt = NULL;



TaskHandle_t Buttontask;
TaskHandle_t Wifitask;
TaskHandle_t Ledtask;

tcpip_adapter_ip_info_t tcp_ip;



// esp web server
esp_err_t get_handler(httpd_req_t *req);
esp_err_t post_handler(httpd_req_t *req);



void led_effect_task(void);
static void smartconfig_example_task(void * parm);

static void event_handler(void* arg, esp_event_base_t event_base,
		int32_t event_id, void* event_data);
static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, uint32_t event_id, void * event_data );

static void initialise_wifi(void);

static void smartconfig_example_task(void * parm);

void mqtt_start();
static void mqtt_event_handle(void * handler_args, esp_event_base_t base, uint32_t event_id, void * event_data);

static void IRAM_ATTR gpio_ISR_handler(void *arg);

static const httpd_uri_t uri_get ={
		.uri = "/device",
		.method = HTTP_GET,
		.handler = get_handler
};
static const httpd_uri_t uri_post ={
		.uri = "/ap",
		.method = HTTP_POST,
		.handler = post_handler
};

static httpd_handle_t start_webservice(void);


// JSON
char * JSON_req_GET();
char * JSON_req_POST(int success);
void JSON_Parser(char * data );



void gpio_init(void);
void button_status(void);

void connect_wifi(void);

void code_deviceid();


void app_main(){
	gpio_init();

	uint8_t sT_t = 1;

	esp_netif_init();
	esp_event_loop_create_default();
	nvs_flash_init();


	qClick = xQueueCreate(1, sizeof(uint32_t));
	qBtSt = xQueueCreate(1,sizeof(uint32_t));
	qledt = xQueueCreate(1,sizeof(led_t));


	if(nvs_open("storage",NVS_READWRITE, &nvsHandle) == ESP_OK){
		nvs_get_i32(nvsHandle,"restart_flag",&flag_restart);
		printf("Flag_restart = %d\n",flag_restart);
		if(flag_restart == 1){ //flag smart config on
			printf("Smart config is on!\n");
			flag_restart = 0;
			nvs_set_i32(nvsHandle,"restart_flag",flag_restart);
			nvs_commit(nvsHandle);
			xTaskCreate(initialise_wifi,"smart_config",4096,NULL,10,NULL);
			xTaskCreate(button_status, "button_status", 4096, NULL, 10, NULL );
			xQueueSend(qBtSt,&sT_t,portMAX_DELAY);


		}
		else{
			printf("Normal Wifi mode !\n");
			//nvs_open("storage",NVS_READWRITE, &nvsHandle1);
			size_t required_size;
			nvs_get_str(nvsHandle, "deviceToken", NULL, &required_size);
			mqtt_pass = malloc(required_size);
			nvs_get_str(nvsHandle, "deviceToken", mqtt_pass, &required_size);
			nvs_get_str(nvsHandle, "deviceIdabc", NULL, &required_size);
			device_id = malloc(required_size);
			nvs_get_str(nvsHandle,"deviceIdabc",device_id,&required_size);
//			nvs_close(nvsHandle);
			printf("Get MQTT info success!\n deviceToken: %s\n deviceID2:%s\n",mqtt_pass,device_id);

			xTaskCreate(connect_wifi,"connect_wifi",4096,NULL,10,NULL);
			xTaskCreate(button_status, "button_status", 4096, NULL, 10, NULL );

		}
	}
}
void gpio_init(){
	// config input
	gpio_config_t gpiocf = {};
	gpiocf.pin_bit_mask = GPIO_IN_PIN;
	gpiocf.mode = GPIO_MODE_INPUT;
	gpiocf.pull_up_en = 1;
	gpiocf.intr_type = GPIO_INTR_NEGEDGE;
	gpiocf.pull_down_en = 0;
	gpio_config(&gpiocf);
	// config output
	gpio_config_t gpiocfO ={
			.pin_bit_mask = GPIO_OUT_PIN,
			.mode = GPIO_MODE_OUTPUT,
			.pull_down_en =0,
			.pull_up_en = 0,
			.intr_type =  GPIO_INTR_DISABLE
	};
	gpio_config(&gpiocfO);
	gpio_set_level(LED, 0);
	gpio_install_isr_service(0);
	gpio_isr_handler_add(BUTTON_PIN,gpio_ISR_handler,(void *)BUTTON_PIN);
}
static void IRAM_ATTR gpio_ISR_handler(void *arg){
	uint32_t gpio_num = (uint32_t) arg;
	xQueueSendFromISR(qClick, &gpio_num, NULL);
}
void button_status(){
	int gpio_num;
	TickType_t l_time = 0;
	uint32_t cnt  = 0;
	while(1){
		if(xQueueReceive(qClick,&gpio_num,50) ){
			if(gpio_num == BUTTON_PIN && gpio_get_level(gpio_num) == 0){
				cnt = 1;
				l_time = xTaskGetTickCount();
				printf("cnt: %d\n",cnt++);
				while (xTaskGetTickCount() - l_time < 100 ){
					if(xQueueReceive(qClick,&gpio_num,50)&& gpio_num == BUTTON_PIN){
						printf("%d\n",cnt);
						l_time = xTaskGetTickCount();
						if(cnt++ == 5){
							flag_restart = 1;
							nvs_set_i32(nvsHandle,"restart_flag",flag_restart);
							nvs_commit(nvsHandle);
							nvs_close(nvsHandle);
							printf("Restart esp!");
							fflush(stdout);
							esp_restart();

						}

					}
				}
			}


		}
	}
}

static void initialise_wifi (void){
	uint32_t bt = 0;
	while(1){
		if(xQueueReceive(qBtSt, &bt, 100) && bt){
			printf("Start smart Config!!\n");


			s_wifi_event_group = xEventGroupCreate();
			esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
			assert(sta_netif);

			wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
			ESP_ERROR_CHECK( esp_wifi_init(&cfg) );

			ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );
			ESP_ERROR_CHECK( esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL) );
			ESP_ERROR_CHECK( esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );

			ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
			ESP_ERROR_CHECK( esp_wifi_start() );
		}
	}
}
static void smartconfig_example_task(void * parm)
{
	EventBits_t uxBits;
	ESP_ERROR_CHECK( (SC_TYPE_ESPTOUCH) );
	smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
	ESP_ERROR_CHECK( esp_smartconfig_start(&cfg) );
	while (1) {
		uxBits = xEventGroupWaitBits(s_wifi_event_group, CONNECTED_BIT | ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY);
		if(uxBits & CONNECTED_BIT) {
			start_webservice();
			ESP_LOGI(TAG, "WiFi Connected to ap");
		}
		if(uxBits & ESPTOUCH_DONE_BIT) {
			ESP_LOGI(TAG, "smartconfig over");
			esp_smartconfig_stop();
			vTaskDelete(NULL);
		}
	}
}

static void event_handler(void* arg, esp_event_base_t event_base,
		int32_t event_id, void* event_data)
{

	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		xTaskCreate(smartconfig_example_task, "smartconfig_example_task", 4096, NULL, 3, NULL);
	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		esp_wifi_connect();
		xEventGroupClearBits(s_wifi_event_group, CONNECTED_BIT);
	} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		xEventGroupSetBits(s_wifi_event_group, CONNECTED_BIT);
	} else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE) {
		ESP_LOGI(TAG, "Scan done");
	} else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL) {
		ESP_LOGI(TAG, "Found channel");
	} else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD) {
		ESP_LOGI(TAG, "Got SSID and password");

		smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
		wifi_config_t wifi_config;
		uint8_t ssid[33] = { 0 };
		uint8_t password[65] = { 0 };
		uint8_t rvd_data[33] = { 0 };

		bzero(&wifi_config, sizeof(wifi_config_t));
		memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
		memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
		// DECODE password
		for(int i = 0;i< sizeof(evt->password)-1; i++){
			if(wifi_config.sta.password[i]>=32 && wifi_config.sta.password[i] <= 125){
				if(i%2 == 0){
					wifi_config.sta.password[i] -= 2;
				}else wifi_config.sta.password[i] -= 7;
			}
			else break;
		}
		wifi_config.sta.bssid_set = evt->bssid_set;
		if (wifi_config.sta.bssid_set == true) {
			memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
		}

		memcpy(ssid, evt->ssid, sizeof(evt->ssid));
		memcpy(password, evt->password, sizeof(evt->password));

		ESP_LOGI(TAG, "SSID:%s", ssid);
		ESP_LOGI(TAG, "PASSWORD:%s", wifi_config.sta.password);
		if (evt->type == SC_TYPE_ESPTOUCH_V2) {
			ESP_ERROR_CHECK( esp_smartconfig_get_rvd_data(rvd_data, sizeof(rvd_data)) );
			ESP_LOGI(TAG, "RVD_DATA:");
			for (int i=0; i<33; i++) {
				printf("%02x ", rvd_data[i]);
			}
			printf("\n");
		}

		ESP_ERROR_CHECK( esp_wifi_disconnect() );

		ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
		esp_wifi_connect();
	} else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE) {
		xEventGroupSetBits(s_wifi_event_group, ESPTOUCH_DONE_BIT);
	}
}


static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, uint32_t event_id, void * event_data ){
	switch (event_id){
	case WIFI_EVENT_STA_START:
		printf("WIFI connecting ...\n");
		break;
	case WIFI_EVENT_STA_CONNECTED:

		printf("WIFI connected ..\n");

		char str[256];

		tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &tcp_ip);
		sprintf(str, "IP Address: %d\n", tcp_ip.ip.addr);
		printf("%s",str);
		if(flag_restart == 1)start_webservice();
		//else xQueueSend(qMQTT,&mqtt_start,100);
		//else mqtt_start();
		break;
	case WIFI_EVENT_STA_DISCONNECTED:
		printf("WIFI lost connection ..\n");
		break;
	case IP_EVENT_STA_GOT_IP:
		printf("WIFI got IP ... \n");
		break;
	default:
		break;
	}
}

void connect_wifi(void){
	uint32_t bt = 0;

	esp_event_loop_create_default();
	esp_netif_create_default_wifi_sta();
	wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&wifi_initiation);


	// 2- wifi configuration Phase
	esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler,NULL);
	esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler,NULL);


	wifi_config_t wifi_configuration = {0} ;
	if(esp_wifi_get_config(ESP_IF_WIFI_STA,&wifi_configuration) != ESP_OK){
		printf("Get config error!");
		flag_restart = 1;
		nvs_set_i32(nvsHandle,"restart_flag",flag_restart);
		nvs_commit(nvsHandle);
		nvs_close(nvsHandle);
		printf("Restart esp!");
		fflush(stdout);

	}
	else {
		printf("Get wifi success!\n");
		printf("SSID:%s\n",wifi_configuration.sta.ssid);
		printf("PASS:%s\n",wifi_configuration.sta.password);
	}


	// 3- wifi start Phase
	esp_wifi_start();

	// 4 wifi connect phase
	esp_wifi_connect();
	mqtt_start();
	while(1){
		if(xQueueReceive(qBtSt, &bt, portMAX_DELAY) && bt){

			esp_netif_destroy(&wifi_initiation);
		}
	};
}

// led display
void led_effect_task(void){
	led_t ledt;
	while(1){
		if(xQueueReceive(qledt,&ledt, portMAX_DELAY)){
			if(ledt == WIFI_ERROR){
				gpio_set_level(LED, 1);
				vTaskDelay(200/portTICK_PERIOD_MS);
				gpio_set_level(LED, 1);
				vTaskDelay(800/portTICK_PERIOD_MS);
			}
			if(ledt == SMART_CONFIG){
				gpio_set_level(LED, 1);
				vTaskDelay(100/portTICK_PERIOD_MS);
				gpio_set_level(LED, 1);
				vTaskDelay(100/portTICK_PERIOD_MS);
			}
			if(ledt == MQTT_CONNECTED){
				gpio_set_level(LED, 1);
			}
			if(ledt == WIFI_CONNECT_NOT_MQTT){
				gpio_set_level(LED, 1);
				vTaskDelay(200/portTICK_PERIOD_MS);
				gpio_set_level(LED, 1);
				vTaskDelay(200/portTICK_PERIOD_MS);
				gpio_set_level(LED, 1);
				vTaskDelay(200/portTICK_PERIOD_MS);
				gpio_set_level(LED, 1);
				vTaskDelay(1400/portTICK_PERIOD_MS);
			}

		}
	}
}
static void mqtt_event_handle(void * handler_args, esp_event_base_t base, uint32_t event_id, void * event_data){
	esp_mqtt_event_handle_t event = event_data;
	mqtt_client = event->client;
	int msg_id;
	switch((esp_mqtt_event_id_t) event_id){
	case MQTT_EVENT_CONNECTED: // MQTT connected
		ESP_LOGI("MQTT","MQTT_EVENT_CONNECTED");
		char topic[100] = "";
		sprintf(topic,"ont2mqtt/%s/commands/set",device_id);
		msg_id = esp_mqtt_client_subscribe(mqtt_client, topic, 0);

		break;
	case MQTT_EVENT_DISCONNECTED: // MQTT disconnected
		ESP_LOGI("MQTT","MQTT_EVENT_DISCONNECTED");

		break;
	case MQTT_EVENT_DATA:
		printf("Data= %.*s\r\n",event->data_len,event->data);
		break;
	case MQTT_EVENT_SUBSCRIBED:
		ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
		break;
	case MQTT_EVENT_UNSUBSCRIBED:
		ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
		break;
	case MQTT_EVENT_PUBLISHED:
		ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
		break;

	case MQTT_EVENT_ERROR:
		ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
		if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {

			ESP_LOGI(TAG, "Last error string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

		}
		break;
	default:
		ESP_LOGI(TAG, "Other event id:%d", event->event_id);
		break;
	}
}


esp_err_t get_handler(httpd_req_t *req){
	const char * resp = JSON_req_GET();
	printf("%s",resp);
	httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}
esp_err_t post_handler(httpd_req_t *req){
	char content[150];
	uint8_t mqtt_start = 1;
	/* Truncate if content length larger than the buffer */
	size_t recv_size = MIN(req->content_len, sizeof(content));

	int ret = httpd_req_recv(req, content, recv_size);

	printf("\nPOST content: %s\n",content);
	// esp_mqtt_client_config_t mqtt_cfg;
	JSON_Parser(content);
	if (ret <= 0) {  /* 0 return value indicates connection closed */
		/* Check if timeout occurred */
		if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
			/* In case of timeout one can choose to retry calling
			 * httpd_req_recv(), but to keep it simple, here we
			 * respond with an HTTP 408 (Request Timeout) error */
			httpd_resp_send(req, JSON_req_POST(false), HTTPD_RESP_USE_STRLEN);
			httpd_resp_send_408(req);

		}
		/* In case of error, returning ESP_FAIL will
		 * ensure that the underlying socket is closed */
		return ESP_FAIL;
	}

	/* Send a simple response */
	httpd_resp_send(req, JSON_req_POST(true), HTTPD_RESP_USE_STRLEN);
	ESP_LOGI("NVS_save","11");
	vTaskDelay(1000/portTICK_PERIOD_MS);
	nvs_set_str(nvsHandle1, "deviceToken", mqtt_pass);
	ESP_LOGI("NVS_save","22");
	nvs_set_str(nvsHandle1, "deviceIdabc", device_id);
	ESP_LOGI("NVS_save","88");
	nvs_set_str(nvsHandle1, "deviceId1", device_id1);
	ESP_LOGI("NVS_save","33");
	nvs_set_str(nvsHandle1, "deviceId2", device_id2);
	ESP_LOGI("NVS_save","44");
	nvs_set_str(nvsHandle1, "deviceId3", device_id3);
	ESP_LOGI("NVS_save","55");
	nvs_set_str(nvsHandle1, "deviceId4", device_id4);
	ESP_LOGI("NVS_save","66");
	nvs_set_str(nvsHandle1, "deviceId5", device_id5);
	ESP_LOGI("NVS_save","77");
	printf("save_success! \n");

	nvs_commit(nvsHandle1);
	nvs_close(nvsHandle1);
	printf("Restart esp!");
	fflush(stdout);

	//const char* resp = "abc";

	esp_restart();
	return ESP_OK;
}

static httpd_handle_t start_webservice(void){
	//start httpd server
	ESP_LOGI("ws","Starting server");
	httpd_config_t config = HTTPD_DEFAULT_CONFIG();
	httpd_handle_t server = NULL;
	if(httpd_start(&server, &config) == ESP_OK){
		httpd_register_uri_handler(server, &uri_get);
		httpd_register_uri_handler(server, &uri_post);
	}
	return server;
}

void stop_webserver(httpd_handle_t server){
	httpd_stop(server);
}


//JSON
char * JSON_req_GET(){
	cJSON * req = cJSON_CreateObject();

	char * string = NULL;

	cJSON * ipdevice = NULL;
	cJSON * type = NULL;
	cJSON * devicename = NULL;
	cJSON *endpoint_count = NULL;

	ipdevice = cJSON_CreateString("192.168.1.100");
	type  = cJSON_CreateNumber(18);
	devicename = cJSON_CreateString("Smart Switch");
	endpoint_count = cJSON_CreateNumber(4);

	cJSON_AddItemToObject(req,"ipdevice",ipdevice);
	cJSON_AddItemToObject(req,"type",type);
	cJSON_AddItemToObject(req,"devicename",devicename);
	cJSON_AddItemToObject(req,"endpoint_count",endpoint_count);

	string = cJSON_Print(req);
	cJSON_Delete(req);

	return string;


}

char * JSON_req_POST(int success){
	cJSON * req = cJSON_CreateObject();
	char * string = NULL;

	cJSON * error = NULL;

	if(success) error = cJSON_CreateNumber(0);
	else error = cJSON_CreateNumber(1);

	cJSON_AddItemToObject(req,"error",error);

	string = cJSON_Print(req);

	cJSON_Delete(req);
	return string;

}
void JSON_Parser(char data[150]){
	cJSON * json = cJSON_Parse(data);
	ESP_LOGI("CJSON","11");
	cJSON *idDevice = cJSON_GetObjectItemCaseSensitive(json, "deviceid");
	ESP_LOGI("CJSON","22");
	cJSON * devicetoken = cJSON_GetObjectItemCaseSensitive(json, "devicetoken");
	ESP_LOGI("CJSON","33");
	mqtt_pass = devicetoken->valuestring;
	printf("%s\n",mqtt_pass);
	device_id = idDevice->valuestring;

	printf("%s\n",device_id);
	code_deviceid();
	cJSON_Delete(json);
	ESP_LOGI("CJSON","44");

	//	cJSON_Delete(idDevice);
	//	cJSON_Delete(devicetoken);

}

// MQTT
void mqtt_start(){

	uint8_t start_mqtt;
	esp_mqtt_client_config_t mqtt_cfg;
	mqtt_cfg.password = mqtt_pass;
	mqtt_cfg.username = "Duydev";
	mqtt_cfg.uri = MQTT_BROKER;
	ESP_LOGI("2","2");
	esp_mqtt_client_handle_t client_mqtt = esp_mqtt_client_init(&mqtt_cfg);
	ESP_LOGI("1","1");
	esp_mqtt_client_register_event (client_mqtt,ESP_EVENT_ANY_ID,mqtt_event_handle,NULL);
	ESP_ERROR_CHECK(esp_mqtt_client_start(client_mqtt));
	printf("setup mqtt success!\n ");
}
void code_deviceid(){


		for(int i = 0; i< 8; i ++){
			device_id1[i] = device_id[i] ;
		}
		device_id1[8] = '\0';
	printf("deviceid1:%s\n",device_id1);
	for(int i = 9; i< 13; i ++){
		device_id2[i-9] = device_id[i];
	}
	device_id2[13-9] = '\0';
	printf("deviceid2:%s\n",device_id2);
	for(int i = 14; i< 18; i ++){
		device_id3[i-14] = device_id[i] ;
	}
	device_id3[4] = '\0';
	printf("deviceid3:%s\n",device_id3);
	for(int i = 19;i< 23; i ++){
			device_id4[i-19] = device_id[i] ;
		}
	device_id4[4] = '\0';
	printf("deviceid4:%s\n",device_id4);
	for(int i = 24; i< 36; i ++){
			device_id5[i-24] = device_id[i] ;
		}
	device_id5[12] = '\0';
	printf("deviceid5:%s\n",device_id5);

}
