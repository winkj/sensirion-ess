/*
 * Copyright (c) 2017-2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SENSIRION_ESS
#define SENSIRION_ESS

#include <stdint.h>

class SensirionESS
{
public:
    SensirionESS();

    int initSensors();

    int measureRHT();
    int measureIAQ();

    bool isInitialized();

    float getTemperature() const;
    float getHumidity() const;

    float getTVOC() const;

    const char* getError() const;

    void setLedRYG(int r, int y, int g);
    void setLedAutoSync(bool autoSync);

    int remainingWaitTimeMS();

    const static int SGP_INTERMEASURE_DELAY = 2000; // SGPC3

    const static int LED_RED                = 9;
    const static int LED_YEL                = 10;
    const static int LED_GRN                = 11;

private:
    static int8_t i2c_write(uint8_t addr, const uint8_t* data, uint16_t count);
    static int8_t i2c_read(uint8_t addr, uint8_t* data, uint16_t count);
    static uint8_t crc8(const uint8_t* data, uint8_t len);

    inline void setError(const char* error);
    int measureRHTInt();
    int initSGP();
    void setLedRYGInt(int r, int y, int g);

    const static int ERROR_BUF_LENGTH       = 255;
    const static int CMD_LENGTH             = 2;

    const static int SHT_DATA_LENGTH        = 6;
    const static int SHT_I2C_ADDR           = 0x70;
    const static int SHT_MEASURE_DELAY      = 15;   // SHTC1 worst case

    const static int SGP_DATA_LENGTH        = 3;
    const static int SGP_I2C_ADDR           = 0x58;
    const static int SGP_MEASURE_DELAY      = 50;

    const static int SGP_RED_THRESHOLD     = 150;
    const static int SGP_YEL_THRESHOLD     = 125;

    float mTemperature = -250;
    float mHumidity    = -1;
    float mTVOC        = -1;

    bool mLedAutoSync  = true;

    unsigned long mSGPMeasurementTimestamp = 0;

    bool mInitialized = false;
    char mErrorBuf[ERROR_BUF_LENGTH];
    uint8_t mDataBuf[SHT_DATA_LENGTH]; // max(SHT_DATA_SIZE, SGP_DATA_SIZE)
};

#endif /* SENSIRION_ESS */
