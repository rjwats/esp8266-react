import { AxiosError } from "axios";

export const extractErrorMessage = (error: AxiosError<{ message?: string }>, defaultMessage: string) => (
  (error?.response?.data?.message ? error.response.data.message : error.message) || defaultMessage
);
