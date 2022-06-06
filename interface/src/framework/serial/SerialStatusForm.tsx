import { FC, useContext, useState } from "react";
import { useSnackbar } from "notistack";

import {
  Avatar, Box, Button, Dialog, DialogActions, DialogContent, DialogTitle,
  Divider, List, ListItem, ListItemAvatar, ListItemText, TextField, Theme, useTheme
} from "@mui/material";
import DeviceHubIcon from '@mui/icons-material/DeviceHub';
import RefreshIcon from '@mui/icons-material/Refresh';
import AccessTimeIcon from '@mui/icons-material/AccessTime';
import ReportIcon from '@mui/icons-material/Report';

import * as SerialApi from "../../api/serial";
import { SerialStatus } from "../../types";
import { ButtonRow, FormLoader, SectionContent } from "../../components";
import { extractErrorMessage, formatDateTime, formatDuration, formatLocalDateTime, useRest } from "../../utils";
import { AuthenticatedContext } from "../../contexts/authentication";

// import { RestFormProps, FormActions, FormButton, HighlightAvatar } from '../../components';
// import { SectionContent, BlockFormControlLabel } from '../../components';
// import { serialStatusHighlight, serialStatus  } from './SerialStatus';
// import { SerialStatus, Config } from '../../types';

export const serialStatusHighlight = ({ enabled }: SerialStatus, theme: Theme) => {
  if (!enabled) {
    return theme.palette.info.main;
  }
  return theme.palette.error.main;
};

export const serialStatus = ({ enabled }: SerialStatus) => {
  if (!enabled) {
    return "Not enabled";
  }
  return "Disconnected";
};

const SerialStatusForm: FC = () => {
  const { loadData, data, errorMessage } = useRest<SerialStatus>({ read: SerialApi.readSerialStatus });

  const theme = useTheme();

  const content = () => {
    if (!data) {
      return (<FormLoader onRetry={loadData} errorMessage={errorMessage} />);
    }

    return (
      <>
        <List>
          <ListItem>
            <ListItemAvatar>
              <Avatar sx={{ bgcolor: serialStatusHighlight(data, theme) }}>
                <DeviceHubIcon />
              </Avatar>
            </ListItemAvatar>
            <ListItemText primary="Status" secondary={serialStatus(data)} />
          </ListItem>
          <Divider variant="inset" component="li" />
          <ListItem>
            <ListItemAvatar>
              <Avatar>#</Avatar>
            </ListItemAvatar>
            <ListItemText primary="Baud rate" secondary={data.baud} />
          </ListItem>
          <ListItem>
            <ListItemAvatar>
              <Avatar>#</Avatar>
            </ListItemAvatar>
            <ListItemText primary="Config" secondary={data.config} />
          </ListItem>
          <Divider variant="inset" component="li" />
        </List>
        <ButtonRow mt={1}>
          <Button startIcon={<RefreshIcon />} variant="contained" color="secondary" onClick={loadData}>
            Refresh
          </Button>
        </ButtonRow>
      </>
    );
  };

return (
    <SectionContent title='NTP Status' titleGutter>
      {content()}
    </SectionContent>
  );

};

export default SerialStatusForm;
