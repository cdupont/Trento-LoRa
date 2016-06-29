
Trento LoRa experiment
======================

The Trento LoRa experiment is a sensing network deployed in Trento.
It measures temperature in various points in Trento and is based on a LoRa network.
It uses a Raspberry PI for the Gateway and an Arduino for the sensor board.
The Arduino board is measuring the ambient temperature and broadcasting the readings through the LoRa network.
The Raspberry gets the readings and decode them using a small Node-RED dataflow program.
The Node-RED program then uploads the results on the RaptorBox Cloud instance through its API.

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

### Gateway

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

Install additional softwares:

```
sudo apt-get install vim tmux
```


### Sensor node

Install Arduino GUI on your PC from [here](https://www.arduino.cc/en/Main/Software).
Run Arduino GUI.
Import the LoRa library into Arduino GUI:

```
wget http://www.cooking-hacks.com/media/cooking/images/documentation/tutorial_SX1272/SX1272_library_arduino_v1.4.zip
unzip SX1272_library_arduino_v1.4.zip
```
Click on `Sketch/Include Library/import .ZIP library` and select the file arduinoLoRa_v1_4.zip.
Do the same with the file arduino-api_v1_4.zip.

Copy the code from this [repo](https://github.com/hdoukas/lora_openiot/tree/master/Arduino_code).

Select the correct port in the menu "Tools" and open the Serial Monitor.
Upload the code.
You should see the messages with the measurements being send in the serial monitor.

Running
-------

Start the receiver program from the LoRa examples:

```
ssh root@192.168.8.71
cd cooking/examples/LoRa
./cook.sh LoRa_gateway_RX.cpp
./LoRa_gateway_RX.cpp_exe
```

You should see the messages being received.
Now open your browser using the given IP:

```
firefox http://192.168.8.71/
```

This should display the Node-RED interface.
Click on Menu/Import/Clipboard and paste the network from this [file](./LoRaTrento).
Start the network using the button "Activate".
If the sensor board is running, you should see the network blinking.
The data is uploaded on the RaptorBox website.

Configuration
-------------

In order to restart when the gateway in rebooted, the sender program need to be started at boot-up:

In /etc/rc.local, add:
```
tmux new-session -d "/root/cooking/examples/LoRa/LoRa_gateway_RX.cpp_exe &"
```



Testing
-------

Once the gateway and sensor node running, the data should be uploaded to Raptor.
You can see the data by issuing:

```
curl -XGET -k  https://api.raptorbox.eu/6da85a54-c7ca-49e4-a309-964549b67476/streams/environmentaldata -H"Authorization: Bearer d28bcf86fa9623f8bf67257c432866469d8044f7"
```


Deployment
----------

- Raffaele's home
- Fabio's home
- Corentin's home
- Andrea's home
- Abdur's home 
- CNET offices (already there for meeting rooms)





