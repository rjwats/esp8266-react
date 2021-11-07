import { FC } from 'react';

import { List } from '@mui/material';
import SettingsRemoteIcon from '@mui/icons-material/SettingsRemote';

import LayoutMenuItem from '../components/layout/LayoutMenuItem';
import { PROJECT_PATH } from '../api/env';

const ProjectMenu: FC = () => (
  <List>
    <LayoutMenuItem icon={SettingsRemoteIcon} label="Demo Project" to={`/${PROJECT_PATH}/demo`} />
  </List>
);

export default ProjectMenu;
