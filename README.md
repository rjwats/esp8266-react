# ESP8266 React

A simple, extensible framework for IoT projects built on ESP8266/ESP32 platforms with responsive React front-end.

Designed to work with the PlatformIO IDE with limited setup, please read below for build and flash instructions.

![Screenshots](/media/screenshots.png?raw=true "Screenshots")

## Features

Provides many of the features required for IoT projects:

* Configurable WiFi - Network scanner and WiFi connection screen
* Configurable Access Point - Can be continuous or automatically enabled when WiFi connection fails
* Network Time - Synchronization with NTP
* Remote Firmware Updates - Enable secured OTA updates
* Security - Protected RESTful endpoints and a secured user interface

The back end is provided by a set of RESTful endpoints and the React based front end is responsive and scales well to various screen sizes.

The front end has the prerequisite manifest/icon file, so it can be added to the home screen of a mobile device if required.

## Getting Started

### Prerequisites

You will need the following before you can get started.

* [PlatformIO](https://platformio.org/) - IDE for development
* [Node.js](https://nodejs.org) - For building the interface with npm
* Bash shell, or Git Bash if you are under windows

### Building in PlatformIO

Pull the project and open it in PlatformIO. PlatformIO should download the ESP8266 platform and the project library dependencies automatically.

The project directory structure is as follows:

Directory | Description
---- | -----------
/data/ | The file system image directory
/interface/ | React based front end
/src/ | C++ back end for the ESP8266 device
/platformio.ini | PlatformIO project configuration file

Once the platform and libraries are downloaded the back end should be compiling.

> **WINDOWS BUILDS**: If building under Windows you need to delete .piolibdeps/Time/Time.h - due to a [file system case insensitivity issue](https://github.com/me-no-dev/ESPAsyncWebServer/issues/96)

### Building the interface

The interface has been configured with create-react-app and react-app-rewired so the build can customized for the target device. The large artefacts are gzipped and source maps and service worker are excluded from the production build.

Change to this directory with your bash shell (or Git Bash) and use the standard commands you would with any react app built with create-react-app:

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

> **Note**: The build command will also delete the previously built interface (the ./data/www directory) and replace it with the freshly built one, ready for upload to the device.

#### Running the interface locally

You can run a local development server during development to preview changes to the front end them without uploading a file system image to the device.

```bash
npm start
```

> **Note**: To run the interface locally you will need to modify the endpoint root path and enable CORS.

The endpoint root path can be found in .env.development, defined as the environment variable 'REACT_APP_ENDPOINT_ROOT'. This needs to be the root URL of the device running the back end, for example:

```js
REACT_APP_ENDPOINT_ROOT=http://192.168.0.6/rest/
```

CORS can be enabled on the back end by uncommenting the -D ENABLE_CORS build flag in platformio.ini and re-deploying.

## Customizing and theming

The framework, and MaterialUI allows for a good degree of custoimzation with little effort.

### Theming the app

The app can be easily themed by editing the [MaterialUI theme](https://material-ui.com/customization/themes/). Edit the theme in ./interface/src/App.js as you desire:

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

You can replace the app icon is located at [./interface/public/app/icon.png](interface/public/app/icon.png) with one of your preference. A 256 x 256 PNG is recommended for best compatibility.


### Changing the app name

The app name displayed on the login page and on the menu bar can be modified by editing the REACT_APP_NAME property in [./interface/.env](interface/.env)

```js
REACT_APP_NAME=Funky IoT Project
```

There is also a manifest file which contains the app name to use when adding the app to a mobile device, so you may wish to also edit [./interface/public/app/manifest.json](interface/public/app/manifest.json):

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

## Building for different devices

This project supports ESP8266 and ESP32 platforms. To support OTA programming, enough free space to upload the new sketch and file system image will be required. It is recommended that a board with at least 2mb of flash is used.

By default, the target device is "esp12e". This is a common ESP8266 variant with 4mb of flash:

![ESP12E](/media/esp12e.jpg?raw=true "ESP12E")

The settings file platformio.ini configures the platform and board:

```
[env:esp12e]
platform = espressif8266
board = esp12e
```

If you want to build for an ESP32 device, all you need to do is re-configure playformio.ini with your devices settings. 

![ESP32](/media/esp32.jpg?raw=true "ESP32")

Building for the common esp32 "node32s" board for example requires the following configuration:

```
[env:node32s]
platform = espressif32
board = node32s
```

## Configuration & Deployment

Standard configuration settings, such as build flags, libraries and device configuration can be found in platformio.ini. See the [PlatformIO docs](http://docs.platformio.org/en/latest/projectconf.html) for full details on what you can do with this.

***** TODO ******
The project is configured to upload via serial by default, you can change the upload mechanism to OTA by uncommenting the relevant lines.

As well as containing the interface, the SPIFFS image (in the ./data folder) contains a JSON settings file for each of the configurable features. The config files can be found in the ./data/config directory:

File | Description
---- | -----------
apSettings.json | Access point settings
ntpSettings.json | NTP synchronization settings
otaSettings.json | OTA update configuration
securitySettings.json | Security settings and user credentials
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
- [x] Introduce authentication to secure the device

## Libraries Used

* [React](https://reactjs.org/)
* [Material-UI](https://material-ui-next.com/)
* [Time](https://github.com/PaulStoffregen/Time)
* [NtpClient](https://github.com/gmag11/NtpClient)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
