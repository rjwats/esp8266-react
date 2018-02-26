import React, { Component } from 'react';

import { Route, Redirect, Switch } from 'react-router';

// containers
import WiFiConfiguration from './containers/WiFiConfiguration';
import NTPConfiguration from './containers/NTPConfiguration';
import OTAConfiguration from './containers/OTAConfiguration';
import APConfiguration from './containers/APConfiguration';

class AppRouting extends Component {
	render() {
	   return (
       <Switch>
         <Route exact path="/wifi-configuration" component={WiFiConfiguration} />
				 <Route exact path="/ap-configuration" component={APConfiguration} />
				 <Route exact path="/ntp-configuration" component={NTPConfiguration} />
				 <Route exact path="/ota-configuration" component={OTAConfiguration} />
         <Redirect to="/wifi-configuration" />
       </Switch>
		)
	}
}

export default AppRouting;
