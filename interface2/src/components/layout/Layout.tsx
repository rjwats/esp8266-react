
import React, { FC } from 'react';

import { Box, Toolbar } from '@mui/material';

import LayoutDrawer from './LayoutDrawer';
import LayoutAppBar from './LayoutAppBar';

export const DRAWER_WIDTH = 280;

interface LayoutProps {
  title: string;
}

const Layout: FC<LayoutProps> = ({ title, children }) => {

  const [mobileOpen, setMobileOpen] = React.useState(false);

  const handleDrawerToggle = () => {
    setMobileOpen(!mobileOpen);
  };

  return (
    <Box sx={{ display: 'flex' }}>
      <LayoutAppBar title={title} onToggleDrawer={handleDrawerToggle} />
      <LayoutDrawer mobileOpen={mobileOpen} onClose={handleDrawerToggle} />
      <Box
        component="main"
        sx={{ flexGrow: 1, width: { sm: `calc(100% - ${DRAWER_WIDTH}px)` } }}
      >
        <Toolbar />
        {children}
      </Box>
    </Box>
  );

};

export default Layout;
