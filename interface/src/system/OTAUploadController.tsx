import React, { Component } from 'react';

import { SectionContent } from '../components';
import { OTA_UPLOAD_ENDPOINT } from '../api';

import OTAUploadForm from './OTAUploadForm';
import { redirectingAuthorizedUpload } from '../authentication';
import { withSnackbar, WithSnackbarProps } from 'notistack';

interface OTAUploadControllerState {
  xhr?: XMLHttpRequest;
  progress?: ProgressEvent;
}

class OTAUploadController extends Component<WithSnackbarProps, OTAUploadControllerState> {

  state: OTAUploadControllerState = {
    xhr: undefined,
    progress: undefined
  };

  componentWillUnmount() {
    this.state.xhr?.abort();
  }

  updateProgress = (progress: ProgressEvent) => {
    this.setState({ progress });
  }

  uploadFile = (file: File) => {
    if (this.state.xhr) {
      return;
    }
    var xhr = new XMLHttpRequest();
    this.setState({ xhr });
    redirectingAuthorizedUpload(xhr, OTA_UPLOAD_ENDPOINT, file, this.updateProgress).then(() => {
      if (xhr.status !== 200) {
        throw Error("Invalid status code: " + xhr.status);
      }
      this.props.enqueueSnackbar("Activating new firmware", { variant: 'success' });
      this.setState({ xhr: undefined, progress: undefined  });
    }).catch((error: Error) => {
      if (error.name === 'AbortError') {
        this.props.enqueueSnackbar("Upload aborted by user", { variant: 'warning' });
      } else {
        const errorMessage = error.name === 'UploadError' ? "Error during upload" : (error.message || "Unknown error");
        this.props.enqueueSnackbar("Problem uploading: " + errorMessage, { variant: 'error' });
        this.setState({ xhr: undefined, progress: undefined });
      }
    });
  }

  render() {
    const { xhr, progress } = this.state;
    return (
      <SectionContent title="OTA Upload">
        <OTAUploadForm onFileSelected={this.uploadFile} uploading={!!xhr} progress={progress} />
      </SectionContent>
    );
  }

}

export default withSnackbar(OTAUploadController);
