export const PROJECT_NAME = process.env.REACT_APP_PROJECT_NAME!;
export const PROJECT_PATH = process.env.REACT_APP_PROJECT_PATH!;
export const ENDPOINT_ROOT = process.env.REACT_APP_ENDPOINT_ROOT!;

// TODO use same approach for rest endpoint?
export const WEB_SOCKET_ROOT = calculateWebSocketPrefix("/ws");

function calculateWebSocketPrefix(webSocketPath: string) {
    const webSocketRoot = process.env.REACT_APP_WEB_SOCKET_ROOT;
    if (!webSocketRoot || webSocketRoot.length === 0) {
        var loc = window.location, webSocketURI;
        if (loc.protocol === "https:") {
            webSocketURI = "wss:";
        } else {
            webSocketURI = "ws:";
        }
        return webSocketURI + "//" + loc.host + webSocketPath;
    }
    return webSocketRoot + webSocketPath;
}
