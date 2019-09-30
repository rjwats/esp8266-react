import history from '../history';
import { PROJECT_PATH } from '../constants/Env';

export const ACCESS_TOKEN = 'access_token';
export const LOGIN_PATHNAME = 'loginPathname';
export const LOGIN_SEARCH = 'loginSearch';

export function storeLoginRedirect(location) {
  if (location) {
    localStorage.setItem(LOGIN_PATHNAME, location.pathname);
    localStorage.setItem(LOGIN_SEARCH, location.search);
  }
}

export function clearLoginRedirect() {
  localStorage.removeItem(LOGIN_PATHNAME);
  localStorage.removeItem(LOGIN_SEARCH);
}

export function fetchLoginRedirect() {
  const loginPathname = localStorage.getItem(LOGIN_PATHNAME);
  const loginSearch = localStorage.getItem(LOGIN_SEARCH);
  clearLoginRedirect();
  return {
    pathname: loginPathname || `/${PROJECT_PATH}/`,
    search: (loginPathname && loginSearch) || undefined
  };
}

/**
 * Wraps the normal fetch routene with one with provides the access token if present.
 */
export function authorizedFetch(url, params) {
  const accessToken = localStorage.getItem(ACCESS_TOKEN);
  if (accessToken) {
    params = params || {};
    params.credentials = 'include';
    params.headers = params.headers || {};
    params.headers.Authorization = 'Bearer ' + accessToken;
  }
  return fetch(url, params);
}

/**
 * Wraps the normal fetch routene which redirects on 401 response.
 */
export function redirectingAuthorizedFetch(url, params) {
  return new Promise(function (resolve, reject) {
    authorizedFetch(url, params).then(response => {
      if (response.status === 401) {
        history.push("/unauthorized");        
      } else {
        resolve(response);
      }
    }).catch(error => {
      reject(error);
    });
  });
}
