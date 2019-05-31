import * as React from 'react';
import {
  Redirect, Route
} from "react-router-dom";

import { withAuthenticationContext } from './Context.js';
import * as Authentication from './Authentication';
import { withNotifier } from '../components/SnackbarNotification';

export class AuthenticatedRoute extends React.Component {

  render() {
    const { raiseNotification, authenticationContext, component: Component, ...rest } = this.props;
    const { location } = this.props;
    const renderComponent = (props) => {
      if (authenticationContext.isAuthenticated()) {
        return (
          <Component {...props} />
        );
      }
      Authentication.storeLoginRedirect(location);
      raiseNotification("Please log in to continue.");
      return (
        <Redirect to='/' />
      );
    }
    return (
      <Route {...rest} render={renderComponent} />
    );
  }

}

export default withNotifier(withAuthenticationContext(AuthenticatedRoute));
