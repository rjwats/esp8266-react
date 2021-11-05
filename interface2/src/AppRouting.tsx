import { FC, useContext, useEffect } from 'react';
import { Route, useLocation } from 'react-router-dom';
import { Switch, Redirect } from 'react-router';
import { useSnackbar, VariantType } from 'notistack';

import { Authentication, AuthenticationContext } from './contexts/authentication';
import { FeaturesContext } from './contexts/features';
import { AuthenticatedRoute, UnauthenticatedRoute } from './components';

import SignIn from './SignIn';
import AuthenticatedRouting from './AuthenticatedRouting';

interface SecurityRedirectProps {
  message: string;
  variant?: VariantType;
}

const SecurityRedirect: FC<SecurityRedirectProps> = ({ message, variant }) => {
  const authenticationContext = useContext(AuthenticationContext);
  const { enqueueSnackbar } = useSnackbar();
  useEffect(() => {
    authenticationContext.signOut(false);
    enqueueSnackbar(message, { variant });
  }, [message, variant, authenticationContext, enqueueSnackbar]);
  return (<Redirect to="/" />);
};

const AppRouting: FC = () => {
  const { pathname } = useLocation();
  const { features } = useContext(FeaturesContext);

  return (
    <Authentication>
      <Switch>
        <Redirect from="/:url*(/+)" to={pathname.slice(0, -1)} />
        <Route exact path="/unauthorized">
          <SecurityRedirect message="Please log in to continue" />
        </Route>
        {
          features.security && (
            <UnauthenticatedRoute exact path="/" >
              <SignIn />
            </UnauthenticatedRoute>
          )
        }
        <AuthenticatedRoute path="/" >
          <AuthenticatedRouting />
        </AuthenticatedRoute>
        {
          /*
        <AuthenticatedRoute exact path="/wifi/*" component={WiFiConnection} />
        <AuthenticatedRoute exact path="/ap/*" component={AccessPoint} />
        {features.ntp && (
          <AuthenticatedRoute exact path="/ntp/*" component={NetworkTime} />
        )}
        {features.mqtt && (
          <AuthenticatedRoute exact path="/mqtt/*" component={Mqtt} />
        )}
        {features.security && (
          <AuthenticatedRoute exact path="/security/*" component={Security} />
        )}
        <AuthenticatedRoute exact path="/system/*" component={System} />
        */
        }
      </Switch>
    </Authentication>
  );
};

export default AppRouting;
