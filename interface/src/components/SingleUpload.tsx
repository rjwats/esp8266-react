import React, { FC } from 'react';
import { useDropzone, DropzoneState } from 'react-dropzone';

import { makeStyles, createStyles } from '@material-ui/styles';
import CloudUploadIcon from '@material-ui/icons/CloudUpload';
import { Theme, Box, Typography, CircularProgress, LinearProgress } from '@material-ui/core';

const getBorderColor = (theme: Theme, props: DropzoneState) => {
  if (props.isDragAccept) {
    return theme.palette.success.dark;
  }
  if (props.isDragReject) {
    return theme.palette.error.dark;
  }
  if (props.isDragActive) {
    return theme.palette.info.dark;
  }
  return theme.palette.grey[700];
}

const useStyles = makeStyles((theme: Theme) => createStyles({
  dropZone: {
    padding: theme.spacing(3),
    borderWidth: 2,
    borderRadius: 2,
    borderStyle: 'dashed',
    color: theme.palette.grey[700],
    transition: 'border .24s ease-in-out',
    cursor: 'pointer',
    width: '100%',
    borderColor: (props: DropzoneState) => getBorderColor(theme, props)
  }
}));

export interface UploadDropzoneProps {
  onDrop: (acceptedFiles: File[]) => void;
  accept?: string | string[];
  uploading?: boolean;
}

const UploadDropzone: FC<UploadDropzoneProps> = ({ onDrop, accept, uploading }) => {
  const dropZoneState = useDropzone({ onDrop, accept, disabled: uploading, multiple: false });
  const { getRootProps, getInputProps } = dropZoneState;
  const classes = useStyles(dropZoneState);

  const renderProgressText = () => {
    if (uploading) {
      return "Uploading...";
    }
    return "Drop file here, or click to browse";
  }

  return (
    <div {...getRootProps({ className: classes.dropZone })}>
      <input {...getInputProps()} />
      <Box flexDirection="column" display="flex" alignItems="center">
        <CloudUploadIcon fontSize='large' />
        <Typography variant="h6">
          {renderProgressText()}
        </Typography>
        {uploading && (
          <Box width="100%" pt={2}>
            <LinearProgress style={{ width: "100%" }} variant="indeterminate" />
          </Box>
        )}
      </Box>
    </div>
  );
}

export default UploadDropzone;