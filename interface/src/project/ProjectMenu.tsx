import { FC } from 'react';

import { List } from '@mui/material';
import FlareIcon from '@mui/icons-material/Flare';

import { PROJECT_PATH } from '../api/env';
import LayoutMenuItem from '../components/layout/LayoutMenuItem';

const ProjectMenu: FC = () => (
  <List>
    <LayoutMenuItem icon={FlareIcon} label="Lights" to={`/${PROJECT_PATH}/lights`} />
  </List>
);

export default ProjectMenu;
