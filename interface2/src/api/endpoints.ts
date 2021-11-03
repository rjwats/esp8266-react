import axios from 'axios';

export const API_BASE_URL = '/rest/';
export const ACCESS_TOKEN = 'accessToken';

export const AXIOS = axios.create({
  baseURL: API_BASE_URL,
  headers: {
    'Content-Type': 'application/json',
  },
  transformRequest: [(data, headers) => {
    if (headers) {
      if (localStorage.getItem(ACCESS_TOKEN)) {
        headers.Authorization = 'Bearer ' + localStorage.getItem(ACCESS_TOKEN);
      }
      if (headers['Content-Type'] !== 'application/json') {
        return data;
      }
    }
    return JSON.stringify(data);
  }]
});
