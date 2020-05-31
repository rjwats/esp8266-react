import React, { Component } from 'react';

import { Features } from './types';
import { FeaturesContext } from './context';
import FullScreenLoading from '../components/FullScreenLoading';
import ApplicationError from '../components/ApplicationError';
import { FEATURES_ENDPOINT } from '../api';

interface ApplicationState {
  features?: Features;
  error?: string;
};

class Application extends Component<{}, ApplicationState> {

  state: ApplicationState = {};

  componentDidMount() {
    this.fetchFeaturesDetails();
  }

  fetchFeaturesDetails = () => {
    fetch(FEATURES_ENDPOINT)
      .then(response => {
        if (response.status === 200) {
          return response.json();
        } else {
          throw Error("Invalid status code: " + response.status);
        }
      }).then(features => {
        this.setState({ features });
      })
      .catch(error => {
        this.setState({ error: error.message });
      });
  }

  render() {
    const { features, error } = this.state;
    if (features) {
      return (
        <FeaturesContext.Provider value={{
          features
        }}>
          {this.props.children}
        </FeaturesContext.Provider>
      );
    }
    if (error) {
      return (
        <ApplicationError error={error} />
      );
    }
    return (
      <FullScreenLoading />
    );
  }

}

export default Application;