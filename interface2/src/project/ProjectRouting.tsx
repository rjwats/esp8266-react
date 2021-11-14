import { FC } from 'react';
import { Redirect, Switch } from 'react-router';
import { Route } from 'react-router-dom';

import { PROJECT_PATH } from '../api/env';

import DemoProject from './DemoProject';

const ProjectRouting: FC = () => {
  return (
    <Switch>
      {
        /*
        * Add your project page routing below.
        */
      }
      <Route exact path={`/${PROJECT_PATH}/demo/*`}>
        <DemoProject />
      </Route>
      {
        /*
        * The redirect below caters for the default project route and redirecting invalid paths.
        * The "to" property must match one of the routes above for this to work correctly.
        */
      }
      <Redirect to={`/${PROJECT_PATH}/demo/information`} />
    </Switch>
  );
};

export default ProjectRouting;
