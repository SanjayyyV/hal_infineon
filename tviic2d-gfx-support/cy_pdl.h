/*
 * Copyright (c) 2026 Linumiz
 * SPDX-License-Identifier: Apache-2.0
 *
 * Shim for the ModusToolbox "cy_pdl.h" umbrella header.
 *
 * The vendor IT6263 driver files (cy_base_hdmi.c, cy_lvds_hdmi.c) were copied
 * byte-for-byte from the MTB "Graphics_Middleware_FPD-link_HDMI_basic" example.
 * Inside Zephyr we do not have the MTB BSP, so instead of the full PDL umbrella
 * header we pull only the few PDL drivers those files actually use:
 * GPIO (reset pin), SysLib (delays) and the device register map.
 */
#ifndef T2G_HDMI_SHIM_CY_PDL_H
#define T2G_HDMI_SHIM_CY_PDL_H

#include <string.h>

#include "cy_device_headers.h"
#include "cy_gpio.h"
#include "cy_syslib.h"
#include "cy_sysclk.h"

#endif /* T2G_HDMI_SHIM_CY_PDL_H */
