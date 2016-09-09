/* SIS8300.cpp
 *
 * This is a driver for a Struck SIS8300 digitizer.
 * Based on ADCSimDetector ADExample.
 *
 * Author: Hinko Koceavar
 *         ESS ERIC, Lund, Sweden
 *
 * Created:  September 11, 2016
 *
 */

#include <stdint.h>
#include <epicsEvent.h>
#include <epicsTime.h>
#include "asynNDArrayDriver.h"

#include <sis8300drv.h>

#define SisMessageString               "SIS_MESSAGE"
#define SisAcquireString               "SIS_ACQUIRE"
#define SisAcquireTimeString           "SIS_ACQUIRE_TIME"
#define SisElapsedTimeString           "SIS_ELAPSED_TIME"
#define SisTimeStepString              "SIS_TIME_STEP"
#define SisNumTimePointsString         "SIS_NUM_TIME_POINTS"
#define SisClockSourceString           "SIS_CLOCK_SOURCE"
#define SisClockFreqString             "SIS_CLOCK_FREQ"
#define SisClockDivString              "SIS_CLOCK_DIV"
#define SisTrigSourceString            "SIS_TRIG_SOURCE"
#define SisTrigLineString              "SIS_TRIG_LINE"
#define SisTrigDoString                "SIS_TRIG_DO"
#define SisTrigDelayString             "SIS_TRIG_DELAY"
#define SisTrigRepeatString            "SIS_TRIG_REPEAT"
#define SisChannelEnableString         "SIS_ENABLE"
#define SisChannelConvFactorString     "SIS_CONV_FACTOR"
#define SisChannelConvOffsetString     "SIS_CONV_OFFSET"
#define SisChannelAttenuationString    "SIS_ATTENUATION"
#define SisChannelDecimFactorString    "SIS_DECIM_FACTOR"
#define SisChannelDecimOffsetString    "SIS_DECIM_OFFSET"
#define SisResetString                 "SIS_RESET"
#define SisFirmwareVersionString       "SIS_FW_VERSION"
#define SisDeviceTypeString            "SIS_DEVICE_TYPE"
#define SisSerialNumberString          "SIS_SERIAL_NUMBER"
#define SisMemorySizeString            "SIS_MEMORY_SIZE"

#define MAX_PATH_LEN                   32
#define MAX_ERROR_STR_LEN              32

/** Struck SIS8300 driver; does 1-D waveforms on 10 channels.
  * Inherits from asynNDArrayDriver */
class epicsShareClass ADSIS8300 : public asynNDArrayDriver {
public:
	ADSIS8300(const char *portName, const char *devicePath,
			int numTimePoints, NDDataType_t dataType,
			int maxBuffers, size_t maxMemory,
			int priority, int stackSize);
	~ADSIS8300();

    /* These are the methods that we override from asynNDArrayDriver */
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
    virtual void report(FILE *fp, int details);
    /**< Should be private, but gets called from C, so must be public */
    void sisTask();

protected:
    int P_Acquire;
    #define FIRST_SIS8300_PARAM P_Acquire
    int P_AcquireTime;
    int P_ElapsedTime;
    int P_TimeStep;
    int P_NumTimePoints;
    int P_ClockSource;
    int P_ClockFreq;
    int P_ClockDiv;
    int P_TrigSource;
    int P_TrigLine;
    int P_TrigDo;
    int P_TrigDelay;
    int P_TrigRepeat;
    int P_Enable;
    int P_ConvFactor;
    int P_ConvOffset;
    int P_Attenuation;
    int P_DecimFactor;
    int P_DecimOffset;
    int P_Reset;
    int P_Message;
    int P_FirmwareVersion;
    int P_SerialNumber;
    int P_DeviceType;
    int P_MemorySize;

    int P_Dummy;
    #define LAST_SIS8300_PARAM P_Dummy


private:
    /* These are the methods that are new to this class */
    template <typename epicsType> int acquireArraysT();
    int acquireArrays();
    void setAcquire(int value);
    int initDevice();
    int destroyDevice();
    int enableChannel(unsigned int channel);
    int disableChannel(unsigned int channel);

    /* Our data */
    epicsEventId startEventId_;
    epicsEventId stopEventId_;
    int uniqueId_;
    int acquiring_;
    double elapsedTime_;

    char mSisDevicePath[MAX_PATH_LEN];
    sis8300drv_usr *mSisDevice;
    char mSisErrorStr[MAX_ERROR_STR_LEN];
//    unsigned int mSisDeviceType;
//    unsigned int mSisFirmwareVersion;
//    unsigned long mSisMemorySizeMb;
//    unsigned int mSisSerialNumber;
    unsigned int mSisFirmwareOptions;
    uint32_t mChannelMask;
    NDArray *mRawDataArray;
};


#define NUM_SIS8300_PARAMS ((int)(&LAST_SIS8300_PARAM - &FIRST_SIS8300_PARAM + 1))

