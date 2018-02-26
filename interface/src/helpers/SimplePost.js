/**
* Executes a post request for saving data to an endpoint, updating the local
* state with the response. The calling component must bind setState before
* using this function.
*
* This is designed for re-use in simple situations, we arn't using redux here!
*/
export const simplePost = (
  endpointUrl,
  state,
  setState,
  raiseNotification = null,
  dataKey="settings",
  fetchedKey="fetched",
  errorMessageKey = "errorMessage"
) => {
  setState({[fetchedKey]: false});
  fetch(endpointUrl, {
    method: 'POST',
    body: JSON.stringify(state[dataKey]),
    headers: new Headers({
      'Content-Type': 'application/json'
    })
  })
  .then(response => {
    if (response.status === 200) {
      return response.json();
    }
    throw Error("Invalid status code: " + response.status);
  })
  .then(json => {
    raiseNotification("Changes successfully applied.");
    setState({[dataKey]: json, [fetchedKey]:true});
  }).catch(error => {
    raiseNotification("Problem saving. " + error.message);
    setState({[dataKey]: null, [fetchedKey]:true, [errorMessageKey]:error.message});
  });
}
