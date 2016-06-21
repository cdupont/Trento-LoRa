/*
 *  LoRa 868 / 915MHz SX1272 LoRa module
 *
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L.
 *  http://www.libelium.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 *  Version:           1.1
 *  Design:            David Gascón
 *  Implementation:    Covadonga Albiñana & Victor Boria
 *  
 */

// Include the SX1272 and SPI library:
#include "arduPiLoRa.h"
#include "regex.h"

int e;
char my_packet[100];
char messageCheck[100];
char pattern[200];

//Setting pattern for measurement row. E.g.: Ax|H=58.40|TA=22.80|TB=22.70|P=96901.00|L=711|N=41
const char * dataregularexpr="([A-Za-z])\\w+\\|H=[0-9]+(.[0-9]*)\\|TA=[0-9]+(.[0-9]*)\\|TB=[0-9]+(.[0-9]*)\\|P=[0-9]+(.[0-9]*)\\|L=[0-9]+(.[0-9]*)\\|N=[0-9]+(.[0-9]*)";

void setup()
{
  //Setting regular expression for matching measurement row
  strcpy(pattern, dataregularexpr);

  // Print a start message
  printf("SX1272 module and Raspberry Pi: receive packets without ACK\n");

  //strcpy(pattern, "GGG");

  // Print a start message
  printf("SX1272 module and Raspberry Pi: receive packets without ACK\n");

  // Power ON the module
  e = sx1272.ON();

  printf("Setting power ON: state %d\n", e);

  // Set transmission mode
  e = sx1272.setMode(4);
  printf("Setting Mode: state %d\n", e);

  // Set header
  e = sx1272.setHeaderON();
  printf("Setting Header ON: state %d\n", e);

  // Select frequency channel
  e = sx1272.setChannel(CH_10_868);
  printf("Setting Channel: state %d\n", e);

  // Set CRC
  e = sx1272.setCRC_ON();
  printf("Setting CRC ON: state %d\n", e);

  // Select output power (Max, High or Low)
  e = sx1272.setPower('H');
  printf("Setting Power: state %d\n", e);

  // Set the node address
  e = sx1272.setNodeAddress(8);
  printf("Setting Node address: state %d\n", e);

  // Print a success message
  printf("SX1272 successfully configured\n\n");
  delay(1000);
}

int
match(const char *string, char *pattern)
{
    int    status;
    regex_t    re;


    //printf("Match: %s##%s\n",string, pattern);
     
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) {
        //printf("no Match\n");
        return(0);      /* Report error. */
    }
    status = regexec(&re, string, (size_t) 0, NULL, 0);
    regfree(&re);
    if (status != 0) {
        //printf("no Match 2\n");
        return(0);      /* Report error. */
    }
    return(1);
}

void loop(void)
{
  // Receive message
  e = sx1272.receivePacketTimeout(10000);
  if ( e == 0 )
  {
    printf("Receive packet, state %d\n",e);

    for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      my_packet[i] = (char)sx1272.packet_received.data[i];
    }
    printf("Message: %s\n", my_packet);
    //add measures from LoRa Gateway to textfile
    strcpy(messageCheck, my_packet);
    if (match (messageCheck, pattern) !=0) { 
      printf("measurement row found\n");
      FILE *file;
      file = fopen("/root/cooking/examples/LoRa/measures.txt", "w");
      fputs(my_packet, file);
      fclose(file);
    }
  }
  else {
    //printf("Receive packet, state %d\n",e);
  }
}

int main (){
  setup();
  while(1){
    loop();
  }
  return (0);
}
