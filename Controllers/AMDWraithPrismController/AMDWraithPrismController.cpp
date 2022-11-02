/*-----------------------------------------*\
|  AMDWraithPrismController.h               |
|                                           |
|  Driver for AMD Wraith Prism RGB lighting |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/6/2019   |
\*-----------------------------------------*/

#include "AMDWraithPrismController.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

AMDWraithPrismController::AMDWraithPrismController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

AMDWraithPrismController::~AMDWraithPrismController()
{
    hid_close(dev);
}

std::string AMDWraithPrismController::GetLocationString()
{
    return("HID: " + location);
}

std::string AMDWraithPrismController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

std::string AMDWraithPrismController::GetFirmwareVersionString()
{
    std::string ret_string = "";

    unsigned char usb_buf[] =
    {
        0x00,
        0x12, 0x20, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    unsigned char   fw_buf[16] = {0x00};

    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);

    for(int char_idx = 0; char_idx < 16; char_idx+=2)
    {
        if(usb_buf[char_idx + 0x08] != 0)
        {
            fw_buf[char_idx / 2] = usb_buf[char_idx + 0x08];
        }
        else
        {
            break;
        }
    }

    ret_string.append((char *)fw_buf);

    return(ret_string);
}

void AMDWraithPrismController::SendEnableCommand()
{
    unsigned char usb_buf[] =
    {
        0x00,
        0x41, 0x03, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);
}

void AMDWraithPrismController::SendApplyCommand()
{
    unsigned char usb_buf[] =
    {
        0x00,
        0x51, 0x28, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);
}

void AMDWraithPrismController::SendDirectPacket
    (
    unsigned char   size,
    unsigned char * led_ids,
    RGBColor *      colors
    )
{
    unsigned char usb_buf[] =
    {
        0x00,
        0xC0, 0x01, size, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    for(unsigned int led_idx = 0; led_idx < size; led_idx++)
    {
        unsigned int index  = led_idx * 4;

        usb_buf[index + 5]  = led_ids[led_idx];
        usb_buf[index + 6]  = RGBGetRValue(colors[led_idx]);
        usb_buf[index + 7]  = RGBGetGValue(colors[led_idx]);
        usb_buf[index + 8]  = RGBGetBValue(colors[led_idx]);
    }

    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 64);
}
