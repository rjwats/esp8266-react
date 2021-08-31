import { redirectingAuthorizedFetch } from '../authentication';
export function fetchAPI(param:any) {
    return new Promise ((resolve)=>{
      redirectingAuthorizedFetch(param.url, {
            method: 'POST',
            body: JSON.stringify(param.data),
            headers: {
              'Content-Type': 'application/json'
            }
          
      }).then(response => {
        if (response.status === 200) {
          return response.json();
        }
        throw Error("Invalid status code: " + response.status);
      }).then(json => {
        resolve (json);
      }).catch(error => {
        const errorMessage = error.message || "Unknown error";
        console.log(errorMessage);
        // this.props.enqueueSnackbar("Problem fetching: " + errorMessage, { variant: 'error' });
        // this.setState({ data: undefined, loading: false, errorMessage });
      }); 
    })
      
}