import React, { FC } from 'react';
import { useDropzone, DropzoneState } from 'react-dropzone';

import { makeStyles, createStyles } from '@material-ui/styles';
import CloudUploadIcon from '@material-ui/icons/CloudUpload';
import { Theme, Box, Typography, LinearProgress } from '@material-ui/core';

interface DropZoneStyleProps extends DropzoneState {
  uploading: boolean;
}

const getBorderColor = (theme: Theme, props: DropZoneStyleProps) => {
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
    padding: theme.spacing(8, 2),
    borderWidth: 2,
    borderRadius: 2,
    borderStyle: 'dashed',
    color: theme.palette.grey[700],
    transition: 'border .24s ease-in-out',
    cursor: (props: DropZoneStyleProps) => props.uploading ? 'default' : 'pointer',
    width: '100%',
    borderColor: (props: DropZoneStyleProps) => getBorderColor(theme, props)
  }
}));

export interface SingleUploadProps {
  accept?: string | string[];
  onDrop: (acceptedFiles: File[]) => void;
  uploading: boolean;
  progress?: ProgressEvent;
}

const SingleUpload: FC<SingleUploadProps> = ({ onDrop, accept, uploading, progress }) => {
  const dropZoneState = useDropzone({ onDrop, accept, disabled: uploading, multiple: false });
  const { getRootProps, getInputProps } = dropZoneState;
  const classes = useStyles({ ...dropZoneState, uploading });

  const renderProgressText = () => {
    if (uploading) {
      if (progress?.lengthComputable) {
        return `Uploading: ${progress.loaded} / ${progress.total} bytes`;
      }
      return "Uploading\u2026";
    }
    return "Drop file here or click to browse";
  }

  const renderProgress = (progress?: ProgressEvent) => (
    <LinearProgress
      variant={!progress || progress.lengthComputable ? "determinate" : "indeterminate"}
      value={!progress ? 0 : progress.lengthComputable ? Math.round((progress.loaded * 100) / progress.total) : 0}
    />
  );

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
            {renderProgress(progress)}
          </Box>
        )}
      </Box>
    </div>
  );
}

export default SingleUpload;
