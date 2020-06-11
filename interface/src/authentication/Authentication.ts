import * as H from 'history';

import history from '../history';
import { Features } from '../features/types';
import { getDefaultRoute } from '../AppRouting';

export const ACCESS_TOKEN = 'access_token';
export const LOGIN_PATHNAME = 'loginPathname';
export const LOGIN_SEARCH = 'loginSearch';

/**
 * Fallback to sessionStorage if localStorage is absent. WebView may not have local storage enabled.
 */
export function getStorage() {
  return localStorage || sessionStorage;
}

export function storeLoginRedirect(location?: H.Location) {
  if (location) {
    getStorage().setItem(LOGIN_PATHNAME, location.pathname);
    getStorage().setItem(LOGIN_SEARCH, location.search);
  }
}

export function clearLoginRedirect() {
  getStorage().removeItem(LOGIN_PATHNAME);
  getStorage().removeItem(LOGIN_SEARCH);
}

export function fetchLoginRedirect(features: Features): H.LocationDescriptorObject {
  const loginPathname = getStorage().getItem(LOGIN_PATHNAME);
  const loginSearch = getStorage().getItem(LOGIN_SEARCH);
  clearLoginRedirect();
  return {
    pathname: loginPathname || getDefaultRoute(features),
    search: (loginPathname && loginSearch) || undefined
  };
}

/**
 * Wraps the normal fetch routene with one with provides the access token if present.
 */
export function authorizedFetch(url: RequestInfo, params?: RequestInit): Promise<Response> {
  const accessToken = getStorage().getItem(ACCESS_TOKEN);
  if (accessToken) {
    params = params || {};
    params.credentials = 'include';
    params.headers = {
      ...params.headers,
      "Authorization": 'Bearer ' + accessToken
    };
  }
  return fetch(url, params);
}

/**
 * Wraps the normal fetch routene which redirects on 401 response.
 */
export function redirectingAuthorizedFetch(url: RequestInfo, params?: RequestInit): Promise<Response> {
  return new Promise<Response>((resolve, reject) => {
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

export function addAccessTokenParameter(url: string) {
  const accessToken = getStorage().getItem(ACCESS_TOKEN);
  if (!accessToken) {
    return url;
  }
  const parsedUrl = new URL(url);
  parsedUrl.searchParams.set(ACCESS_TOKEN, accessToken);
  return parsedUrl.toString();
}
