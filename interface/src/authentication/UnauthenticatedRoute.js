import * as React from 'react';
import {
  Redirect, Route
} from "react-router-dom";

import { withAuthenticationContext } from './Context.js';
import * as Authentication from './Authentication';

class UnauthenticatedRoute extends React.Component {
  render() {
    const { component:Component, ...rest } = this.props;
    const renderComponent = (props) => {
      if (this.props.authenticationContext.jwt) {
        return (<Redirect to={Authentication.fetchLoginRedirect()} />);
      }
      return (<Component {...props} />);
    }
    return (
      <Route {...rest} render={renderComponent} />
    );
  }
}

export default withAuthenticationContext(UnauthenticatedRoute);
