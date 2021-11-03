import { FC, useCallback, useContext, useEffect, useState } from 'react';
import { useHistory } from 'react-router-dom';

import * as AuthenticationApi from '../../api/authentication';
import { ACCESS_TOKEN } from '../../api/endpoints';

import { LoadingSpinner } from '../../components';
import { Me } from '../../types';
import { FeaturesContext } from '../features';

import { AuthenticationContext } from './context';

const Authentication: FC = ({ children }) => {
  const { features } = useContext(FeaturesContext);
  const history = useHistory();

  const [initialized, setInitialized] = useState<boolean>(false);
  const [me, setMe] = useState<Me>();

  const updateMe = (authenticatedUser?: Me) => {
    setMe(authenticatedUser);
    setInitialized(true);
  };

  const signOut = (redirect: boolean) => {
    AuthenticationApi.clearAccessToken();
    if (redirect) {
      history.push('/');
    }
  };

  const refresh = useCallback(async () => {
    if (!features.security) {
      setInitialized(true);
      setMe({ admin: true, username: "admin" });
      return;
    }

    const accessToken = AuthenticationApi.getStorage().getItem(ACCESS_TOKEN);
    if (accessToken) {
      try {
        await AuthenticationApi.verifyAuthorization();
        setInitialized(true);
        setMe(AuthenticationApi.decodeMeJWT(accessToken));
      } catch (error: any) {
        setInitialized(true);
        setMe(undefined);
      }
    }
  }, [features]);

  useEffect(() => {
    refresh();
  }, [refresh]);

  if (initialized) {
    return (
      <AuthenticationContext.Provider
        value={{
          updateMe,
          signOut,
          me
        }}
      >
        {children}
      </AuthenticationContext.Provider >
    );
  }

  return (
    <LoadingSpinner height="100vh" />
  );
};

export default Authentication;
