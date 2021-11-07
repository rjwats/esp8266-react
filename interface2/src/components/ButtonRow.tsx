import { FC } from 'react';
import { Box, BoxProps } from '@mui/material';

const ButtonRow: FC<BoxProps> = ({ children, ...rest }) => {
  return (
    <Box
      sx={(theme) => ({
        '& button, & a, & .MuiCard-root': {
          margin: theme.spacing(0, .5),
          '&:last-child': {
            marginRight: 0,
          },
          '&:first-child': {
            marginLeft: 0,
          }
        }
      })}
      {...rest}
    >
      {children}
    </Box>
  );
};

export default ButtonRow;
