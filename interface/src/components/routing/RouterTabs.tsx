
import React, { FC } from 'react';
import { useNavigate } from 'react-router-dom';

import { Tabs } from '@mui/material';

interface RouterTabsProps {
  value: string | false;
}

const RouterTabs: FC<RouterTabsProps> = ({ value, children }) => {
  const navigate = useNavigate();

  const handleTabChange = (event: React.ChangeEvent<{}>, path: string) => {
    navigate(path);
  };

  return (
    <Tabs value={value} onChange={handleTabChange} variant="fullWidth">
      {children}
    </Tabs>
  );
};

export default RouterTabs;
