import * as React from 'react';
import { Redirect, Route, RouteProps, RouteComponentProps } from "react-router-dom";

import { withAuthenticationContext, AuthenticationContextProps } from './AuthenticationContext';
import * as Authentication from './Authentication';

interface UnauthenticatedRouteProps extends RouteProps {
  component: React.ComponentType<RouteComponentProps<any>> | React.ComponentType<any>;
}

type RenderComponent = (props: RouteComponentProps<any>) => React.ReactNode;

class UnauthenticatedRoute extends Route<UnauthenticatedRouteProps & AuthenticationContextProps> {
  public render() {
    const { authenticationContext, component:Component, ...rest } = this.props;
    const renderComponent: RenderComponent = (props) => {
      if (authenticationContext.user) {
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
