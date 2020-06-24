import React, { Component } from 'react';

import { SectionContent } from '../components';
import { OTA_UPLOAD_ENDPOINT } from '../api';

import OTAUploadForm from './OTAUploadForm';
import { redirectingAuthorizedFetch } from '../authentication';
import { withSnackbar, WithSnackbarProps } from 'notistack';

// TODO - factor out into controller component

interface OTAUploadControllerState {
  uploading: boolean;
}

class OTAUploadController extends Component<WithSnackbarProps, OTAUploadControllerState> {
  controller = new AbortController();
  signal = this.controller.signal;

  state: OTAUploadControllerState = {
    uploading: false
  };

  componentWillUnmount() {
    this.controller.abort();
  }

  uploadFile = (file: File) => {
    const formData = new FormData();
    formData.append('file', file);

    this.setState({ uploading: true });
    redirectingAuthorizedFetch(OTA_UPLOAD_ENDPOINT, {
      method: 'POST',
      body: formData,
      signal: this.signal
    }).then(response => {
      if (response.status !== 200) {
        throw Error("Invalid status code: " + response.status);
      }
    }).then(() => {
      this.props.enqueueSnackbar("Activating new firmware", { variant: 'success' });
      this.setState({ uploading: false });
    }).catch(error => {
      const errorMessage = error.message || "Unknown error";
      this.props.enqueueSnackbar("Problem uploading: " + errorMessage, { variant: 'error' });
      this.setState({ uploading: false });
    });
  }

  render() {
    return (
      <SectionContent title="OTA Upload">
        <OTAUploadForm onFileSelected={this.uploadFile} uploading={this.state.uploading} />
      </SectionContent>
    );
  }

}

export default withSnackbar(OTAUploadController);
