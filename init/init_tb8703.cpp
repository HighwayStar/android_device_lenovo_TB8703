/*
   Copyright (c) 2016, The CyanogenMod Project

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

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
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <stdio.h>
#include <sys/system_properties.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

#define PROP_BOOT_BASEBAND "ro.boot.baseband"

void property_override(char const prop[], char const value[])
{
	prop_info *pi;

	pi = (prop_info*) __system_property_find(prop);
	if (pi)
		__system_property_update(pi, value, strlen(value));
	else
		__system_property_add(prop, strlen(prop), value, strlen(value));
}

static void set_fingerprint()
{
	std::string baseband = property_get(PROP_BOOT_BASEBAND);
	if (baseband == "apq") {
		property_override("ro.build.description", "msm8953_64-user 6.0.1 MMB29M 65 release-keys");
		property_override("ro.build.product", "TB-8703F");
		property_override("ro.product.device", "TB-8703F");
		property_override("ro.build.fingerprint", "Lenovo/TB-8703F/TB-8703F:6.0.1/MMB29M/TB-8703F_USR_S035_180326_Q1241_ROW:user/release-keys");
		property_override("ro.product.model", "Lenovo TB-8703F");
		property_override("ro.qc.sdk.audio.fluencetype", "none");
		property_override("persist.audio.fluence.speaker", "true");
	//for installing stock OTA with TWRP
		property_override("ro.product.ota.model", "LenovoTB-8703F_ROW");
    } else if(baseband == "msm") {
		property_override("ro.build.description", "msm8953_64-user 6.0.1 MMB29M 559 release-keys");
		property_override("ro.build.product", "TB-8703X");
		property_override("ro.product.device", "TB-8703X");
		property_override("ro.build.fingerprint", "Lenovo/TB-8703X/TB-8703X:6.0.1/MMB29M/TB-8703X_USR_S037_180404_Q1241_ROW:user/release-keys");
		property_override("ro.product.model", "Lenovo TB-8703X");
		property_override("ro.qc.sdk.audio.fluencetype", "fluence");
		property_override("persist.audio.fluence.speaker", "false");
		//for installing stock OTA with TWRP
		property_override("ro.product.ota.model", "LenovoTB-8703X_ROW");
	}
}

void vendor_load_properties()
{
	set_fingerprint();
}
