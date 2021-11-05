import { FC, useCallback, useContext, useEffect } from 'react';
import { Redirect, Switch } from 'react-router';
import { AxiosError } from 'axios';

import { AXIOS } from './api/endpoints';
import { getDefaultRoute, storeLoginRedirect } from './api/authentication';
import { Layout } from './components/layout';
import { AuthenticatedRoute } from './components';
import { FeaturesContext } from './contexts/features';
import { useHistory, useLocation } from 'react-router-dom';
import { PROJECT_PATH } from './api/env';

const AuthenticatedRouting: FC = () => {

  const { features } = useContext(FeaturesContext);
  const location = useLocation();
  const history = useHistory();

  const handleApiResponseError = useCallback((error: AxiosError) => {
    if (error.response && error.response.status === 401) {
      storeLoginRedirect(location);
      history.push("/unauthorized");
    }
    return Promise.reject(error);
  }, [location, history]);

  useEffect(() => {
    const axiosHandlerId = AXIOS.interceptors.response.use((response) => response, handleApiResponseError);
    return () => AXIOS.interceptors.response.eject(axiosHandlerId);
  }, [handleApiResponseError]);

  return (
    <Layout title="TODO - REPLACE ME">
      <Switch>
        {
          features.project && (
            <AuthenticatedRoute exact path={`/${PROJECT_PATH}`}>
              This will be the project screen
            </AuthenticatedRoute>
          )
        }
        {
          features.project && (
            <AuthenticatedRoute exact path="/wifi">
              This will be the wifi screen
            </AuthenticatedRoute>
          )
        }
        <Redirect to={getDefaultRoute(features)} />
      </Switch>
    </Layout>
  );
};

export default AuthenticatedRouting;
