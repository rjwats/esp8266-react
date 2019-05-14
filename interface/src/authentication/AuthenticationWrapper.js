import * as React from 'react';
import history from '../history'
import { withNotifier } from '../components/SnackbarNotification';

import { ACCESS_TOKEN } from './Authentication';
import { AuthenticationContext } from './Context';
import jwtDecode from 'jwt-decode';

class AuthenticationWrapper extends React.Component {

  constructor(props) {
    super(props);
    this.refresh = this.refresh.bind(this);
    this.signIn = this.signIn.bind(this);
    this.signOut = this.signOut.bind(this);
    this.state = {
      context: {
        refresh: this.refresh,
        signIn: this.signIn,
        signOut: this.signOut
      },
      initialized: false
    };
  }

  componentDidMount() {
    this.refresh();
  }

  render() {
    return (
      <React.Fragment>
        {this.state.initialized ? this.renderContent() : this.renderContentLoading()}
      </React.Fragment>
    );
  }

  renderContent() {
    return (
      <AuthenticationContext.Provider value={this.state.context}>
        {this.props.children}
      </AuthenticationContext.Provider>
    );
  }

  renderContentLoading() {
    return (
      <div>THIS IS WHERE THE LOADING MESSAGE GOES</div>
    );
  }

  refresh() {
    var accessToken = localStorage.getItem(ACCESS_TOKEN);
    if (accessToken) {
      try {
        this.setState({ initialized: true, context: { ...this.state.context, jwt: jwtDecode(accessToken) } });
      } catch (err) {
        localStorage.removeItem(ACCESS_TOKEN);
        this.props.raiseNotification("Please log in again.");
        history.push('/');
      }
    } else {
      this.setState({ initialized: true });
    }
  }

  signIn(accessToken) {
    try {
      this.setState({ context: { ...this.state.context, jwt: jwtDecode(accessToken) } });
      localStorage.setItem(ACCESS_TOKEN, accessToken);
    } catch (err) {
      this.props.raiseNotification("JWT did not parse.");
      history.push('/');
    }
  }

  signOut() {
    localStorage.removeItem(ACCESS_TOKEN);
    this.setState({
      context: {
        ...this.state.context,
        me: undefined
      }
    });
    this.props.raiseNotification("You have signed out.");
    history.push('/');
  }

}

export default withNotifier(AuthenticationWrapper)
