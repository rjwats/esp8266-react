import React from 'react';
import { withSnackbar } from 'notistack';
import { redirectingAuthorizedFetch } from '../authentication/Authentication';

/*
* It is unlikely this application will grow complex enough to require redux.
*
* This HOC acts as an interface to a REST service, providing data and change
* event callbacks to the wrapped components along with a function to persist the
* changes.
*/
export const restComponent = (endpointUrl, FormComponent) => {

  return withSnackbar(
    class extends React.Component {

      constructor(props) {
        super(props);

        this.state = {
          data: null,
          fetched: false,
          errorMessage: null
        };

        this.setState = this.setState.bind(this);
        this.loadData = this.loadData.bind(this);
        this.saveData = this.saveData.bind(this);
        this.setData = this.setData.bind(this);
      }

      setData(data) {
        this.setState({
          data: data,
          fetched: true,
          errorMessage: null
        });
      }

      loadData() {
        this.setState({
          data: null,
          fetched: false,
          errorMessage: null
        });
        redirectingAuthorizedFetch(endpointUrl)
          .then(response => {
            if (response.status === 200) {
              return response.json();
            }
            throw Error("Invalid status code: " + response.status);
          })
          .then(json => { this.setState({ data: json, fetched: true }) })
          .catch(error => {
            const errorMessage = error.message || "Unknown error";
            this.props.enqueueSnackbar("Problem fetching: " + errorMessage, {
              variant: 'error',
            });
            this.setState({ data: null, fetched: true, errorMessage  });
          });
      }

      saveData(e) {
        this.setState({ fetched: false });
        redirectingAuthorizedFetch(endpointUrl, {
          method: 'POST',
          body: JSON.stringify(this.state.data),
          headers: {
            'Content-Type': 'application/json'
          }
        })
          .then(response => {
            if (response.status === 200) {
              return response.json();
            }
            throw Error("Invalid status code: " + response.status);
          })
          .then(json => {
            this.props.enqueueSnackbar("Changes successfully applied.", {
              variant: 'success',
            });
            this.setState({ data: json, fetched: true });
          }).catch(error => {
            const errorMessage = error.message || "Unknown error";
            this.props.enqueueSnackbar("Problem saving: " + errorMessage, {
              variant: 'error',
            });
            this.setState({ data: null, fetched: true, errorMessage  });
          });
      }

      handleValueChange = name => (event) => {
        const { data } = this.state;
        data[name] = event.target.value;
        this.setState({ data });
      };

      handleSliderChange = name => (event, newValue) => {
        const { data } = this.state;
        data[name] = newValue;
        this.setState({ data });
      };

      handleCheckboxChange = name => event => {
        const { data } = this.state;
        data[name] = event.target.checked;
        this.setState({ data });
      }

      render() {
        return <FormComponent
          handleValueChange={this.handleValueChange}
          handleCheckboxChange={this.handleCheckboxChange}
          handleSliderChange={this.handleSliderChange}
          setData={this.setData}
          saveData={this.saveData}
          loadData={this.loadData}
          {...this.state}
          {...this.props}
        />;
      }

    }
  );
}
