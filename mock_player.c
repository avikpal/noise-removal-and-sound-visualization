/*
 *  Copyright 2014  Avik Pal<avikpal.me@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License or (at your option) version 3 or any later version
 *  accepted by the membership of KDE e.V. (or its successor approved
 *  by the membership of KDE e.V.), which shall act as a proxy
 *  defined in Section 14 of version 3 of the license.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char CommandString[500];

int main(int argc, char *argv[])
{
/*
Two arguments, first one is the option specifying whether recording or playing. And the second option is the
file name to which it has to be saved/ or which has to be played back.
*/
    printf("num args %d\n",argc);
  
    if(argc==3 || argc==2){
  
        if(strcmp(argv[1], "record")==0){ //start recording
            printf("Starting recording\n");
      /* Save the recorded audio file with a default name 
       */
        if(argc==2){
            system("gst-launch alsasrc ! audioconvert ! audioresample ! 
	        vorbisenc ! oggmux ! filesink location= SampleTest.ogg");
        }
      
        if(argc==3){
      /* Save the recorded audio file with the name provided by user 
       */
	
            strcpy(CommandString,"gst-launch alsasrc ! audioconvert ! 
	    audioresample ! vorbisenc ! oggmux ! filesink location=");
	    strcat(CommandString,argv[2]);
	    printf("%s", CommandString);
	    system(CommandString);
	
        }
	
	
        }
    
        if(strcmp(argv[1], "playback")==0){
      
            printf("Starting playback\n");
      /* Play the recorded audio file with the default name 
       */
            if(argc==2){
	        system("gst-launch filesrc location=SampleTest.ogg ! decodebin ! audioconvert ! audioresample ! alsasink");
            }      
      /* Play recorded audio file with the specified name
       */
      
            if(argc==3){
            
                printf("starting playback\n");
                strcpy(CommandString,"gst-launch filesrc location=");
                strcat(CommandString, argv[2]);
                strcat(CommandString," ! decodebin ! audioconvert ! audioresample ! alsasink");
                system(CommandString);
	
            }
        }
    }
    
    else{ 
        printf("Error in input formatting");
    }    
}
