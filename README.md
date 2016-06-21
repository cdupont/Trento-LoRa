
Trento LoRa experiment
======================

The LoRa experiment is a sensing network deployed in Trento.
It measures temperature in various points in Trento and is based on a LoRa network.
It is based on a Raspberry PI for the Gateway and an Arduino for the sensor board.
The Arduino board is measuring the ambient temperature and broadcasting the readings through the LoRa network.
The Raspberry gets the readings and decode them using a small Node-RED dataflow program.
The Node-RED program then uploads the results on the RaptorBox Cloud instance through it API.

Hardware
--------

Gateway:
- 1 Raspberry 2
- 1 Rasberry shield
- 1 LoRa module
- 1 flash card
- 1 box

Sensor board:
- 1 Arduino
- 1 sensor shield
- 1 extension shield
- 1 LoRa module
- 1 temperature sensor
- 1 box


Software installation
---------------------

Gateway:

The gateway flash drive should be flashed with the "ThingBox" image.
Download it at this [link](http://thethingbox.io/index.html#packagestable).
Unzip it.
Flash the downloaded image (.img) with [unetbootin](https://unetbootin.github.io/).

You are now ready to boot your gateway!
Plug the Gateway in CN network with cable.
To find out its IP, issue:

```
nmap -sP 192.168.8.0/24
```
Repeat with and without the cable plugged. The difference should show you its IP.
SSH inside the raspberry (password: raspberry):

```
ssh root@192.168.8.71
```

Once inside the raspberry, install the LoRa library using this [method](https://www.cooking-hacks.com/documentation/tutorials/extreme-range-lora-sx1272-module-shield-arduino-raspberry-pi-intel-galileo/#step4_2).

```
wget http://www.cooking-hacks.com/media/cooking/images/documentation/raspberry_arduino_shield/raspberrypi2.zip && unzip raspberrypi2.zip && cd cooking/arduPi && chmod +x install_arduPi && ./install_arduPi && rm install_arduPi && cd ../..
wget http://www.cooking-hacks.com/media/cooking/images/documentation/tutorial_SX1272/arduPi-api_LoRa_v1_4.zip && unzip -u arduPi-api_LoRa_v1_4.zip && cd cooking/examples/LoRa && chmod +x cook.sh && cd ../../..  
```

Start the receiver program from the LoRa examples:

```
cd cooking/examples/LoRa
./cook.sh SX_00_CONFIG_LoRa.cpp
./SX_00_CONFIG_LoRa.cpp_exe
```

Now open your browser using the given IP:

```
firefox http://192.168.8.71/
```
This should display the Node-RED interface.
Click on Menu/Import/Clipboard and paste the network from this [file](./LoRaTrento).




Deployment
----------

- Gateway to Raff + WiFi dongle
- Gateway to Fabio + WiFi dongle
- Gateway to Corentin + WiFi dongle
- Gateway to Andrea + WiFi dongle
- Gateway in CNET (already there for meeting rooms)





