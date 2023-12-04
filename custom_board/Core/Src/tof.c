#include "tof.h"
#include "i2c.h"
#include "vl53l1_platform.h"
#include "VL53L1X_api.h"
#include "VL53L1_custom_api.h"
#include "debug.h"
#include "timer_control.h"

#define TOF_ROI_NUMS    4//16
#define TOF_SAMPLING    3

uint16_t tof_dev_1 = 0x52;
// volatile uint8_t dataReady;

static volatile uint8_t flag_tof_boot = 0;
static volatile uint8_t flag_tof_init = 0;
static volatile uint8_t flag_tof_start = 0;

#if (TOF_ROI_NUMS == 16)
const uint8_t tof_roi_centre[TOF_ROI_NUMS][2] =\
{
    {(0+3+1)/2, (3+0+1)/2}, {(4+7+1)/2, (3+0+1)/2}, {(8+11+1)/2, (3+0+1)/2}, {(12+15+1)/2, (3+0+1)/2},
    {(0+3+1)/2, (7+4+1)/2}, {(4+7+1)/2, (7+4+1)/2}, {(8+11+1)/2, (7+4+1)/2}, {(12+15+1)/2, (7+4+1)/2}, 
    {(0+3+1)/2, (11+8+1)/2}, {(4+7+1)/2, (11+8+1)/2}, {(8+11+1)/2, (11+8+1)/2}, {(12+15+1)/2, (11+8+1)/2}, 
    {(0+3+1)/2, (15+12+1)/2}, {(4+7+1)/2, (15+12+1)/2}, {(8+11+1)/2, (15+12+1)/2}, {(12+15+1)/2, (15+12+1)/2}
};

#elif (TOF_ROI_NUMS == 4)
const uint8_t tof_roi_centre[TOF_ROI_NUMS][2] =\
{
    {4, 4}, {12, 4}, {4, 12}, {12, 12}
};
#else
#endif

uint16_t tof_roi_value[TOF_ROI_NUMS] = {0,};
static volatile uint8_t tof_roi_idx = 0;
static volatile uint32_t total_dist = 0;
static volatile uint8_t meas_cnt = 0;


uint8_t ROICenter = 0;
uint16_t ROI_X = 0;
uint16_t ROI_Y = 0;

void tof_show_res(void)
{
    printf("\r\n distance \r\n");
#if (TOF_ROI_NUMS == 16)
    printf("\t y|  %d %d %d %d \r\n", tof_roi_value[12], tof_roi_value[13], tof_roi_value[14], tof_roi_value[15]);
    printf("\t  |  %d %d %d %d \r\n", tof_roi_value[8], tof_roi_value[9], tof_roi_value[10], tof_roi_value[11]);
    printf("\t  |  %d %d %d %d \r\n", tof_roi_value[4], tof_roi_value[5], tof_roi_value[6], tof_roi_value[7]);
    printf("\t  |  %d %d %d %d \r\n", tof_roi_value[0], tof_roi_value[1], tof_roi_value[2], tof_roi_value[3]);
#elif (TOF_ROI_NUMS == 4)
    printf("\t y|  %d %d \r\n", tof_roi_value[2], tof_roi_value[3]);
    printf("\t  |  %d %d \r\n", tof_roi_value[0], tof_roi_value[1]);
#else
#endif
    printf("\t  |_______________ x \r\n");
}

void tof_start_measurement(void)
{
    tof_start(0);
    set_delay_ms(2);
#if (TOF_ROI_NUMS == 16)
    VL53L1X_custom_set_roi(tof_dev_1, tof_roi_centre[tof_roi_idx][0], tof_roi_centre[tof_roi_idx][1], 4, 4);
#elif (TOF_ROI_NUMS == 4)
    VL53L1X_custom_set_roi(tof_dev_1, tof_roi_centre[tof_roi_idx][0], tof_roi_centre[tof_roi_idx][1], 8, 8);
#else
#endif
    set_delay_ms(2);
    if (VL53L1X_GetROICenter(tof_dev_1, &ROICenter))
    {
        printf("[VL53L1X_GetROICenter] wrong \r\n");
    }
    if (VL53L1X_GetROI_XY(tof_dev_1, &ROI_X, &ROI_Y))
    {
        printf("[VL53L1X_SetROI] wrong \r\n");
    }
    tof_start(1);
}

void tof_start(bool state)
{
    if (state)
    {
        if (VL53L1X_StartRanging(tof_dev_1))
        {
            printf("[VL53L1X_StartRanging] wrong \r\n");
        }
        else
        {
            flag_tof_start = 1;
        }
    }
    else
    {
        if (VL53L1X_StopRanging(tof_dev_1))
        {
            printf("[VL53L1X_StopRanging] wrong \r\n");
        }
        else
        {
            flag_tof_start = 0;
        }
    }
}

void tof_get_config(void)
{
    uint8_t y = 0;
    uint8_t x = 0;
    if (ROICenter > 127)
    {
        x = 8 + ((255-ROICenter) & 0x07);
        y = (ROICenter-128) >> 3;
    }
    else
    {
        x = ROICenter & 0x07;
        y = (127-ROICenter) >> 3;
    }
    printf("ROI center %d %d\r\n", x, y);
    printf("ROI width %d, height %d\r\n", ROI_X, ROI_Y);

}

void tof_init(void)
{
    uint8_t byteData = 0;
    uint8_t sensorState = 0;
    uint16_t wordData = 0;
    int status = 0;

    LL_GPIO_SetOutputPin(XSHUT_GPIO_Port, XSHUT_Pin);
    if (!LL_GPIO_IsOutputPinSet(XSHUT_GPIO_Port, XSHUT_Pin))
    {
        printf("XSHUT not high \r\n");
    }

    set_delay_ms(1);

    /* Those basic I2C read functions can be used to check your own I2C functions */
    status = VL53L1_RdByte(tof_dev_1, 0x010F, &byteData);
    printf("VL53L1X Model_ID: %X\r\n", byteData);
    status = VL53L1_RdByte(tof_dev_1, 0x0110, &byteData);
    printf("VL53L1X Module_Type: %X\r\n", byteData);
    status = VL53L1_RdWord(tof_dev_1, 0x010F, &wordData);
    printf("VL53L1X: %X\r\n", wordData);

    if (VL53L1X_BootState(tof_dev_1, &sensorState))
    {
        printf("bootstate ?!?! \r\n");
    }
    if (sensorState)
    {
        printf("booted \r\n");
    }
    else{
        printf("not booted \r\n");
    }

    if (VL53L1X_SensorInit(tof_dev_1))
    {
        printf("not init \r\n");
    }
    else
    {
        printf("sensor init \r\n");
    }

    // uint16_t signal = 0;
    // if (VL53L1X_GetSigmaThreshold(tof_dev_1, &signal))
    // {
    //     printf("[VL53L1X_SetSigmaThreshold] wrong \r\n");
    // }
    // else
    // {
    //     printf("[VL53L1X_SetSigmaThreshold] success, %d \r\n", signal);
    // }

    // short mode : dist < 1.3m
    if (VL53L1X_SetDistanceMode(tof_dev_1, 1))
    // if (VL53L1X_SetDistanceMode(tof_dev_1, 2))
    {
        printf("[VL53L1X_SetDistanceMode] wrong \r\n");
    }
    else
    {
        // printf("[VL53L1X_SetDistanceMode] success \r\n");
    }

    if (VL53L1X_SetTimingBudgetInMs(tof_dev_1, 100))
    {
        printf("[VL53L1X_SetTimingBudgetInMs] wrong \r\n");
    }
    else
    {
        // printf("[VL53L1X_SetTimingBudgetInMs] success \r\n");
    }

    // only autonomous mode?
    if (VL53L1X_SetInterMeasurementInMs(tof_dev_1, 100))
    {
        printf("[VL53L1X_SetInterMeasurementInMs] wrong \r\n");
    }
    else
    {
        // printf("[VL53L1X_SetInterMeasurementInMs] success \r\n");
    }

    // if (VL53L1X_GetROICenter(tof_dev_1, &ROICenter))
    // {
    //     printf("[VL53L1X_GetROICenter] wrong \r\n");
    // }
    // if (VL53L1X_GetROI_XY(tof_dev_1, &ROI_X, &ROI_Y))
    // {
    //     printf("[VL53L1X_SetROI] wrong \r\n");
    // }

    // if (VL53L1X_SetROI(tof_dev_1, 4, 4))
    // {
    //     printf("[VL53L1X_SetROI] wrong \r\n");
    // }
    // else
    // {
    //     // printf("[VL53L1X_GetROICenter] success , %X \r\n", ROICenter);
    //     // printf("[VL53L1X_SetROI] success \r\n");
    // }

    flag_tof_init = 1;
    printf("tof_init done \r\n");
}

void tof_receive_data(void)
{
    int status;
    uint16_t Distance;
    uint16_t SignalRate;
    uint16_t AmbientRate;
    uint16_t SpadNum; 
    uint8_t RangeStatus;
    uint8_t dataReady;

    if (!flag_tof_init || !flag_tof_start )
    {
        return ;
    }

    // if (!dataReady)
    // {
    //     VL53L1X_CheckForDataReady(tof_dev_1, &dataReady);
    //     set_delay_ms(2);
    // }

    VL53L1X_CheckForDataReady(tof_dev_1, &dataReady);
    set_delay_ms(2);

    if (dataReady)
    {
        status = VL53L1X_GetRangeStatus(tof_dev_1, &RangeStatus);
        status = VL53L1X_GetDistance(tof_dev_1, &Distance);
        status = VL53L1X_GetSignalRate(tof_dev_1, &SignalRate);
        status = VL53L1X_GetAmbientRate(tof_dev_1, &AmbientRate);
        status = VL53L1X_GetSpadNb(tof_dev_1, &SpadNum);
        status = VL53L1X_ClearInterrupt(tof_dev_1);
        dataReady = 0;
#if 0
        printf("%u, %u, %u, %u, %u \r\n", RangeStatus, Distance, SignalRate, AmbientRate,SpadNum);        
#else
        if (!RangeStatus)
        {
            total_dist += Distance;
            if (++meas_cnt == TOF_SAMPLING)
            {
                // printf("%u : %ld \r\n", tof_roi_idx+1, total_dist);
                meas_cnt = 0;
                tof_roi_value[tof_roi_idx] = total_dist/TOF_SAMPLING;
                total_dist = 0;
                // tof_get_config();
                if(++tof_roi_idx == TOF_ROI_NUMS)
                // if(++tof_roi_idx == 1)
                {
                    tof_roi_idx = 0;
                    tof_start(0);
                    tof_show_res();
                }
                else
                {
                    tof_start_measurement();
                }
            }
        }
        else
        {
            printf("range status error, %d %d \r\n", RangeStatus, Distance);
        }
#endif
    }
}
