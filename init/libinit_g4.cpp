/*
   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <stdio.h>
#include "vendor_init.h"
#include "property_service.h"
#include "util.h"
#include <ctype.h>

#define CMDLINE_MODEL        "model.name="
#define CMDLINE_MODEL_LEN    (strlen(CMDLINE_MODEL))
#define DEVID_MAX 10
#define CMDLINE_USU          "slub_debug="
#define CMDLINE_USU_LEN      (strlen(CMDLINE_USU))
#define USU_MAX 10


char product_model[PROP_VALUE_MAX];
char usu_detect[PROP_VALUE_MAX];

void sanitize_product_model(void) {
        const char* whitelist ="-._";
        char str[DEVID_MAX];
        char* c = str;

        snprintf(str, DEVID_MAX, "%s", product_model);
        memset(product_model, 0, strlen(product_model));
        while (*c) {
                if (isalnum(*c) || strchr(whitelist, *c))
                        strncat(product_model, c, 1);
                c++;
        }
        return;
}

void sanitize_usu_detect(void) {
        const char* whitelist ="-._";
        char str[USU_MAX];
        char* c = str;

        snprintf(str, USU_MAX, "%s", usu_detect);
        memset(usu_detect, 0, strlen(usu_detect));
        while (*c) {
                if (isalnum(*c) || strchr(whitelist, *c))
                        strncat(usu_detect, c, 1);
                c++;
        }
        return;
}

void get_device_model(void)
{
    FILE *fp;
    char line[2048];
    char* token;

    // Check the cmdline to see if the serial number was supplied
    fp = fopen("/proc/cmdline", "rt");
    if (fp != NULL) {
        fgets(line, sizeof(line), fp);
        fclose(fp); // cmdline is only one line long

        token = strtok(line, " ");
        while (token) {
            if (memcmp(token, CMDLINE_MODEL, CMDLINE_MODEL_LEN) == 0) {
                token += CMDLINE_MODEL_LEN;
                snprintf(product_model, DEVID_MAX, "%s", token);
                sanitize_product_model(); // also removes newlines
                return;
            }
            token = strtok(NULL, " ");
        }
     }
     strcpy(product_model, "NOBLMODEL"); // bootloader hasn't provided the model name
     return;
}

void get_usu_model(void)
{
    FILE *fp;
    
    fp = fopen("/dev/block/bootdevice/by-name/raw_resources", "rt");
    fseek(fp,3145722,SEEK_SET); // set UsU offset
    fread(buff,1,6,fp); 
    strcpy(product_model, buff); // set UsU device model
    fclose(fp);
} 

void get_usu(void)
{
    FILE *fp;
    char line[2048];
    char* token;

    // Check the cmdline to see if the serial number was supplied
    fp = fopen("/proc/cmdline", "rt");
    if (fp != NULL) {
        fgets(line, sizeof(line), fp);
        fclose(fp); // cmdline is only one line long

        token = strtok(line, " ");
        while (token) {
            if (memcmp(token, CMDLINE_USU, CMDLINE_USU_LEN) == 0) {
                strcpy(usu_detect, "UsU_unlocked"); // UsU found
                void_get_usu_model();
                return;
            }
            token = strtok(NULL, " ");
        }
     }
     strcpy(usu_detect, "officially_unlocked"); // no UsU found
     return;
}


void vendor_load_properties()
{
    char product_name[PROP_VALUE_MAX];
    char product_device[PROP_VALUE_MAX];
    char build_product[PROP_VALUE_MAX];

    get_usu();
    get_device_model();

    // Check what device types we have and set their prop accordingly
    if (strstr(product_model,"LG-H815")) {
        property_set("ro.product.detection","success");
        property_set("ro.device.unlockmode",usu_detect);
        property_set("ro.product.model","LG-H815");
        property_set("ro.product.name","p1_global_com");
        property_set("ro.product.device","h815");
        property_set("ro.build.product","h815");
    } else if (strstr(product_model,"H810")) {
        property_set("ro.product.detection","success");
        property_set("ro.device.unlockmode",usu_detect);
        property_set("ro.product.model","LG-H810");
        property_set("ro.product.name","p1");
        property_set("ro.product.device","h810");
        property_set("ro.build.product","h810");
    } else if (strstr(product_model,"LG-H811")) {
        property_set("ro.product.detection","success");
        property_set("ro.device.unlockmode",usu_detect);
        property_set("ro.product.model","LG-H811");
        property_set("ro.product.name","p1_tmo_us");
        property_set("ro.product.device","h811");
        property_set("ro.build.product","h811");
    } else if (strstr(product_model,"H812")) {
        property_set("ro.product.detection","success");
        property_set("ro.device.unlockmode",usu_detect);
        property_set("ro.product.model","LG-H812");
        property_set("ro.product.name","p1");
        property_set("ro.product.device","h812");
        property_set("ro.build.product","h812");
    } else if (strstr(product_model,"H818")) {
        property_set("ro.product.detection","success");
        property_set("ro.device.unlockmode",usu_detect);
        property_set("ro.product.model","LG-H818");
        property_set("ro.product.name","p1");
        property_set("ro.product.device","h818");
        property_set("ro.build.product","h818");
    } else if (strstr(product_model,"H819")) {
        property_set("ro.product.detection","success");
        property_set("ro.device.unlockmode",usu_detect);
        property_set("ro.product.model","LG-H819");
        property_set("ro.product.name","p1");
        property_set("ro.product.device","h819");
        property_set("ro.build.product","h819");
    } else if (strstr(product_model,"F500")) {
        property_set("ro.product.detection","success");
        property_set("ro.device.unlockmode",usu_detect);
        property_set("ro.product.model","LG-F500");
        property_set("ro.product.name","p1");
        property_set("ro.product.device","f500");
        property_set("ro.build.product","f500");
    } else if (strstr(product_model,"LS991")) {
        property_set("ro.product.detection","success");
        property_set("ro.device.unlockmode",usu_detect);
        property_set("ro.product.model","LG-LS991");
        property_set("ro.product.name","p1");
        property_set("ro.product.device","ls991");
        property_set("ro.build.product","ls991");
    } else if (strstr(product_model,"US991")) {
        property_set("ro.product.detection","success");
        property_set("ro.device.unlockmode",usu_detect);
        property_set("ro.product.model","LG-US991");
        property_set("ro.product.name","p1");
        property_set("ro.product.device","us991");
        property_set("ro.build.product","us991");
    } else if (strstr(product_model,"VS986")) {
        property_set("ro.product.detection","success");
        property_set("ro.device.unlockmode",usu_detect);
        property_set("ro.product.model","LG-VS986");
        property_set("ro.product.name","p1");
        property_set("ro.product.device","vs986");
        property_set("ro.build.product","vs986");
    // Only these above should exists.. no others can!
    } else {
        //The wont work on other devices so just let them be their own props
        property_set("ro.product.detection","unknown_model");
        property_set("ro.device.unlockmode",usu_detect);
        property_set("ro.product.model",product_model);
        property_set("ro.product.name",product_name);
        property_set("ro.product.device",product_device);
        property_set("ro.build.product",build_product);
    }
}
