import React from 'react';
import { Typography, Box, Switch } from '@material-ui/core';
import { SectionContent, BlockFormControlLabel } from '../components';
import { ActionType,useAppDispatch,useAppSelector} from '../redux/actions';

export default function StateWebSocketControllerForm() {
  const dispatch = useAppDispatch();
  const led_state:any = useAppSelector((state:any) => state.led);
  const changeLedOn = (event: React.ChangeEvent<HTMLInputElement>) => {
    dispatch({type:ActionType.LED_SET_STATE, payload :{ led_on: event.target.checked }});
  }
  return (
    <SectionContent title='Controller' titleGutter>
      <Box bgcolor="primary.main" color="primary.contrastText" p={2} mt={2} mb={2}>
        <Typography variant="body1">
          </Typography>
      </Box>
      <BlockFormControlLabel
        control={
          <Switch
            checked={led_state.led_on}
            onChange={changeLedOn}
            color="primary"
          />
        }
        label="LED State?"
      />
    </SectionContent>
  );
}
