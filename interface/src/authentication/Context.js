import * as React from "react";

export const AuthenticationContext = React.createContext(
  {}
);

export function withAuthenticationContext(Component) {
  return function AuthenticationContextComponent(props) {
    return (
      <AuthenticationContext.Consumer>
        {authenticationContext => <Component {...props} authenticationContext={authenticationContext} />}
      </AuthenticationContext.Consumer>
    );
  };
}
