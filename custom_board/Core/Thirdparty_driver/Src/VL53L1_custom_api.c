#include "VL53L1_custom_api.h"



VL53L1X_ERROR VL53L1X_custom_set_roi(uint16_t dev, 
uint8_t x_centre, uint8_t y_centre, uint8_t x_width, uint8_t y_height)
{
    VL53L1X_ERROR status = 0;
    uint16_t data = 0;
    if (y_centre > 7)
    {
        data = 128 + (x_centre<<3) + (15-y_centre);
    }
    else
    {
        data = ((15-x_centre) << 3) + y_centre;
    }
	status |= VL53L1_WrByte(dev, ROI_CONFIG__USER_ROI_CENTRE_SPAD, data);

    // size , 0 = 1, 15=16
    data = ((y_height-1) << 4) + (x_width-1);
    status |= VL53L1_WrByte(dev, 
    ROI_CONFIG__USER_ROI_REQUESTED_GLOBAL_XY_SIZE, data);

	return status;
}