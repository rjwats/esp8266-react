import React from 'react';
import {withNotifier} from '../components/SnackbarNotification';

/*
* It is unlikely this application will grow complex enough to require redux.
*
* This HOC acts as an interface to a REST service, providing data and change
* event callbacks to the wrapped components along with a function to persist the
* changes.
*/
export const restComponent = (endpointUrl, FormComponent) => {

  return withNotifier(
    class extends React.Component {

      constructor(props) {
        super(props);

        this.state={
                 data:null,
                 fetched: false,
                 errorMessage:null
               };

        this.setState = this.setState.bind(this);
        this.loadData = this.loadData.bind(this);
        this.saveData = this.saveData.bind(this);
        this.setData = this.setData.bind(this);
        this.setDataAndSave = this.setDataAndSave.bind(this);
      }

      setData(data) {
        this.setState({
                 data:data,
                 fetched: true,
                 errorMessage:null
               });
      }

      setDataAndSave(data, silent = false, setOnly = false) {
        this.setState({
                 data:data,
                 fetched: false,
                 errorMessage:null
               }, () => this.saveData(silent, setOnly));
      }

      loadData() {
        this.setState({
                 data:null,
                 fetched: false,
                 errorMessage:null
               });
        fetch(endpointUrl)
          .then(response => {
            if (response.status === 200) {
              return response.json();
            }
            throw Error("Invalid status code: " + response.status);
          })
          .then(json => {this.setState({data: json, fetched:true})})
          .catch(error =>{
            this.props.raiseNotification("Problem fetching: " + error.message);
            this.setState({data: null, fetched:true, errorMessage:error.message});
          });
      }

      saveData(silent = false, setOnly = false) {
        if (!setOnly){
          this.setState({fetched: false});
        }
        fetch(endpointUrl, {
          method: 'POST',
          body: JSON.stringify(this.state.data),
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
          if (!silent){
            this.props.raiseNotification("Changes successfully applied.");
          }
          if (!setOnly) {
            this.setState({data: json, fetched:true});
          }
        }).catch(error => {
          this.props.raiseNotification("Problem saving: " + error.message);
          this.setState({data: null, fetched:true, errorMessage:error.message});
        });
      }

      handleChange = (name, callback) => value => {
        const { data } = this.state;
        data[name] = value;
        this.setState({data}, callback);
      };

      handleColorChange = (name, callback) => color => {
        const { data } = this.state;
        data[name] = color.hex;
        console.log(color.hex);
        console.log(callback);
        this.setState({data}, callback);
      };

      handleValueChange = (name, callback) => event => {
        const { data } = this.state;
        data[name] = event.target.value;
        this.setState({data}, callback);
      };

      handleCheckboxChange = (name, callback) => event => {
        const { data } = this.state;
        data[name] = event.target.checked;
        this.setState({data}, callback);
      }

      render() {
        return <FormComponent
                  handleValueChange={this.handleValueChange}
                  handleCheckboxChange={this.handleCheckboxChange}
                  handleColorChange={this.handleColorChange}
                  handleChange={this.handleChange}
                  setData={this.setData}
                  setDataAndSave={this.setDataAndSave}
                  saveData={this.saveData}
                  loadData={this.loadData}
                  {...this.state}
                  {...this.props}
                />;
      }

    }
  );
}
