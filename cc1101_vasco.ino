//New receiving method. This method checks the Rx Fifo for any data it contains.
//It allows you to do several things in a loop.
//In addition, the gdo0 and gdo2 pin are not required.
//based on code by https://github.com/LSatan/SmartRC-CC1101-Driver-Lib
//by Little_S@tan
// vasco changes by Bram Vreugdenhil https://github.com/bramvreugd/Vasco-WTW-CC1101
//
// Not all codes work very well. Sometime you need to send it a couple of times.
// I primarly use the auto command to enable the 0-10v input.

#include <ELECHOUSE_CC1101_SRC_DRV.h>
unsigned long lastTime;
unsigned long interval = 1000;

void setup(){

    Serial.begin(115200);
    Serial.println("");
    if (ELECHOUSE_cc1101.getCC1101()){      // Check the CC1101 Spi connection.
       Serial.println("Connection OK");
    }else{
       Serial.println("Connection Error");
    }

    ELECHOUSE_cc1101.Init();                // must be set to initialize the cc1101!
    ELECHOUSE_cc1101.setCCMode(1);          // set config for internal transmission mode.
    ELECHOUSE_cc1101.setModulation(0);      // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
    ELECHOUSE_cc1101.setMHZ(868.299); // was 293       // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
    ELECHOUSE_cc1101.setDeviation(100);   // Set the Frequency deviation in kHz. Value from 1.58 to 380.85. Default is 47.60 kHz.
    ELECHOUSE_cc1101.setChannel(0);         // Set the Channelnumber from 0 to 255. Default is cahnnel 0.
    ELECHOUSE_cc1101.setChsp(199.951172);       // The channel spacing is multiplied by the channel number CHAN and added to the base frequency in kHz. Value from 25.39 to 405.45. Default is 199.95 kHz.
    //ELECHOUSE_cc1101.setRxBW(325.0000);       // Set the Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz.
    ELECHOUSE_cc1101.setDRate(38.4);          // Set the Data Rate in kBaud. Value from 0.02 to 1621.83. Default is 99.97 kBaud!
    ELECHOUSE_cc1101.setPA(10);             // Set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max!
    ELECHOUSE_cc1101.setSyncMode(1);        // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
    ELECHOUSE_cc1101.setSyncWord(255,0); // Set sync word. Must be the same for the transmitter and receiver. (Syncword high, Syncword low)
    ELECHOUSE_cc1101.setAdrChk(0);          // Controls address check configuration of received packages. 0 = No address check. 1 = Address check, no broadcast. 2 = Address check and 0 (0x00) broadcast. 3 = Address check and 0 (0x00) and 255 (0xFF) broadcast.
    ELECHOUSE_cc1101.setAddr(0);            // Address used for packet filtration. Optional broadcast addresses are 0 (0x00) and 255 (0xFF).
    ELECHOUSE_cc1101.setWhiteData(0);       // Turn data whitening on / off. 0 = Whitening off. 1 = Whitening on.
    ELECHOUSE_cc1101.setPktFormat(0);       // Format of RX and TX data. 0 = Normal mode, use FIFOs for RX and TX. 1 = Synchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins. 2 = Random TX mode; sends random data using PN9 generator. Used for test. Works as normal mode, setting 0 (00), in RX. 3 = Asynchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins.
    ELECHOUSE_cc1101.setLengthConfig(1);    // 0 = Fixed packet length mode. 1 = Variable packet length mode. 2 = Infinite packet length mode. 3 = Reserved
    ELECHOUSE_cc1101.setPacketLength(0);    // Indicates the packet length when fixed packet length mode is enabled. If variable packet length mode is used, this value indicates the maximum packet length allowed.
    ELECHOUSE_cc1101.setCrc(0);             // 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.
    ELECHOUSE_cc1101.setCRC_AF(0);          // Enable automatic flush of RX FIFO when CRC is not OK. This requires that only one packet is in the RXIFIFO and that packet length is limited to the RX FIFO size.
    ELECHOUSE_cc1101.setDcFilterOff(0);     // Disable digital DC blocking filter before demodulator. Only for data rates ≤ 250 kBaud The recommended IF frequency changes when the DC blocking is disabled. 1 = Disable (current optimized). 0 = Enable (better sensitivity).
    ELECHOUSE_cc1101.setManchester(0);      // Enables Manchester encoding/decoding. 0 = Disable. 1 = Enable.
    ELECHOUSE_cc1101.setFEC(0);             // Enable Forward Error Correction (FEC) with interleaving for packet payload (Only supported for fixed packet length mode. 0 = Disable. 1 = Enable.
    ELECHOUSE_cc1101.setPQT(0);             // Preamble quality estimator threshold. The preamble quality estimator increases an internal counter by one each time a bit is received that is different from the previous bit, and decreases the counter by 8 each time a bit is received that is the same as the last bit. A threshold of 4∙PQT for this counter is used to gate sync word detection. When PQT=0 a sync word is always accepted.
    ELECHOUSE_cc1101.setAppendStatus(0);    // When enabled, two status bytes will be appended to the payload of the packet. The status bytes contain RSSI and LQI values, as well as CRC OK.
    
    Serial.println("Rx Mode");
    lastTime=millis();
}

byte buffer[255] = {0};
byte transmitt_1[44] = {0xCA,0xAA,0xCA,0xA5,0x65,0xAA,0xA6,0x99,0x96,0x6A,0x5A,0x66,0x66,0x95,0xA5,0x59,0xA6,0x55,0x96,0x65,0x59,0x96,0x65,0xAA,0xA9,0x59,0x56,0xA5,0x95,0x65,0x59,0x56,0x65,0x95,0x66,0x9A,0x5A,0x59,0xAB,0x2A,0xAA,0xAA,0xAA,0xF8};
byte transmitt_2[44] = {0xCA,0xAA,0xCA,0xA5,0x65,0xAA,0xA6,0x99,0x96,0x6A,0x5A,0x66,0x66,0x95,0xA5,0x59,0xA6,0x55,0x96,0x65,0x59,0x96,0x65,0xAA,0xA9,0x59,0x56,0xA5,0x95,0x65,0x59,0x56,0xA5,0x95,0x66,0x9A,0x5A,0xA5,0xAB,0x2A,0xAA,0xAA,0xAA,0xFC};
byte transmitt_3[44] = {0xCA,0xAA,0xCA,0xA5,0x65,0xAA,0xA6,0x99,0x96,0x6A,0x5A,0x66,0x66,0x95,0xA5,0x59,0xA6,0x55,0x96,0x65,0x59,0x96,0x65,0xAA,0xA9,0x59,0x56,0xA5,0x95,0x65,0x59,0x56,0x59,0x95,0x66,0x9A,0x5A,0x65,0xAB,0x2A,0xAA,0xAA,0xAA,0xF8};
byte transmitt_timer[44] = {0xCA,0xAA,0xCA,0xA5,0x65,0xAA,0xA6,0x99,0x96,0x6A,0x5A,0x66,0x66,0x95,0xA5,0x59,0xA6,0x55,0x96,0x65,0x59,0x96,0x65,0xAA,0xAA,0x59,0x56,0xA9,0x95,0x65,0x59,0x56,0x65,0xA5,0x66,0xA9,0x56,0x59,0x95,0x66,0x99,0x56,0x55,0x95};
byte transmitt_auto[44] = {0xCA,0xAA,0xCA,0xA5,0x65,0xAA,0xA6,0x99,0x96,0x6A,0x5A,0x66,0x66,0x95,0xA5,0x59,0xA6,0x55,0x96,0x65,0x59,0x96,0x65,0xAA,0xA9,0x59,0x56,0xA5,0x95,0x65,0x59,0x56,0x99,0x95,0x66,0x9A,0x5A,0x95,0xAB,0x2A,0xAA,0xAA,0xAA,0xF8};

int autoMode=1;

void loop(){

  //Checks whether something has been received.
  //When something is received we give some time to receive the message in full.(time in millis)
  if (ELECHOUSE_cc1101.CheckRxFifo(100)){
  
    //CRC Check. If "setCrc(false)" crc returns always OK!
    if (ELECHOUSE_cc1101.CheckCRC()){ 
      float rssi=ELECHOUSE_cc1101.getRssi();
      float lqi= ELECHOUSE_cc1101.getLqi();
      int len = ELECHOUSE_cc1101.ReceiveData(buffer);
      buffer[len] = '\0';
  
      //if(rssi>-70.0){ use this to display only strong incoming data
         //Rssi Level in dBm
         Serial.println();
         Serial.print("Rssi: ");
         Serial.println(rssi);
  
         //Link Quality Indicator
         Serial.print("LQI: ");
         Serial.println(lqi);
  
         //Print received in bytes format.
         for (int i = 0; i<len; i++){
           Serial.print(buffer[i], HEX);
           Serial.print(",");
         }
         Serial.println();
     //} else{
     // Serial.print(".");
     //}
    }
  }
  char incomingByte = Serial.read();
  if(incomingByte!=255){
     Serial.println(incomingByte, DEC);
  }
  if(incomingByte==97){ // key a
     ELECHOUSE_cc1101.SendData(transmitt_auto, sizeof(transmitt_auto), 100);
     Serial.println("Send auto command to wtw");
  } else if(incomingByte==98){  // key b
    autoMode=1; // repeat auto command every minute
    Serial.println("Set mode to auto");
  } else if(incomingByte==99){  // key c    
    autoMode=0; // stop repeating command
    Serial.println("Set mode to manual");
  } else if(incomingByte==49){
    ELECHOUSE_cc1101.SendData(transmitt_1, sizeof(transmitt_1), 100);
    Serial.println("Send wtw low");
    autoMode=0;
  } else if(incomingByte==50){
    ELECHOUSE_cc1101.SendData(transmitt_2, sizeof(transmitt_1), 100);
    Serial.println("Send wtw middle");
    autoMode=0;
  } else if(incomingByte==51){
    ELECHOUSE_cc1101.SendData(transmitt_3, sizeof(transmitt_1), 100);
    Serial.println("Send wtw high");
    autoMode=0;
  } else  if(incomingByte==52){
    ELECHOUSE_cc1101.SendData(transmitt_timer, sizeof(transmitt_1), 100);
    Serial.println("Send wtw timed high");
    autoMode=0;
  } else if(incomingByte==100){ // key press d
    Serial.print("Mode:");
    Serial.println(autoMode);
  }
  unsigned long currentMillis = millis();
  if(currentMillis-lastTime > 60000 && autoMode==1){
     lastTime=currentMillis;
     ELECHOUSE_cc1101.SendData(transmitt_auto, sizeof(transmitt_auto), 100);
     Serial.println("Send auto command to wtw");
  }
//Serial.print(".");
}
