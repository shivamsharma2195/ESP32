/**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 */

#include "BLEDevice.h"
//#include "BLEScan.h"


// The remote service nordics Cyberium ble device.
static BLEUUID serviceUUID("6e400001-b5a3-f393-e0a9-e50e24dcca9e");
// The characteristic of the remote service.
static BLEUUID    charUUID("6e400003-b5a3-f393-e0a9-e50e24dcca9e");



static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
volatile boolean ble1_flag = false, ble2_flag = false;
static BLERemoteCharacteristic* pRemoteCharacteristic1;
static BLERemoteCharacteristic* pRemoteCharacteristic2;
static BLEAdvertisedDevice* myDevice;
static BLEScan* pBLEScan;

volatile byte connected_device = 0;






//////////////////////////////////////////////////////////////////////////////////////ble server 1

static void notifyCallback1(
  BLERemoteCharacteristic* pBLERemoteCharacteristic1,
  uint8_t* pData1,
  size_t length,
  bool isNotify) {
      Serial.print("ble 1 data");
//    Serial.print("Notify callback for characteristic ");
//    Serial.print(pBLERemoteCharacteristic1->getUUID().toString().c_str());
//    Serial.print(" of data length ");
//    Serial.println(length);
//    Serial.print("data: ");
//    Serial.println((char*)pData1);
}

class MyClientCallback1 : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient1) {
    connected_device++;
  }

  void onDisconnect(BLEClient* pclient1) {
    connected = false;
    connected_device--;
    Serial.println("onDisconnect");
  }
};

bool connectToServer1() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient1  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient1->setClientCallbacks(new MyClientCallback1());

    // Connect to the remove BLE Server.
    pClient1->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService1 = pClient1->getService(serviceUUID);
    if (pRemoteService1 == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient1->disconnect();
      
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic1 = pRemoteService1->getCharacteristic(charUUID);
    if (pRemoteCharacteristic1 == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient1->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic1->canRead()) {
      std::string value = pRemoteCharacteristic1->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }

    if(pRemoteCharacteristic1->canNotify())
      pRemoteCharacteristic1->registerForNotify(notifyCallback1);

    connected = true;
}





//////////////////////////////////////////////////////////////////////////////////////ble server 2

static void notifyCallback2(
  BLERemoteCharacteristic* pBLERemoteCharacteristic2,
  uint8_t* pData2,
  size_t length,
  bool isNotify) {
    Serial.print("ble 2 data");
//    Serial.print("Notify callback for characteristic ");
//    Serial.print(pBLERemoteCharacteristic2->getUUID().toString().c_str());
//    Serial.print(" of data length ");
//    Serial.println(length);
//    Serial.print("data: ");
//    Serial.println((char*)pData2);
}

class MyClientCallback2 : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient2) {
    connected_device++;
  }

  void onDisconnect(BLEClient* pclient2) {
    connected_device--;
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer2() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient2  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient2->setClientCallbacks(new MyClientCallback2());

    // Connect to the remove BLE Server.
    pClient2->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService2 = pClient2->getService(serviceUUID);
    if (pRemoteService2 == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient2->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic2 = pRemoteService2->getCharacteristic(charUUID);
    if (pRemoteCharacteristic2 == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient2->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic2->canRead()) {
      std::string value = pRemoteCharacteristic2->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }

    if(pRemoteCharacteristic2->canNotify())
      pRemoteCharacteristic2->registerForNotify(notifyCallback2);

    connected = true;
}




///////////////////////////////////////////////////////////// ble's common profiles for multiple server





//////////////////////////////////////////////////////////// ble client init and connection part

/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks


void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
} // End of setup.


// This is the Arduino main loop function.
void loop() {

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
  if (connected_device <2 ){
    pBLEScan->start(5, false);
  if (doConnect == true) {
    if (!ble1_flag) {
      connectToServer1();
      Serial.println("We are now connected to the BLE1 Server.");
    } else if (!ble2_flag){
      connectToServer2();
      Serial.println("We are now connected to the BLE2 Server.");
    }
    else
    {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }
  }
  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
    String newValue = "Time since boot: " + String(millis()/1000);
    Serial.println("Setting new characteristic value to \"" + newValue + "\"");
    
    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic1->writeValue(newValue.c_str(), newValue.length());
  }else if(doScan){
    BLEDevice::getScan()->start(0);  // this is just eample to start scan after disconnect, most likely there is better way to do it in arduino
  }
  
  delay(1000); // Delay a second between loops.
}
