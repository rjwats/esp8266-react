import { useCallback, useEffect, useRef, useState } from 'react';
import axios, { AxiosPromise, AxiosProgressEvent } from 'axios';
import { useSnackbar } from 'notistack';

import { extractErrorMessage } from '../../utils';
import { FileUploadConfig } from '../../api/endpoints';

interface MediaUploadOptions {
  upload: (file: File, config?: FileUploadConfig) => AxiosPromise<void>;
}

const useFileUpload = ({ upload }: MediaUploadOptions) => {
  const { enqueueSnackbar } = useSnackbar();

  const [uploading, setUploading] = useState(false);
  const [uploadProgress, setUploadProgress] = useState<AxiosProgressEvent>();

  const abortControllerRef = useRef<AbortController | null>(null);

  const resetUploadingStates = () => {
    setUploading(false);
    setUploadProgress(undefined);
    abortControllerRef.current = null;
  };

  const cancelUpload = useCallback(() => {
    abortControllerRef.current?.abort();
    resetUploadingStates();
  }, []);

  useEffect(() => {
    return () => {
      abortControllerRef.current?.abort();
    };
  }, []);

  const uploadFile = async (images: File[]) => {
    try {
      const controller = new AbortController();
      abortControllerRef.current = controller;

      setUploading(true);

      await upload(images[0], {
        signal: controller.signal,
        onUploadProgress: setUploadProgress
      });

      resetUploadingStates();
      enqueueSnackbar('Upload successful', { variant: 'success' });
    } catch (error: any) {
      if (axios.isCancel(error)) {
        enqueueSnackbar('Upload aborted', { variant: 'warning' });
      } else {
        resetUploadingStates();
        enqueueSnackbar(extractErrorMessage(error, 'Upload failed'), { variant: 'error' });
      }
    }
  };

  return [uploadFile, cancelUpload, uploading, uploadProgress] as const;
};

export default useFileUpload;
