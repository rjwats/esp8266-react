import { FC } from 'react';

import { Box, BoxProps } from '@mui/material';

const ButtonRow: FC<BoxProps> = ({ children, ...rest }) => {
  return (
    <Box
      sx={{
        '& button, & a, & .MuiCard-root': {
          mx: .5,
          '&:last-child': {
            mr: 0,
          },
          '&:first-of-type': {
            ml: 0,
          }
        }
      }}
      {...rest}
    >
      {children}
    </Box>
  );
};

export default ButtonRow;
