import React, { FC, RefObject } from 'react';
import { Redirect, Switch } from "react-router";
import { Route } from 'react-router-dom';
import { SnackbarProvider } from 'notistack';

import { IconButton } from '@mui/material';

import CloseIcon from '@mui/icons-material/Close';
import { FeaturesLoader } from './contexts/features';
import CustomTheme from './CustomTheme';

const App: FC = () => {
  const notistackRef: RefObject<any> = React.createRef();

  const onClickDismiss = (key: string | number | undefined) => () => {
    notistackRef.current.closeSnackbar(key);
  };

  return (
    <CustomTheme>
      <SnackbarProvider
        maxSnack={3}
        anchorOrigin={{ vertical: 'bottom', horizontal: 'left' }}
        ref={notistackRef}
        action={(key) => (
          <IconButton onClick={onClickDismiss(key)} size="small">
            <CloseIcon />
          </IconButton>
        )}
      >
        <FeaturesLoader>
          <Switch>
            <Route exact path="/unauthorized" >
              <Redirect to="/" />
            </Route>
            {
              // <Route component={AppRouting} />
            }
          </Switch>
        </FeaturesLoader>
      </SnackbarProvider>
    </CustomTheme>
  );
};

export default App;
