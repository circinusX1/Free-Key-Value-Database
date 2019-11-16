/**
  Original file taken form:
  Rui Santos
    Complete project details at https://RandomNerdTutorials.com
    
    
    Takes a camera snapshot every 10 seconds and sens the image to meeopt.org
*/

#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "fb_gfx.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_http_server.h"

const char* ssid = "SSID_MINE";
const char* password = "CLEAR_TEXT_PASSWD";
#define PART_bound "123456789000000000000987654321"
volatile int _WebMode = 0;
volatile int _MainMode = 0;

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;bound=" PART_bound;
static const char* _STREAM_bound = "\r\n--" PART_bound "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

httpd_handle_t HttpStream = NULL;

static void _motion(const camera_fb_t* fb, uint8_t* jpgbuf, size_t jpglen);
static void _stream_mode(void);
static void _motion_mode(void);
static void _send_image(void);

static  esp_err_t stream_handler(httpd_req_t *req)
{
    if(_MainMode)
        return 0;
    _WebMode=1;
    int enough=0;
    camera_fb_t * fb = NULL;
    esp_err_t res = ESP_OK;
    size_t _jpg_buf_len = 0;
    uint8_t * _jpg_buf = NULL;
    char * part_buf[64];

    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    if(res != ESP_OK){
        _WebMode=0;
        return res;
    }
    while(1)
    {
        _WebMode=1;
        fb = esp_camera_fb_get();
        if (!fb) {
            Serial.println("Camera capture failed");
            res = ESP_FAIL;
        } else {
            if(fb->width > 400)
            {
                if(fb->format != PIXFORMAT_JPEG){

                    bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
                    esp_camera_fb_return(fb);
                    fb = NULL;
                    if(!jpeg_converted){
                        Serial.println("JPEG compression failed");
                        res = ESP_FAIL;
                    }
                } else {
                    _jpg_buf_len = fb->len;
                    _jpg_buf = fb->buf;
                }
            }
        }

        if(res == ESP_OK){
            size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
            res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
        }
        if(res == ESP_OK){
            res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
        }
        if(res == ESP_OK){
            res = httpd_resp_send_chunk(req, _STREAM_bound, strlen(_STREAM_bound));
        }
        if(fb){
            esp_camera_fb_return(fb);
            fb = NULL;
            _jpg_buf = NULL;
        } else if(_jpg_buf){
            free(_jpg_buf);
            _jpg_buf = NULL;
        }
        if(res != ESP_OK){
            break;
        }
        //Serial.printf("MJPG: %uB\n",(uint32_t)(_jpg_buf_len));
        delay(64); /*with processing 10- fps*/
    }
    _WebMode=0;
    return res;
}

void startCameraServer()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;
    httpd_uri_t index_uri = {
        .uri       = "/",
        .method    = HTTP_GET,
        .handler   = stream_handler,
        .user_ctx  = NULL
    };

    //Serial.printf("Starting web server on port: '%d'\n", config.server_port);
    if (httpd_start(&HttpStream, &config) == ESP_OK)
    {
        httpd_register_uri_handler(HttpStream, &index_uri);
    }
}

void setup()
{
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    Serial.begin(115200);
    Serial.setDebugOutput(false);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    startCameraServer();
    _stream_mode();
}

int Kstatic=0;
void loop()
{
    delay(100);
    if(Kstatic%100==0) //snap every 10 seconds
    {
        _send_image();
    }
}

// we use cpature mode
void _stream_mode(void)
{
    if(EMODE==eCAPTURE)
        return;

    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        esp_camera_deinit();
        err = esp_camera_init(&config);
        if (err != ESP_OK) {
            Serial.printf("Camera init failed with error 0x%x", err);
        }
    }
    Serial.printf("\nCAPTURE MODE\n");
    EMODE=eCAPTURE;
}


// not used yet
void _motion_mode(void)
{

    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;//CAPTURE
    config.pin_d7 = Y9_GPIO_NUM;      // binary file

    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_GRAYSCALE;
    config.frame_size = FRAMESIZE_QQVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    // Camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        esp_camera_deinit();
        err = esp_camera_init(&config);
        if (err != ESP_OK) {
            Serial.printf("Camera init failed with error 0x%x", err);
        }
    }
    EMODE=eMOTION;
    Serial.printf("\nMOTION MODE\n");
}

const char* Host = "www.meeiot.org";
const int   HttpsPort = 443;
const char* Fingerprint = "48:3B:E1:2E:C7:9F:DC:FC:5E:A7:31:D7:FC:A8:2A:B2:72:D4:D5:0A";

static void _send_image(void)
{
    if(_WebMode)
        return;
    _MainMode=1;
    WiFiClientSecure client;
    esp_err_t       res = ESP_OK;
    size_t          _jpg_buf_len = 0;
    uint8_t *       _jpg_buf = NULL;
    char            cl[64];

    camera_fb_t * fb = esp_camera_fb_get();
    if(fb && PrevPix != fb->buf[fb->len/2])
    {
        PrevPix = fb->buf[fb->len/2];
        Serial.print("PUT connecting to ");
        Serial.println(Host);
        if (!client.connect(Host, HttpsPort)) {
            Serial.println("connection failed");
            goto END;
        }

        if (client.verify(Fingerprint, Host))
        {
            Serial.println("certificate matches");
        }
        else
        {
            Serial.println("certificate doesn't match");
        }
        String bnd = "------------070002080409050901090203";
        String endtran = "\r\n--"; endtran+=bnd; endtran+="--\r\n\r\n";
        String bound = bnd; bound += "\r\n";
        bound += "Content-Disposition: form-data; name=\"esp-name1\"; \r\n\r\n";
        bound += "file"; bound +="\r\n";
        bound += "--";
        bound += bnd; bound += "\r\n";
        bound += "Content-Disposition: form-data; name=\"esp-img\"; filename=\"espimg.jpg\"\r\n";
        bound += "Content-Type: image/jpeg\r\n\r\n";
        sprintf(cl,"Content-Length: %d\r\n",fb->len + bound.length()+endtran.length());
        String hdr = "POST /push/GET_A_TOKEN_AT_MEEIOT.ORG HTTP/1.1\r\n";
        hdr += "Host: www.meeiot.org\r\n";
        hdr += "Content-Type: multipart/form-data; boundary="; hdr += bnd; hdr+="\r\n";
        hdr += cl;
        hdr += "Connection: Keep-Alive\r\n\r\n";
        Serial.println(hdr);
        Serial.println(bound);
        client.print(hdr);
        client.print(bound);

        int all = fb->len;
        int chunk=512;
        int offset=0;
        while(all>0)
        {
            client.write(fb->buf+offset, chunk);
            offset += chunk;
            all -= chunk;
            if(all<chunk) chunk=all;
        }
        client.print(endtran);
        Serial.println(endtran);
        String line;

        while (client.connected())
        {
            line = client.readStringUntil('\n');
            if (line == "\r")
            {
                break;
            }
        }
        line = client.readStringUntil('\n');
        Serial.println(line);
    }
END:
    _MainMode=0;
}
