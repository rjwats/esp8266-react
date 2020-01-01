import * as React from "react";

export interface User {
  username: string;
  admin: boolean;
}

export interface AuthenticationContext {
  refresh: () => void;
  signIn: (accessToken: string) => void;
  signOut: () => void;
  isAuthenticated: () => boolean;
  isAdmin: () => boolean;
  user?: User;
}

const AuthenticationContextDefaultValue = {} as AuthenticationContext
export const AuthenticationContext = React.createContext(
  AuthenticationContextDefaultValue
);

export interface AuthenticationContextProps {
  authenticationContext: AuthenticationContext;
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