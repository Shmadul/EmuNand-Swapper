// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

// Main program entrypoint
int main(int argc, char* argv[])
{
    consoleInit(NULL);

    //define integers
    int disable, middle, enable;

    //set paths
    char enabled[] = "/emuMMC/emummc.ini";
    char disabled[] = "/emuMMC/emummc.disabled";
    char inbetween[] = "/emuMMC/disabled.ini";
    
    //Printf Stuff
    printf("\033[1;32m");
    printf("RAW Nand Swapper v1.0\n");
    printf("@shmadul 2019\n");
    printf("\033[0m");
    printf("\033[1;31m");
    printf("Press A to Swap Active Raw Nand\n");
    printf("Press X to reboot\n");
    printf("Press + to exit\n");
    printf("\033[0m");



    // Main loop
    while (appletMainLoop())
    {
        // Scan all the inputs. This should be done once for each frame
        hidScanInput();

        // hidKeysDown returns information about which buttons have been
        // just pressed in this frame compared to the previous one
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break; // break in order to return to hbmenu


        if (kDown & KEY_A)
            {
                //Allow for error Reporting
                disable = rename(enabled, inbetween);
                middle = rename(disabled, enabled);
                enable = rename(inbetween, disabled);   

                //error reporting
                if(disable != 0) 
                   {
                        printf("\033[1;31m");
                        printf("Error Renaming %s to %s \n", enabled, inbetween);
                        printf("\033[0m");
                   }
                    
                if(middle != 0) 
                  {
                        printf("\033[1;31m");
                        printf("Error Renaming %s to %s \n", disabled, enabled);
                        printf("\033[0m");
                  }

                if(enable != 0) 
                  {
                        printf("\033[1;31m");
                        printf("Error Renaming %s to %s \n", inbetween, disabled);
                        printf("\033[0m");
                  } else {
                        printf("\033[1;36m");
                        printf("Successfully Swapped Active Raw Nand\n");
                        printf("\033[0m");   

                        printf("\033[1;31m");
                        printf("Press X to reboot, Press + to exit\n");
                        printf("\033[0m");
                    
                  }
            }

        if (kDown & KEY_X)
           {
                bpcInitialize();
                bpcRebootSystem();
            }        

        // Update the console, sending a new frame to the display
        consoleUpdate(NULL);
    }

    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}
