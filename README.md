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

### Building and uploading the firmware

Pull the project and open it in PlatformIO. PlatformIO should download the ESP8266 platform and the project library dependencies automatically.

The project structure is as follows:

Resource | Description
---- | -----------
[data/](data) | The file system image directory
[interface/](interface) | React based front end
[src/](src) | The main.cpp and demo project to get you started
[platformio.ini](platformio.ini) | PlatformIO project configuration file
[lib/framework/](lib/framework) | C++ back end for the ESP8266 device

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

### Building & uploading the interface

The interface has been configured with create-react-app and react-app-rewired so the build can customized for the target device. The large artefacts are gzipped and source maps and service worker are excluded from the production build. This reduces the production build to around ~200k, which easily fits on the device.

The interface will be automatically built by PlatformIO before it builds the firmware. The project can be configured to serve the interface from either SPIFFS or PROGMEM as your project requires. The default configuration is to serve the content from SPIFFS which requires an additional upload step which is documented below.

#### Uploading the file system image

If service content from SPIFFS (default), build the project first. Then the compiled interface may be uploaded to the device by pressing the "Upload File System image" button:

![uploadfs](/media/uploadfs.png?raw=true "uploadfs")

Alternatively run the 'uploadfs' target:

```bash
platformio run -t uploadfs
```

#### Serving the interface from PROGMEM

You can configure the project to serve the interface from PROGMEM by uncommenting the -D PROGMEM_WWW build flag in ['platformio.ini'](platformio.ini) then re-building and uploading the firmware to the device. 

Be aware that this will consume ~150k of program space which can be especially problematic if you already have a large build artefact or if you have added large javascript dependencies to the interface. The ESP32 binaries are large already, so this will be a problem if you are using one of these devices and require this type of setup.

A method for working around this issue can be to reduce the amount of space allocated to SPIFFS by configuring the device to use a differnt strategy partitioning. If you don't require SPIFFS other than for storing config one approach might be to configure a minimal SPIFFS partition.

For a ESP32 (4mb variant) there is a handy "min_spiffs.csv" partition table which can be enabled easily:

```yaml
[env:node32s]
board_build.partitions = min_spiffs.csv
platform = espressif32
board = node32s
```

This is largley left as an exersise for the reader as everyone's requirements will vary.

### Running the interface locally

You can run a local development server to allow you preview changes to the front end without the need to upload a file system image to the device after each change. 

Change to the ['interface'](interface) directory with your bash shell (or Git Bash) and use the standard commands you would with any react app built with create-react-app:

```bash
cd interface
```

Install the npm dependencies, if required and start the development server:

```bash
npm install
npm start
```

> **Note**: To run the interface locally you may need to modify the endpoint root path and enable CORS.

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

The SPIFFS image (in the ['data'](data) folder) contains a JSON settings file for each of the configurable features. 

The config files can be found in the ['data/config'](data/config) directory:

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

The pre-configured environments are "esp12e" and "node32s". These are common ESP8266/ESP32 variants with 4mb of flash:

![ESP12E](/media/esp12e.jpg?raw=true "ESP12E") ![ESP32](/media/esp32.jpg?raw=true "ESP32")

The settings file ['platformio.ini'](platformio.ini) configures the supported environments. Modify these, or add new environments for the devides you need to support. The default environments are as follows:

```yaml
[env:esp12e]
platform = espressif8266
board = esp12e
board_build.f_cpu = 160000000L

[env:node32s]
platform = espressif32
board = node32s
```

If you want to build for a different device, all you need to do is re-configure ['platformio.ini'](platformio.ini) and select an alternative environment by modifying the default_envs variable. Building for the common esp32 "node32s" board for example:

```yaml
[platformio]
;default_envs = esp12e
default_envs = node32s
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

## Back end overview

The back end is a set of REST endpoints hosted by a [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) instance. The ['lib/framework'](lib/framework) directory contains the majority of the back end code. The framework contains of a number of useful utility classes which you can use when extending it. The project also comes with a demo project to give you some help getting started. 

The framework's source is split up by feature, for example [WiFiScanner.h](lib/framework/WiFiScanner.h) implements the end points for scanning for available networks where as [WiFiSettingsService.h](lib/framework/WiFiSettingsService.h) handles configuring the WiFi settings and managing the WiFi connection.

### Initializing the framework

The ['src/main.cpp'](src/main.cpp) file constructs the webserver and initializes the framework. You can add endpoints to the server here to support your IoT project. The main loop is also accessable so you can run your own code easily. 

The following code creates the web server, esp8266React framework and the demo project instance:

```cpp
AsyncWebServer server(80);
ESP8266React esp8266React(&server, &SPIFFS);
DemoProject demoProject = DemoProject(&server, &SPIFFS, esp8266React.getSecurityManager());
```

Now in the `setup()` function the initialization is performed:

```cpp
void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);

  // start the file system (must be done before starting the framework)
#ifdef ESP32
  SPIFFS.begin(true);
#elif defined(ESP8266)
  SPIFFS.begin();
#endif

  // start the framework and demo project
  esp8266React.begin();

  // start the demo project
  demoProject.begin();

  // start the server
  server.begin();
}
```

Finally the loop calls the framework's loop function to service the frameworks features. You can add your own code in here, as shown with the demo project:

```cpp
void loop() {
  // run the framework's loop function
  esp8266React.loop();

  // run the demo project's loop function
  demoProject.loop();
}
```

### Adding endpoints

There are some simple classes that support adding configurable services/features to the device:

Class | Description
----- | -----------
[SimpleService.h](lib/framework/SimpleService.h) | Exposes an endpoint to read and write settings as JSON. Extend this class and implement the functions which serialize the settings to/from JSON.
[SettingsService.h](lib/framework/SettingsService.h) | As above, however this class also handles persisting the settings as JSON to the file system.
[AdminSettingsService.h](lib/framework/AdminSettingsService.h) | Extends SettingsService to secure the endpoint to administrators only, the authentication predicate can be overridden if required.

The demo project shows how these can be used, explore the framework classes for more examples.

### Security features

The framework has security features to prevent unauthorized use of the device. This is driven by [SecurityManager.h](lib/framework/SecurityManager.h).

On successful authentication, the /rest/signIn endpoint issues a JWT which is then sent using Bearer Authentication. The framework come with built in predicates for verifying a users access level. The built in AuthenticationPredicates can be found in [SecurityManager.h](lib/framework/SecurityManager.h):

Predicate            | Description
-------------------- | -----------
NONE_REQUIRED        | No authentication is required.
IS_AUTHENTICATED     | Any authenticated principal is permitted.
IS_ADMIN             | The authenticated principal must be an admin.

You can use the security manager to wrap any web handler with an authentication predicate:

```cpp
server->on("/rest/someService", HTTP_GET, 
  _securityManager->wrapRequest(std::bind(&SomeService::someService, this, std::placeholders::_1), AuthenticationPredicates::IS_AUTHENTICATED)
);
```

Alternatively you can extend [AdminSettingsService.h](lib/framework/AdminSettingsService.h) and optionally override `getAuthenticationPredicate()` to secure an endpoint.

## Extending the framework

It is recommend that you explore the framework code to gain a better understanding of how to use it's features. The framework provides APIs so you can add your own services or features or, if required, directly configure or observe changes to core framework features. Some of these capabilities are detailed below.

### Adding a service with persistant settings

The following code demonstrates how you might extend the framework with a feature which requires a username and password to be configured to drive an unspecified feature. 

```cpp
#include <SettingsService.h>

class ExampleSettings {
 public:  
    String username;
    String password;
};

class ExampleSettingsService : public SettingsService<ExampleSettings> {

  public:

    ExampleSettingsService(AsyncWebServer* server, FS* fs)
    : SettingsService(server, fs, "/exampleSettings", "/config/exampleSettings.json") {}

    ~ExampleSettingsService(){}

  protected:

    void readFromJsonObject(JsonObject& root) {
      _settings.username = root["username"] | "";
      _settings.password = root["password"] | "";
    }

    void writeToJsonObject(JsonObject& root) {
      root["username"] = _settings.username;
      root["password"] = _settings.password;
    }

};
```

Now this can be constructed, added to the server, and started as such:

```cpp
ExampleSettingsService exampleSettingsService = ExampleSettingsService(&server, &SPIFFS);

exampleSettingsService.begin();
```

There will now be a REST service exposed on "/exampleSettings" for reading and writing (GET/POST) the settings. Any modifications will be persisted in SPIFFS, in this case to "/config/exampleSettings.json"

Sometimes you need to perform an action when the settings are updated, you can achieve this by overriding the onConfigUpdated() function which gets called every time the settings are updated. You can also perform an action when the service starts by overriding the begin() function, being sure to call SettingsService::begin(). You can also provide a "loop" function in order to allow your service class continuously perform an action, calling this from the main loop.

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

void loop() {
  // execute somthing as part of the main loop
}

```

### Accessing settings and services

The framework supplies access to it's SettingsService instances and the SecurityManager via getter functions:

SettingsService               | Description
---------------------------- | ----------------------------------------------
getSecurityManager()         | The security manager - detailed above
getSecuritySettingsService() | Configures the users and other security settings
getWiFiSettingsService()     | Configures and manages the WiFi network connection
getAPSettingsService()       | Configures and manages the Access Point
getNTPSettingsService()      | Configures and manages the network time
getOTASettingsService()      | Configures and manages the Over-The-Air update feature

These can be used to observe changes to settings. They can also be used to fetch or update settings directly via objects, JSON strings and JsonObjects. Here are some examples of how you may use this.

Inspect the current WiFi settings:

```cpp
WiFiSettings wifiSettings = esp8266React.getWiFiSettingsService()->fetch();
Serial.print("The ssid is:");
Serial.println(wifiSettings.ssid);
```

Configure the SSID and password:

```cpp
WiFiSettings wifiSettings = esp8266React->getWiFiSettingsService()->fetch();
wifiSettings.ssid = "MyNetworkSSID";
wifiSettings.password = "MySuperSecretPassword";
esp8266React.getWiFiSettingsService()->update(wifiSettings);
```

Observe changes to the WiFiSettings:

```cpp
esp8266React.getWiFiSettingsService()->addUpdateHandler([]() {
   Serial.println("The WiFi Settings were updated!");
});
```

## Libraries Used

* [React](https://reactjs.org/)
* [Material-UI](https://material-ui-next.com/)
* [notistack](https://github.com/iamhosseindhv/notistack)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
