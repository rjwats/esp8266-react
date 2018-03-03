/**
* Executes a get request for an endpoint, updating the local state of the calling
* component. The calling component must bind setState before using this
* function.
*
* This is designed for re-use in simple situations, we arn't using redux here!
*/
export const simpleGet = (
  endpointUrl,
  setState,
  raiseNotification = null,
  dataKey="status",
  fetchedKey="fetched",
  errorMessageKey = "errorMessage"
) => {
  setState({
           [dataKey]:null,
           [fetchedKey]: false,
           [errorMessageKey]:null
         });
  fetch(endpointUrl)
    .then(response => {
      if (response.status === 200) {
        return response.json();
      }
      throw Error("Invalid status code: " + response.status);
    })
    .then(json => {setState({[dataKey]: json, [fetchedKey]:true})})
    .catch(error =>{
      if (raiseNotification) {
        raiseNotification("Problem fetching: " + error.message);
      }
      setState({[dataKey]: null, [fetchedKey]:true, [errorMessageKey]:error.message});
    });
}
