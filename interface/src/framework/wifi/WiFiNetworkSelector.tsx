import { FC, useContext } from 'react';

import { Avatar, Badge, List, ListItem, ListItemAvatar, ListItemButton, ListItemIcon, ListItemText } from '@mui/material';
import LockOpenIcon from '@mui/icons-material/LockOpen';
import LockIcon from '@mui/icons-material/Lock';
import WifiIcon from '@mui/icons-material/Wifi';

import { WiFiNetwork, WiFiNetworkList } from '../../types';
import { MessageBox } from '../../components';

import { WiFiConnectionContext } from './WiFiConnectionContext';
import { isNetworkOpen, networkSecurityMode } from './wifiUtils';

interface WiFiNetworkSelectorProps {
  networkList: WiFiNetworkList;
}

const WiFiNetworkSelector: FC<WiFiNetworkSelectorProps> = ({ networkList }) => {
  const wifiConnectionContext = useContext(WiFiConnectionContext);

  const renderNetwork = (network: WiFiNetwork) => {
    return (
      <ListItem key={network.bssid}>
        <ListItemButton onClick={() => wifiConnectionContext.selectNetwork(network)}>
          <ListItemAvatar>
            <Avatar>
              {isNetworkOpen(network) ? <LockOpenIcon /> : <LockIcon />}
            </Avatar>
          </ListItemAvatar>
          <ListItemText
            primary={network.ssid}
            secondary={"Security: " + networkSecurityMode(network) + ", Ch: " + network.channel}
          />
          <ListItemIcon>
            <Badge badgeContent={network.rssi + "db"}>
              <WifiIcon />
            </Badge>
          </ListItemIcon>
        </ListItemButton>
      </ListItem>
    );
  };

  if (networkList.networks.length === 0) {
    return (
      <MessageBox mt={2} mb={1} message="No WiFi networks found" level="info" />
    );
  }

  return (
    <List>
      {networkList.networks.map(renderNetwork)}
    </List>
  );

};

export default WiFiNetworkSelector;
