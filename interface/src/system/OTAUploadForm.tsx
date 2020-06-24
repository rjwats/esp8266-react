import React, { Fragment } from 'react';
import { SingleUpload } from '../components';
import { Typography, Box } from '@material-ui/core';

interface OTAUploadFormProps {
  uploading: boolean;
  onFileSelected: (file: File) => void;
}

class OTAUploadForm extends React.Component<OTAUploadFormProps> {

  handleDrop = (files: File[]) => {
    if (files.length) {
      this.props.onFileSelected(files[0]);
    }
  };

  render() {
    return (
      <Fragment>
        <Box py={2}>
          Upload the new "firmware.bin" file using the form below. This will immediately start the firmware replacement.
        </Box>
        <SingleUpload onDrop={this.handleDrop} uploading={this.props.uploading} />
      </Fragment>
    );
  }

}

export default OTAUploadForm;
