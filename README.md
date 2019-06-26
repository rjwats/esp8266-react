# ESP8266 React

[![Build Status](https://travis-ci.org/rjwats/esp8266-react.svg?branch=master)](https://travis-ci.org/rjwats/esp8266-react)

A simple, secure and extensible framework for IoT projects built on ESP8266/ESP32 platforms with responsive React front-end.

Designed to work with the PlatformIO IDE with [limited setup](#getting-started). Please read below for setup, build and upload instructions.

![Screenshots](/media/screenshots.png?raw=true "Screenshots")

## Features

Provides many of the features required for IoT projects:

* Configurable WiFi - Network scanner and WiFi configuration screen
* Configurable Access Point - Can be continuous or automatically enabled when WiFi connection fails
* Network Time - Synchronization with NTP
* Remote Firmware Updates - Enable secured OTA updates
* Security - Protected RESTful endpoints and a secured user interface

The back end is provided by a set of RESTful endpoints and the React based front end is responsive and scales well to various screen sizes.

The front end has the prerequisite manifest file and icon, so it can be added to the home screen of a mobile device if required.

## Getting Started

### Prerequisites

You will need the following before you can get started.

* [PlatformIO](https://platformio.org/) - IDE for development
* [Node.js](https://nodejs.org) - For building the interface with npm
* Bash shell, or [Git Bash](https://gitforwindows.org/) if you are under windows

### Building and uploading the firmware

Pull the project and open it in PlatformIO. PlatformIO should download the ESP8266 platform and the project library dependencies automatically.

The project structure is as follows:

Resource | Description
---- | -----------
[data/](data) | The file system image directory
[interface/](interface) | React based front end
[src/](src) | C++ back end for the ESP8266 device
[platformio.ini](platformio.ini) | PlatformIO project configuration file

### Building the firmware

Once the platform and libraries are downloaded the back end should successfully build within PlatformIO. 

The firmware may be built by pressing the "Build" button:

![build](/media/build.png?raw=true "build")

Alternatively type the run command:

```bash
platformio run
```

#### Uploading the firmware

The project is configured to upload over a serial connection by default. You can change this to use OTA updates by uncommenting the relevant lines in ['platformio.ini'](platformio.ini). 

The firmware may be uploaded to the device by pressing the "Upload" button:

![uploadfw](/media/uploadfw.png?raw=true "uploadfw")

Alternatively run the 'upload' target:

```bash
platformio run -t upload
```

### Building the interface

The interface has been configured with create-react-app and react-app-rewired so the build can customized for the target device. The large artefacts are gzipped and source maps and service worker are excluded from the production build. This reduces the production build to around ~200k, which easily fits on the device.

Change to the ['interface'](interface) directory with your bash shell (or Git Bash) and use the standard commands you would with any react app built with create-react-app:

#### Change to interface directory

```bash
cd interface
```

#### Download and install the node modules

```bash
npm install
```

#### Build the interface

```bash
npm run build
```

> **Note**: The build command will also delete the previously built interface, in the ['data/www'](data/www) directory, replacing it with the freshly built one ready to upload to the device.

#### Uploading the file system image

The compiled user interface may be uploaded to the device by pressing the "Upload File System image" button:

![uploadfs](/media/uploadfs.png?raw=true "uploadfs")

Alternatively run the 'uploadfs' target:

```bash
platformio run -t uploadfs
```

### Running the interface locally

You can run a local development server to allow you preview changes to the front end without the need to upload a file system image to the device after each change. Change to the interface directory and run the following command:

```bash
npm start
```

> **Note**: To run the interface locally you will need to modify the endpoint root path and enable CORS.

#### Changing the endpoint root

The endpoint root path can be found in ['interface/.env.development'](interface/.env.development), defined as the environment variable 'REACT_APP_ENDPOINT_ROOT'. This needs to be the root URL of the device running the back end, for example:

```js
REACT_APP_ENDPOINT_ROOT=http://192.168.0.6/rest/
```

#### Enabling CORS

You can enable CORS on the back end by uncommenting the -D ENABLE_CORS build flag in ['platformio.ini'](platformio.ini) then re-building and uploading the firmware to the device. The default settings assume you will be accessing the development server on the default port on [http://localhost:3000](http://localhost:3000) this can also be changed if required:

```
-D ENABLE_CORS
-D CORS_ORIGIN=\"http://localhost:3000\"
```

## Device Configuration

As well as containing the interface, the SPIFFS image (in the ['data'](data) folder) contains a JSON settings file for each of the configurable features. The config files can be found in the ['data/config'](data/config) directory:

File | Description
---- | -----------
[apSettings.json](data/config/apSettings.json) | Access point settings
[ntpSettings.json](data/config/ntpSettings.json) | NTP synchronization settings
[otaSettings.json](data/config/otaSettings.json) | OTA update configuration
[securitySettings.json](data/config/securitySettings.json) | Security settings and user credentials
[wifiSettings.json](data/config/wifiSettings.json) | WiFi connection settings

### Access point settings

The default settings configure the device to bring up an access point on start up which can be used to configure the device:

* SSID: ESP8266-React
* Password: esp-react

### Security settings and user credentials

The security settings and user credentials provide the following users by default:

Username | Password
-------- | --------
admin    | admin
guest    | guest

It is recommended that you change the JWT secret and user credentials from their defaults protect your device. You can do this in the user interface, or by modifying [securitySettings.json](data/config/securitySettings.json) before uploading the file system image. 

## Building for different devices

This project supports ESP8266 and ESP32 platforms. To support OTA programming, enough free space to upload the new sketch and file system image will be required. It is recommended that a board with at least 2mb of flash is used.

By default, the target device is "esp12e". This is a common ESP8266 variant with 4mb of flash:

![ESP12E](/media/esp12e.jpg?raw=true "ESP12E")

The settings file ['platformio.ini'](platformio.ini) configures the platform and board:

```
[env:esp12e]
platform = espressif8266
board = esp12e
```

If you want to build for an ESP32 device, all you need to do is re-configure ['platformio.ini'](platformio.ini) with your devices settings. 

![ESP32](/media/esp32.jpg?raw=true "ESP32")

Building for the common esp32 "node32s" board for example requires the following configuration:

```
[env:node32s]
platform = espressif32
board = node32s
```

## Customizing and theming

The framework, and MaterialUI allows for a reasonable degree of customization with little effort.

### Theming the app

The app can be easily themed by editing the [MaterialUI theme](https://material-ui.com/customization/themes/). Edit the theme in ['interface/src/App.js'](interface/src/App.js) as you desire:

```js
const theme = createMuiTheme({
  palette: {
    primary: red,
    secondary: deepOrange,
    highlight_idle: blueGrey[900],
    highlight_warn: orange[500],
    highlight_error: red[500],
    highlight_success: green[500],
  },
});
```

### Changing the app icon

You can replace the app icon is located at ['interface/public/app/icon.png'](interface/public/app/icon.png) with one of your preference. A 256 x 256 PNG is recommended for best compatibility.


### Changing the app name

The app name displayed on the login page and on the menu bar can be modified by editing the REACT_APP_NAME property in ['interface/.env'](interface/.env)

```js
REACT_APP_NAME=Funky IoT Project
```

There is also a manifest file which contains the app name to use when adding the app to a mobile device, so you may wish to also edit ['interface/public/app/manifest.json'](interface/public/app/manifest.json):

```json
{
  "name":"Funky IoT Project",
  "icons":[
    {
      "src":"/app/icon.png",
      "sizes":"48x48 72x72 96x96 128x128 256x256"
    }
  ],
  "start_url":"/",
  "display":"fullscreen",
  "orientation":"any"
}
```

## Back End Overview

The back end is a set of REST endpoints hosted by a [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) instance. The source is split up by feature, for example [WiFiScanner.h](src/WiFiScanner.h) implements the end points for scanning for available networks.

There is an abstract class [SettingsService.h](src/SettingsService.h) that provides an easy means of adding configurable services/features to the device. It takes care of writing the settings as JSON to SPIFFS. All you need to do is extend the class with your required configuration and implement the functions which serialize the settings to/from JSON. JSON serialization utilizes the excellent [ArduinoJson](https://github.com/bblanchon/ArduinoJson) library. 

Here is a example of a service with username and password settings:

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

## Libraries Used

* [React](https://reactjs.org/)
* [Material-UI](https://material-ui-next.com/)
* [Time](https://github.com/PaulStoffregen/Time)
* [NtpClient](https://github.com/gmag11/NtpClient)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
