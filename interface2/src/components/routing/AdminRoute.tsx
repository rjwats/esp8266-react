import { FC, useContext } from 'react';
import { RouteComponentProps, Redirect, Route } from "react-router-dom";

import { AuthenticatedContext } from '../../contexts/authentication';

const AdminRoute: FC = ({ children, ...rest }) => {
  const authenticatedContext = useContext(AuthenticatedContext);
  const renderComponent: FC<RouteComponentProps<any>> = () => {
    if (authenticatedContext.me.admin) {
      return (<>{children}</>);
    }
    return (<Redirect to='/' />);
  };
  return (<Route {...rest} render={renderComponent} />);
};

export default AdminRoute;
