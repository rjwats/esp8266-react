import { FC } from 'react';

import { Box, Button, LinearProgress, Typography } from '@mui/material';

interface FormLoaderProps {
  errorMessage?: string;
  loadData: () => void;
}

const FormLoader: FC<FormLoaderProps> = ({ errorMessage, loadData }) => {
  if (errorMessage) {
    return (
      <Box m={1} display="flex" flexDirection="column">
        <Typography variant="h6" textAlign="center">
          {errorMessage}
        </Typography>
        <Button variant="contained" color="secondary" onClick={loadData}>
          Retry
        </Button>
      </Box>
    );
  }
  return (
    <Box m={1} display="flex" flexDirection="column">
      <LinearProgress />
      <Typography variant="h6" textAlign="center">
        Loading&hellip;
      </Typography>
    </Box>
  );
};

export default FormLoader;
