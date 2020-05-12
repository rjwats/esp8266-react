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
* MQTT - Connection to an MQTT broker for automation and monitoring
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

A method for working around this issue can be to reduce the amount of space allocated to SPIFFS by configuring the device to use different partitioning. If you don't require SPIFFS other than for storing config one approach might be to configure a minimal SPIFFS partition.

For a ESP32 (4mb variant) there is a handy "min_spiffs.csv" partition table which can be enabled easily:

```yaml
[env:node32s]
board_build.partitions = min_spiffs.csv
platform = espressif32
board = node32s
```

This is left as an exercise for the reader as everyone's requirements will vary.

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

The interface has a development environment which is enabled when running the development server using `npm start`. The environment file can be found in ['interface/.env.development'](interface/.env.development) and contains the HTTP root URL and the WebSocket root URL:

```properties
REACT_APP_HTTP_ROOT=http://192.168.0.99
REACT_APP_WEB_SOCKET_ROOT=ws://192.168.0.99
```

The `REACT_APP_HTTP_ROOT` and `REACT_APP_WEB_SOCKET_ROOT` properties can be modified to point a ESP device running the back end firmware.

> **Note**: You must restart the development server for changes to the environment to become effective.

#### Enabling CORS

You can enable CORS on the back end by uncommenting the -D ENABLE_CORS build flag in ['platformio.ini'](platformio.ini) then re-building and uploading the firmware to the device. The default settings assume you will be accessing the development server on the default port on [http://localhost:3000](http://localhost:3000) this can also be changed if required:

```properties
-D ENABLE_CORS
-D CORS_ORIGIN=\"http://localhost:3000\"
```

## Device Configuration

The SPIFFS image (in the ['data'](data) folder) contains a JSON settings file for each of the configurable features. 

The config files can be found in the ['data/config'](data/config) directory:

File | Description
---- | -----------
[apSettings.json](data/config/apSettings.json)             | Access point settings
[mqttSettings.json](data/config/mqttSettings.json)         | MQTT connection settings
[ntpSettings.json](data/config/ntpSettings.json)           | NTP synchronization settings
[otaSettings.json](data/config/otaSettings.json)           | OTA update configuration
[securitySettings.json](data/config/securitySettings.json) | Security settings and user credentials
[wifiSettings.json](data/config/wifiSettings.json)         | WiFi connection settings

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

The app can be easily themed by editing the [MaterialUI theme](https://material-ui.com/customization/theming/). Edit the theme in ['interface/src/CustomMuiTheme.tsx'](interface/src/CustomMuiTheme.tsx) as you desire. For example, here is a dark theme:

```js
const theme = createMuiTheme({
  palette: {
    type:"dark",
    primary: {
      main: '#222',
    },
    secondary: {
      main: '#666',
    },
    info: {
      main: blueGrey[900]
    },
    warning: {
      main: orange[500]
    },
    error: {
      main: red[500]
    },
    success: {
      main: green[500]
    }
  }
});
```

![Dark Theme](/media/dark.png?raw=true "Dark Theme")

### Changing the app icon

You can replace the app icon is located at ['interface/public/app/icon.png'](interface/public/app/icon.png) with one of your preference. A 256 x 256 PNG is recommended for best compatibility.


### Changing the app name

The app name displayed on the login page and on the menu bar can be modified by editing the REACT_APP_NAME property in ['interface/.env'](interface/.env)

```properties
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

## Back end

The back end is a set of REST endpoints hosted by a [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) instance. The ['lib/framework'](lib/framework) directory contains the majority of the back end code. The framework contains of a number of useful utility classes which you can use when extending it. The project also comes with a demo project to give you some help getting started. 

The framework's source is split up by feature, for example [WiFiScanner.h](lib/framework/WiFiScanner.h) implements the end points for scanning for available networks where as [WiFiSettingsService.h](lib/framework/WiFiSettingsService.h) handles configuring the WiFi settings and managing the WiFi connection.

### Initializing the framework

The ['src/main.cpp'](src/main.cpp) file constructs the webserver and initializes the framework. You can add endpoints to the server here to support your IoT project. The main loop is also accessable so you can run your own code easily. 

The following code creates the web server and esp8266React framework:

```cpp
AsyncWebServer server(80);
ESP8266React esp8266React(&server, &SPIFFS);
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

  // start the server
  server.begin();
}
```

Finally the loop calls the framework's loop function to service the frameworks features.

```cpp
void loop() {
  // run the framework's loop function
  esp8266React.loop();
}
```

### Developing with the framework

The framework promotes a modular design and exposes features you may re-use to speed up the development of your project. Where possible it is recommended that you use the features the frameworks supplies. These are documented in this section and a comprehensive example is provided by the demo project.

The following diagram visualises how the framework's modular components fit together, each feature is described in detail below.

![framework diagram](/media/framework.png?raw=true "framework diagram")

#### Settings service

The [SettingsService.h](lib/framework/SettingsService.h) class is a responsible for managing settings and interfacing with code which wants to change or respond to changes in them. You can define a data class to hold settings then build a SettingsService instance to manage them:

```cpp
class LightSettings {
 public:
  bool on = false;
  uint8_t brightness = 255;
};

class LightSettingsService : public SettingsService<LightSettings> {
};
```

You may listen for changes to settings by registering an update handler callback. It is possible to remove an update handler later if required.

```cpp
// register an update handler
update_handler_id_t myUpdateHandler = lightSettingsService.addUpdateHandler(
  [&](String originId) {
    Serial.println("The light settings have been updated"); 
  }
);

// remove the update handler
lightSettingsService.removeUpdateHandler(myUpdateHandler);
```

An "originId" is passed to the update handler which may be used to identify the origin of the update. The default origin values the framework provides are:

Origin                | Description
--------------------- | -----------
http                  | An update sent over REST (HttpEndpoint)
mqtt                  | An update sent over MQTT (MqttPubSub)
websocket:{clientId}  | An update sent over WebSocket (WebSocketRxTx)

SettingsService exposes a read function which you may use to safely read the settings. This function takes care of protecting against parallel access to the settings in multi-core enviornments such as the ESP32.

```cpp
lightSettingsService.read([&](LightSettings& settings) {
  digitalWrite(LED_PIN, settings.on ? HIGH : LOW)
});
```

SettingsService also exposes an update function which allows the caller to update the settings with a callback. This approach automatically calls the registered update handlers when complete. The example below turns on the lights using the arbitrary origin "timer":

```cpp
lightSettingsService.update([&](LightSettings& settings) {
  settings.on = true;  // turn on the lights!
}, "timer");
```

#### Serialization

When transmitting settings over HTTP, WebSockets, or MQTT they must to be marshalled into a serializable form (JSON). The framework uses ArduinoJson for serialization and the functions defined in [JsonSerializer.h](lib/framework/JsonSerializer.h) and [JsonDeserializer.h](lib/framework/JsonDeserializer.h) facilitate this.

The static functions below can be used to facilitate the serialization/deserialization of the example settings:

```cpp
class LightSettings {
 public:
  bool on = false;
  uint8_t brightness = 255;
  
  static void serialize(LightSettings& settings, JsonObject& root) {
    root["on"] = settings.on;
    root["brightness"] = settings.brightness;
  }

  static void deserialize(JsonObject& root, LightSettings& settings) {
    settings.on = root["on"] | false;
    settings.brightness = root["brightness"] | 255;
  }
};
```

For convenience, the SettingsService class provides overloads of its `update` and `read` functions which utilize these functions.

Copy the settings to a JsonObject using a serializer:

```cpp
JsonObject jsonObject = jsonDocument.to<JsonObject>();
lightSettingsService->read(jsonObject, serializer);
```

Update the settings from a JsonObject using a deserializer:

```cpp
JsonObject jsonObject = jsonDocument.as<JsonObject>();
lightSettingsService->update(jsonObject, deserializer, "timer");
```

#### Endpoints

The framework provides a [HttpEndpoint.h](lib/framework/HttpEndpoint.h) class which may be used to register GET and POST handlers to read and update the settings over HTTP. You may construct a HttpEndpoint as a part of the SettingsService or separately if you prefer. 

The code below demonstrates how to extend the LightSettingsService class to provide an unsecured endpoint:

```cpp
class LightSettingsService : public SettingsService<LightSettings> {
 public:
  LightSettingsService(AsyncWebServer* server) :
      _httpEndpoint(LightSettings::serialize, LightSettings::deserialize, this, server, "/rest/lightSettings") {
  }

 private:
  HttpEndpoint<LightSettings> _httpEndpoint;
};
```

Endpoint security is provided by authentication predicates which are [documented below](#security-features). The SecurityManager and authentication predicate may be provided if a secure endpoint is required. The demo project shows how endpoints can be secured.

#### Persistence

[FSPersistence.h](lib/framework/FSPersistence.h) allows you to save settings to the filesystem. FSPersistence automatically writes changes to the file system when settings are updated. This feature can be disabled by calling `disableUpdateHandler()` if manual control of persistence is required.

The code below demonstrates how to extend the LightSettingsService class to provide persistence:

```cpp
class LightSettingsService : public SettingsService<LightSettings> {
 public:
  LightSettingsService(FS* fs) :
      _fsPersistence(LightSettings::serialize, LightSettings::deserialize, this, fs, "/config/lightSettings.json") {
  }

 private:
  FSPersistence<LightSettings> _fsPersistence;
};
```

#### WebSockets

[SettingsSocket.h](lib/framework/SettingsSocket.h) allows you to read and update settings over a WebSocket connection. SettingsSocket automatically pushes changes to all connected clients when settings are updated.

The code below demonstrates how to extend the LightSettingsService class to provide an unsecured websocket:

```cpp
class LightSettingsService : public SettingsService<LightSettings> {
 public:
  LightSettingsService(AsyncWebServer* server) :
      _settingsSocket(LightSettings::serialize, LightSettings::deserialize, this, server, "/ws/lightSettings"), {
  }

 private:
  SettingsSocket<LightSettings> _settingsSocket;
};
```

WebSocket security is provided by authentication predicates which are [documented below](#security-features). The SecurityManager and authentication predicate may be provided if a secure WebSocket is required. The demo project shows how WebSockets can be secured.

#### MQTT

The framework includes an MQTT client which can be configured via the UI. MQTT requirements will differ from project to project so the framework exposes the client for you to use as you see fit. The framework does however provide a utility to interface SettingsService to a pair of pub/sub (state/set) topics. This utility can be used to synchronize state with software such as Home Assistant.

[SettingsBroker.h](lib/framework/SettingsBroker.h) allows you to read and update settings over a pair of MQTT topics. SettingsBroker automatically pushes changes to the pub topic and reads updates from the sub topic.

The code below demonstrates how to extend the LightSettingsService class to interface with MQTT:

```cpp
class LightSettingsService : public SettingsService<LightSettings> {
 public:
  LightSettingsService(AsyncMqttClient* mqttClient) :
    _settingsBroker(LightSettings::serialize, 
                    LightSettings::deserialize,
                    this,
                    mqttClient,
                    "homeassistant/light/my_light/set",
                    "homeassistant/light/my_light/state") {
  }

 private:
  SettingsBroker<LightSettings> _settingsBroker;
};
```

You can also re-configure the pub/sub topics at runtime as required:

```cpp
_settingsBroker.configureBroker("homeassistant/light/desk_lamp/set", "homeassistant/light/desk_lamp/state");
```

The demo project allows the user to modify the MQTT topics via the UI so they can be changed without re-flashing the firmware.

### Security features

The framework has security features to prevent unauthorized use of the device. This is driven by [SecurityManager.h](lib/framework/SecurityManager.h).

On successful authentication, the /rest/signIn endpoint issues a [JSON Web Token (JWT)](https://jwt.io/) which is then sent using Bearer Authentication. The framework come with built-in predicates for verifying a users access privileges. The built in AuthenticationPredicates can be found in [SecurityManager.h](lib/framework/SecurityManager.h) and are as follows:

Predicate            | Description
-------------------- | -----------
NONE_REQUIRED        | No authentication is required.
IS_AUTHENTICATED     | Any authenticated principal is permitted.
IS_ADMIN             | The authenticated principal must be an admin.

You can use the security manager to wrap any request handler function with an authentication predicate:

```cpp
server->on("/rest/someService", HTTP_GET, 
  _securityManager->wrapRequest(std::bind(&SomeService::someService, this, std::placeholders::_1), AuthenticationPredicates::IS_AUTHENTICATED)
);
```

### Accessing settings and services

The framework supplies access to various features via getter functions:

SettingsService               | Description
---------------------------- | ----------------------------------------------
getSecurityManager()         | The security manager - detailed above
getSecuritySettingsService() | Configures the users and other security settings
getWiFiSettingsService()     | Configures and manages the WiFi network connection
getAPSettingsService()       | Configures and manages the Access Point
getNTPSettingsService()      | Configures and manages the network time
getOTASettingsService()      | Configures and manages the Over-The-Air update feature
getMqttSettingsService()     | Configures and manages the MQTT connection
getMQTTClient()              | Provides direct access to the MQTT client instance

These can be used to observe changes to settings. They can also be used to fetch or update settings.

Inspect the current WiFi settings:

```cpp
esp8266React.getWiFiSettingsService()->read([&](WiFiSettings& wifiSettings) {
  Serial.print("The ssid is:");
  Serial.println(wifiSettings.ssid);
});
```

Configure the WiFi SSID and password manually:

```cpp
esp8266React.getWiFiSettingsService()->update([&](WiFiSettings& wifiSettings) {
  wifiSettings.ssid = "MyNetworkSSID";
  wifiSettings.password = "MySuperSecretPassword";
}, "myapp");
```

Observe changes to the WiFiSettings:

```cpp
esp8266React.getWiFiSettingsService()->addUpdateHandler(
  [&](String originId) {
    Serial.println("The WiFi Settings were updated!");
  }
);
```

## Libraries Used

* [React](https://reactjs.org/)
* [Material-UI](https://material-ui-next.com/)
* [notistack](https://github.com/iamhosseindhv/notistack)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
* [AsyncMqttClient](https://github.com/marvinroger/async-mqtt-client)
