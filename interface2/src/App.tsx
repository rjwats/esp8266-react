import React, { FC, RefObject } from 'react';
import { SnackbarProvider } from 'notistack';

import { IconButton } from '@mui/material';
import CloseIcon from '@mui/icons-material/Close';

import { FeaturesLoader } from './contexts/features';

import CustomTheme from './CustomTheme';
import AppRouting from './AppRouting';

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
          <AppRouting />
        </FeaturesLoader>
      </SnackbarProvider>
    </CustomTheme>
  );
};

export default App;
