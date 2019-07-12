// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

#define MAXCHAR 1000
//define integers
int disable, middle, enable;

//set paths
char enabled[] = "/emuMMC/emummc.ini";
char disabled[] = "/emuMMC/emummc.disabled";
char inbetween[] = "/emuMMC/disabled.ini";

//Read emummc config
void readini()
    {
         FILE *fp = fopen(enabled, "r");
         if(fp == NULL) 
           {
                perror("Unable to open emummc.ini");
                exit(1);
           }
         char line[128];
         while(fgets(line, sizeof(line), fp) != NULL) 
              {
                    if (strstr(line, "sector=") != NULL) 
                       {
                            fputs(line, stdout);
                       } 
                   else if (strstr(line, "title=") != NULL) 
                       {
                            fputs(line, stdout);
                       } 
                        
              }

         fclose(fp);
    }

// Main program entrypoint
int main(int argc, char* argv[])
{
    consoleInit(NULL);


    //Menu Stuff
    printf("RAW Nand Swapper v2.1\n");
    printf("@shmadul 2019\n");
    printf("Current Active Nand: \n");
    readini();
    printf("\033[1;31m");
    printf("Press A to Swap Active Raw Nand\n");
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
                //Clear screen
                printf("\e[1;1H\e[2J");
                printf("RAW Nand Swapper v2.1\n");
                printf("@shmadul 2019\n");

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
                        printf("\033[1;32m");
                        printf("Successfully Swapped Active Raw Nand\n");
                        printf("\033[0m");   
                        printf("Current Active Nand: \n");
                        readini();
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
