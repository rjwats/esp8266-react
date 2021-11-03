import { FC, useCallback, useEffect, useState } from 'react';

import * as FeaturesApi from '../../api/features';

import { extractErrorMessage } from '../../utils';
import { Features } from '../../types';

import { FeaturesContext } from '.';
import ApplicationError from '../../components/ApplicationError';
import LoadingSpinner from '../../components/LoadingSpinner';

const FeaturesLoader: FC = (props) => {

  const [errorMessage, setErrorMessage] = useState<string>();
  const [features, setFeatures] = useState<Features>();

  const loadFeatures = useCallback(async () => {
    try {
      const response = await FeaturesApi.readFeatures();
      setFeatures(response.data);
    } catch (error: any) {
      setErrorMessage(extractErrorMessage(error, 'Failed to fetch application details.'));
    }
  }, []);

  useEffect(() => {
    loadFeatures();
  }, [loadFeatures]);

  if (features) {
    return (
      <FeaturesContext.Provider
        value={{
          features
        }}
      >
        {props.children}
      </FeaturesContext.Provider>
    );
  }

  if (errorMessage) {
    return (
      <ApplicationError message={errorMessage} />
    );
  }

  return (
    <LoadingSpinner height="100vh" />
  );

};

export default FeaturesLoader;
