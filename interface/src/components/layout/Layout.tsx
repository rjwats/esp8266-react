
import { FC, useState } from 'react';

import { Box, Toolbar } from '@mui/material';

import { PROJECT_NAME } from '../../api/env';
import LayoutDrawer from './LayoutDrawer';
import LayoutAppBar from './LayoutAppBar';
import { LayoutContext } from './context';
import { useEffect } from 'react';
import { useLocation } from 'react-router-dom';

export const DRAWER_WIDTH = 280;

const Layout: FC = ({ children }) => {

  const [mobileOpen, setMobileOpen] = useState(false);
  const [title, setTitle] = useState(PROJECT_NAME);
  const { pathname } = useLocation();

  const handleDrawerToggle = () => {
    setMobileOpen(!mobileOpen);
  };

  useEffect(() => setMobileOpen(false), [pathname]);

  return (
    <LayoutContext.Provider value={{ title, setTitle }}>
      <LayoutAppBar title={title} onToggleDrawer={handleDrawerToggle} />
      <LayoutDrawer mobileOpen={mobileOpen} onClose={handleDrawerToggle} />
      <Box
        component="main"
        sx={{ marginLeft: { md: `${DRAWER_WIDTH}px` } }}
      >
        <Toolbar />
        {children}
      </Box>
    </LayoutContext.Provider >
  );

};

export default Layout;
