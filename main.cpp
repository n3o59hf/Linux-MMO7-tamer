#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <usb.h>

#include "mouseemu.h"
#include "mouseconfig.h"

#define DEBUG_LEVEL 0

#define PROTOCOL_MOUSE 2
#define PROTOCOL_KEYBOARD 1

using namespace std;

struct usb_bus *busses,*bus,*dbus;

int findDevice(int PROTOCOL,struct usb_device *device)
{
    bool found=false;
    struct usb_device *dev;

    usb_find_busses();
    usb_find_devices();
    busses=usb_get_busses();
    for (bus=busses; bus && !found; bus=bus->next)
    {
        for (dev=bus->devices; dev; dev=dev->next)
        {
            cout << dev << endl;
            if (dev->config->interface->altsetting->bInterfaceProtocol==PROTOCOL)
            {
                if (dev->descriptor.idVendor == MOUSE_MANUFACTURER)
                {
                    dbus=bus;
                    found=true;
                    break;
                }
            }
        }//end of devices loop
    }//end of busses loop
    if (!found) return -1;
    cout << dev << endl;
    *device=*dev;
    return 1;
}

void printInfo(struct usb_device * dev)
{
    printf("Device Protocol : %d\n",dev->descriptor.bDeviceProtocol);
    printf("Report Length : %d\n",dev->descriptor.bLength);
    printf("Decriptor Type : %d\n",dev->descriptor.bDescriptorType);
    printf("End Points : %d\n",dev->config->interface->altsetting->bNumEndpoints);
    printf("Interface Class : %d\n",dev->config->interface->altsetting->bInterfaceClass);
    printf("Protocol : %d\n",dev->config->interface->altsetting->bInterfaceProtocol);
    printf("Interface Number: %d\n",dev->config->interface->altsetting->bInterfaceNumber);
    printf("Device Filename : %s\n",dev->filename);
    printf("Bus Dir Name : %s\n",dbus->dirname);
}

int main(int argc, char *argv[])
{

    if (!initMouseEmu())
    {
        printf("Failure to enable mouseEmu... Exiting...\n");
        exit(1);
    }

    struct usb_device devx;
    struct usb_device * dev;
    char *buf;
    int n,x,r;
    char string[PACKET_SIZE];

    bool found=false;
    usb_dev_handle *fdev;
    usb_set_debug(DEBUG_LEVEL);
    usb_init(); //initilize the usb library



    if (findDevice(PROTOCOL_MOUSE,&devx)<0)  //search for a USB mouse , you can change it to keyboard,joystick…etc
    {
        printf("Unable to find the required device !nexsitingn");
        exit(1);
    }
    dev = &devx;


//    cout << "Now we are dealing with device from vendor ID : "<< dev->descriptor.idVendor << " " << dev->descriptor.idVendor << endl;
    cout << "Trying to open the device..." << endl;
    if (fdev=usb_open(dev)) cout << "Device opened successfully" << endl; //Here we open the device , just like fopen
    else
    {
        printf("Operation failed  n");
        exit(1);
    }

    buf=(char*)calloc(1,100);
    if (usb_get_driver_np(fdev,0,buf,100)) cout <<"Kernel Using Driver :" <<buf << endl; //Trying to get the kernel driver name (optional)
    else printf("Could not read the driver name  \n");

    if (usb_detach_kernel_driver_np(fdev,0)) cout << "Device detached successfully from the kernel." << endl; //detach the driver from the kernel , seems to be just like rmmod , but it always returns errors , however , it wroks
    else printf("Error detaching the device  \n");

    if (r=usb_claim_interface(fdev,0)) printf("Interface Claimed !!\n"); //reserving the device interface for our applicatoin , if another driver/software is using the device , it will return ‘interface busy’
    printf("Interface Claim Status : %d\n",r);

    printInfo(dev);
    usb_get_string_simple(fdev,dev->descriptor.iManufacturer,string,sizeof(string));
    printf("Device Manfucaturer : %s\n",string);
    usb_get_string_simple(fdev,dev->descriptor.iProduct,string,sizeof(string));
    printf("Product Name : %s\n",string);
    usb_get_string_simple(fdev,dev->descriptor.iSerialNumber,string,sizeof(string));
    printf("Device Serial Number: %s\n",string);
    printf("End point addresses : 0x%x\n",dev->config->interface->altsetting->endpoint->bEndpointAddress);

    while (true)
    {

        r=usb_interrupt_read(fdev,0x81,string,PACKET_SIZE,0); //I am reading 4 bytes using interrupt read , note that not every usb device supports interrupt read/write
        cout << "Read: " << r << endl;

        for (x=0; x<r && r>0; x++)
        {
            unsigned char y = 0x01;
            while (y!= 0)
            {
                if ((string[x] & y) != 0x00)
                {
                    printf("1");
                }
                else
                {
                    printf("0");
                }
                y=y<<1;
            }
            printf("\n");
        }
        if (r == PACKET_SIZE)
        {
            mouseButton(string+BUTTON_OFFSET);
            mouseMove(string[X_OFFSET],string[Y_OFFSET]);
            mouseScroll(string[SCROLL_OFFSET]);
        }
//Not needed for mouse
//        usb_clear_halt(fdev,0x81); //I need to reset the device node because of some devices (usually keyboards) keep sending the same bytes even after releasing the key !
    }

    printf("Closing Device.\n");
    usb_release_interface(fdev,0);
    usb_close(fdev);

    return EXIT_SUCCESS;
}

