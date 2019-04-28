# ESP8266 React

A simple, extensible framework for getting up and running with the ESP8266/ESP32 microchip and a react front end.

Designed to work with the PlatformIO IDE with limited setup. 

This project supports ESP8266 and ESP32 devices, see build instruction below for more details.

## Why I made this project

I found I was repeating a lot of work when starting new IoT projects with the ESP8266 chip.

Most of my IoT projects have required:

* Configurable WiFi
* Configurable access point
* Synchronization with NTP
* The ability to perform OTA updates

I also wanted to adopt a decent client side framework so the back end could be simplified to a set of REST endpoints.

All of the above features are included in this framework, which I plan to use as a basis for my IoT projects.

The interface is responsive and should work well on mobile devices. It also has the prerequisite manifest/icon file, so it can be added to the home screen if desired.

![Screenshots](/screenshots/screenshots.png?raw=true "Screenshots")

## Getting Started

### Prerequisites

You will need the following before you can get started.

* [PlatformIO](https://platformio.org/) - IDE for development
* [NPM](https://www.npmjs.com/) - For building the interface
* Bash shell, or Git Bash if you are under windows

### Installing in PlatformIO

Pull the project and add it to PlatformIO as a project folder (File > Add Project Folder).

PlatformIO should download the ESP8266 platform and the project library dependencies automatically.

Once the platform and libraries are downloaded the back end should be compiling.

### Building the interface

The interface has been configured with create-react-app and react-app-rewired so the build can customized for the target device. The large artefacts are gzipped and source maps and service worker are excluded from the production build.

You will find the interface code in the ./interface directory. Change to this directory with your bash shell (or Git Bash) and use the standard commands you would with any react app built with create-react-app:

#### Download and install the node modules

```bash
npm install
```

#### Build the interface

```bash
npm run build
```

**NB: The build command will also delete the previously built interface (the ./data/www directory) and replace it with the freshly built one, ready for upload to the device.**

#### Running the interface locally

```bash
npm start
```

**NB: To run the interface locally you will need to modify the endpoint root path and enable CORS.**

The endpoint root path can be found in .env.development, defined as the environment variable 'REACT_APP_ENDPOINT_ROOT'. This needs to be the root URL of the device running the back end, for example:

```
REACT_APP_ENDPOINT_ROOT=http://192.168.0.6/rest/
```

CORS can be enabled on the back end by uncommenting the -D ENABLE_CORS build flag in platformio.ini and re-deploying.

## Building for different devices

This project supports ESP8266 and ESP32 platforms however your target device will need at least a 1MB flash chip to support OTA programming.

By default this project is configured to build for the esp12e device. This is an esp8266 device with 4MB of flash. The following config in platformio.ini configures the build:

```
[env:esp12e]
platform = espressif8266
board = esp12e
```

If you want to build for an ESP32 device, all you need to do is re-configure playformio.ini with your devices settings:

```
[env:node32s]
platform = espressif32
board = node32s
```

Microcontroller	ESP8266
Frequency	80MHz
Flash	4MBl

**NB: If building under Windows you need to delete .piolibdeps/Time/Time.h - due [filesystem case insensitivity](https://github.com/me-no-dev/ESPAsyncWebServer/issues/96)*

## Configuration & Deployment

Standard configuration settings, such as build flags, libraries and device configuration can be found in platformio.ini. See the [PlatformIO docs](http://docs.platformio.org/en/latest/projectconf.html) for full details on what you can do with this.

By default, the target device is "esp12e". This is a common ESP8266 variant with 4mb of flash though any device with at least 2mb of flash should be fine. The settings configure the interface to upload via serial by default, you can change the upload mechanism to OTA by uncommenting the relevant lines.

As well as containing the interface, the SPIFFS image (in the ./data folder) contains a JSON settings file for each of the configurable features. The config files can be found in the ./data/config directory:

File | Description
---- | -----------
apSettings.json | Access point settings
ntpSettings.json | NTP synchronization settings
otaSettings.json | OTA Update configuration
wifiSettings.json | WiFi connection settings

The default settings configure the device to bring up an access point on start up which can be used to configure the device:

* SSID: ESP8266-React
* Password: esp-react

## Software Overview

### Back End

The back end is a set of REST endpoints hosted by a [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) instance. The source is split up by feature, for example [WiFiScanner.h](src/WiFiScanner.h) implements the end points for scanning for available networks.

There is an abstract class [SettingsService.h](src/SettingsService.h) that provides an easy means of adding configurable services/features to the device. It takes care of writing the settings as JSON to SPIFFS. All you need to do is extend the class with your required configuration and implement the functions which serialize the settings to/from JSON. JSON serialization utilizes the excellent [ArduinoJson](https://github.com/bblanchon/ArduinoJson) library. Here is a example of a service with username and password settings:

```cpp
#include <SettingsService.h>

class ExampleSettingsService : public SettingsService {

  public:

    ExampleSettingsService(AsyncWebServer* server, FS* fs)
    : SettingsService(server, fs, "/exampleSettings", "/config/exampleSettings.json") {}

    ~ExampleSettingsService(){}

  protected:

    void readFromJsonObject(JsonObject& root) {
      _username = root["username"] | "";
      _password = root["password"] | "";
    }

    void writeToJsonObject(JsonObject& root) {
      root["username"] = _username;
      root["password"] = _password;
    }

  private:

    String _username;
    String _password;

};
```

Now this can be constructed, added to the server, and started as such:

```cpp
ExampleSettingsService exampleSettingsService = ExampleSettingsService(&server, &SPIFFS);

exampleSettingsService.begin();
```

There will now be a REST service exposed on "/exampleSettings" for reading and writing (GET/POST) the settings. Any modifications will be persisted in SPIFFS, in this case to "/config/exampleSettings.json"

Sometimes you need to perform an action when the settings are updated, you can achieve this by overriding the onConfigUpdated() function which gets called every time the settings are updated. You can also perform an action when the service starts by overriding the begin() function, being sure to call SettingsService::begin():

```cpp

void begin() {
  // make sure we call super, so the settings get read!
  SettingsService::begin();  
  reconfigureTheService();
}

void onConfigUpdated() {
  reconfigureTheService();
}

void reconfigureTheService() {
  // do whatever is required to react to the new settings
}

```

### Front End

The front end is a bit of a work in progress (as are my react skills), but it has been designed to be a "mobile first" interface and as such should feel very much like an App.

I've tried to keep the use of libraries to a minimum to reduce the artefact size (it's about 150k gzipped ATM).

## Future Improvements

- [x] Reduce boilerplate in interface
- [ ] Provide an emergency config reset feature, via a pin held low for a few seconds
- [x] Access point should provide captive portal
- [ ] Perhaps have more configuration options for Access point: IP address, Subnet, etc
- [ ] Enable configurable mDNS
- [ ] Introduce authentication to secure the device

## Libraries Used

* [React](https://reactjs.org/)
* [Material-UI](https://material-ui-next.com/)
* [Time](https://github.com/PaulStoffregen/Time)
* [NtpClient](https://github.com/gmag11/NtpClient)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)

**NB: The project doesn't currently fix it's dependencies to a particular version. PlatformIO will always download the latest version of the specified dependencies.**
