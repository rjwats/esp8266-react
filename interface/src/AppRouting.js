import React, { Component } from 'react';

import { Route, Redirect, Switch } from 'react-router';

// containers
import WiFiConfiguration from './containers/WiFiConfiguration';
import NTPConfiguration from './containers/NTPConfiguration';
import OTAConfiguration from './containers/OTAConfiguration';
import APConfiguration from './containers/APConfiguration';
import ChamberConfiguration from './containers/ChamberConfiguration';

class AppRouting extends Component {
	render() {
	   return (
       <Switch>
			   <Route exact path="/chamber-configuration" component={ChamberConfiguration} />
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
