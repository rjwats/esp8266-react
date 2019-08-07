import * as React from 'react';
import {
  Redirect, Route
} from "react-router-dom";

import { withAuthenticationContext } from './Context.js';
import * as Authentication from './Authentication';
import { withSnackbar } from 'notistack';

export class AuthenticatedRoute extends React.Component {

  render() {
    const { enqueueSnackbar, authenticationContext, component: Component, ...rest } = this.props;
    const { location } = this.props;
    const renderComponent = (props) => {
      if (authenticationContext.isAuthenticated()) {
        return (
          <Component {...props} />
        );
      }
      Authentication.storeLoginRedirect(location);
      enqueueSnackbar("Please log in to continue.", {
        variant: 'info',
      });
      return (
        <Redirect to='/' />
      );
    }
    return (
      <Route {...rest} render={renderComponent} />
    );
  }

}

export default withSnackbar(withAuthenticationContext(AuthenticatedRoute));
