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
#include <util.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <stdarg.h>
#include <android-base/logging.h>
#include <android-base/properties.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include "property_service.h"

#include "libinit_g4.h"

char product_model[PROP_VALUE_MAX];
char usu_detect[PROP_VALUE_MAX];
const char* detected_usudev;
    
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
} // sanitize_product_model
    
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
} // sanitize_usu_detect
    
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
} // get_device_model

void set_usudev(void) 
    {
	detected_usudev = "not-existent";

 	std::ifstream disk (USUDEV, std::ifstream::binary);
        if (disk) {
	   detected_usudev = USUDEV;
	} else {
	   LOG(INFO) << "UsU: No partition found at: '" << USUDEV << "'";
           std::ifstream disk (USUDEVPF, std::ifstream::binary);
           if (disk) {
		 detected_usudev = USUDEVPF;
           } else {
	        LOG(INFO) << "UsU: No partition found at: '" << USUDEVPF << "'";
                std::ifstream disk (USUDEVPFSOC, std::ifstream::binary);
                if (disk) {
		  detected_usudev = USUDEVPFSOC;
                } else {
	          LOG(ERROR) << "UsU: No partition found at: '" << USUDEVPFSOC << "' SO CAN NOT DETECT YOUR USU MODEL!!";
		}
	   }
	}
	LOG(INFO) << "UsU: device result: '" << detected_usudev << "'";
} // set_usudev
    
void get_usu_model(void)
    {
	set_usudev();

        std::ifstream disk (detected_usudev, std::ifstream::binary);
        if (disk) {
          disk.seekg (0, disk.end);
          int length = disk.tellg();
          if ( length < USUOFFSET) {
	    LOG(ERROR) << "UsU: disk length is too small !!!";
          } else {
            disk.seekg (USUOFFSET, disk.beg);
            char * buffer = new char [USUCOUNT+1];
            disk.read (buffer,USUCOUNT);
            disk.close();
            strcpy(product_model,buffer);
	    LOG(INFO) << "UsU: model detected was: '" << product_model << "'";
            delete[] buffer;
	  }
	} else {
	    LOG(ERROR) << "UsU: can't open disk !!! --> '" << detected_usudev << "'";
        }
} // get_usu_model

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
            if (memcmp(token, CMDLINE_MODEL, CMDLINE_MODEL_LEN) == 0 and (strstr(product_model,"LGLS991") or strstr(product_model,"LG-USU"))) {
                strcpy(usu_detect, "UsU_unlocked"); // UsU found
                get_usu_model();
                return;
            }
            token = strtok(NULL, " ");
            }
         }
         strcpy(usu_detect, "officially_unlocked"); // no UsU found
         return;
} // get_usu

void real_vendor_load_properties()
    {
        char product_name[PROP_VALUE_MAX];
        char product_device[PROP_VALUE_MAX];
        char build_product[PROP_VALUE_MAX];
    
        get_device_model();
        get_usu();
    
        // Check what device types we have and set their prop accordingly
        if (strstr(product_model,"LG-H815") or strstr(product_model,"H815")) {
            android::init::property_set("ro.usu.product.detection","success");
            android::init::property_set("ro.usu.device.unlockmode",usu_detect);
            android::init::property_set("ro.product.model","LG-H815");
            android::init::property_set("ro.product.name","p1_global_com");
            android::init::property_set("ro.product.device","h815");
            android::init::property_set("ro.build.product","h815");
        } else if (strstr(product_model,"H810")) {
            android::init::property_set("ro.usu.product.detection","success");
            android::init::property_set("ro.usu.device.unlockmode","UsU_unlocked");
            android::init::property_set("ro.product.model","LG-H810");
            android::init::property_set("ro.product.name","p1");
            android::init::property_set("ro.product.device","h810");
            android::init::property_set("ro.build.product","h810");
        } else if (strstr(product_model,"LG-H811")) {
            android::init::property_set("ro.usu.product.detection","success");
            android::init::property_set("ro.usu.device.unlockmode",usu_detect);
            android::init::property_set("ro.product.model","LG-H811");
            android::init::property_set("ro.product.name","p1_tmo_us");
            android::init::property_set("ro.product.device","h811");
            android::init::property_set("ro.build.product","h811");
        } else if (strstr(product_model,"H812")) {
            android::init::property_set("ro.usu.product.detection","success");
            android::init::property_set("ro.usu.device.unlockmode","UsU_unlocked");
            android::init::property_set("ro.product.model","LG-H812");
            android::init::property_set("ro.product.name","p1");
            android::init::property_set("ro.product.device","h812");
            android::init::property_set("ro.build.product","h812");
        } else if (strstr(product_model,"H818")) {
            android::init::property_set("ro.usu.product.detection","success");
            android::init::property_set("ro.usu.device.unlockmode","UsU_unlocked");
            android::init::property_set("ro.product.model","LG-H818");
            android::init::property_set("ro.product.name","p1");
            android::init::property_set("ro.product.device","h818");
            android::init::property_set("ro.build.product","h818");
        } else if (strstr(product_model,"H819")) {
            android::init::property_set("ro.usu.product.detection","success");
            android::init::property_set("ro.usu.device.unlockmode","UsU_unlocked");
            android::init::property_set("ro.product.model","LG-H819");
            android::init::property_set("ro.product.name","p1");
            android::init::property_set("ro.product.device","h819");
            android::init::property_set("ro.build.product","h819");
        } else if (strstr(product_model,"F500")) {
            android::init::property_set("ro.usu.product.detection","success");
            android::init::property_set("ro.usu.device.unlockmode","UsU_unlocked");
            android::init::property_set("ro.product.model","LG-F500");
            android::init::property_set("ro.product.name","p1");
            android::init::property_set("ro.product.device","f500");
            android::init::property_set("ro.build.product","f500");
        } else if (strstr(product_model,"LS991")) {
            android::init::property_set("ro.usu.product.detection","success");
            android::init::property_set("ro.usu.device.unlockmode","UsU_unlocked");
            android::init::property_set("ro.product.model","LG-LS991");
            android::init::property_set("ro.product.name","p1");
            android::init::property_set("ro.product.device","ls991");
            android::init::property_set("ro.build.product","ls991");
        } else if (strstr(product_model,"US991")) {
            android::init::property_set("ro.usu.product.detection","success");
            android::init::property_set("ro.usu.device.unlockmode","UsU_unlocked");
            android::init::property_set("ro.product.model","LG-US991");
            android::init::property_set("ro.product.name","p1");
            android::init::property_set("ro.product.device","us991");
            android::init::property_set("ro.build.product","us991");
        } else if (strstr(product_model,"VS986")) {
            android::init::property_set("ro.usu.product.detection","success");
            android::init::property_set("ro.usu.device.unlockmode","UsU_unlocked");
            android::init::property_set("ro.product.model","LG-VS986");
            android::init::property_set("ro.product.name","p1");
            android::init::property_set("ro.product.device","vs986");
            android::init::property_set("ro.build.product","vs986");
        // Only these above should exists.. no others can!
        } else {
            //The wont work on other devices so just let them be their own props
            android::init::property_set("ro.usu.product.detection","unknown_model");
            android::init::property_set("ro.usu.device.unlockmode",usu_detect);
            android::init::property_set("ro.product.model",product_model);
            android::init::property_set("ro.product.name",product_name);
            android::init::property_set("ro.product.device",product_device);
            android::init::property_set("ro.build.product",build_product);
        }
} // real_vendor_load_properties

// libinit_g4.h will rename vendor_load_properties() to real_vendor_load_properties()
// Call the appropriate real_vendor_load_properties() depending on android version
#undef vendor_load_properties

// Android 8.1 has to use namespace android::init
namespace android {
namespace init {
        void vendor_load_properties() { 
		real_vendor_load_properties();
	}
}  // namespace init
}  // namespace android
 
