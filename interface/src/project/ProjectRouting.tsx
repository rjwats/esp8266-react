import { FC } from 'react';
import { Navigate, Routes, Route } from 'react-router-dom';

import LightsProject from './LightsProject';

const ProjectRouting: FC = () => {
  return (
    <Routes>
      {
        // Add the default route for your project below
      }
      <Route path="/*" element={<Navigate to="lights/settings" />} />
      {
        // Add your project page routes below.
      }
      <Route path="lights/*" element={<LightsProject />} />
    </Routes>
  );
};

export default ProjectRouting;
