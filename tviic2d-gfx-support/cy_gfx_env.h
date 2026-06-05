/*
 * Copyright (c) 2026 Linumiz
 * SPDX-License-Identifier: Apache-2.0
 *
 * Slim shim for MTB "cy_gfx_env.h".
 *
 * The vendor IT6263 driver (cy_lvds_hdmi.c) only consumes the IT6263 reset-pin
 * definitions from the full MTB gfx-env header. The IT6263 reset line on the
 * KIT_T2G_C2D6M_LITE board is P2_5 (driven directly via the PDL GPIO driver,
 * independent of Zephyr's GPIO driver). Everything else from the original
 * header is unused once the I2C transport is provided by the Zephyr i2c API.
 */
#ifndef T2G_HDMI_SHIM_CY_GFX_ENV_H
#define T2G_HDMI_SHIM_CY_GFX_ENV_H

#include "cy_device_headers.h"

/* IT6263 LVDS-to-HDMI bridge reset control: P2_5 (active low). */
#define CY_FPD_HDMI_RST_PORT      GPIO_PRT2
#define CY_FPD_HDMI_RST_PIN       (5u)
#define CY_FPD_HDMI_RST_PIN_MUX   P2_5_GPIO

#endif /* T2G_HDMI_SHIM_CY_GFX_ENV_H */
