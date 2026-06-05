/*
 * Copyright (c) 2026 Linumiz
 * SPDX-License-Identifier: Apache-2.0
 *
 * Zephyr i2c transport for the ITE IT6263 LVDS-to-HDMI bridge.
 *
 * This file provides the exact API the byte-for-byte ported MTB IT6263 driver
 * (cy_base_hdmi.c / cy_lvds_hdmi.c) expects from "cy_i2c_hdmi.c", but implements
 * it on the Zephyr i2c controller API instead of the PDL SCB driver. The IT6263
 * register sequences in the vendor files are therefore unchanged; only the
 * physical transport differs.
 *
 * The bridge sits on SCB1 (P15_2 SDA / P15_3 SCL) which is described in the
 * board overlay as the "hdmi_i2c" i2c controller node.
 */
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>

#include "cy_syslib.h"
#include "cy_i2c_hdmi.h"

#define HDMI_I2C_NODE DT_NODELABEL(hdmi_i2c)
static const struct device *const hdmi_i2c_dev = DEVICE_DT_GET(HDMI_I2C_NODE);

/*
 * The MTB driver passes 7-bit device addresses (IT6263 HDMI-TX = 0x4C,
 * LVDS = 0x33) directly as "deviceAddr"; Zephyr's i2c API is likewise 7-bit.
 */

cy_en_i2c_result_t Cy_IT6263_I2C_Init(void)
{
	if (!device_is_ready(hdmi_i2c_dev)) {
		return CY_I2C_ERROR;
	}

	return CY_I2C_SUCCESS;
}

uint8_t Cy_IT6263_I2C_Read(uint8_t deviceAddr, uint8_t regAddr)
{
	uint8_t value = 0u;
	int ret;

	/* Register read = address write + repeated-START + data read. */
	for (uint8_t cnt = 0u; cnt < CY_IT6263_CMD_RETRY_COUNT; cnt++) {
		ret = i2c_write_read(hdmi_i2c_dev, deviceAddr, &regAddr, 1u, &value, 1u);
		Cy_SysLib_Delay(CY_IT6263_CMD_IN_BW_DELAY / 1000u);
		if (ret == 0) {
			break;
		}
	}

	return value;
}

cy_en_i2c_result_t Cy_IT6263_I2C_Write(uint8_t deviceAddr, uint8_t regAddr, uint8_t value)
{
	uint8_t buf[2] = { regAddr, value };
	int ret = -1;

	for (uint8_t cnt = 0u; cnt < CY_IT6263_CMD_RETRY_COUNT; cnt++) {
		ret = i2c_write(hdmi_i2c_dev, buf, sizeof(buf), deviceAddr);
		Cy_SysLib_Delay(CY_IT6263_CMD_IN_BW_DELAY / 1000u);
		if (ret == 0) {
			break;
		}
	}

	return (ret == 0) ? CY_I2C_SUCCESS : CY_I2C_ERROR;
}

cy_en_i2c_result_t Cy_IT6263_I2C_ReadWrite(uint8_t deviceAddr, uint8_t regAddr, uint8_t value,
					   cy_en_i2c_op_type_t op)
{
	uint8_t readValue = Cy_IT6263_I2C_Read(deviceAddr, regAddr);

	if (op == CY_I2C_OP_AND) {
		readValue &= value;
	} else {
		readValue |= value;
	}

	return Cy_IT6263_I2C_Write(deviceAddr, regAddr, readValue);
}

cy_en_i2c_result_t Cy_IT6263_I2C_WriteConfigurationSettings(uint8_t deviceAddr,
							    cy_stc_It6263_reg_t *configSettings,
							    uint8_t configSettingsSize)
{
	cy_en_i2c_result_t status = CY_I2C_SUCCESS;

	for (uint16_t i = 0u; (i < configSettingsSize) && (status == CY_I2C_SUCCESS); i++) {
		status = Cy_IT6263_I2C_Write(deviceAddr, configSettings[i].regAddr,
					     configSettings[i].regValue);
	}

	return status;
}
