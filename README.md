# ESP8266 React

[![Build Status](https://travis-ci.org/rjwats/esp8266-react.svg?branch=master)](https://travis-ci.org/rjwats/esp8266-react)

A simple, secure and extensible framework for IoT projects built on ESP8266/ESP32 platforms with responsive [React](https://reactjs.org/) front-end built with [Material-UI](https://mui.com/).

Designed to work with the PlatformIO IDE with [limited setup](#getting-started). Please read below for setup, build and upload instructions.

![Screenshots](/media/screenshots.png?raw=true "Screenshots")

## Features

Provides many of the features required for IoT projects:

* Configurable WiFi - Network scanner and WiFi configuration screen
* Configurable Access Point - Can be continuous or automatically enabled when WiFi connection fails
* Network Time - Synchronization with NTP
* MQTT - Connection to an MQTT broker for automation and monitoring
* Remote Firmware Updates - Firmware replacement using OTA update or upload via UI
* Security - Protected RESTful endpoints and a secured user interface

Features may be [enabled or disabled](#selecting-features) as required at compile time.

## Getting Started

### Prerequisites

You will need the following before you can get started.

* [PlatformIO](https://platformio.org/) - IDE for development
* [Node.js](https://nodejs.org) - For building the interface with npm

### Building and uploading the firmware

Pull the project and open it in PlatformIO. PlatformIO should download the ESP8266 platform and the project library dependencies automatically.

The project structure is as follows:

Resource                         | Description
-------------------------------- | ----------------------------------------------------------------------
[interface/](interface)          | React based front end
[lib/framework/](lib/framework)  | C++ back end for the ESP8266/ESP32 device
[src/](src)                      | The main.cpp and demo project to get you started
[scripts/](scripts)              | Scripts that build the React interface as part of the platformio build
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

### Building & uploading the interface

The interface has been configured with create-react-app and react-app-rewired so the build can customized for the target device. The large artefacts are gzipped and source maps and service worker are excluded from the production build. This reduces the production build to around ~150k, which easily fits on the device.

The interface will be automatically built by PlatformIO before it builds the firmware. The project can be configured to serve the interface from either PROGMEM or the filesystem as your project requires. The default configuration is to serve the content from PROGMEM, serving from the filesystem requires an additional upload step which is [documented below](#serving-the-interface-from-the-filesystem).

#### Serving the interface from PROGMEM

By default, the project is configured to serve the interface from PROGMEM. 

> **Tip**: You do not need to upload a file system image unless you configure the framework to [serve the interface from the filesystem](#serving-the-interface-from-the-filesystem).

The interface will consume ~150k of program space which can be problematic if you already have a large binary artefact or if you have added large dependencies to the interface. The ESP32 binaries are fairly large in there simplest form so the addition of the interface resources requires us to use special partitioning for the ESP32.

When building using the "node32s" profile, the project uses the custom [min_spiffs.csv](https://github.com/espressif/arduino-esp32/blob/master/tools/partitions/min_spiffs.csv) partitioning mode. You may want to disable this if you are manually uploading the file system image:


```ini
[env:node32s]
board_build.partitions = min_spiffs.csv
platform = espressif32
board = node32s
```

#### Serving the interface from the filesystem

If you choose to serve the interface from the filesystem you will need to change the default configuration and upload the file system image manually. 

Disable `-D PROGMEM_WWW build` flag in ['platformio.ini'](platformio.ini) and re-build the firmware. The build process will now copy the compiled interface to the `data/` directory and it may be uploaded to the device by pressing the "Upload File System image" button:

![uploadfs](/media/uploadfs.png?raw=true "uploadfs")

Alternatively run the 'uploadfs' target:

```bash
platformio run -t uploadfs
```

### Developing the interface locally

UI development is an iterative process so it's best to run a development server locally during interface development (using `npm start`). This can be accomplished by deploying the backend to a device and configuring the interface to point to it:

![Development Server](/media/devserver.png?raw=true "Development Server")

The following steps can get you up and running for local interface development:

- Deploy firmware to device
- [Configuring the dev proxy](#configuring-the-dev-proxy) with device's IP in interface/package.json
- [Start the development server](#starting-the-development-server) with "npm start"
- Develop interface locally

#### Configuring the dev proxy

The interface has a development environment which is enabled when running the development server using `npm start`. The [package.json](interface/package.json) file defines the location of the services which the development server will proxy. This is defined by the "proxy" propery, which will need to be change to the the IP address or hostname of the device running the firmware.

```json
"proxy": "http://192.168.0.77"
```

> **Tip**: You must restart the development server for changes to the proxy location to come into effect. Note that the proxy is configured to handle http and WebSocket connections from this location, see [setupProxy.js](interface/src/setupProxy.js) for details.

#### Starting the development server

Change to the [interface](interface) directory with your bash shell (or Git Bash) and use the standard commands you would with any react app built with create-react-app:

```bash
cd interface
```

Install the npm dependencies, if required and start the development server:

```bash
npm install
npm start
```
> **Tip**: You can (optionally) speed up the build by commenting out the call to build_interface.py under "extra scripts" during local development. This will prevent the npm process from building the production release every time the firmware is compiled significantly decreasing the build time.

## Selecting features

Many of the framework's built in features may be enabled or disabled as required at compile time. This can help save sketch space and memory if your project does not require the full suite of features. The access point and WiFi management features are "core features" and are always enabled. Feature selection may be controlled with the build flags defined in [features.ini](features.ini).

Customize the settings as you see fit. A value of 0 will disable the specified feature:

```ini
  -D FT_PROJECT=1
  -D FT_SECURITY=1
  -D FT_MQTT=1
  -D FT_NTP=1
  -D FT_OTA=1
  -D FT_UPLOAD_FIRMWARE=1
```

Flag               | Description
------------------ | ----------------------------------------------
FT_PROJECT         | Controls whether the "project" section of the UI is enabled. Disable this if you don't intend to have your own screens in the UI.
FT_SECURITY        | Controls whether the [security features](#security-features) are enabled. Disabling this means you won't need to authenticate to access the device and all authentication predicates will be bypassed.
FT_MQTT            | Controls whether the MQTT features are enabled. Disable this if your project does not require MQTT support.
FT_NTP             | Controls whether network time protocol synchronization features are enabled. Disable this if your project does not require accurate time.
FT_OTA             | Controls whether OTA update support is enabled. Disable this if you won't be using the remote update feature.
FT_UPLOAD_FIRMWARE | Controls the whether the manual upload firmware feature is enabled. Disable this if you won't be manually uploading firmware.

## Factory settings

The framework has built-in factory settings which act as default values for the various configurable services where settings are not saved on the file system. These settings can be overridden using the build flags defined in [factory_settings.ini](factory_settings.ini).

Customize the settings as you see fit, for example you might configure your home WiFi network as the factory default:

```ini
  -D FACTORY_WIFI_SSID=\"My Awesome WiFi Network\"
  -D FACTORY_WIFI_PASSWORD=\"secret\"
  -D FACTORY_WIFI_HOSTNAME=\"awesome_light_controller\"
```

### Default access point settings

By default, the factory settings configure the device to bring up an access point on start up which can be used to configure the device:

* SSID: ESP8266-React
* Password: esp-react

### Security settings and user credentials

By default, the factory settings configure two user accounts with the following credentials: 

Username | Password
-------- | --------
admin    | admin
guest    | guest

It is recommended that you change the user credentials from their defaults better protect your device. You can do this in the user interface, or by modifying [factory_settings.ini](factory_settings.ini) as mentioned above.

### Customizing the factory time zone setting

Changing factory time zone setting is a common requirement. This requires a little effort because the time zone name and POSIX format are stored as separate values for the moment. The time zone names and POSIX formats are contained in the UI code in [TZ.tsx](interface/src/ntp/TZ.tsx). Take the appropriate pair of values from there, for example, for Los Angeles you would use:

```ini
  -D FACTORY_NTP_TIME_ZONE_LABEL=\"America/Los_Angeles\"
  -D FACTORY_NTP_TIME_ZONE_FORMAT=\"PST8PDT,M3.2.0,M11.1.0\"
```

### Placeholder substitution

Various settings support placeholder substitution, indicated by comments in [factory_settings.ini](factory_settings.ini). This can be particularly useful where settings need to be unique, such as the Access Point SSID or MQTT client id. The following placeholders are supported:

Placeholder  | Substituted value 
-----------  | -----------------
#{platform}  | The microcontroller platform, e.g. "esp32" or "esp8266"
#{unique_id} | A unique identifier derived from the MAC address, e.g. "0b0a859d6816"
#{random}    | A random number encoded as a hex string, e.g. "55722f94"

You may use SettingValue::format in your own code if you require the use of these placeholders. This is demonstrated in the demo project:

```cpp
  static StateUpdateResult update(JsonObject& root, LightMqttSettings& settings) {
    settings.mqttPath = root["mqtt_path"] | SettingValue::format("homeassistant/light/#{unique_id}");
    settings.name = root["name"] | SettingValue::format("light-#{unique_id}");
    settings.uniqueId = root["unique_id"] | SettingValue::format("light-#{unique_id}");
    return StateUpdateResult::CHANGED;
  }
};
```

## Building for different devices

This project supports ESP8266 and ESP32 platforms. To support OTA programming, enough free space to upload the new sketch and file system image will be required. It is recommended that a board with at least 2mb of flash is used.

The pre-configured environments are "esp12e" and "node32s". These are common ESP8266/ESP32 variants with 4mb of flash:

![ESP12E](/media/esp12e.jpg?raw=true "ESP12E") ![ESP32](/media/esp32.jpg?raw=true "ESP32")

The settings file ['platformio.ini'](platformio.ini) configures the supported environments. Modify these, or add new environments for the devides you need to support. The default environments are as follows:

```ini
[env:esp12e]
platform = espressif8266
board = esp12e
board_build.f_cpu = 160000000L

[env:node32s]
platform = espressif32
board = node32s
```

If you want to build for a different device, all you need to do is re-configure ['platformio.ini'](platformio.ini) and select an alternative environment by modifying the default_envs variable. Building for the common esp32 "node32s" board for example:

```ini
[platformio]
;default_envs = esp12e
default_envs = node32s
```

## Customizing and theming

The framework, and MaterialUI allows for a reasonable degree of customization with little effort.

### Theming the app

The app can be easily themed by editing the [MaterialUI theme](https://mui.com/customization/typography/). Edit the theme in ['interface/src/CustomMuiTheme.tsx'](interface/src/CustomTheme.tsx) as you desire. For example, here is a dark theme:

```js
const theme = responsiveFontSizes(
  createTheme({
    palette: {
      mode: "dark",
      text: {
        primary: '#fff',
        secondary: grey[500],
      },
      primary: {
        main: indigo[500]
      },
      secondary: {
        main: blueGrey[800]
      },
      info: {
        main: indigo[800]
      },
      warning: {
        main: orange[800]
      },
      error: {
        main: red[800]
      },
      success: {
        main: green[800]
      }
    }
  })
);
```

![Dark Theme](/media/dark.png?raw=true "Dark Theme")

### Changing the app icon

You can replace the app icon is located at ['interface/public/app/icon.png'](interface/public/app/icon.png) with one of your preference. A 256 x 256 PNG is recommended for best compatibility.


### Changing the app name

The app name displayed on the sign in page and on the menu bar can be modified by editing the REACT_APP_NAME property in ['interface/.env'](interface/.env)

```ini
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
ESP8266React esp8266React(&server);
```

Now in the `setup()` function the initialization is performed:

```cpp
void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);

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

#### Stateful service

The [StatefulService.h](lib/framework/StatefulService.h) class is responsible for managing state. It has an API which allows other code to update or respond to updates in the state it manages. You can define a data class to hold state, then build a StatefulService class to manage it. After that you may attach HTTP endpoints, WebSockets or MQTT topics to the StatefulService instance to provide commonly required features.

Here is a simple example of a state class and a StatefulService to manage it:

```cpp
class LightState {
 public:
  bool on = false;
  uint8_t brightness = 255;
};

class LightStateService : public StatefulService<LightState> {
};
```

You may listen for changes to state by registering an update handler callback. It is possible to remove an update handler later if required.

```cpp
// register an update handler
update_handler_id_t myUpdateHandler = lightStateService.addUpdateHandler(
  [&](const String& originId) {
    Serial.print("The light's state has been updated by: "); 
    Serial.println(originId); 
  }
);

// remove the update handler
lightStateService.removeUpdateHandler(myUpdateHandler);
```

An "originId" is passed to the update handler which may be used to identify the origin of an update. The default origin values the framework provides are:

Origin                | Description
--------------------- | -----------
http                  | An update sent over REST (HttpEndpoint)
mqtt                  | An update sent over MQTT (MqttPubSub)
websocket:{clientId}  | An update sent over WebSocket (WebSocketRxTx)

StatefulService exposes a read function which you may use to safely read the state. This function takes care of protecting against parallel access to the state in multi-core enviornments such as the ESP32.

```cpp
lightStateService.read([&](LightState& state) {
  digitalWrite(LED_PIN, state.on ? HIGH : LOW); // apply the state update to the LED_PIN
});
```

StatefulService also exposes an update function which allows the caller to update the state with a callback. This function automatically calls the registered update handlers if the state has been changed. The example below changes the state of the light (turns it on) using the arbitrary origin "timer" and returns the "CHANGED" state update result, indicating that a change was made:

```cpp
lightStateService.update([&](LightState& state) {
   if (state.on) {
    return StateUpdateResult::UNCHANGED; // lights were already on, return UNCHANGED
  }
  state.on = true;  // turn on the lights
  return StateUpdateResult::CHANGED; // notify StatefulService by returning CHANGED
}, "timer");
```

There are three possible return values for an update function which are as follows:

Origin                        | Description
----------------------------- | ---------------------------------------------------------------------------
StateUpdateResult::CHANGED    | The update changed the state, propagation should take place if required
StateUpdateResult::UNCHANGED  | The state was unchanged, propagation should not take place
StateUpdateResult::ERROR      | There was an error updating the state, propagation should not take place

#### Serialization

When reading or updating state from an external source (HTTP, WebSockets, or MQTT for example) the state must be marshalled into a serializable form (JSON). SettingsService provides two callback patterns which facilitate this internally:

Callback         | Signature                                                | Purpose
---------------- | -------------------------------------------------------- | ---------------------------------------------------------------------------------
JsonStateReader  | void read(T& settings, JsonObject& root)                 | Reading the state object into a JsonObject
JsonStateUpdater | StateUpdateResult update(JsonObject& root, T& settings)  | Updating the state from a JsonObject, returning the appropriate StateUpdateResult


The static functions below can be used to facilitate the serialization/deserialization of the light state:

```cpp
class LightState {
 public:
  bool on = false;
  uint8_t brightness = 255;
  
  static void read(LightState& state, JsonObject& root) {
    root["on"] = state.on;
    root["brightness"] = state.brightness;
  }

  static StateUpdateResult update(JsonObject& root, LightState& state) {
    state.on = root["on"] | false;
    state.brightness = root["brightness"] | 255;
    return StateUpdateResult::CHANGED;
  }
};
```

For convenience, the StatefulService class provides overloads of its `update` and `read` functions which utilize these functions.

Read the state to a JsonObject using a serializer:

```cpp
JsonObject jsonObject = jsonDocument.to<JsonObject>();
lightStateService->read(jsonObject, LightState::read);
```

Update the state from a JsonObject using a deserializer:

```cpp
JsonObject jsonObject = jsonDocument.as<JsonObject>();
lightStateService->update(jsonObject, LightState::update, "timer");
```

#### Endpoints

The framework provides an [HttpEndpoint.h](lib/framework/HttpEndpoint.h) class which may be used to register GET and POST handlers to read and update the state over HTTP. You may construct an HttpEndpoint as a part of the StatefulService or separately if you prefer. 

The code below demonstrates how to extend the LightStateService class to provide an unsecured endpoint:

```cpp
class LightStateService : public StatefulService<LightState> {
 public:
  LightStateService(AsyncWebServer* server) :
      _httpEndpoint(LightState::read, LightState::update, this, server, "/rest/lightState") {
  }

 private:
  HttpEndpoint<LightState> _httpEndpoint;
};
```

Endpoint security is provided by authentication predicates which are [documented below](#security-features). The SecurityManager and authentication predicate may be provided if a secure endpoint is required. The placeholder project shows how endpoints can be secured.

#### Persistence

[FSPersistence.h](lib/framework/FSPersistence.h) allows you to save state to the filesystem. FSPersistence automatically writes changes to the file system when state is updated. This feature can be disabled by calling `disableUpdateHandler()` if manual control of persistence is required.

The code below demonstrates how to extend the LightStateService class to provide persistence:

```cpp
class LightStateService : public StatefulService<LightState> {
 public:
  LightStateService(FS* fs) :
      _fsPersistence(LightState::read, LightState::update, this, fs, "/config/lightState.json") {
  }

 private:
  FSPersistence<LightState> _fsPersistence;
};
```

#### WebSockets

[WebSocketTxRx.h](lib/framework/WebSocketTxRx.h) allows you to read and update state over a WebSocket connection. WebSocketTxRx automatically pushes changes to all connected clients when state is updated.

The code below demonstrates how to extend the LightStateService class to provide an unsecured WebSocket:

```cpp
class LightStateService : public StatefulService<LightState> {
 public:
  LightStateService(AsyncWebServer* server) :
      _webSocket(LightState::read, LightState::update, this, server, "/ws/lightState"), {
  }

 private:
  WebSocketTxRx<LightState> _webSocket;
};
```

WebSocket security is provided by authentication predicates which are [documented below](#security-features). The SecurityManager and authentication predicate may be provided if a secure WebSocket is required. The placeholder project shows how WebSockets can be secured.

#### MQTT

The framework includes an MQTT client which can be configured via the UI. MQTT requirements will differ from project to project so the framework exposes the client for you to use as you see fit. The framework does however provide a utility to interface StatefulService to a pair of pub/sub (state/set) topics. This utility can be used to synchronize state with software such as Home Assistant.

[MqttPubSub.h](lib/framework/MqttPubSub.h) allows you to publish and subscribe to synchronize state over a pair of MQTT topics. MqttPubSub automatically pushes changes to the "pub" topic and reads updates from the "sub" topic.

The code below demonstrates how to extend the LightStateService class to interface with MQTT:

```cpp

class LightStateService : public StatefulService<LightState> {
 public:
  LightStateService(AsyncMqttClient* mqttClient) :
      _mqttPubSub(LightState::read,
                  LightState::update,
                  this,
                  mqttClient,
                  "homeassistant/light/my_light/set",
                  "homeassistant/light/my_light/state") {
  }

 private:
  MqttPubSub<LightState> _mqttPubSub;
};
```

You can re-configure the pub/sub topics at runtime as required:

```cpp
_mqttPubSub.configureBroker("homeassistant/light/desk_lamp/set", "homeassistant/light/desk_lamp/state");
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

SettingsService              | Description
---------------------------- | ----------------------------------------------
getFS()                      | The filesystem used by the framework
getSecurityManager()         | The security manager - detailed above
getSecuritySettingsService() | Configures the users and other security settings
getWiFiSettingsService()     | Configures and manages the WiFi network connection
getAPSettingsService()       | Configures and manages the Access Point
getNTPSettingsService()      | Configures and manages the network time
getOTASettingsService()      | Configures and manages the Over-The-Air update feature
getMqttSettingsService()     | Configures and manages the MQTT connection
getMqttClient()              | Provides direct access to the MQTT client instance

The core features use the [StatefulService.h](lib/framework/StatefulService.h) class and can therefore you can change settings or observe changes to settings through the read/update API.

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
  return StateUpdateResult::CHANGED; 
}, "myapp");
```

Observe changes to the WiFiSettings:

```cpp
esp8266React.getWiFiSettingsService()->addUpdateHandler(
  [&](const String& originId) {
    Serial.println("The WiFi Settings were updated!");
  }
);
```

## Libraries Used

* [React](https://reactjs.org/)
* [Material-UI](https://mui.com/)
* [notistack](https://github.com/iamhosseindhv/notistack)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
* [AsyncMqttClient](https://github.com/marvinroger/async-mqtt-client)
