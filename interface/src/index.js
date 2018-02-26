import React from 'react';
import { render } from 'react-dom';

import history from './history';
import { Router, Route, Redirect, Switch } from 'react-router';

import App from './App';

render((
  <Router history={history}>
    <Switch>
      <Redirect exact from='/' to='/home'/>
      <Route path="/" component={App} />
    </Switch>
  </Router>
), document.getElementById("root"))
