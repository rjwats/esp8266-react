import * as React from "react";

export interface Me {
  username: string;
  admin: boolean;
}

export interface AuthenticationContextValue {
  refresh: () => void;
  signIn: (accessToken: string) => void;
  signOut: () => void;
  me?: Me;
}

const AuthenticationContextDefaultValue = {} as AuthenticationContextValue
export const AuthenticationContext = React.createContext(
  AuthenticationContextDefaultValue
);

export interface AuthenticationContextProps {
  authenticationContext: AuthenticationContextValue;
}

export function withAuthenticationContext<T extends AuthenticationContextProps>(Component: React.ComponentType<T>) {
  return class extends React.Component<Omit<T, keyof AuthenticationContextProps>> {
    render() {
      return (
        <AuthenticationContext.Consumer>
          {authenticationContext => <Component {...this.props as T} authenticationContext={authenticationContext} />}
        </AuthenticationContext.Consumer>
      );
    }
  };
}

export interface AuthenticatedContextValue extends AuthenticationContextValue {
  me: Me;
}

const AuthenticatedContextDefaultValue = {} as AuthenticatedContextValue
export const AuthenticatedContext = React.createContext(
  AuthenticatedContextDefaultValue
);

export interface AuthenticatedContextProps {
  authenticatedContext: AuthenticatedContextValue;
}

export function withAuthenticatedContext<T extends AuthenticatedContextProps>(Component: React.ComponentType<T>) {
  return class extends React.Component<Omit<T, keyof AuthenticatedContextProps>> {
    render() {
      return (
        <AuthenticatedContext.Consumer>
          {authenticatedContext => <Component {...this.props as T} authenticatedContext={authenticatedContext} />}
        </AuthenticatedContext.Consumer>
      );
    }
  };
}
