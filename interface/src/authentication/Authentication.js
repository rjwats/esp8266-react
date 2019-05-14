import history from '../history';

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
    pathname: loginPathname || "/",
    search: (loginPathname && loginSearch) || undefined
  };
}

/**
 * Wraps the normal fetch routene with one with provides the access token if present.
 */
export function secureFetch(url, params) {
  if (localStorage.getItem(ACCESS_TOKEN)) {
    params = params || {};
    params.headers = params.headers || new Headers();
    params.headers.Authorization = 'Bearer ' + localStorage.getItem(ACCESS_TOKEN)
  }
  return fetch(url, params);
}

/**
 * Wraps the normal fetch routene which redirects on 401 response.
 */
export function redirectingSecureFetch(url, params) {
  return new Promise(function (resolve, reject) {
    secureFetch(url, params).then(response => {
      if (response.status === 401) {
        history.go("/");        
      } else {
        resolve(response);
      }
    }).catch(error => {
      reject(error);
    });
  });
}
