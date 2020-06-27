import React, { Fragment } from 'react';
import { SingleUpload } from '../components';
import { Box } from '@material-ui/core';

interface OTAUploadFormProps {
  uploading: boolean;
  progress?: ProgressEvent;
  onFileSelected: (file: File) => void;
}

class OTAUploadForm extends React.Component<OTAUploadFormProps> {

  handleDrop = (files: File[]) => {
    const file = files[0];
    if (file) {
      this.props.onFileSelected(files[0]);
    }
  };

  render() {
    const { uploading, progress } = this.props;
    return (
      <Fragment>
        <Box py={2}>
          Upload the new "firmware.bin" file using the form below. This will immediately start the firmware replacement.
        </Box>
        <SingleUpload accept="application/octet-stream" onDrop={this.handleDrop} uploading={uploading} progress={progress} />
      </Fragment>
    );
  }

}

export default OTAUploadForm;
