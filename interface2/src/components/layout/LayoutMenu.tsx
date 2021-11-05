import { FC, useContext } from 'react';

import { List, ListSubheader } from '@mui/material';

import SettingsInputAntennaIcon from '@mui/icons-material/SettingsInputAntenna';
import AccessTimeIcon from '@mui/icons-material/AccessTime';
import DeviceHubIcon from '@mui/icons-material/DeviceHub';
import SettingsIcon from '@mui/icons-material/Settings';
import LockIcon from '@mui/icons-material/Lock';
import WifiIcon from '@mui/icons-material/Wifi';

import { FeaturesContext } from '../../contexts/features';
import LayoutMenuItem from './LayoutMenuItem';

const LayoutMenu: FC = () => {
  const { features } = useContext(FeaturesContext);
  return (
    <>
      <List disablePadding component="nav">
        <ListSubheader>
          Project
        </ListSubheader>
        <LayoutMenuItem icon={WifiIcon} label="TODO" to="/todo" />
      </List>
      <List disablePadding component="nav">
        <ListSubheader>
          Settings
        </ListSubheader>
        <LayoutMenuItem icon={WifiIcon} label="WiFi Connection" to="/wifi" />
        <LayoutMenuItem icon={SettingsInputAntennaIcon} label="Access Point" to="/ap" />
        {features.ntp && (
          <LayoutMenuItem icon={AccessTimeIcon} label="Network Time" to="/ntp" />
        )}
        {features.mqtt && (
          <LayoutMenuItem icon={DeviceHubIcon} label="MQTT" to="/mqtt" />
        )}
        {features.security && (
          <LayoutMenuItem icon={LockIcon} label="Security" to="/security" />
        )}
        <LayoutMenuItem icon={SettingsIcon} label="System" to="/system" />
      </List>
    </>
  );
};

export default LayoutMenu;
