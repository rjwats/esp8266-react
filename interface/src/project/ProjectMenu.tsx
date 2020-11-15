import React, { Component } from 'react';
import { Link, withRouter, RouteComponentProps } from 'react-router-dom';

import { List, ListItem, ListItemIcon, ListItemText } from '@material-ui/core';
import FlareIcon from '@material-ui/icons/Flare';

import { PROJECT_PATH, WEB_SOCKET_ROOT } from '../api';

export const LIGHT_SETTINGS_WEBSOCKET_URL = WEB_SOCKET_ROOT + "spectrum";

class ProjectMenu extends Component<RouteComponentProps> {

  render() {
    const path = this.props.match.url;
    return (
      <List>
        <ListItem to={`/${PROJECT_PATH}/lights/`} selected={path.startsWith(`/${PROJECT_PATH}/lights/`)} button component={Link}>
          <ListItemIcon>
            <FlareIcon />
          </ListItemIcon>
          <ListItemText primary="Lights" />
        </ListItem>
      </List>
    )
  }

}

export default withRouter(ProjectMenu);
