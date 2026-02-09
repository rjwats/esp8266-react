import { FC, Fragment } from 'react';
import { useDropzone, DropzoneState, Accept } from 'react-dropzone';

import { Box, Button, LinearProgress, Theme, Typography, useTheme } from '@mui/material';

import CloudUploadIcon from '@mui/icons-material/CloudUpload';
import CancelIcon from '@mui/icons-material/Cancel';

import { AxiosProgressEvent } from 'axios';

// Updated to handle AxiosProgressEvent safely
const progressPercentage = (progress: AxiosProgressEvent) => {
  if (!progress.loaded || !progress.total) {
    return 0;
  }
  return Math.round((progress.loaded * 100) / progress.total);
};

const getBorderColor = (theme: Theme, props: DropzoneState) => {
  if (props.isDragAccept) {
    return theme.palette.success.main;
  }
  if (props.isDragReject) {
    return theme.palette.error.main;
  }
  if (props.isDragActive) {
    return theme.palette.info.main;
  }
  return theme.palette.grey[700];
};

export interface SingleUploadProps {
  onDrop: (acceptedFiles: File[]) => void;
  onCancel: () => void;
  accept?: Accept;
  uploading: boolean;
  progress?: AxiosProgressEvent; // updated type
}

const SingleUpload: FC<SingleUploadProps> = ({
  onDrop,
  onCancel,
  accept,
  uploading,
  progress
}) => {
  const dropzoneState = useDropzone({ onDrop, accept, disabled: uploading, multiple: false });
  const { getRootProps, getInputProps } = dropzoneState;
  const theme = useTheme();

  const progressText = () => {
    if (uploading) {
      if (progress?.loaded != null && progress?.total != null) {
        return `Uploading: ${progressPercentage(progress)}%`;
      }
      return "Uploading…";
    }
    return "Drop file or click here";
  };

  return (
    <Box
      {...getRootProps({
        sx: {
          py: 8,
          px: 2,
          borderWidth: 2,
          borderRadius: 2,
          borderStyle: 'dashed',
          color: theme.palette.grey[700],
          transition: 'border .24s ease-in-out',
          width: '100%',
          cursor: uploading ? 'default' : 'pointer',
          borderColor: getBorderColor(theme, dropzoneState)
        }
      })}
    >
      <input {...getInputProps()} />
      <Box flexDirection="column" display="flex" alignItems="center">
        <CloudUploadIcon fontSize='large' />
        <Typography variant="h6">
          {progressText()}
        </Typography>
        {uploading && (
          <Fragment>
            <Box width="100%" p={2}>
              <LinearProgress
                variant={progress?.loaded != null && progress?.total != null ? "determinate" : "indeterminate"}
                value={progress?.loaded != null && progress?.total != null ? progressPercentage(progress) : 0}
              />
            </Box>
            <Button startIcon={<CancelIcon />} variant="contained" color="secondary" onClick={onCancel}>
              Cancel
            </Button>
          </Fragment>
        )}
      </Box>
    </Box>
  );
};

export default SingleUpload;
